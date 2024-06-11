/*
 * server.c -- TCP Socket Server
 * 
 * adapted from: 
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>      // For printf, perror
#include <stdlib.h>     // For exit
#include <string.h>     // For memset

#include <fcntl.h>      // For flags O_WRONLY, O_CREAT, O_TRUNC
#include <sys/socket.h> // For socket functions
#include <sys/file.h>   // For flock()
#include <sys/types.h>  // Provides types like ssize_t
#include <arpa/inet.h>
#include <unistd.h>     // For close()
#include <errno.h>
#include <netinet/in.h> // For sockaddr_in
#include <signal.h>     // For signal handling
#include <pthread.h>    // For pthread_create, pthread_detach
#include <libgen.h>     // for dirname
#include <sys/stat.h> // for mkdir

#include "../include/command.h"

#define PORT 2000
#define BUFFER_SIZE 1024
#define SERVER_FAILED -1
#define SERVER_READY 1
// #define ROOT_FOLDER "/Users/elfina/Documents/CS5600ComputerSys/Practicum2/socket/remote/"


// void ensure_directory_exists(const char* file_path) {
//     char temp[1024];
//     strcpy(temp, file_path);  // Use a temp copy because dirname may modify the input
//     char* dir_path = dirname(temp);

//     struct stat st = {0};

//     if (stat(dir_path, &st) == -1) {
//         // Directory does not exist
//         printf("Directory %s does not exist, creating...\n", dir_path);
//         if (mkdir(dir_path, 0755) == -1) { // 0755 permissions: drwxr-xr-x
//             perror("Failed to create directory");
//             exit(EXIT_FAILURE);  // Exit if fail to create directory
//         }
//         printf("Directory created.\n");
//     } else {
//         printf("Directory %s already exists.\n", dir_path);
//     }
// }

/** @brief Server's response function to the write operation
 * 
 */
void writeServerResponse(int client_socket) {
  int server_ready = 1; // Signal that the server is ready for WRITE operation
  int server_failed;
  // 1. Operation -> main() -> handle_client_connection() -> parse command
  // [Server] 2. Notify client of readiness to WRITE
  
  if (send(client_socket, &server_ready, sizeof(server_ready), 0) < 0) {
    perror("Unable to send readiness of WRITE Operation.\n");
    return;
  }
  printf("Client's operation received: Server is ready for WRITE Operation.\n");

  
  // [Server] 3. Receive the remote file path from the client
  char file_path[FILE_PATH_MAX];            // Buffer for the file path
  memset(file_path, 0, sizeof(file_path));  // Initialize buffer to zeros

  if (recv(client_socket, file_path, sizeof(file_path), 0) < 0) {
    perror("Cannot receive file path from the client.\n");  
  }

  // // Set root folder for the server
  // char full_path[1024];
  // snprintf(full_path, sizeof(full_path), "%s%s", ROOT_FOLDER, file_path);

  // [Server] 3.1 Receive File path from the client
  printf("Remote file path '%s' received at server.\n", file_path);
  
  // [Server] 3.1 Receive File size from the client
  int file_size;
  if (recv(client_socket, &file_size, sizeof(file_size), 0) < 0) {
    perror("Cannot receive the file size. \n");
    return;
  }
  printf("Successfully received %d file size in server.\n", file_size);


  // [Server] 4. Set up file descriptor with permission setting and send file descriptor readiness to the client
  int permission_input;
  server_ready = 1;

  // 4.1 Check file existence
    // if not exist, check for permission request from client
  if(access(file_path, F_OK) < 0) {  // access: check the existence of file without looking at its metadata
    // Notify client that server is ready for permission setting
    send(client_socket, &server_ready, sizeof(server_ready), 0);  // Indicate readiness for permission
    if (recv(client_socket, &permission_input, sizeof(permission_input), 0) < 0) {
        perror("Couldn't receive permission setting\n");
        return;
    }
    printf("Permission setting received: %d\n", permission_input);

  } else {  // if exist, check for file permission
    server_failed = -1;
    if (send(client_socket, &server_failed, sizeof(server_failed), 0) < 0) {
      perror("Server failed to send permission setting to client\n");
      return;
    }
  }

  // Permission setting
  // 4.2 Set up file descriptor
  // Ensure the directory exists or create it
  // make_path(full_path);  // Create the directory path before opening the file
  int file_descriptor;
  // Create file with the specified permissions
  file_descriptor = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, permission_input);
  if (file_descriptor < 0) {
    perror("Failed to open file");
    int err = errno; // Store errno locally to avoid changes and send it to client
    if (send(client_socket, &err, sizeof(err), 0) < 0) {
      perror("Unable to send error code to client\n");
    }
    return; // Exit after handling the error
    if (err == EACCES) {
      int permission_denied = PERMISSION_DENIED;
      if (send(client_socket, &permission_denied, sizeof(permission_denied), 0) < 0) {
        perror("Unable to send permission denied\n");
        return;
      }
      perror("Couldn't open file: Permission denied\n");
    } else {
      server_failed = -1;
      if (send(client_socket, &server_failed, sizeof(server_failed), 0) < 0 ) {
        perror("Unable to send failed in write mode\n");
        return;
      }
      perror("Couldn't open file\n");
    }
  }

  // [Server] 5. Send file descriptor readiness to the client
  server_ready = 1;
  // File opened or created successfully, send the signal to the client
  if (send(client_socket, &server_ready, sizeof(server_ready), 0) < 0) {
    perror("Server failed to send success status of opening/creating file to client\n");
    return;
  }

  // Lock the file descriptor
  if(flock(file_descriptor, LOCK_EX) < 0) {
    perror("Could not lock the file descriptor\n");
    return;
  }

  printf("File Descriptor is set up. File '%s' is ready for data transfer.\n", file_path);

  // [Server] 6. Receive data stream from client
  char buffer[8196];
  int read_size;
  int total_size;
  while (total_size < file_size) {
    read_size = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (read_size < 0) {
        perror("Couldn't receive data stream from the client\n");
        return;
    } else if (read_size == 0) {
      break;
    }
    int written_bytes = 0;
    while (written_bytes < read_size) {
        int n = write(file_descriptor, buffer + written_bytes, read_size - written_bytes);
        if (n < 0) {
            perror("Cannot write to the remote file\n");
            close(file_descriptor);
            return;
        }
        written_bytes += n;
    }
    total_size += read_size;
    // if (write(file_descriptor,buffer, read_size) < 0) {
    //     perror("Cannot write to the remote file\n"); 
    //     return;
    // }
    // total_size += read_size;
  }
  if (total_size != file_size) {
    fprintf(stderr, "File size mismatch: expected %d, got %d\n", file_size, total_size);
  }

  printf("Data stream received and remote file written successfully.\n");


  // 6. Send write status to client and close file 
  server_ready = 1;
  if (send(client_socket, &server_ready, sizeof(server_ready, 0), 0) < 0) {
    perror("Unable to send write success status to client\n");
    return;
  }

  printf("Remote file written successfully\n");

  // Unlock the file descriptor
  if(flock(file_descriptor, LOCK_UN) < 0) {
    perror("Could not unlock the file descriptor\n");
    return;
  }
  close(file_descriptor);
  //  fclose also closes the underlying file descriptor, so no need for close(fd)
  // Final cleanup: Since fclose(remote_file) also closes the file descriptor fd,
  // no need to call close(fd) again here. We have unlocked the fd, so resource management is complete.
}

