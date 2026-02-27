#include "../System.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

static std::string currentWorkingDirectory = std::filesystem::current_path().string().append("\\");

void System::Initalize() {
}

const char* System::GetWorkingDirectory() {
  return currentWorkingDirectory.c_str();
}
