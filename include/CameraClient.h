#pragma once
#include "SessionInfo.h"
#include "UniqueCamPtr.h"


class CameraClient {
private:
  std::unique_ptr<GPContext, ContextDeleter> m_context;
  std::unique_ptr<Camera, CameraDeleter> m_camera;
  int m_photo_count = 0;
  bool m_connected = false;

public:
  CameraClient();
  ~CameraClient();
  SessionInfo session;
  std::string m_save_path = DEFUALT_PHOTO_LOCATION;

  /**
   * @brief Create or Set File path
   * @param string of the fodler or existing folder
   * @return bool of success/fail
   */
  bool connect();
  bool isConnected() const { return m_connected; }
  bool setupFolder(std::string folderName);

  bool createSession(std::string sessionImageEffect);
  bool endSession();

  bool capturePhoto();

  bool printSelectedPhoto(const std::string &filePath);

  const std::vector<std::string> &getPhotoPaths();
  const std::vector<std::string> &getCollagePaths();
  const std::vector<PhotoURLs> &getUrlData();

};