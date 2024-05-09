#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_TEXT_SIZE 50
#define MAX_BINARY_SIZE 50

void write_error(const char *msg) {
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
}

void log_action(const char *log_filename, const char *action_message) {
    char log_dir[] = "log";
    struct stat st = {0};

    // Check if "log" directory exists, create if not
    if (stat(log_dir, &st) == -1) {
        mkdir(log_dir, 0755);
    }

    char log_filepath[256];
    snprintf(log_filepath, sizeof(log_filepath), "%s/%s", log_dir, log_filename);

    int log_fd = open(log_filepath, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (log_fd == -1) {
        write_error("Error opening/creating log file");
        perror("open");
        return;
    }
    write(log_fd, action_message, strlen(action_message));
    close(log_fd);
}

void rename_file(const char *old_name, const char *new_name, const char *log_filename) {
    if (rename(old_name, new_name) == -1) {
        write_error("Error renaming file");
        perror("rename");
        exit(EXIT_FAILURE);
    }
    char success_message[100];
    snprintf(success_message, sizeof(success_message), "File '%s' renamed to '%s' successfully.\n", old_name, new_name);
    write(STDOUT_FILENO, success_message, strlen(success_message));

    // Log the action
    char log_message[200];
    snprintf(log_message, sizeof(log_message), "Renamed file: %s -> %s\n", old_name, new_name);
    log_action(log_filename, log_message);
}

void rename_dir(const char *old_name, const char *new_name, const char *log_filename) {
    if (rename(old_name, new_name) == -1) {
        write_error("Error renaming directory");
        perror("rename");
        exit(EXIT_FAILURE);
    }
    char success_message[100];
    snprintf(success_message, sizeof(success_message), "Directory '%s' renamed to '%s' successfully.\n", old_name, new_name);
    write(STDOUT_FILENO, success_message, strlen(success_message));

    // Log the action
    char log_message[200];
    snprintf(log_message, sizeof(log_message), "Renamed directory: %s -> %s\n", old_name, new_name);
    log_action(log_filename, log_message);
}

void add_text_to_file(const char *filename, const char *text, const char *log_filename) {
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        write_error("Error opening file");
        perror("open");
        return;
    }

    size_t text_len = strlen(text);
    if (text_len > MAX_TEXT_SIZE) {
        text_len = MAX_TEXT_SIZE;  // Set text length to 50 if it exceeds
    }

    if (write(fd, text, text_len) != (ssize_t)text_len) {
        write_error("Error writing text to file");
        perror("write");
    }

    close(fd);

    // Log the action
    char log_message[200];
    snprintf(log_message, sizeof(log_message), "Added text '%s' to file: %s\n", text, filename);
    log_action(log_filename, log_message);
}

void add_integer_to_file(const char *filename, int number, const char *log_filename) {
    int fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1) {
        write_error("Error opening file");
        perror("open");
        return;
    }

    char ascii_value[16]; // Assuming an integer won't exceed 16 digits
    int len = 0;
    int temp = number;
    while (temp != 0) {
        temp /= 10;
        len++;
    }

    temp = number;
    for (int i = len - 1; i >= 0; i--) {
        ascii_value[i] = '0' + (temp % 10);
        temp /= 10;
    }

    if (write(fd, ascii_value, len) != len) {
        write_error("Error writing integer to file");
        perror("write");
    }

    close(fd);

    // Log the action
    char log_message[200];
    snprintf(log_message, sizeof(log_message), "Added integer '%d' to file: %s\n", number, filename);
    log_action(log_filename, log_message);
}

void delete_file(const char *file_name, const char *log_filename) {
    if (unlink(file_name) == -1) {
        char error_message[100];
        snprintf(error_message, sizeof(error_message), "Error deleting file '%s'\n", file_name);
        write(STDERR_FILENO, error_message, strlen(error_message));
        perror("unlink");
        exit(EXIT_FAILURE);
    }
    char success_message[100];
    snprintf(success_message, sizeof(success_message), "File '%s' deleted successfully.\n", file_name);
    write(STDOUT_FILENO, success_message, strlen(success_message));

    // Log the action
    char log_message[200];
    snprintf(log_message, sizeof(log_message), "Deleted file: %s\n", file_name);
    log_action(log_filename, log_message);
}

