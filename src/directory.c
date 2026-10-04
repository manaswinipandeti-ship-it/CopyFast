#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>

#include "directory.h"
#include "copier.h"

#define MAX_THREADS 4

typedef struct {
    char source[PATH_MAX];
    char destination[PATH_MAX];
} CopyTask;

void *copy_worker(void *arg)
{
    CopyTask *task = (CopyTask *)arg;

    printf("[Thread %lu] Copying: %s\n",
           (unsigned long)pthread_self(),
           task->source);

    if (copy_file(task->source, task->destination) != 0) {
        printf("[Thread %lu] FAILED\n",
               (unsigned long)pthread_self());
    } else {
        printf("[Thread %lu] Completed\n",
               (unsigned long)pthread_self());
    }

    free(task);
    return NULL;
}

int copy_directory(const char *source, const char *destination)
{
    DIR *dir = opendir(source);

    if (dir == NULL) {
        perror("Error opening source directory");
        return -1;
    }

    if (mkdir(destination, 0755) != 0) {
        struct stat st;

        if (stat(destination, &st) != 0 ||
            !S_ISDIR(st.st_mode)) {
            perror("Error creating destination directory");
            closedir(dir);
            return -1;
        }
    }

    pthread_t threads[MAX_THREADS];
    int active_threads = 0;

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char source_path[PATH_MAX];
        char destination_path[PATH_MAX];

        snprintf(source_path, PATH_MAX,
                 "%s/%s", source, entry->d_name);

        snprintf(destination_path, PATH_MAX,
                 "%s/%s", destination, entry->d_name);

        struct stat file_info;

        if (stat(source_path, &file_info) != 0) {
            perror("stat");
            continue;
        }

        /* Only regular files in this version */
        if (!S_ISREG(file_info.st_mode)) {
            continue;
        }

        /*
         * If all worker slots are busy,
         * wait for the first one to finish.
         */
        if (active_threads == MAX_THREADS) {
            pthread_join(threads[0], NULL);

            for (int i = 1; i < active_threads; i++) {
                threads[i - 1] = threads[i];
            }

            active_threads--;
        }

        CopyTask *task = malloc(sizeof(CopyTask));

        if (task == NULL) {
            perror("malloc");
            continue;
        }

        snprintf(task->source, PATH_MAX, "%s", source_path);
        snprintf(task->destination, PATH_MAX, "%s",
                 destination_path);

        if (pthread_create(&threads[active_threads],
                           NULL,
                           copy_worker,
                           task) != 0) {
            perror("pthread_create");
            free(task);
            continue;
        }

        active_threads++;
    }

    /* Wait for remaining threads */
    for (int i = 0; i < active_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    closedir(dir);

    return 0;
}
