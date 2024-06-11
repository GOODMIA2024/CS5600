#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "polybius.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <-e/-d> \"text\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Initialize the Polybius table
    struct PolybiusTable table;
    initializePolybiusTable(&table);

    // Get the option (-e for encrypt, -d for decrypt)
    char *option = argv[1];
    char *text = argv[2];
    char *result;

    if (strcmp(option, "-e") == 0) {
        // Encrypt the text
        result = pbEncode(text, &table);
        printf("Encrypted text: %s\n", result);
    } else if (strcmp(option, "-d") == 0) {
        // Decrypt the text
        result = pbDecode(text, &table);
        printf("Decrypted text: %s\n", result);
    } else {
        printf("Invalid option. Please use -e for encryption or -d for decryption.\n");
        return EXIT_FAILURE;
    }

    free(result); // Free dynamically allocated memory
    return EXIT_SUCCESS;
}