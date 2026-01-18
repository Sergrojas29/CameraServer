#include "CameraClient.h"
#include <chrono>
#include <stdexcept>
#include <string>

void kill_auto_mount() {
  // This simply asks the shell to kill the volume monitor process
  // It prevents the OS from "grabbing" the camera
  system("pkill -f gphoto");
  system("pkill -f gvfs-gphoto2");
  system("sleep 1");
}

std::string setSessionId() {
  const auto now = std::chrono::system_clock::now();
  // Format into an ISO 8601 string (e.g., 2024-06-15T13:45:30Z)
  const std::string timestamp_str = std::format("{}", now);
  return timestamp_str;
}

void resetArray(std::array<std::string, 4> &array_of_4) {
  std::fill(array_of_4.begin(), array_of_4.end(), std::string(""));
}

CameraClient::CameraClient() : m_context(gp_context_new()) {
  Camera *raw_cam = nullptr;
  gp_camera_new(&raw_cam);
  m_camera.reset(raw_cam);

  m_photo_count = 0;
  m_save_path = ".";
  m_save_path = "serverPhotos";
  m_connected = false;

  //! debugging __-----------------------------////
  session.activeSession = true;
  m_save_path = "test1";
  session.photoPaths = {"test1/img_0.jpg", "test1/img_1.jpg", "test1/img_2.jpg",
                        "test1/img_3.jpg"};
  session.collagePaths = {"collages/Overlay-FlamaLamaimg_0.png",
                          "collages/Overlay-FlamaLamaimg_1.png",
                          "collages/Overlay-FlamaLamaimg_2.png",
                          "collages/Overlay-FlamaLamaimg_3.png"};
}

CameraClient::~CameraClient() {
  if (m_camera && m_context) {
    gp_camera_exit(m_camera.get(), m_context.get());
  }
}

// SetupFolder
bool CameraClient::setupFolder(std::string folderName) {
  // see if it already folder already exists
  if (std::filesystem::exists(folderName)) {
    m_save_path = folderName;
    int count = 0;
    for (const auto &entry : std::filesystem::directory_iterator(folderName)) {
      if (entry.is_regular_file())
        count++;
    }

    m_photo_count = count;
    return true;
  } else {
    std::filesystem::create_directories(folderName);
    m_save_path = folderName;
    std::cout << "Saving photos to :" << m_save_path << "\n";
    return true;
  }
  return false;
}

bool CameraClient::connect() {
  if (isConnected()) {
    // Connected Previously
    return true;
  } else {
    kill_auto_mount();
    std::cout << "Connection Attempt ... " << std::endl;
    int ret = gp_camera_init(m_camera.get(), m_context.get());
    if (ret < GP_OK) {
      std::cout << "Connection Error: " << ret << std::endl;
      return false;
    }
    std::cout << "Connected Successfully!" << std::endl;
    return true;
  }
}

bool CameraClient::capturePhoto() {
  try {
    if (session.activeSession == false)
      throw std::runtime_error("No Active Session - Can't Allocate photo");
    //! add check to not add too many photos
    if (session.sessionPhotoCount > (PHOTOS_PER_SESSION - 1))
      throw std::runtime_error("Too many photos take in Session");

    CameraFilePath camera_file_path;
    // pointer to camera, Opt , Filepath, pointer to context
    int ret = gp_camera_capture(m_camera.get(), GP_CAPTURE_IMAGE,
                                &camera_file_path, m_context.get());

    if (ret < GP_OK) {
      std::string err = "Camera Capture Fail Error: " + std::to_string(ret);
      throw std::runtime_error(err);
    }

    std::cout << "Image capture on camera: " << camera_file_path.name
              << std::endl;

    // 5.Download
    std::cout << "Downloading Image ..." << std::endl;

    // Init Camera File Raw pointer
    CameraFile *raw_file = nullptr;
    gp_file_new(&raw_file);
    std::unique_ptr<CameraFile, FileDeleter> file(raw_file);

    ret = gp_camera_file_get(m_camera.get(), camera_file_path.folder,
                             camera_file_path.name, GP_FILE_TYPE_NORMAL,
                             file.get(), m_context.get());

    if (ret < GP_OK)
      throw std::runtime_error("Could NOT download image");

    // 6. save
    std::string full_path =
        m_save_path + "/img_" + std::to_string(m_photo_count) + ".jpg";

    gp_file_save(file.get(), full_path.c_str());
    std::cout << "Success! saved to " << full_path << std::endl;
    //?is this neccesary?
    m_photo_count++;

    // updateSession

    session.photoPaths[session.sessionPhotoCount] = full_path;
    session.sessionPhotoCount++;

    return true;

  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return false;
  }
}

std::vector<std::string> CameraClient::getPhotoList() {
  std::vector<std::string> file_list;
  try {
    for (const auto &entry : std::filesystem::directory_iterator(m_save_path)) {
      if (std::filesystem::is_regular_file(entry.status())) {
        file_list.push_back(entry.path().filename().string());
      }
    }
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Filesystem error: " << e.what() << std::endl;
  }
  return file_list;
}

bool CameraClient::creatCollageList() {
  try {

    return true;
  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return false;
  }
};

bool CameraClient::createSession() {
  try {
    if (session.activeSession)
      throw std::runtime_error("Session Already Active");
    session.activeSession = true;
    session.sessionID = setSessionId();
    session.sessionCount++;
    session.sessionPhotoCount = 0;

    return true;
  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return false;
  }
}

bool CameraClient::endSession() {
  session.activeSession = false;
  session.sessionPhotoCount = 0;

  resetArray(session.photoPaths);
  resetArray(session.collagePaths);

  return true;
}


bool CameraClient::printSelectedPhoto(const std::string &filePath) {
    cups_dest_t *dests = nullptr;
    cups_option_t *options = nullptr;
    int num_dests = 0;
    int num_options = 0;
    bool success = false;

    try {
      const char *filename = filePath.c_str();
      const char *job_title = "CollagePrint from booth";

      // 1. Get Printers
      num_dests = cupsGetDests(&dests);
      if (num_dests == 0 || dests == nullptr) {
        throw std::runtime_error("No printers found on this system.");
      }

      // 2. Identify Target Printer
      cups_dest_t *dest = cupsGetDest(nullptr, nullptr, num_dests, dests);
      if (!dest) {
        dest = &dests[0];
        std::cout << "No default printer found. Using: " << dest->name
                  << std::endl;
      } else {
        std::cout << "Target Printer: " << dest->name << std::endl;
      }

      // 3. Set Options
      num_options =
          cupsAddOption("media", "na_index-4x6_4x6in", num_options, &options);
      num_options = cupsAddOption("MediaType", "Glossy", num_options, &options);

      // 4. Print File
      std::cout << "Sending job to printer..." << std::endl;
      int job_id =
          cupsPrintFile(dest->name, filename, job_title, num_options, options);

      if (job_id == 0) {
        throw std::runtime_error(std::string("Print failed: ") +
                                 cupsLastErrorString());
      }

      std::cout << "Print job submitted successfully (ID: " << job_id << ")"
                << std::endl;
      success = true;

    } catch (const std::exception &e) {
      std::cerr << "Printing Error: " << e.what() << std::endl;
      success = false;
    }

    // 5. Cleanup (Always runs exactly once)
    if (dests) {
      cupsFreeDests(num_dests, dests);
    }
    if (options) {
      cupsFreeOptions(num_options, options);
    }

    return success;
  }