//g++ -o dup test_dup.cpp

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>

void readChildOutput(int pipefd) {
    const int bufferSize = 4096;
    char buffer[bufferSize];
    ssize_t bytesRead;
    
    while ((bytesRead = read(pipefd, buffer, bufferSize)) > 0) {
        // Process the output here
        std::string output(buffer, bytesRead);
        std::cout << "Child output: " << output;
    }
}

int main() {
    int pipefd[2];
    int pipeerr_fd[2];
    if (pipe(pipefd) < 0 || pipe(pipeerr_fd) < 0) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process

        close(pipefd[0]);
        close(pipeerr_fd[0]);

        // Redirect standard output to the write end of the pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1 || dup2(pipeerr_fd[1], STDERR_FILENO) == -1) {
            perror("dup2");
            close(pipefd[1]);
            _exit(1);
        }

        char* argv[] = {const_cast<char*>("bash"), const_cast<char*>("-c"), const_cast<char*>("for ((i=0; i<10; i++)); do echo \"Printing line $i\"; sleep 0.5; done"), nullptr};
        execv("/bin/bash", argv);
        // If execv fails, exit the child process
        // perror("execv");

        close(pipefd[1]);
        close(pipeerr_fd[1]);
        _exit(1);
    } else {
        // Parent process

        // Close the write end of the pipe
        close(pipefd[1]);
        close(pipeerr_fd[1]);

        std::thread readerThread(readChildOutput, pipefd[0]);

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        // Close the read end of the pipe
        close(pipefd[0]);
        close(pipeerr_fd[0]);

        readerThread.join();

        if (WIFEXITED(status)) {
            std::cout << "Child process exited with status " << WEXITSTATUS(status) << std::endl;
        } else {
            std::cout << "Child process terminated abnormally" << std::endl;
        }
    }

    return 0;
}
