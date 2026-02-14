# CamaraClient
For real for real serve to photobooth



# Install

## Overview
This project is built using C++23 and CMake (3.24+). It relies on a combination of system-level packages (installed via the Linux package manager) and source-level dependencies (automatically fetched by CMake during the build process). 

The instructions below are tailored for Debian/Ubuntu-based Linux distributions.

---

# Install

## Overview
This project is built using C++23 and CMake (3.24+). It relies on a combination of system-level packages (installed via the Linux package manager) and source-level dependencies (automatically fetched by CMake during the build process). 

The instructions below are tailored for Debian/Ubuntu-based Linux distributions.

---

## 1. System Requirements & Build Tools
Before building, ensure you have a modern C++ compiler that supports **C++23** and a recent version of CMake. You will also need `git` so CMake can fetch external repositories.

`sudo apt update`
`sudo apt install build-essential cmake git pkg-config`

## 2. System Dependencies
The following libraries must be installed on your system. CMake will look for these using `find_package` and `pkg_check_modules`.

| Dependency | Package Name | Purpose |
| :--- | :--- | :--- |
| **libgphoto2** | `libgphoto2-dev` | Core camera communication and capture control. |
| **CUPS** | `libcups2-dev` | Print server integration for photo printing. |
| **OpenCV** | `libopencv-dev` | Image decoding, processing, and manipulation. |
| **Intel TBB** | `libtbb-dev` | Multi-threading backend (often required by OpenCV). |
| **cURL** | `libcurl4-openssl-dev` | HTTP client backend (required by CPR). |
| **OpenMP** | `libomp-dev` | Multi-processing API for parallel execution. |

**Installation Command:**
`sudo apt install libgphoto2-dev libcups2-dev libopencv-dev libtbb-dev libcurl4-openssl-dev libomp-dev`

---

## 3. Auto-Fetched Dependencies (CMake)
You **do not** need to manually install the following packages. They are automatically downloaded, built, and linked by CMake's `FetchContent` module when you configure the project:

* **Crow:** C++ Microframework for the web server/routing.
* **Nlohmann JSON:** JSON serialization/deserialization.
* **libcpr (CPR):** C++ Requests library for HTTP calls. *(Note: Configured to use the system cURL installed in step 2 to speed up build times).*
* **GoogleTest:** C++ testing framework.

---

## 4. Build Instructions
Once all system dependencies are installed, you can build the `CameraServer` executable and the testing suite.

# 1. Create a build directory
`mkdir build`
`cd build`

# 2. Configure the project
`cmake ..`

# 3. Compile the project (using multiple cores to speed it up)
`make -j$(nproc)`





# Image Filter Naming Convention

## Overview
This document outlines the standard naming convention for image filters used within the CollageClass system. This format must be strictly used for both **file names** (e.g., template files, overlay assets) and **map strings** (keys in the `createEffectMap` dictionary).

## Format Structure
* **Capitalization:** ALL CAPS
* **Separator:** Underscore (`_`)

**Syntax:**
`[ORIENTATION]_[#]_[USE_CASE]_[CATEGORY]_[TITLE]`

---

## Component Breakdown

| Position | Component | Description | Allowed Values & Aspect Ratios |
| :--- | :--- | :--- | :--- |
| **1** | **Orientation** | The layout and aspect ratio of the final output. | `SQR11` (Square 1:1)<br>`PRT45` (IG Portrait 4:5)<br>`STY916` (Story/Vertical 9:16)<br>`LND169` (Widescreen 16:9)<br>`PRT` (Standard Portrait)<br>`LND` (Standard Landscape) |
| **2** | **Photo Count** | The number of photos involved in the collage/filter. | `1`, `2`, `4`, etc. |
| **3** | **Use Case** | The general applicability of the filter. | `GEN` (General)<br>`SPE` (Special) |
| **4** | **Category** | The thematic grouping of the filter. | `PARTY`, `EVENT`, `WEDD`, etc. |
| **5** | **Title** | A short, descriptive name for the specific effect. | `VINTAGE`, `GLOW`, `BW_CONTRAST` |

---

## Examples

**Map String Examples:**
* `"STY916_1_GEN_PARTY_VINTAGE"` (1 photo, 9:16 Story format, general party use, vintage effect)
* `"SQR11_4_SPE_WEDD_SOFTGLOW"` (4 photos, 1:1 Square grid, special wedding use, soft glow effect)
* `"LND169_2_GEN_EVENT_BW"` (2 photos, 16:9 Landscape, general event use, black and white)

**File Name Examples:**
* `STY916_1_GEN_PARTY_VINTAGE.cpp`
* `SQR11_4_SPE_WEDD_SOFTGLOW.json`