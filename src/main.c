#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#include "copier.h"
#include "directory.h"
#include "verify.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    struct stat source_info;

    if (stat(argv[1], &source_info) != 0) {
        perror("Error accessing source");
        return 1;
    }

    printf("\n========================================\n");
    printf("              COPYFAST\n");
    printf("========================================\n");
    printf("Source      : %s\n", argv[1]);
    printf("Destination : %s\n", argv[2]);
    printf("----------------------------------------\n");

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    int result;

    if (S_ISDIR(source_info.st_mode)) {

        printf("Mode        : Concurrent Directory Copy\n\n");

        result = copy_directory(argv[1], argv[2]);

    } else {

        printf("Mode        : Buffered File Copy\n\n");

        result = copy_file(argv[1], argv[2]);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    if (result == 0) {

    printf("\n----------------------------------------\n");
    printf("Copy completed successfully!\n");
    printf("Time taken   : %.6f seconds\n", time_taken);

    if (!S_ISDIR(source_info.st_mode)) {

        printf("\nVerification: ");

        int verified = verify_file(argv[1], argv[2]);

        if (verified == 1) {
            printf("PASSED - Files are identical\n");
        } else if (verified == 0) {
            printf("FAILED - Files differ\n");
        } else {
            printf("ERROR - Verification could not be completed\n");
        }
    }

    printf("----------------------------------------\n");

} else {

        printf("\nCopy failed!\n");
        return 1;
    }

    return 0;
}
