#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

inline constexpr std::string DEFUALT_PHOTO_LOCATION = "serverPhotos";
inline constexpr std::string DEFUALT_COLLAGE_LOCATION = "serverColllages";

struct PhotoURLs {
  std::string url;
  std::string urlThumb;
  int64_t timestamp;
  std::string url_viewer;
  
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PhotoURLs, url, urlThumb, timestamp, url_viewer)
};



struct SessionInfo {
  //WHERE COLLAGE ARE SAVED TO
  std::string save_path = DEFUALT_COLLAGE_LOCATION;

  
  bool activeSession = false;

  int sessionPhotoCount = 0;
  std::string sessionID = "";

  std::vector<std::string> photoPaths;
  std::vector<std::string> collagePaths;
  std::vector<PhotoURLs> urlData;
  



  static std::string generateUUID() {
    std::ifstream file("/proc/sys/kernel/random/uuid");
    std::string uuid;
    if (file.is_open()) {
      std::getline(file, uuid);
      file.close();
    } else {
      // if fail
      uuid = getTimestamp();
    }
    return uuid;
  }

  static std::string getTimestamp() {
    const auto now = std::chrono::system_clock::now();
    // Format into an ISO 8601 string (e.g., 2024-06-15T13:45:30Z)
    const std::string timestamp_str = std::format("{}", now);
    return timestamp_str;
  }
};
