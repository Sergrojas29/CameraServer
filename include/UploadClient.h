#pragma once
#include <cpr/cpr.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <optional>


class UploadClient {
private:

public:
  static std::optional<std::string> UploadImage(const std::string imagePath);
};