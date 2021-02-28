#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

int wrapContent(int input_fd, int output_fd, int width);

int main(int argc, char const *argv[])
{
    if (argc != 2 && argc != 3) {
        perror("Invalid arguments\n");
        return EXIT_FAILURE;
    }

    int width = atoi(argv[1]);

    if (width <= 0) {
        perror("Invalid width\n");
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        printf("case 1: no file\n");
        return EXIT_SUCCESS;
    }

    // Case 2: file provided
    const char* path = argv[2];
    struct stat data;
    int rc = stat(path, &data);
    if (rc) {
        perror("file/path error\n");
        return EXIT_FAILURE;
    }

    if (S_ISREG(data.st_mode)) {
        int input_fd = open(path, O_RDONLY);
        if (input_fd < 0){
            perror("file open error\n");
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
    int bytes_read = 0;
    char buffer[width];
    int char_count = 0;
    int space = 0;
    int current_index = 0; // also tracks length of word
    char current_word[width];
    int newline_counter = 0;
    char space_arr[1] = {' '};
    char newline_arr[1] = {'\n'};

    while ((bytes_read = read(input_fd, buffer, width)) > 0) {
        for (int i = 0; i < bytes_read; i++){
            if (buffer[i] != ' ' && buffer[i] != '\n') {
                current_word[current_index] = buffer[i];
                current_index++;
                char_count++;
                space = 0;
                newline_counter = 0;
            } else {
                if (space == 0) {
                    // print word
                    if (current_index > width) {
                        perror("");
                        return EXIT_FAILURE;
                    }
                    if (char_count > width) {
                        write(output_fd, newline_arr, sizeof(char));
                        char_count = current_index;
                    }
                    write(output_fd, current_word, current_index);
                    current_index = 0;
                    
                    // print space
                    if (char_count + 1 <= width) {
                        write(output_fd, space_arr, sizeof(char));
                        char_count++;
                    }
                }
                space = 1;

                if (buffer[i] == '\n') {
                    newline_counter++;
                    if (newline_counter >= 2) {
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
    // print word
    if (current_index + char_count <= width) {
        write(output_fd, current_word, current_index);
    } else {
        write(output_fd, newline_arr, sizeof(char));
        write(output_fd, current_word, current_index);
        char_count = current_index;
    }
    current_index = 0;
    
    // print space
    if (char_count + 1 <= width) {
        write(output_fd, space_arr, sizeof(char));
    }

    close(input_fd);
    
    return EXIT_SUCCESS;
}