#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>

/*
[piazza] posts to consider: 118, 119, 126
things to fix: the idea of if the len(word) > width
*/

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
        printf("case 1: no file");
        return EXIT_SUCCESS;
    }

    const char* path = argv[2];
    struct stat data;
    int rc = stat(path, &data);
    if (rc) {
        perror("ERROR"); // no such file or directory error
        return EXIT_FAILURE;
    }

    if (S_ISREG(data.st_mode)) {
        int input_fd = open(path, O_RDONLY);
        if (input_fd < 0){
            perror("ERROR"); // file cannot be opened
            return EXIT_FAILURE;
        }
        int output_fd = 1;
        if (wrapContent(input_fd, output_fd, width) != EXIT_SUCCESS) {
            return EXIT_FAILURE;
        }
    } else if (S_ISDIR(data.st_mode)) {
        struct dirent* parentDirectory;
        DIR* parentDir;
        parentDir = opendir(path);

        if (!parentDir) {
            perror("Failed to open directory!\n");
            return EXIT_FAILURE;
        }

        char* filename;
        while ((parentDirectory = readdir(parentDir))) {
            filename = parentDirectory->d_name;
            stat(filename, &data);

            if (S_ISREG(data.st_mode)) {
                printf("%s\n", filename);
                int input_fd = open(filename, O_RDONLY);
                if (input_fd < 0){
                    perror("file open error");
                    return EXIT_FAILURE;
                }
                int output_fd = 1;
                if (wrapContent(input_fd, output_fd, width) != EXIT_SUCCESS) {
                    return EXIT_FAILURE;
                }
            }
        }

        closedir(parentDir);
    }

    return EXIT_SUCCESS;
}

int wrapContent(int input_fd, int output_fd, int width) {
    int rc = EXIT_SUCCESS;

    int bytes_read = 0;
    char buffer[width];

    int char_count = 0;
    int current_index = 0; // also tracks word length
    char current_word[width];
    int space = 0;
    int newline_counter = 0;

    char space_arr[1] = {' '};
    char newline_arr[1] = {'\n'};

    while ((bytes_read = read(input_fd, buffer, width)) > 0) {
        for (int i = 0; i < bytes_read; i++){
            if (!isspace(buffer[i])) {
                current_word[current_index] = buffer[i];
                current_index++;
                char_count++;
                space = 0;
                newline_counter = 0;
            } else {
                if (space == 0 && current_index != 0) {
                    if (current_index > width) {
                        perror("");
                        rc = EXIT_FAILURE;
                    }
                    if (char_count > width) {
                        write(output_fd, newline_arr, sizeof(char));
                        char_count = current_index;
                    }
                    write(output_fd, current_word, current_index);
                    current_index = 0;
                    
                    if (char_count + 1 <= width) {
                        write(output_fd, space_arr, sizeof(char));
                        char_count++;
                    }
                }
                space = 1;

                if (buffer[i] == '\n') {
                    newline_counter++;
                    if (newline_counter == 2) {
                        write(output_fd, newline_arr, sizeof(char));
                        write(output_fd, newline_arr, sizeof(char));
                        write(output_fd, current_word, current_index);
                        char_count = current_index;
                        current_index = 0;
                    }
                }
            }
        }
    }
    
    if (current_index + char_count <= width) {
        write(output_fd, current_word, current_index);
    } else {
        write(output_fd, newline_arr, sizeof(char));
        write(output_fd, current_word, current_index);
        char_count = current_index;
    }
    current_index = 0;
    
    if (char_count + 1 <= width) {
        write(output_fd, space_arr, sizeof(char));
    }

    close(input_fd);
    
    return rc;
}