#include "message.h" // Include the header where message_t and cache_t are defined

/**
 *  @brief id counter of cache
 */ 
static int id_counter = 0;

/**
 * @brief generate file path for message
 * @param id Unique identifier for the message
 * @param filePath Store the file path
 * @param size Size of the filePath
*/
void msg_file_path(int id, char* filePath, size_t size) {
    // Generate the file path directly into the filePath buffer
    snprintf(filePath, size, "msg_store/msg-%d.txt", id);
}

/**
 * @brief Free the message object
 * @param msg pointer to the message obeject
 * 
 */
void free_msg(message_t* msg) {
    if (msg != NULL) {
        free(msg);
    }
}

/**
 * @brief print the details of a single message
 * @param index The index of the message in teh cache
 * @param msg A pointer to the message
 */
void print_message_details(int cache_id, const message_t *msg) {
    printf("Cache[%d]: id=%d, sender=%s, receiver=%s, content=%s\n", cache_id, 
            msg->id, msg->sender, msg->receiver, msg->content);
}

/**
 * @brief Print the current state of the cache, including all messages
 */
void print_cache() {
    cache_t *cache = fetch_cache_singleton();
    printf("Cache size:%d\n", cache->size);

    // iterating through the cache
    for (int i = 0; i < cache -> size; i++) {
       print_message_details(i, cache->messages[i]); // 这里i如何确认 
    }
}

/**
 * @brief Page Replacement, remove an exisiting message when there is no slot open in the cache
 *        Policy1 LRU: The oldest message(at index 0) is removed and the rest of the messages are shited forward
 *        Policy2 Random Replacement: A message at a random index is removed. The last message is moved to the removed message's 
 *                                    position to ensure the cache array is used efficiently.
 */
void remove_message(cache_t* cache) {
    #if LRU
    // Remove the oldest message
    free_msg(cache->messages[0]);
    memmove(&cache->messages[0], &cache->messages[1], (cache->size - 1) * sizeof(message_t*))
    cache->size--;
    #else 
    // Random Replacement
    int index = rand() % cache -> size; // use cache->size for bounds
    free_msg(cache->messages[index]);
    if (index != cache->size-1){         // if not the last one, move last item to the removed posiiton
        cache->messages[index] = cache->messages[cache->size-1];
    }
    cache->size--;
    #endif
}

/**
 * @brief Add message to cache
 */
void add_to_cache(message_t* msg) {
    cache_t *cache = fetch_cache_singleton();
    

    // Check if the cache is full and remove message if necessary
    if (cache->size == MAX_CACHE_SIZE){
        remove_message(cache);
    }
    
    // Add new message to the end of the cache
    cache->messages[cache->size++] = msg; // pointer?

    printf("Message added to the cache.\n Message details: id=%d, sender=%s, receiver=%s, content=%s\n",
            msg->id, msg->sender, msg->receiver, msg->content);
    
    //Print the current state of the cache
    print_cache();
}

message_t* create_msg(const char* sender, const char* receiver, const char* content) {
    message_t* msg = (message_t*)malloc(sizeof(message_t));
    if (msg == NULL) return NULL;

    msg->id = ++id_counter;

    // Ensure strings are safely copied into fixed-size arrays
    strncpy(msg->sender, sender, MAX_SENDER_LENGTH - 1);
    msg->sender[MAX_SENDER_LENGTH - 1] = '\0';

    strncpy(msg->receiver, receiver, MAX_RECEIVER_LENGTH - 1);
    msg->receiver[MAX_RECEIVER_LENGTH - 1] = '\0';

    strncpy(msg->content, content, MAX_CONTENT_LENGTH - 1);
    msg->content[MAX_CONTENT_LENGTH - 1] = '\0';

    msg->delivered = false;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    msg->timestamp = (unsigned int)(tv.tv_sec * 1000000 + tv.tv_usec); // Combined seconds and microseconds

    printf("Message created: id=%d, sender=%s, receiver=%s, content=%s\n",
           msg->id, msg->sender, msg->receiver, msg->content);

    return msg;
}

