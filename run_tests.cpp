#include <cerrno>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

int main() {

  pid_t pid = fork();

  if (pid == 0) {
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

    if (dup2(inputFileFd, STDIN_FILENO) == -1 ||
        dup2(outputFileFd, STDOUT_FILENO) == -1) {
      std::cerr << "dup2 failed: " << std::strerror(errno) << '\n';
      _exit(1);
    }

    close(inputFileFd);
    close(outputFileFd);

    std::string fileName = "./bin/solution";
    char *args[] = {(char *)"solution", nullptr};
    execv(fileName.c_str(), args);
    std::cerr << "Error in executing the provided file.\n"
              << "Error: " << std::strerror(errno) << "\n";
    _exit(1);
  } else if (pid > 0) {

    constexpr int TIMEOUT = 2;
    auto start = std::chrono::steady_clock::now();

    while (true) {
      pid_t ret = waitpid(pid, nullptr, WNOHANG);
      if (ret == pid) {
        break;
      }
      auto elapsed = std::chrono::steady_clock::now() - start;
      if (elapsed >= std::chrono::seconds(TIMEOUT)) {
        kill(pid, SIGKILL);
        waitpid(pid, nullptr, 0);
        std::cout << "Time Limit Exceeded.\n";
        return 1;
      }
      usleep(100000); // Sleep 100 ms
    }
    std::cout << "Comparing answer\n";
    execl("/usr/bin/delta", "delta", "--side-by-side", "output.txt",
          "expected_output.txt", nullptr);
    std::cerr << "Failed to execute delta: " << std::strerror(errno) << '\n';
    return 1;
  } else {
    std::cerr << "Fork failed: " << std::strerror(errno) << '\n';
    return 1;
  }
}
