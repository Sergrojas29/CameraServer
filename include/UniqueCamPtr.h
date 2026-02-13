#pragma once
#include <gphoto2/gphoto2.h>
#include "nlohmann/json.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <cpr/cpr.h>
#include <cstddef>
#include <cstdlib>
#include <cups/cups.h>
#include <execution>
#include <filesystem>
#include <format>
#include <future>
#include <iostream>
#include <memory>
#include <regex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

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