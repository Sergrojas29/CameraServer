#pragma once
#include "SessionInfo.h"
#include <cpr/cpr.h>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <stdexcept>
#include <string>

using json = nlohmann::json;

class UploadClient {
private:
public:
  static bool UploadImages(SessionInfo& session);

  static bool saveToJSON(const std::string &filename, const SessionInfo &newEntry);
};