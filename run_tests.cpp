#include <fcntl.h>
#include <iostream>
#include <string>
#include <unistd.h>

int main() {
  int inputFileFd = open("input.txt", O_RDONLY);
  int outputFileFd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

  dup2(inputFileFd, 0);
  dup2(outputFileFd, 1);

  close(inputFileFd);
  close(outputFileFd);

  std::string fileName = "./bin/solution";
  char *args[] = {(char *)"solution", nullptr};
  if (auto err = execv(fileName.c_str(), args)) {
    std::cout << "Error in executing the provided file.\n"
              << "Error Code: " << err << "\n";
  }
}
