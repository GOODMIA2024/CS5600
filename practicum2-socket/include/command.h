
#ifndef COMMAND_H
#define COMMAND_H

#include <sys/stat.h> // Read-only, read write

#define FILE_PATH_MAX 1024

#define READ_ONLY (S_IRUSR | S_IRGRP | S_IROTH) // Read permission for owner, group, others

#define READ_WRITE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH) // Read/write for owner and group, read for others

#define PERMISSION_DENIED 123456
#define FILE_NOT_FOUND 789012

/**
 * @brief Defines the set of operations supported by the remote file system (RFS) interface.
 * 
 * This enumeration is used to specify the type of operation to be performed by the RFS command. 
 * It is utilized as part of the command parsing and execution process to determine the action 
 * required based on user input. The operations include:
 * 
 * - WRITE: Represents a request to write a local file to a remote file path.
 *          This operation requires specifying both a local file path and a remote file path.
 * 
 * - GET:   Represents a request to retrieve a file from a remote path and save it locally.
 *          This operation requires specifying both a remote file path and a local file path where the file will be saved.
 * 
 * - RM:    Represents a request to remove a file from a remote path.
 *          This operation requires specifying the remote file path of the file to be removed.
 */
typedef enum operation {
    WRITE,
    GET,
    RM,
    INVALID // Added to handle unknown commands
} operation_t;

/**
 * @brief Data structure for parsing command line arguments related to file operations.
 * 
 * This structure represents a command parsed from command line arguments, 
 * encapsulating the operation to be performed and the necessary file paths 
 * as required by the operation. The operation types and their associated file 
 * paths are specified as follows:
 * 
 * @param op The type of operation to be performed, defined by the `operations` enum.
 *           This parameter dictates the meaning of `file_path1` and `file_path2`.
 * 
 * @param file_path1 This parameter serves multiple purposes based on the operation:
 *                   - WRITE: Represents the local file path of the file to be written to the remote system.
 *                   - GET: Represents the remote file path of the file to be fetched to the local system.
 *                   - RM: Represents the remote file path of the file to be removed.
 *                   The path should be a null-terminated string.
 * 
 * @param file_path2 This parameter also serves multiple purposes based on the operation:
 *                   - WRITE: Represents the remote file path where the local file will be written.
 *                   - GET: Represents the local file path where the fetched file will be saved.
 *                   - RM: This field is not used (should be an empty string or NULL).
 *                   Similar to `file_path1`, the path should be a null-terminated string.
 * 
 * Note: `file_path1` and `file_path2` are pointers to arrays of characters. The size of these arrays 
 * should be defined by `FILE_PATH_MAX`, which specifies the maximum path length supported by the system.
 */

typedef struct command {
    enum operation op;
    char file_path1[FILE_PATH_MAX];
    char file_path2[FILE_PATH_MAX]; // A single file path string
} command_t;


/**
 * @brief Struct for arguments passed to the thread
 *
 */ 
typedef struct {
    int client_socket;
} client_thread_args_t;

void close_socket(int socket_desc);


int setFilePermission(const char* file_path, int permission_flag);
int checkFileDescriptorReady(int client_socket, const char* file_path, int permission_flag);
void writeServerResponse(int client_socket);
void getServerResponse(int client_socket);
void removeServerResponse(int client_socket);

#endif // COMMAND_H