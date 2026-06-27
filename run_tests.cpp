#include <iostream>
#include <string>
#include <unistd.h>

int main() {
  std::string fileName = "./solution.cpp";
  char *args[] = {(char *)"solution", nullptr};
  if (auto err = execv(fileName.c_str(), args)) {
    std::cout << "Error in executing the provided file.\n"
              << "Error Code: " << err << "\n";
  }
}
