#ifndef POLYBIUS_H
#define POLYBIUS_H

struct PolybiusTable {
    char table[5][5];
};

void initializePolybiusTable(struct PolybiusTable *table);
char* pbEncode(const char *plaintext, const struct PolybiusTable *table);
char* pbDecode(const char *ciphertext, const struct PolybiusTable *table);

#endif  // POLYBIUS_H