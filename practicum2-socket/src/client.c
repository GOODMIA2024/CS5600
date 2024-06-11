/*
 * client.c -- TCP Socket Client
 * 
 * adapted from: 
 *   https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/command.h"

#define PORT 2000           // Define the port on which the server is listening
#define BUFFER_SIZE 1024    // Define the size of the buffer for data transmission
#define SERVER_IP_ADDRESS "127.0.0.1"


/**
 * @brief Utility function to determine the operation type
 * 
 */
operation_t parseOperation(const char* opStr) {
  if (strcmp(opStr, "WRITE") == 0) return WRITE;
  else if (strcmp(opStr, "GET") == 0) return GET;
  else if (strcmp(opStr, "RM") == 0) return RM;
  else return INVALID;
}

/**
 * @brief Parse Command Line Arguments to determine the operation type and associated file paths.
 * 
 * @param argc The number of command line arguments
 * @param argv The array of command line argument strings
 * @return pointer of command_t
 */

command_t* parseCommand(int argc, char* argv[], int socket_desc) {
  
  // Validates the number of arguments 
  if (argc < 3 || argc > 4) {
    // print the error message
    perror("Invalid number of arguments\n");
    // print the usage information
    printf("Usage:\n"
          "  ./rfs WRITE <local-file-path> <remote-file-path>\n"
          "  ./rfs GET <remote-file-path> <local-file-path>\n"
          "  ./rfs RM <remote-file-path>\n");
    exit(1);
  }

  //Allocate memory for the command structure
  command_t* cmd = (command_t*)malloc(sizeof(command_t));

  // Handle memory allcoation failure
  if (!cmd) {
    perror("Failed to allocate memory for command\n");
    exit(1);
  }

  if (strcmp(argv[1], "WRITE") == 0) {
    cmd->op = WRITE;
  } else if (strcmp(argv[1], "GET") == 0) {
    cmd->op = GET;
  } else if (strcmp(argv[1], "RM") == 0) {
    cmd->op = RM;
  } else {
    perror("Invalid operation");
    free(cmd);
    close_socket(socket_desc);
  }

  strncpy(cmd->file_path1, argv[2], FILE_PATH_MAX - 1);
  cmd->file_path1[FILE_PATH_MAX - 1] = '\0';  // Ensure null termination
  strcpy(cmd->file_path1, argv[2]);

  if (argc == 4) {
    strncpy(cmd->file_path2, argv[3], FILE_PATH_MAX - 1);
    cmd->file_path2[FILE_PATH_MAX - 1] = '\0';  // Ensure null termination
        // strcpy(cmd->remote_file_path, argv[3]);
  } else {
    cmd->file_path2[0] ='\0';  // No remote file path provided, set to empty string
  }
  return cmd;
}
  

// Close the socket
void close_socket(int socket_desc) {
  close(socket_desc);
  printf("Close the client socket\n");
  exit(0);
}


/**
 * @brief Send WRITE Command request to the server
 *        Command Input: ./rfs WRITE <local_file_path> <remote_file_path>
 * @param cmd
 */
