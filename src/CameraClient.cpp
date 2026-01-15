#include "CameraClient.h"

void kill_auto_mount()
{
    // This simply asks the shell to kill the volume monitor process
    // It prevents the OS from "grabbing" the camera
    system("pkill -f gphoto");
    system("sleep 1");
}

CameraClient::CameraClient()
{
    Camera *raw_cam = nullptr;
    gp_camera_new(&raw_cam);
    m_camera.reset(raw_cam);

    
    GPContext *raw_context = gp_context_new();
    m_context.reset(raw_context);

    m_photo_count = 0;
    m_save_path = ".";
}

CameraClient::~CameraClient()
{
    if (m_camera && m_context)
    {
        gp_camera_exit(m_camera.get(), m_context.get());
    }
}

void CameraClient::setupFolder(std::string folderName)
    {
        m_save_path = folderName;
        std::filesystem::create_directories(m_save_path);
        std::cout << "Saving photos to :" << m_save_path << "\n";

    }


bool  CameraClient::connect()
    {
        std::cout << "Connection Attempt ... " << std::endl;
        int ret = gp_camera_init(m_camera.get(), m_context.get());
        if (ret < GP_OK)
        {
            std::cout << "Connection Error: " << ret << std::endl;
            return false;
        }
        std::cout << "Connected Successfully!" << std::endl;
        return true;
    }

std::string CameraClient::capturePhoto(){
    std::cout << "Capturing image" << std::endl;

        CameraFilePath camera_file_path;
        // pointer to camera, Opt , Filepath, pointer to context
        int ret = gp_camera_capture(m_camera.get(), GP_CAPTURE_IMAGE, &camera_file_path, m_context.get());




        if (ret < GP_OK)
        {
            std::cout << "Camera Capture Fail Error: " << ret << std::endl;
            return "Error";
        }
        else
        {
            std::cout << "Image capture on camera: " << camera_file_path.folder << "/" << camera_file_path.name << std::endl;

            // 5.Download
            std::cout << "Downloading Image ..." << std::endl;

            // Init Camera File Raw pointer
            CameraFile *raw_file = nullptr;
            gp_file_new(&raw_file);

            std::unique_ptr<CameraFile, FileDeleter> file(raw_file);

            ret = gp_camera_file_get(m_camera.get(), camera_file_path.folder,
                                     camera_file_path.name, GP_FILE_TYPE_NORMAL, file.get(), m_context.get());

            if (ret < GP_OK)
            {
                std::cout << "Could NOT download image" << std::endl;
                return "false";
            }
            else
            {
                // 6. save
                
                std::string full_path = m_save_path + "/img_" + std::to_string(m_photo_count) + ".jpg";

                gp_file_save(file.get(), full_path.c_str());
                std::cout << "Success! saved to " << full_path << std::endl;
                
                m_photo_count++;
                return full_path;
            }
        }
}

std::vector<std::string> CameraClient::getPhotoList(){
    std::vector<std::string> list;
    return list;
}