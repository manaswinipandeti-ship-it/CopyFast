#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "copier.h"

#define BUFFER_SIZE 65536

int copy_file(const char *source, const char *destination)
{
    int src_fd = open(source, O_RDONLY);

    if (src_fd < 0) {
        perror("Error opening source file");
        return -1;
    }

    int dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (dest_fd < 0) {
        perror("Error opening destination file");
        close(src_fd);
        return -1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    ssize_t bytes_written;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {

        bytes_written = write(dest_fd, buffer, bytes_read);

        if (bytes_written != bytes_read) {
            perror("Error writing file");
            close(src_fd);
            close(dest_fd);
            return -1;
        }
    }

    if (bytes_read < 0) {
        perror("Error reading file");
        close(src_fd);
        close(dest_fd);
        return -1;
    }

    close(src_fd);
    close(dest_fd);

    return 0;
}
