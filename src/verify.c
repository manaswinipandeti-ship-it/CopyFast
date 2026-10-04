#include <stdio.h>
#include <openssl/sha.h>

#include "verify.h"

#define BUFFER_SIZE 65536

int calculate_hash(const char *filename,
                   unsigned char hash[SHA256_DIGEST_LENGTH])
{
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        return -1;
    }

    SHA256_CTX context;
    SHA256_Init(&context);

    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        SHA256_Update(&context, buffer, bytes_read);
    }

    SHA256_Final(hash, &context);

    fclose(file);

    return 0;
}

int verify_file(const char *source, const char *destination)
{
    unsigned char source_hash[SHA256_DIGEST_LENGTH];
    unsigned char destination_hash[SHA256_DIGEST_LENGTH];

    if (calculate_hash(source, source_hash) != 0) {
        printf("Could not read source file.\n");
        return -1;
    }

    if (calculate_hash(destination, destination_hash) != 0) {
        printf("Could not read destination file.\n");
        return -1;
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        if (source_hash[i] != destination_hash[i]) {
            return 0;
        }
    }

    return 1;
}