void delete_dir(const char *dir_name, const char *log_filename) {
    char command[1024];
    snprintf(command, sizeof(command), "rm -r %s", dir_name);
    if (system(command) == -1) {
        char error_message[100];
        snprintf(error_message, sizeof(error_message), "Error deleting directory '%s'\n", dir_name);
        write(STDERR_FILENO, error_message, strlen(error_message));
        perror("system");
        exit(EXIT_FAILURE);
    }
    char success_message[100];
    snprintf(success_message, sizeof(success_message), "Directory '%s' deleted successfully.\n", dir_name);
    write(STDOUT_FILENO, success_message, strlen(success_message));

    // Log the action
    char log_message[200];
    snprintf(log_message, sizeof(log_message), "Deleted directory: %s\n", dir_name);
    log_action(log_filename, log_message);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        return EXIT_FAILURE;
    }

    const char *option = argv[1];
    const char *name = argv[2];
    const char *log_filename = NULL;
    struct stat statbuf;

    // Parse command line arguments
    if (argc < 4) {
        write_error("Insufficient arguments.");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[argc - 2], "-l") == 0) {
        log_filename = argv[argc - 1];  // Last argument before log file name is the log option
        argc -= 2;  // Decrement argc to exclude log option and log file name from further processing
    }

    if (strcmp(option, "-c") == 0) {
        if (argc != 3) {
            write_error("Usage: <executable> -c <dir_name> [-l <log_file_name>]");
            return EXIT_FAILURE;
        }

        // Check if the directory already exists
        if (stat(name, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            char exists_message[100];
            snprintf(exists_message, sizeof(exists_message), "Directory '%s' already exists.\n", name);
            write(STDERR_FILENO, exists_message, strlen(exists_message));
            return EXIT_FAILURE;
        }

        // Attempt to create the directory
        if (mkdir(name, 0777) == 0) {
            char success_message[100];
            snprintf(success_message, sizeof(success_message), "Directory '%s' created successfully.\n", name);
            write(STDOUT_FILENO, success_message, strlen(success_message));
        } else {
            perror("Error creating directory");
            return EXIT_FAILURE;
        }

        // Log the action
        char log_message[200];
        snprintf(log_message, sizeof(log_message), "Created directory: %s\n", name);
        log_action(log_filename, log_message);
    } else if (strcmp(option, "-d") == 0) {
        if (argc != 3) {
            write_error("Usage: <executable> -d <name> [-l <log_file_name>]");
            return EXIT_FAILURE;
        }

        if (stat(name, &statbuf) == -1) {
            char error_message[100];
            snprintf(error_message, sizeof(error_message), "Error getting file/directory info for '%s'\n", name);
            write(STDERR_FILENO, error_message, strlen(error_message));
            perror("stat");
            return EXIT_FAILURE;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            delete_dir(name, log_filename);
        } else {
            delete_file(name, log_filename);
        }
    } else if (strcmp(option, "-r") == 0) {
        if (argc != 4) {
            write_error("Usage: <executable> -r <old_name> <new_name> [-l <log_file_name>]");
            return EXIT_FAILURE;
        }
        const char *old_name = argv[2];
        const char *new_name = argv[3];

        if (stat(old_name, &statbuf) == -1) {
            write_error("Error getting file/directory info for old_name");
            perror("stat");
            return EXIT_FAILURE;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            rename_dir(old_name, new_name, log_filename);
        } else {
            rename_file(old_name, new_name, log_filename);
        }
    } else if (strcmp(option, "-s") == 0) {
        if (argc != 4) {
            write_error("Usage: <executable> -s <filename> <text> [-l <log_file_name>]");
            return EXIT_FAILURE;
        }
        const char *filename = argv[2];
        const char *text = argv[3];
        add_text_to_file(filename, text, log_filename);
        char msg[] = "Added text to file.";
        write(STDOUT_FILENO, msg, strlen(msg));
    } else if (strcmp(option, "-e") == 0) {
        if (argc != 4) {
            write_error("Usage: <executable> -e <filename> <start_number> [-l <log_file_name>]");
            return EXIT_FAILURE;
        }
        const char *filename = argv[2];
        int start_number = atoi(argv[3]);
        if (start_number < 51 || start_number > 199) {
            write_error("Starting number must be between 51 and 199");
            return EXIT_FAILURE;
        }

        int bytes_written = 0;
        if (start_number % 2 != 0) {
            start_number++; // Skip the odd number
        }

        for (int i = start_number; i <= 199; i += 2) {
            if (bytes_written >= MAX_BINARY_SIZE) {
                break;
            }
            add_integer_to_file(filename, i, log_filename);
            bytes_written += snprintf(NULL, 0, "%d", i);
        }

        char msg[] = "Added even numbers to file.";
        write(STDOUT_FILENO, msg, strlen(msg));
    } else {
        write_error("Invalid option.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
