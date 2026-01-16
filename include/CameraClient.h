#pragma once
#include <iostream>
#include <gphoto2/gphoto2.h>
#include <string>
#include <memory>
#include <vector>
#include <cstdlib>
#include <filesystem>



// helper to tell unique_ptr how to free a Camera
struct CameraDeleter
{
    void operator()(Camera *p) const
    {
        // We only free if p is not null
        if (p)
        {
            gp_camera_free(p);
        }
    }
};

// helper to tell unique_ptr how to free a Context
struct ContextDeleter
{
    void operator()(GPContext *p) const
    {
        if (p)
            gp_context_unref(p);
    }
};

struct FileDeleter
{
    void operator()(CameraFile *p) const
    {
        if (p)
            gp_file_free(p); // Free the file memory
    }
};


class CameraClient {
private:
    std::unique_ptr<GPContext, ContextDeleter> m_context;
    std::unique_ptr<Camera, CameraDeleter> m_camera;
    std::string m_save_path;
    int m_photo_count;
    bool m_connected;

public:
    CameraClient();
    ~CameraClient();

   /**
    * @brief Create or Set File path
    * @param string of the fodler or existing folder
    * @return bool of success/fail
    */
    bool setupFolder(std::string folderName);
    
    // Connection
    bool connect();
    bool isConnected() const { return m_connected; }
    
    // Actions
    // Returns the path of the saved file, or empty string on failure
    std::string capturePhoto(); 
    
    // Future proofing: Ability to list all photos in the session
    std::vector<std::string> getPhotoList();
};