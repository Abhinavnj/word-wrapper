#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "strbuf.h"

#define BUFFER_SIZE 1

int wrapFile(const char* input_path, int output_fd, int width);
int wrapContent(int input_fd, int output_fd, int width);

int main(int argc, char const *argv[])
{
    if (argc != 2 && argc != 3) {
        perror("ERROR [improper arguments]");
        return EXIT_FAILURE;
    }

    int width = atoi(argv[1]);

    if (width <= 0) {
        perror("ERROR [invalid width]");
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        if (wrapContent(0, 1, width)){
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    const char* path = argv[2];
    struct stat arg_data;
    if (stat(path, &arg_data)) {
        perror("ERROR"); // no such file or directory error
        return EXIT_FAILURE;
    }

    int rc = EXIT_SUCCESS;

    if (S_ISREG(arg_data.st_mode)) { // if the argument is a file
        if (wrapFile(path, 1, width)) {
            return EXIT_FAILURE;
        }
    } else if (S_ISDIR(arg_data.st_mode)) { // if the argument is a directory
        struct dirent* parentDirectory;
        DIR* parentDir;

        parentDir = opendir(path);
        if (!parentDir) {
            perror("Failed to open directory!\n");
            return EXIT_FAILURE;
        }

        struct stat data;
        char* filename;
        char* filepath = malloc(0);
        char* output_file = malloc(0);
        while ((parentDirectory = readdir(parentDir))) {
            filename = parentDirectory->d_name;

            int filepath_size = strlen(path)+strlen(filename) + 2;
            filepath = realloc(filepath, filepath_size * sizeof(char));
            strcpy(filepath, path);
            strcat(filepath, "/");
            strcat(filepath, filename);

            stat(filepath, &data);
            if (S_ISREG(data.st_mode)) {
                if ((filename[0] != '.') && (strncmp("wrap", filename, strlen("wrap")) != 0)) {
                    int output_file_size = strlen(path) + strlen("/wrap.") + strlen(filename) + 1;

                    output_file = realloc(output_file, sizeof(char) * output_file_size);
                    strcpy(output_file, path);
                    strcat(output_file, "/wrap.");
                    strcat(output_file, filename);

                    int output_fd = open(output_file, O_RDWR|O_TRUNC|O_CREAT, 0777);
                    if (wrapFile(filepath, output_fd, width)) {
                        rc = EXIT_FAILURE;
                    }
                }
            }
        }

        free(filepath);
        free(output_file);

        closedir(parentDir);
    }

    return rc;
}

int wrapFile(const char* input_path, int output_fd, int width){
    int input_fd = open(input_path, O_RDONLY);
    if (input_fd < 0){
        perror("ERROR"); // file cannot be opened
        return EXIT_FAILURE;
    }

    if (wrapContent(input_fd, output_fd, width) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    close(input_fd);

    return EXIT_SUCCESS;
}

int wrapContent(int input_fd, int output_fd, int width) {
    int rc = EXIT_SUCCESS;
    
    int bytes_read = 0;
    char buffer[BUFFER_SIZE];

    int char_count = 0;
    strbuf_t current_word;
    sb_init(&current_word, 1);

    int space = 0;
    int newline_counter = 0;
    int character_counter = 0;
    char space_arr[1] = {' '};
    char newline_arr[1] = {'\n'};

    int i = 0;
    while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
        for (i = 0; i < bytes_read; i++){
            if (!isspace(buffer[i])){
                char_count++;
                sb_append(&current_word, buffer[i]);
                space = 0;
                newline_counter = 0;
                character_counter++;
            } else {
                if (space == 0 && current_word.used != 0) {
                    if (current_word.used > width) {
                        write(output_fd, newline_arr, sizeof(char));
                        write(output_fd, current_word.data, current_word.used);
                        write(output_fd, newline_arr, sizeof(char));
                        sb_destroy(&current_word);
                        sb_init(&current_word, 1);
                        char_count = 0;
                        rc = EXIT_FAILURE;
                    } else {
                        if (char_count+1 > width) {
                            write(output_fd, newline_arr, sizeof(char));
                            char_count = current_word.used;
                        }
                        if (char_count != current_word.used) {
                            write(output_fd, space_arr, sizeof(char));
                            char_count++;
                        }

                        write(output_fd, current_word.data, current_word.used);
                        sb_destroy(&current_word);
                        sb_init(&current_word, 1);
                    }
                }
                space = 1;
                
                if (buffer[i] == '\n') {
                    newline_counter++;
                    if (newline_counter == 2 && character_counter > 0) {
                        write(output_fd, newline_arr, sizeof(char));
                        write(output_fd, newline_arr, sizeof(char));
                        write(output_fd, current_word.data, current_word.used);
                        char_count = current_word.used;
                    }
                }
            }
        }
    }

    // print remaining buffer contents after reading is finished
    printf("%d  %d\n", newline_counter, char_count);
    printf("%d  %d")
    if (current_word.used + char_count + 1 <= width) {
        if (current_word.used != 0){
            write(output_fd, space_arr, sizeof(char));
            write(output_fd, current_word.data, current_word.used);
        }
    } else {
        write(output_fd, newline_arr, sizeof(char));
        write(output_fd, current_word.data, current_word.used);
        char_count = current_word.used;
    }

    sb_destroy(&current_word);

    write(output_fd, newline_arr, sizeof(char));

    return rc;
}