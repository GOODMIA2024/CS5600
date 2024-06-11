/**
 * @brief Cache is a fixed sized array of message_t pointers.
 *        Messages are stored on disk, a file per message stored in a folder.
 *        Each message is stored in both the cache and the messgae store.
 *        When cache is full, two policies are adopted to remove a message to make room for the new message: either LRU or random replacement.
 *        When the messages is retrieved, the cache is checked first. If not found, then the message needs to be loaded from disk and into cache.
*/
#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <stdio.h>

/*forward declaration of cache_t*/
typedef struct MESSAGE message_t;

/**
 * @brief The reason why using an array for the cache lies in that it is efficient for either LRU replacement policy and random replacement mechanism.
 *        It is simple to implement for a good fit for the simple cache system.
 *        Page Replacement Policy when cache is full:
 *        - Random Replacement: a random number in the range of the cache size will be gnerated and the chosen random message will be replaced;
 *        - LRU: the least recently used page is replaced with its index at 0. The messages in the cache array are moved ahead/overwrite using memmove()
 *              - Add/Update: any message added/updated will be moved to the end of the array
 * @brief Alternatives:Linkedlist + HashTable. 
 *                      Linkedlist is not efficient for random replacement algorithm in cache implementation.
 *                      HashTable: Tried but too complicated to implement.
*/

/* Define maximum size of cache */
#define MAX_CACHE_SIZE 1024

typedef struct CACHE {
    int size;
    message_t* messages[MAX_CACHE_SIZE];
} cache_t;

/**
 * @brief Get the cache instance object as a singleton
 * @return cache_t The pointer to the cache;
 */
cache_t* fetch_cache_singleton();

/**
 * @brief Free the cache instance object and message objects in the cache
*/
void free_cache();

#endif