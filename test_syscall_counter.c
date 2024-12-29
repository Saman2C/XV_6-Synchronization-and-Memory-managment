#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

void write_to_file(char *filename, char *data) {
    int fd = open(filename, O_CREATE | O_RDWR);
    if (fd < 0) {
        printf(2, "Failed to open or create file %s\n", filename);
        exit();
    }

    if (write(fd, data, strlen(data)) != strlen(data)) {
        printf(2, "Failed to write to file %s\n", filename);
        close(fd);
        exit();
    }

    printf(1, "Child %d successfully wrote to %s\n", getpid(), filename);
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf(2, "Usage: program_name filename data\n");
        exit();
    }

    char *filename = argv[1];
    char *data = argv[2];
    int total_syscall = 0;

    for (int i = 0; i < 5; i++) {
        int pid = fork();
        if (pid < 0) {
            printf(1, "Fork failed\n");
            exit();
        }

        if (pid == 0) {
            // Child process writes to the file
            write_to_file(filename, data);
            exit();
        }
        // Parent continues to fork more children
    }

    // Parent waits for all children
    for (int i = 0; i < 5; i++) {
        wait();
    }

    total_syscall = sysCallCounter(); // Assuming sysCallCounter is implemented
    printf(1, "Total system calls used: %d\n", total_syscall);
    exit();
}
