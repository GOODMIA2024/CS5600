#include "polybius.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void initializePolybiusTable(struct PolybiusTable *table) {
    // Initialize the Polybius table with the alphabet (excluding 'J')
    char alphabet[] = "ABCDEFGHIKLMNOPQRSTUVWXYZ";
    int index = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            table->table[i][j] = alphabet[index++];
        }
    }
}

char* pbEncode(const char *plaintext, const struct PolybiusTable *table) {
    // Encode the plaintext using the Polybius table
    int len = strlen(plaintext);
    char* result = (char*)malloc((2 * len + 1) * sizeof(char));  // Each character may be replaced by two digits

    if (result == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int resultIndex = 0;

    for (int i = 0; i < len; i++) {
        char currentChar = toupper(plaintext[i]);

        if (currentChar == 'J') {
            // Skip 'J' as it is not included in the table
            continue;
        }

        if (isalpha(currentChar)) {
            // Find the coordinates in the table and append them to the result
            for (int row = 0; row < 5; row++) {
                for (int col = 0; col < 5; col++) {
                    if (table->table[row][col] == currentChar) {
                        result[resultIndex++] = '1' + row;
                        result[resultIndex++] = '1' + col;
                        break;
                    }
                }
            }
        } else {
            // Non-alphabetic characters remain unchanged
            result[resultIndex++] = currentChar;
        }
    }

    result[resultIndex] = '\0';  // Null-terminate the result string
    return result;
}

char* pbDecode(const char *ciphertext, const struct PolybiusTable *table) {
    // Decode the ciphertext using the Polybius table
    int len = strlen(ciphertext);
    char* result = (char*)malloc((len / 2 + 1) * sizeof(char));  // Each pair of digits may be replaced by one character

    if (result == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int resultIndex = 0;

    for (int i = 0; i < len; i += 2) {
        if (isdigit(ciphertext[i]) && isdigit(ciphertext[i + 1])) {
            // Convert the pair of digits to row and column indices
            int row = ciphertext[i] - '1';
            int col = ciphertext[i + 1] - '1';

            // Append the corresponding character from the Polybius table to the result
            result[resultIndex++] = table->table[row][col];
        } else {
            // Non-digit characters remain unchanged
            result[resultIndex++] = ciphertext[i];
            // If there's only one digit left, it's likely an error in encoding,
            // so it can be printed as it is and returned.
            if (i + 1 == len) {
                result[resultIndex++] = ciphertext[i + 1];
            }
        }
    }

    result[resultIndex] = '\0';  // Null-terminate the result string
    return result;
}
