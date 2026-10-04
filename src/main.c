#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "copier.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    struct stat file_info;

    if (stat(argv[1], &file_info) != 0) {
        perror("Error getting file information");
        return 1;
    }

    long long file_size = file_info.st_size;

    struct timespec start, end;

    printf("\n========================================\n");
    printf("              COPYFAST\n");
    printf("========================================\n");
    printf("Source      : %s\n", argv[1]);
    printf("Destination : %s\n", argv[2]);
    printf("File Size   : %lld bytes\n", file_size);
    printf("----------------------------------------\n");

    clock_gettime(CLOCK_MONOTONIC, &start);

    if (copy_file(argv[1], argv[2]) != 0) {
        printf("Copy failed!\n");
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    double throughput =
        (file_size / (1024.0 * 1024.0)) / time_taken;

    printf("Copy completed successfully!\n");
    printf("----------------------------------------\n");
    printf("Bytes copied : %lld\n", file_size);
    printf("Time taken   : %.6f seconds\n", time_taken);
    printf("Throughput   : %.2f MB/s\n", throughput);
    printf("========================================\n\n");

    return 0;
}
