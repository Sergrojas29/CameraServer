#pragma once
#include "nlohmann/json.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <cups/cups.h>
#include <execution>
#include <filesystem>
#include <format>
#include <future>
#include <gphoto2/gphoto2.h>
#include <iostream>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <cpr/cpr.h>

inline constexpr int PHOTOS_PER_SESSION = 4;
inline constexpr std::string OVERLAY_FILE_LOCATION = "overlays/";
inline constexpr std::string COLLAGE_FILE_LOCATION = "collages/";

struct SessionInfo {
  bool activeSession = false;

  int sessionPhotoCount = 0;
  std::string sessionID = "";
  std::string CollageTemplate = "";

  std::array<std::string, PHOTOS_PER_SESSION> photoPaths = {};
  std::array<std::string, PHOTOS_PER_SESSION> collagePaths = {};
};

// helper to tell unique_ptr how to free a Camera
struct CameraDeleter {
  void operator()(Camera *p) const {
    // We only free if p is not null
    if (p) {
      gp_camera_free(p);
    }
  }
};

// helper to tell unique_ptr how to free a Context
struct ContextDeleter {
  void operator()(GPContext *p) const {
    if (p)
      gp_context_unref(p);
  }
};

struct FileDeleter {
  void operator()(CameraFile *p) const {
    if (p)
      gp_file_free(p); // Free the file memory
  }
};

class CameraClient {
private:
  std::unique_ptr<GPContext, ContextDeleter> m_context;
  std::unique_ptr<Camera, CameraDeleter> m_camera;
  int m_photo_count;
  bool m_connected;

public:
  CameraClient();
  ~CameraClient();
  SessionInfo session;
  std::string m_save_path;

  /**
   * @brief Create or Set File path
   * @param string of the fodler or existing folder
   * @return bool of success/fail
   */
  bool connect();
  bool isConnected() const { return m_connected; }
  bool setupFolder(std::string folderName);

  bool createSession();
  bool endSession();

  bool capturePhoto();
  bool creatCollageList();

  bool printSelectedPhoto(const std::string &filePath);

  std::vector<std::string> getPhotoList();
};