/**
 * @brief Server's response in GET Mode
 */
void getServerResponse(int client_socket) {
  int server_ready = 1;
  int server_failed = -1;
  // [Server] 2. Send readiness in write mode to client
  if (send(client_socket, &server_ready, sizeof(server_ready), 0) < 0) {
    printf("Unable to send readiness in GET operation\n");
    return;
  }
  printf("Client's operation received\n Server is in GET operation\n");

  // 2. Receive file path from client, found the file in server
  char file_path[FILE_PATH_MAX];
  memset(file_path, 0, sizeof(file_path));
  if(recv(client_socket, file_path, sizeof(file_path), 0) < 0) {
    perror("Unable to receive file path in GET operation\n");
    return;
  }
  printf("File path received by the server: %s\n", file_path);


  // 3. If not found, send failure information to the client and return
  // If found, send file size and file data to the client
  FILE* remote_file = fopen(file_path, "r");
  if (!remote_file) {
    printf("Remote file not found\n");
    if(send(client_socket, &server_failed, sizeof(server_failed), 0) < 0) {
      perror("Unable to send failure in GET operation to client\n");
      return;
    }
    return;
  } else {
      printf("Remote file is found, preparing to send\n");
      int file_size = 0;
      fseek(remote_file, 0, SEEK_END);
      file_size = ftell(remote_file);
      rewind(remote_file);
      if (send(client_socket, &file_size, sizeof(file_size), 0) < 0) {
        perror("Unable to send remote file size in GET operation.\n");
        return;
      }

      char buffer[BUFFER_SIZE];
      int read_size = 0;
      while ((read_size = fread(buffer, 1, sizeof(buffer), remote_file)) > 0) {
          if (send(client_socket, buffer, read_size, 0) < 0) {
              printf("Unable to send remote file data in GET operation\n");
              return;
          }
      }
  fclose(remote_file);
  printf("File data is successfully sent to the client in GET operation\n");
  }
}

/**
 * @brief Sever's respond function for RM operation
 */