void writeOperation(command_t *cmd, int socket_desc) {
  int server_ready; // Track the status of ths server, ? will be changed by the server

  // 1. Send operation to the server
  if (send(socket_desc, &cmd->op, sizeof(operation_t), 0) < 0) {
    perror("Error sending operation type to server.\n");
    close_socket(socket_desc);
  }
  printf("Operation type sent to server successfully: in WRITE operation.\n");

  // 2. Server response: change to write mode
  if(recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0){
    perror("Error while receiving server's readiness in WRITE operation.\n");
     close_socket(socket_desc);
  }
  if (!server_ready) {
      perror("Server is not ready for WRITE operation.\n");
      close_socket(socket_desc);
      return;
  }
  printf("Server is ready to proceed with WRITE operation.\n");

  // // Debug Added later, set file_path in client, other
  // // Assuming cmd->file_path1 contains "folder/foo2.txt" and is populated based on input arguments
  // char* file_path = cmd->file_path2;  // This should be set from command line args
  // int file_path_length = strlen(file_path) + 1;  // +1 for null terminator

  // printf("Sending file path: %s\n", file_path);  // Debug print

  // if (send(socket_desc, file_path, file_path_length, 0) < 0) {
  //     perror("Failed to send file path to server\n");
  //     close(socket_desc);
  //     return;
  // }


  // 3. Send remote file path and size to the server
  if (server_ready) {
    // If the remote file/path is omitted, use the local file path name
    if (strlen(cmd->file_path2) == 0) {
      strcpy(cmd->file_path2, cmd->file_path1);
    }
    // 3.1 send file path
    if (send(socket_desc, cmd->file_path2, strlen(cmd->file_path2), 0) < 0) {
      printf("Unable to send remote file path.\n");
      close_socket(socket_desc);
    }
    
    // 3.2 send file size
    // Caculate the file size
    int file_size;
    FILE* local_file = fopen(cmd->file_path1, "r");
    if (local_file == NULL) {
      fprintf(stderr,"Unable to open local file\n");
      close_socket(socket_desc);
    }
    fseek(local_file, 0, SEEK_END); // move the current pointer to the end of the file
    file_size = ftell(local_file); // Obtain the current value of the file position
    rewind(local_file); // Set the file pointer to the beginning of the file

    if (send(socket_desc, &file_size, sizeof(file_size), 0) < 0) {
      printf("Unable to send file size\n");
      fclose(local_file);
      close_socket(socket_desc);
    }
    fclose(local_file);

  } else {
    printf("Server is not ready to WRITE operation\n");
    close(socket_desc);
  }
  printf("Successfully sent remote file path and file size to the server.\n");

  // 4. Permission setting
  int need_permission_setting = -1;
  if (recv(socket_desc, &need_permission_setting, sizeof(need_permission_setting), 0) < 0) {
    perror("Error while receiving server's readiness in permission setting\n");
    close_socket(socket_desc);
    return;
  }

  if (need_permission_setting == 1) {
    // Set permission
    printf("Setting permission for remote file. 0: read_only, 1: read_write\n");
    int permission_input;
    scanf("%d", &permission_input);
    if (permission_input != 0) {
      permission_input = READ_WRITE; // ?
    } else {
      permission_input = READ_ONLY; // ?
    }

    if (send(socket_desc, &permission_input, sizeof(permission_input), 0) < 0) {
      perror("Unable to send permission input to the server\n");
      close_socket(socket_desc);
    }
  }
  // else: file have exists and no need to set permission

  int error_fd;
  if (recv(socket_desc, &error_fd, sizeof(error_fd), 0) < 0) {
    perror("Cannot receive error code from the server\n");
  }
  // 5. Server response: file descriptor ready
  server_ready = -1;
  if (recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0) {
    perror("Error while receive server response on file descriptor\n");
    close_socket(socket_desc);
    return;
  }

  printf("File descriptor is ready in Server\n");

  // 6. Send local data stream to server
  if(server_ready == 1) {
    FILE* local_file = fopen(cmd->file_path1, "r");
    if (local_file == NULL){
        perror("Unable to open local file\n");
        close_socket(socket_desc);
        exit(0);
    }
    
    // Declare a buffer to hold parts of the file while it's being read and sent.
    char buffer[1024];
    int read_size = 0; // Variable to track the number of bytes read

    // Read the file in chunks up to 1024 bytes and send each chunk to the server.
    while((read_size = fread(buffer, 1, sizeof(buffer), local_file) > 0)) {
      int sent_bytes = 0;
      while (sent_bytes < read_size) {
        int n = send(socket_desc, buffer + sent_bytes, read_size - sent_bytes, 0);
        if (n < 0) {
          perror("Unable to send data stream to server\n");
          close_socket(socket_desc);
          fclose(local_file);
          exit(EXIT_FAILURE);
        }
        sent_bytes += n;
      }
    }
    if (ferror(local_file)) {
      perror("Error reading from local file\n");
      close_socket(socket_desc);
      fclose(local_file);
      exit(0);
    }

    fclose(local_file);
  } else if (server_ready == PERMISSION_DENIED) {
    printf("Permission denied\n");
    close_socket(socket_desc);
    exit(0);
  } else {
    printf("Server is not ready to write\n");
    close_socket(socket_desc);
    exit(0);
  }
  printf("Local data streams sent to the server successfully\n");
  
  // 6. Server response: Operation status
  server_ready = -1;
  if (recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0) {
    printf("Error while receiving server's response on operation status\n");
    close_socket(socket_desc);
  }

  if (server_ready) {
    printf("WRITE operation succeed.\n");
  } else {
    printf("WRITE operation fail.\n");
  }
  exit(0);
}

/**
 * @brief Send GET Command request to the server
 *        Command Input: ./rfs GET <remote_file_path> <local_file_path>
 * @param cmd Command structure containing operation type, remote and local file paths
 * @param socket_desc Socket descriptor to communicate with the server
 */
