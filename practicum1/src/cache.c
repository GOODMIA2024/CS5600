#include "cache.h"

typedef struct CACHE cache_t;

/**
 * @brief Use singleton pattern
*/
cache_t* fetch_cache_singleton(){
    static cache_t* cache; // 47095 segmentation fault, cannot be initialized to NULL at beginning
    if (!cache) {
        cache = (cache_t*)malloc(sizeof(cache_t));
        if (!cache) {
            perror("Failed to allocate memory for cache");
            return NULL;
        }
        cache->size = 0;
    }
    return cache;
}

void free_cache(){
    cache_t* cache = fetch_cache_singleton();

    if (cache) {
        // Assume messages is a dynamically allocated array of pointers
        // Each message itself must also be dynamically allocated
        for (int i = 0; i < cache->size; i++){
            // Free each message
            free(cache->messages[i]);
        }
        // Free the cache itself
        free(cache);
        cache = NULL;    // Prevent dangling pointer
    }
}