void removeServerResponse(int client_socket) {
  int server_ready = 1;
  int server_failed = -1;

  // [Server] 2. Send server readiness in remove mode to client
  if(send(client_socket, &server_ready, sizeof(server_ready), 0) < 0) {
    perror("Unable to send server readiness in remove mode");
    return;
  }
  printf("Successfully received client's operation. Server is in RM mode.\n");

  char file_path[FILE_PATH_MAX];
  memset(file_path, 0, sizeof(file_path));
  if(recv(client_socket, file_path, sizeof(file_path), 0) < 0) {
    printf("Couldn't receive file path\n");
    return;
  }
  printf("Successfully received file path: %s\n", file_path);
  
  // [Server] 4. receive file path and send confirmation of file removal
  int fd = open(file_path, O_RDWR);
  if(fd < 0) {
    if (errno == EACCES) {
      perror("Permission denied\n");
      if(send(client_socket, &server_failed, sizeof(server_failed), 0) < 0) {
        perror("Server is unable to send the remove status to the client\n");
        return;
      }
      return;
    }
    perror("Cannot open the file in RM mode\n");
    return;
  }

  if (remove(file_path) < 0) {
    perror("Cannot remove the file\n");
    if (send(client_socket, &server_failed, sizeof(server_failed), 0) < 0) {
      perror("Unable to send the RM status\n");
    }
    return;
  } else {
    printf("File is successfully removed\n");
    if (send(client_socket, &server_ready, sizeof(server_ready), 0) < 0) {
      perror("Unable to send the RM status\n");
      return;
    }
  }
}

/**
 * @brief Handle client connection
 */
void* handle_client_connection(void* args) {
  client_thread_args_t *client_args = (client_thread_args_t*)args;
  int client_socket = client_args->client_socket;
  printf("New thread running for the client %d\n", client_socket);
  operation_t op;

  // Read the operation requested by the client
  ssize_t bytes_read = recv(client_socket, &op, sizeof(op), 0);
  if(bytes_read < 0) {
    // Handle error or connection closed by client
    perror("Couldn't receive the operation requested by the client\n");
    close(client_socket); // Close the socket when we have done with it
    return NULL;
  } else if (bytes_read == 0){
    printf("Client closed the connection\n");
    close(client_socket);
    return NULL;
  }

  switch(op) {
    case WRITE:
      writeServerResponse(client_socket);
      break;
    case GET:
      getServerResponse(client_socket);
      break;
    case RM:
      removeServerResponse(client_socket);
      break;
    default:
      printf("Usage:\n"
      "  ./rfs WRITE <local-file-path> <remote-file-path>\n"
      "  ./rfs GET <remote-file-path> <local-file-path>\n"
      "  ./rfs RM <remote-file-path>\n");
  }

  // Free the args allocated in start_new_threads
  free(args);
  close(client_socket);
  return NULL;
}

/**
 * @brief Start a new thread for each client connection
 */
void start_new_thread(int client_socket) {
  pthread_t thread_id;
  client_thread_args_t *args = malloc(sizeof(client_thread_args_t));
  if(args == NULL) {
    perror("Failed to allocate memory for thread arguments\n");
    close(client_socket);
    return;
  }

  args->client_socket = client_socket;

  // Create a new thread for the client
  if (pthread_create(&thread_id, NULL, handle_client_connection, args) != 0) {
    perror("Failed to create a new thread for client connection\n");
    close(client_socket);
    free(args);
  } else {
    // Detach the thread since we won't be joining it later
    pthread_detach(thread_id);
    printf("New thread started for the client %d\n", client_socket);
  }
}


int server_socket;

void close_server(int signum) {
  // Gracefully shutdown the server
  close(server_socket);
  printf("Server shut down.\n");
  exit(signum);
}

int main(void) {
  // Register signal handler
  signal(SIGINT, close_server);      // Ctrl + C

  socklen_t client_size;
  struct sockaddr_in server_addr, client_addr;

  // Create socket (IPv4, TCP)
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if(server_socket < 0){
    printf("Error while creating socket\n");
    return -1;
  }
  printf("Server socket created successfully\n");

  // Set SO_REUSEADDR option for immediately rebinding the port after last
  // closing:
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
      perror("Error setting SO_REUSEADDR option");
      close(server_socket);
      return -1;
    }
  // Set the server address structure
  memset(&server_addr, 0, sizeof(server_addr));
  // Set port and IP:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // Bind to the set port and IP:
  if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
    printf("Couldn't bind to the port\n");
    return -1;
  }
  printf("Done with binding\n");

  // Listen for clients:
  if(listen(server_socket, 1) < 0){
    printf("Error while listening\n");
    close(server_socket);
    return -1;
  }
  printf("\nListening for incoming connections on port %d.....\n", PORT);
  //list_directories("../");  // Change this to the desired starting directory
  while (1) {
    printf("=====================================\n");
    // Accept an incoming connection:
    client_size = sizeof(client_addr);
    int* client_sock_ptr = malloc(sizeof(int));
    *client_sock_ptr = accept(server_socket, (struct sockaddr*)&client_addr, &client_size);

    if (*client_sock_ptr < 0) {
        printf("Can't accept, exit\n");
        close(server_socket);
        close(*client_sock_ptr);
        free(client_sock_ptr);
        return -1;
    }

    printf("\nClient connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Create a new thread for the client
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, handle_client_connection, (void*)client_sock_ptr) < 0) {
        printf("Could not create thread\n");
        free(client_sock_ptr);
        continue;
      }
  }

  return 0;
}

