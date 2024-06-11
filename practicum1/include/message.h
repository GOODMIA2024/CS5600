#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>  // Include this header file to include boolean value
#include <stdbool.h> // to use bool, true, and false in C.
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "cache.h"

#define MESSAGE_SIZE 1024 // Define if not already defined

// Defind a maximum length for sender, receiver and content strings
#define MAX_SENDER_LENGTH 100
#define MAX_RECEIVER_LENGTH 100
#define MAX_CONTENT_LENGTH 512

/**
 * @brief struct for message, unique id, timestamp, sender, receiver, content, flag to indicate whether delivered
 *
*/

typedef struct MESSAGE {
    int id;
    unsigned int timestamp;
    char sender[MAX_SENDER_LENGTH];    // Array to store fields like sender
    char receiver[MAX_RECEIVER_LENGTH];
    char content[MAX_CONTENT_LENGTH];
    bool delivered;                  // Boolean flag indicating delivery status
} message_t;


/**
 * @brief Create a message object
 * @return message_t Pointer of a dynamically allocated message object
*/
message_t* create_msg(const char* sender, const char* receiver, const char* content);

/**
 * @brief Store the message in message store on disk and cache
 * @return true if the message is stored correctly
*/
bool store_msg(const message_t* msg);

/**
 * @brief Find and return a message identified by its identifier
 * @return message_t Pointer of the found message
*/
message_t* retrieve_msg(const int id);


#endif // MESSAGE_H


#ifndef LRU
#define LRU 0
#endif