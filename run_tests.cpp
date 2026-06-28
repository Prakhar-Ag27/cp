#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int inputFileFd = open("input.txt", O_RDONLY);
  if (inputFileFd < 0) {
    std::cerr << "Failed to open file input.txt: " << std::strerror(errno)
              << "\n"
              << "Exiting\n";
    return 1;
  }
  int outputFileFd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (outputFileFd < 0) {
    std::cerr << "Failed to open file output.txt: " << std::strerror(errno)
              << "\n"
              << "Exiting\n";
    return 1;
  }

  int savedStdIn = dup(0);
  int savedStdOut = dup(1);

  dup2(inputFileFd, 0);
  dup2(outputFileFd, 1);

  close(inputFileFd);
  close(outputFileFd);

  pid_t pid = fork();

  if (pid == 0) {
    std::string fileName = "./bin/solution";
    char *args[] = {(char *)"solution", nullptr};
    if (execv(fileName.c_str(), args)) {
      std::cerr << "Error in executing the provided file.\n"
                << "Error: " << std::strerror(errno) << "\n";
    }
  } else if (pid > 0) {
    waitpid(pid, nullptr, 0);

    dup2(savedStdIn, 0);
    dup2(savedStdOut, 1);

    close(savedStdIn);
    close(savedStdOut);

    std::cout << "Comparing answer\n";

    execl("/usr/bin/delta", "delta", "--side-by-side", "output.txt",
          "expected_output.txt", nullptr);
  }
}