bool store_msg(const message_t* msg) {
    // Check if the input message pointer is NULL to avoid dereferencing a NULL pointer.
    if (msg == NULL) {
        return false;
    }
    // Check if the directory exists, if not create it
    struct stat st = {0};
    if (stat("msg_store", &st) == -1) mkdir("msg_store", 0755);

    // Prepare a buffer to hold the file path where the message will be stored.
    char filePath[256];

    // Generate the file path based on the message's ID.
    msg_file_path(msg->id, filePath, sizeof(filePath));

    // Attempt to open the file for appending. If the file does not exist, it will be created.
    FILE *file = fopen(filePath, "a");
    if (file == NULL) {
        // If the file cannot be opened, print an error message and return false.
        perror("Failed to open file");
        return false;
    }

    // Write the message details to the file in a CSV format.
    fprintf(file, "%d,%u,%s,%s,%s,%d\n",
            msg->id, msg->timestamp, msg->sender, msg->receiver, msg->content, msg->delivered);
    fclose(file);

    // Add the message to the cache for quick access; for handling cache size: add and remove policy
    add_to_cache((message_t*)msg);

    // Message successfully stored and addes to the cache, return true
    return true;
}


/**
 * Retrieves a message by its ID, prioritizing the cache, and falling back to disk storage.
 * 
 * @param id The ID of the message to retrieve.
 * @return A pointer to the message if found, NULL otherwise.
 */
message_t* retrieve_msg(const int id) {
    // Attempt to retrieve the message from the cache first.
    cache_t *cache = fetch_cache_singleton();
    for (int i = 0; i < cache->size; i++) {
        if (cache->messages[i]->id == id) {
            // If found and using LRU, update the cache to reflect recent access.
#if LRU
            message_t *temp = cache->messages[i];
            // Only perform the move if the message is not already the most recently used.
            if (i != cache->size - 1) {
                // Shift messages left to remove the gap.
                memmove(&cache->messages[i], &cache->messages[i + 1],
                        (cache->size - i - 1) * sizeof(message_t *));
                // Place the accessed message at the end.
                cache->messages[cache->size - 1] = temp;
            }
#endif
            // Announce the cache hit and the message details.
            printf("Message retrieved from cache: id=%d, sender=%s, receiver=%s, content=%s\n",
                   cache->messages[i]->id, cache->messages[i]->sender,
                   cache->messages[i]->receiver, cache->messages[i]->content);
            return cache->messages[i];
        }
    }

    // If not in cache, attempt to load from persistent storage.
    char filePath[256];
    msg_file_path(id, filePath, sizeof(filePath));

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    // Attempt to read a line (the message) from the file.
    char line[MESSAGE_SIZE];
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Failed to read from file");
        fclose(file);
        return NULL;
    }

    // Allocate a new message object.
    message_t *msg = malloc(sizeof(message_t));
    if (msg == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Parse the line into the message object's fields.
    // Assumes CSV format: id,timestamp,sender,receiver,content,delivered
    char *token = strtok(line, ",");
    int fieldIndex = 0; // To track which field is being read and populated.
    while (token != NULL && fieldIndex < 6) { // Ensure we don't read more fields than expected.
        switch (fieldIndex) {
            case 0: msg->id = atoi(token); break;
            case 1: msg->timestamp = (unsigned int)strtoul(token, NULL, 10); break;
            case 2: strncpy(msg->sender, token, MAX_SENDER_LENGTH - 1); msg->sender[MAX_SENDER_LENGTH - 1] = '\0'; break;
            case 3: strncpy(msg->receiver, token, MAX_RECEIVER_LENGTH - 1); msg->receiver[MAX_RECEIVER_LENGTH - 1] = '\0'; break;
            case 4: strncpy(msg->content, token, MAX_CONTENT_LENGTH - 1); msg->content[MAX_CONTENT_LENGTH - 1] = '\0'; break;
            case 5: msg->delivered = (bool)atoi(token); break;
        }
        token = strtok(NULL, ",");
        fieldIndex++;
    }

    fclose(file); // Always close the file after finished reading.

    // Optionally, add the message to the cache for future accesses.
    add_to_cache(msg);

    // Announce retrieval from storage and return the loaded message.
    printf("Message retrieved from storage: id=%d, sender=%s, receiver=%s, content=%s\n",
           msg->id, msg->sender, msg->receiver, msg->content);

    return msg;
}