void getOperation(command_t* cmd, int socket_desc) {
  int server_ready = -1; 
  
  // 1. Send operation type to server
  if (send(socket_desc, &cmd->op, sizeof(operation_t), 0) < 0) {
    perror("Error sending GET command to server.\n");
    close_socket(socket_desc);
  }

  printf("Operation type sent to server successfully.\n");

  // 2. Server response: change to GET operation
  if(recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0){
    perror("Error while receiving server's readiness\n");
    close_socket(socket_desc);
  }

  // 3. Send remote file path and size to server
  if (server_ready) {
    printf("The remote file path is %s\n", cmd->file_path1);
    if (send(socket_desc, cmd->file_path1, strlen(cmd->file_path1)+1, 0) < 0) {
      printf("Unable to send the remote file path\n");
      close_socket(socket_desc);
    }
  } else {
    printf("Server is not ready for GET Operation\n");
    close_socket(socket_desc);
  }
  printf("Remote file path sent to the server successfully\n");


  int file_size;
  if (recv(socket_desc, &file_size, sizeof(file_size), 0) < 0) {
    printf("Error while receiving file size\n");
    close_socket(socket_desc);
  }

  if (file_size < 0) {
    printf("File is not found in the server\n");
    close_socket(socket_desc);
  } else {
    // If the local file does not exist, use the remote file name
    if (strlen(cmd->file_path2) == 0) {
      strcpy(cmd->file_path2, cmd->file_path1);
      printf("Local file path is defaulted as the remote file path %s", cmd->file_path2);
    }

    FILE* local_file = fopen(cmd->file_path2, "w");
    if (local_file == NULL) {
      perror("Failed to open local file\n");
      close_socket(socket_desc);
    }

    char buffer[9216];
    int read_size;
    int total_size = 0; // Total size received

    while (total_size < file_size) {
      read_size = recv(socket_desc, buffer, sizeof(buffer), 0);
      if (read_size < 0) {
        perror("Couldn't receive data stream from the server\n");
        close_socket(socket_desc);
      }
      if (fwrite(buffer, 1, read_size, local_file) < 0) {
        printf("Couldn't write to file\n");
        close_socket(socket_desc);
      }
      total_size += read_size;
    }

    printf("Data stream written to local file successfully.\n");
    fclose(local_file);
  }
  close_socket(socket_desc);
}

/**
 * @brief Send REMOVE Command request to the server to remove a file
 *        Command format: ./rfs RM <file_path>
 * @param cmd Command structure containing the operation type and file path
 * @param socket_desc Socket descriptor to communicate with the server
 */
void removeOperation(command_t* cmd, int socket_desc) {
  // 1. Send operation type to server
  if (send(socket_desc,&cmd->op, sizeof(operation_t), 0) < 0) {
    perror("Error sending REMOVE command to server\n");
    close_socket(socket_desc);
    return; // 
  }
  printf("REMOVE command sent to server successfully.\n");

    
  // 2. Server response: check server readiness
  int server_ready = -1;
  if (recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0) {
    perror("Error when receiving server readiness for REMOVE.\n");
    close_socket(socket_desc);
    return;
  }

  if (server_ready != 1) {
    printf("Server is not ready for REMOVE operation.\n");
    close_socket(socket_desc);
    return;
  }
  printf("Server is ready for REMOVE operation.\n");

  // 3. Send file path to be removed to the server
  if (send(socket_desc, cmd->file_path1, strlen(cmd->file_path1) + 1, 0) < 0) {
    perror("Error sending file path to the server.\n");
    close_socket(socket_desc);
    return;
  }

  printf("File path %s sent to the server successfully.\n", cmd->file_path1);

  // 4. Server responsse: Confirm file removal
  server_ready = -1;
  if (recv(socket_desc, &server_ready, sizeof(server_ready), 0) < 0) {
    perror("Error receiving file removal confirmation from server\n");
    close_socket(socket_desc);
    return;
  }

  if (server_ready != 1) {
    printf("Server failed to remove the file\n");
    close_socket(socket_desc);
    return;
  }

  printf("File removed successfully/\n");
  close_socket(socket_desc);
}


int main(int argc, char *argv[])
{
  struct sockaddr_in server_addr;
  
  // // Clean buffers:
  // memset(server_message,'\0',sizeof(server_message));
  // memset(client_message,'\0',sizeof(client_message));
  
  // Create socket:
  int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  
  // if(socket_desc < 0){
  //   printf("Unable to create socket\n");
  //   close(socket_desc);
  //   return -1;
  // }
  
  // Set port and IP the same as server-side:
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(2000);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  // Send connection request to server:
  if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
    printf("Unable to connect\n");
    close(socket_desc);
    return -1;
  }
  printf("Connected with server successfully\n");
  
  command_t* cmd = parseCommand(argc, argv, socket_desc);

  switch(cmd->op) {
    case WRITE:
      writeOperation(cmd, socket_desc);
      break;
    case GET:
      getOperation(cmd, socket_desc);
      break;
    case RM:
      removeOperation(cmd, socket_desc);
      break;
    default:
      printf("Usage:\n"
        "  ./rfs WRITE <local-file-path> <remote-file-path>\n"
        "  ./rfs GET <remote-file-path> <local-file-path>\n"
        "  ./rfs RM <remote-file-path>\n");
  }
  close_socket(socket_desc);
  
  free(cmd);
  return 0;
}
