#include "CameraClient.h"
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <string>

// FUNC Help Functions
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

// TODO Delete Function - Might not be used
void resetArray(std::array<std::string, 4> &array_of_4) {
  std::fill(array_of_4.begin(), array_of_4.end(), std::string(""));
}

// FUNC PUBLIC MEMBER
CameraClient::CameraClient() : m_context(gp_context_new()) {
  Camera *raw_cam = nullptr;
  gp_camera_new(&raw_cam);
  m_camera.reset(raw_cam);

  // Create Inital folder
  std::filesystem::create_directories(DEFUALT_PHOTO_LOCATION);
  std::filesystem::create_directories(DEFUALT_COLLAGE_LOCATION);

  m_photo_count = 0;
  m_save_path = DEFUALT_PHOTO_LOCATION;
  m_connected = false;

  // Allocate Vector Size
  session.collagePaths.reserve(6);
  session.photoPaths.reserve(6);
}

CameraClient::~CameraClient() {
  if (m_camera && m_context) {
    gp_camera_exit(m_camera.get(), m_context.get());
  }
}

// SetupFolder
bool CameraClient::setupFolder(std::string folderName) {

  std::filesystem::create_directories(folderName);
  session.save_path = folderName;

  // count any exisiting files
  m_photo_count =
      std::count_if(std::filesystem::directory_iterator(m_save_path),
                    std::filesystem::directory_iterator{},
                    [](const auto &entry) { return entry.is_regular_file(); });

  std::cout << "Saving Collage photos to :" <<  session.save_path << "\n";
  return true;
}

bool CameraClient::connect() {
  if (isConnected()) {
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
    m_connected = true;
    return true;
  }
}

bool CameraClient::capturePhoto() {
  try {
    if (session.activeSession == false)
      throw std::runtime_error("No Active Session - Can't Allocate photo");

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

    // TODO maybe Add a UUID or TIME name
    // 6. save
    std::string full_path =
        m_save_path + "/img_" + std::to_string(m_photo_count) + ".jpg";

    gp_file_save(file.get(), full_path.c_str());
    std::cout << "Success! saved to " << full_path << std::endl;
    m_photo_count++;

    // updateSession
    std::cout << "SessionPhotoCount: " << session.sessionPhotoCount
              << std::endl;
    session.photoPaths.push_back(full_path);
    session.sessionPhotoCount++;

    return true;

  } catch (const std::exception &e) {
    std::cerr << "Exception caught: " << e.what() << std::endl;
    return false;
  }
}


bool CameraClient::creatCollageList() {
  try {
    // TODO Add func to create in parallel in the Collage Create section
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
  session.CollageTemplate = "";

  session.photoPaths.clear();
  session.collagePaths.clear();

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

const std::vector<std::string> &CameraClient::getPhotoPaths() {
  return session.photoPaths;
};
const std::vector<std::string> &CameraClient::getCollagePaths() {
  return session.collagePaths;
};