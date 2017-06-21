/*
 * SOFTWARE LICENSE
 * BY USING YOUR ZED CAMERA YOU AGREE TO THIS SOFTWARE LICENSE. BEFORE SETTING IT UP,
 * PLEASE READ THIS SOFTWARE LICENSE CAREFULLY. IF YOU DO NOT ACCEPT THIS
 * SOFTWARE LICENSE, DO NOT USE YOUR ZED CAMERA. RETURN IT TO UNUSED TO
 * STEREOLABS FOR A REFUND. Contact STEREOLABS at contact@stereolabs.com
 *
 * 1. Definitions
 *
 * "Authorized Accessory" means a STEREOLABS branded ZED, and a STEREOLABS
 * licensed, third party branded, ZED hardware accessory whose packaging
 * bears the official "Licensed for ZED" logo. The ZED Camera is an Authorized
 *  Accessory solely for purpose of this Software license.
 * "Software" means the Software Development Kit, pre-installed in the ZED
 * USB flash drive included in the ZED packaging, and including any
 * updates STEREOLABS may make available from time to time.
 * "Unauthorized Accessories" means all hardware accessories other than
 * an Authorized Accessory.
 * "Unauthorized Software" means any software not distributed by STEREOLABS.
 * "You" means the user of a ZED Camera.
 *
 * 2. License
 *
 * a. The Software is licensed to You, not sold. You are licensed to use the
 * Software only as pre-installed in Your ZED USB flash drive, and updated by
 * STEREOLABS from time to time. You may not copy or reverse engineer the Software.
 *
 * b. As conditions to this Software license, You agree that:
 *       i. You will use Your Software with ZED Camera only and not with any
 * other device (including). You will not use Unauthorized Accessories.
 * They may not work or may stop working permanently after a Software update.
 *       ii. You will not use or install any Unauthorized Software.
 * If You do, Your ZED Camera may stop working permanently at that time
 * or after a later Software update.
 *       iii. You will not attempt to defeat or circumvent any Software
 * technical limitation, security, or anti-piracy system. If You do,
 * Your ZED Camera may stop working permanently at that time or after a
 * later Software update.
 *       iv. STEREOLABS may use technical measures, including Software
 * updates, to limit use of the Software to the ZED Camera, to prevent
 * use of Unauthorized Accessories, and to protect the technical limitations,
 * security and anti-piracy systems in the ZED Camera.
 *       v. STEREOLABS may update the Software from time to time without
 * further notice to You, for example, to update any technical limitation,
 * security, or anti-piracy system.
 *
 * 3. Warranty
 * The Software is covered by the Limited Warranty for Your ZED Camera,
 * and STEREOLABS gives no other guarantee, warranty, or condition for
 * the Software. No one else may give any guarantee, warranty, or condition
 * on STEREOLABS's behalf.
 *
 * 4. EXCLUSION OF CERTAIN DAMAGES
 * STEREOLABS IS NOT RESPONSIBLE FOR ANY INDIRECT, INCIDENTAL, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES; ANY LOSS OF DATA, PRIVACY, CONFIDENTIALITY, OR
 * PROFITS; OR ANY INABILITY TO USE THE SOFTWARE. THESE EXCLUSIONS APPLY
 * EVEN IF STEREOLABS HAS BEEN ADVISED OF THE POSSIBILITY OF THESE DAMAGES,
 * AND EVEN IF ANY REMEDY FAILS OF ITS ESSENTIAL PURPOSE.
 *
 * 5. Choice of Law
 * French law governs the interpretation of this Software license and any
 * claim that STEREOLABS has breached it, regardless of conflict of
 * law principles.
 *
 */


#ifndef __GLOBAL_DEFINE__
#define __GLOBAL_DEFINE__

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <ctype.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <memory.h>
#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#else   /* _WIN32 */
#include <unistd.h>
#include <limits>
#endif  /* _WIN32 */

#define TOO_FAR INFINITY
#define TOO_CLOSE -INFINITY
#define OCCLUSION_VALUE NAN
//macro to detect wrong data measure
#define isValidMeasure(v) (std::isfinite(v))

const int ZED_SDK_MAJOR_VERSION = 1;
const int ZED_SDK_MINOR_VERSION = 2;
const int ZED_SDK_PATCH_VERSION = 0;

#define SL_ZED_SHARED_LIBS

#ifdef WIN32

#ifdef SL_ZED_SHARED_LIBS
#if defined sl_zed64_EXPORTS
#define SLSTEREO_EXPORT_DLL __declspec(dllexport)
#else
#define SLSTEREO_EXPORT_DLL __declspec(dllimport)
#endif
#else
#define SLSTEREO_EXPORT_DLL
#endif

#else
#define SLSTEREO_EXPORT_DLL

#if defined(__arm__) || defined(__aarch64__)
#define _SL_JETSON_
#endif


#endif



namespace sl {
    typedef unsigned char uchar;

    //structure defined to handle a vector of uchar (3 components)

    typedef struct uchar3Struct {
        uchar c1;
        uchar c2;
        uchar c3;

        uchar3Struct(uchar c1_ = 0, uchar c2_ = 0, uchar c3_ = 0) {
            c1 = c1_;
            c2 = c2_;
            c3 = c3_;
        }

        inline void setValue(uchar c1_, uchar c2_ = 0, uchar c3_ = 0) {
            c1 = c1_;
            c2 = c2_;
            c3 = c3_;
        }

    } uchar3;

    //structure defined to handle a vector of float (3 components)

    typedef struct float3Struct {
        float f1;
        float f2;
        float f3;

        float3Struct(float f1_ = 0, float f2_ = 0, float f3_ = 0) {
            f1 = f1_;
            f2 = f2_;
            f3 = f3_;
        }

    } float3;


    //structure defined to handle a vector of double (3 components)

    typedef struct double3Struct {
        double d1;
        double d2;
        double d3;

        double3Struct(float d1_ = 0, float d2_ = 0, float d3_ = 0) {
            d1 = d1_;
            d2 = d2_;
            d3 = d3_;
        }

    } double3;

    /**
     *  \enum DEPTH_FORMAT
     *  \ingroup Enumerations
     *  \brief Enumerate the file format for depth saving
     */
    typedef enum {
        PNG, /*!< image format, in 16bits, the color encoding the depth   \ingroup Enumerations*/
        PFM, /*!< stream of bytes, graphic image file format \ingroup Enumerations*/
        PGM, /*!< grayscale image format \ingroup Enumerations*/
        LAST_DEPTH_FORMAT
    } DEPTH_FORMAT;

    /**
     *  \enum POINT_CLOUD_FORMAT
     *  \ingroup Enumerations
     *  \brief Enumerate the file format avaliable for saving point clouds (depth triangulation)
     */
    typedef enum {
        XYZ, /*!< 3D format, store the 3D coordinates \ingroup Enumerations*/
        PCD, /*!< Point Cloud Data file, store the 3D coordinates and the color \ingroup Enumerations*/
        PLY, /*!< PoLYgon file format, store the 3D coordinates and the color \ingroup Enumerations*/
        VTK, /*!< Visualization ToolKit file, store the 3D coordinates \ingroup Enumerations*/
        LAST_CLOUD_FORMAT
    } POINT_CLOUD_FORMAT;

    namespace zed {

        /*! \defgroup Enumerations Public enumeration types */

        /**
         *  \enum MODE
         *  \ingroup Enumerations
         *  \brief Enumerate for the pre-defined  disparity map computation parameters
         */

        typedef enum {
            NONE, /*!< Disparity map not computed, only the rectified images will be available. \ingroup Enumerations*/
            PERFORMANCE, /*!< Fastest mode, also requires less GPU memory, the disparity map is less robust \ingroup Enumerations*/
            MEDIUM, /*!< Balanced quality mode, requires less GPU memory but the disparity map is a little less detailed \ingroup Enumerations*/
            QUALITY, /*!< Better quality mode, the disparity map is more precise \ingroup Enumerations*/
            LAST_MODE
        } MODE;

        /**
         *  \enum VIEW_MODE
         *  \ingroup Enumerations
         *  \brief Enumerate for available output views for monitoring
         */
        typedef enum {
            STEREO_ANAGLYPH, /*!< Anaglyph (Red & Blue) view \ingroup Enumerations*/
            STEREO_DIFF, /*!< View of the difference between the left image and right image in gray scale \ingroup Enumerations*/
            STEREO_SBS, /*!< Side by Side view (in single image resolution) \ingroup Enumerations*/
            STEREO_OVERLAY, /*!< View of both images in 5O% transparency \ingroup Enumerations*/
            LAST_VIEW_MODE
        } VIEW_MODE;

        /**
         *  \enum SIDE
         *  \ingroup Enumerations
         *  \brief Enumerate for the Left and Right side of stereo Camera
         */
        typedef enum {
            LEFT,
            RIGHT,
            LEFT_GREY,
            RIGHT_GREY,
            LEFT_UNRECTIFIED,
            RIGHT_UNRECTIFIED,
            LEFT_UNRECTIFIED_GREY,
            RIGHT_UNRECTIFIED_GREY,
            LAST_SIDE
        } SIDE;

        /**
         *  \enum SENSING_MODE
         *  \ingroup Enumerations
         *  \brief Enumerate for the different types of disparity map computation
         */
        typedef enum {
            FILL, /*!< Occlusion filling, edge sharpening, advanced post-filtering.
            * Application example : Refocusing, Multi-view generation \ingroup Enumerations*/
            STANDARD, /*!< Structure conservative, no occlusion filling.
            * Application example : Obstacle detection, 3D reconstructions, 3D measures \ingroup Enumerations*/
            LAST_SENSING_MODE
        } SENSING_MODE;

        /**
         *  \enum MEASURE
         *  \ingroup Enumerations
         *  \brief Enumerate for the retrievable measure (each measure should be
         * normalized to be displayed)
         */
        typedef enum {
            DISPARITY, /*!< Disparity map,  1 channel, FLOAT \ingroup Enumerations*/
            DEPTH, /*!< Depth map,  1 channel, FLOAT \ingroup Enumerations*/
            CONFIDENCE, /*!< Certainty/confidence of the diparity map,  1 channel, FLOAT \ingroup Enumerations*/
            XYZ, /*!< 3D coordinates of the image points, 4 channels, FLOAT  (the 4th channel may contains the colors)\ingroup Enumerations*/
            XYZRGBA, /*!< 3D coordinates and Color of the image , 4 channels, FLOAT (the 4th channel encode 4 UCHAR for color in R-G-B-A order) \ingroup Enumerations*/
            XYZBGRA, /*!< 3D coordinates and Color of the image , 4 channels, FLOAT (the 4th channel encode 4 UCHAR for color in B-G-R-A order) \ingroup Enumerations*/
            XYZARGB, /*!< 3D coordinates and Color of the image , 4 channels, FLOAT (the 4th channel encode 4 UCHAR for color in A-R-G-B order) \ingroup Enumerations*/
            XYZABGR, /*!< 3D coordinates and Color of the image , 4 channels, FLOAT (the 4th channel encode 4 UCHAR for color in A-B-G-R order) \ingroup Enumerations*/
            LAST_MEASURE
        } MEASURE;

        /**
         *  \enum ERRCODE
         *  \ingroup Enumerations
         *  \brief Enumerate for error code of the ZED SDK
         */
        typedef enum {
            SUCCESS, /*!< Every step went fine, the camera is ready to use \ingroup Enumerations*/
            NO_GPU_COMPATIBLE, /*!< No GPU found or the CUDA capability of the device
            * is not sufficient or supported \ingroup Enumerations*/
            NOT_ENOUGH_GPUMEM, /*!< not enough GPU memory for this depth calculation mode
            * please try a "lighter" mode (PERFORMANCE...) \ingroup Enumerations*/
            ZED_NOT_AVAILABLE, /*!< The ZED camera is not plugged or detected \ingroup Enumerations*/
            ZED_RESOLUTION_INVALID, /*!< For Jetson only, resolution not yet supported (USB3.0 bandwidth)> \ingroup Enumerations*/
            ZED_SETTINGS_FILE_NOT_AVAILABLE, /*!< ZED Settings file is not found on the host machine. Use ZED Settings App tool to correct it. \ingroup Enumerations*/
            INVALID_SVO_FILE, /*!< The provided SVO file is not valid \ingroup Enumerations*/
            RECORDER_ERROR, /*!< An recorder related error occured (not enough free storage, invalid file) \ingroup Enumerations*/
            INVALID_COORDINATE_SYSTEM, /*!< The requested coordinate systeme is not available.*/
            ZED_WRONG_FIRMWARE, /*!< The firmware of the ZED is out of date. You might install the new version.*/
            NO_NEW_FRAME, /*< in grab() only, the current call return the same frame as last call. Not a new frame.*/
            CUDA_ERROR_THROWN, /*< in grab() only, a CUDA error has been detected in the process. Activate verbose in Init() for more info.*/
            ZED_NOT_INITIALIZED, /*< in grab() only, ZED SDK is not initialized. Probably a missing call to Init().*/
            OUT_OF_DATE_NVIDIA_DRIVER, /*<your NVIDIA driver is too old and no more compatible with your cuda version */
            INVALID_FUNCTION_CALL, /*<the call of the function is not valid in the current context. Could be a missing call of Init(...) */
            LAST_ERRCODE
        } ERRCODE;

        /**
         *  \enum ZED_SELF_CALIBRATION_STATUS
         *  \ingroup Enumerations
         *  \brief Status for self calibration. Since v0.9.3, self-calibration is done in background and start in the Init or Reset function.
         *  \brief You can follow the current status for the self-calibration any time once ZED object has been construct.
         *
         *
         */
        typedef enum {
            SELF_CALIBRATION_NOT_CALLED, /*!< Self Calibration has not yet been called (no Camera::init or Camera::resetSelfCalibration called) \ingroup Enumerations*/
            SELF_CALIBRATION_RUNNING, /*!< Self Calibration is currently running. \ingroup Enumerations*/
            SELF_CALIBRATION_FAILED, /*!< Self Calibration has finished running but did not manage to get coherent values. Old Parameters are taken instead. \ingroup Enumerations*/
            SELF_CALIBRATION_SUCCESS /*!< Self Calibration has finished running and did manage to get coherent values. New Parameters are taken. \ingroup Enumerations*/
        } ZED_SELF_CALIBRATION_STATUS;

        /**
         *  \enum ZEDResolution_mode
         *  \ingroup Enumerations
         *  \brief Enumerate for available resolutions for ZED camera
         *
         * \warning Since v1.0 the VGA mode is wider (from 640*480 to 672*376) and requires a firmware update to operate (>= 1142). It can be done using the ZED Explorer tool
         */
        typedef enum {
            HD2K, /*!< 2208*1242, supported framerate : 15 fps (unsupported by the Jetson TK1 at the moment) \ingroup Enumerations*/
            HD1080, /*!< 1920*1080, supported framerates : 15, 30 fps (unsupported by the Jetson TK1 at the moment) \ingroup Enumerations*/
            HD720, /*!< 1280*720, supported framerates : 15, 30, 60 fps, Jetson TK1 : 15 fps \ingroup Enumerations*/
            VGA, /*!< 672*376, supported framerates : 15, 30, 60, 100 fps, Jetson TK1 : 15, 30 fps \ingroup Enumerations*/
            LAST_RESOLUTION
        } ZEDResolution_mode;

        /**
         *  \enum ZEDCamera_settings
         *  \ingroup Enumerations
         *  \brief Enumerate the available camera settings for the ZED camera (contrast, hue, saturation, gain...)
         *  \brief Each enum defines one of those settings
         */
        typedef enum {
            ZED_BRIGHTNESS, /*!< Defines the brightness control. Affected value should be between 0 and 8   \ingroup Enumerations*/
            ZED_CONTRAST, /*!< Defines the constral control. Affected value should be between 0 and 8   \ingroup Enumerations*/
            ZED_HUE, /*!< Defines the hue control. Affected value should be between 0 and 11   \ingroup Enumerations*/
            ZED_SATURATION, /*!< Defines the saturation control. Affected value should be between 0 and 8   \ingroup Enumerations*/
            ZED_GAIN, /*!< Defines the gain control. Affected value should be between 0 and 100 for manual control. If ZED_EXPOSURE is set to -1, the gain is in auto mode too.   \ingroup Enumerations*/
            ZED_EXPOSURE, /*!< Defines the exposure control. A -1 value enable the AutoExposure/AutoGain control. Affected value should be between 0 and 100 for manual control. A 0 value only disable auto mode without modifing the last auto values, while a 1 to 100 value disable auto mode and set exposure to chosen value \ingroup Enumerations*/
            LAST_SETTINGS
        } ZEDCamera_settings;

        /**
         *  \enum TRACKING_STATE
         *  \ingroup Enumerations
         *  \brief Enumerate the current state of the tracking
         */
        typedef enum {
            TRACKING_RELOCDB_INIT, /*!< has loaded an area database and is looking for a match  \ingroup Enumerations*/
            TRACKING_INIT, /*!< relocalization initialization phase \ingroup Enumerations*/
            TRACKING_RELOC, /*!< The tracking is lost and try to relocate at a previously known position \ingroup Enumerations*/
            TRACKING_LOST, /*!< The tracking failed to relocate and is lost, a reset tracking should be performed \ingroup Enumerations*/
            TRACKING_GOOD, /*!< The tracking operates normally, the path should be correct \ingroup Enumerations*/
            TRACKING_OFF, /*!< The tracking is not enabled \ingroup Enumerations*/
            TRACKING_LAST
        } TRACKING_STATE;

        /**
         *  \enum TRACKING_FRAME_STATE
         *  \ingroup Enumerations
         *  \brief Enumerate the current state of the tracked frame
         */
        typedef enum {
            TRACKING_FRAME_NORMAL, /*!< Not a keyframe, normal behavior \ingroup Enumerations*/
            TRACKING_FRAME_KEYFRAME, /*!< The tracking detect a new reference image \ingroup Enumerations*/
            TRACKING_FRAME_CLOSE, /*!< The tracking find a previously known area and optimize the trajectory\ingroup Enumerations*/
            TRACKING_FRAME_LAST
        } TRACKING_FRAME_STATE;

        /**
         *  \enum MAT_TRACKING_TYPE
         *  \ingroup Enumerations
         *  \brief Define which type of position matrix is used to store the tracking position
         */
        typedef enum {
            PATH, /*!< The matrix contains the displacement from the first camera to the current one (\f$ Path_n= Path_{n-1} * Pose_n^{-1} \f$) \ingroup Enumerations*/
            POSE /*!< The matrix contains the displacement from the previous camera position to the current one \ingroup Enumerations*/
        } MAT_TRACKING_TYPE;

        /**
         *  \enum UNIT
         *  \ingroup Enumerations
         *  \brief Enumerate for available metric unit of the depth
         */
        typedef enum {
            MILLIMETER,
            METER,
            INCH,
            FOOT,
            LAST_UNIT
        } UNIT;

        /**
         *  \enum COORDINATE_SYSTEM
         *  \ingroup Enumerations
         *  \brief Enumerate for available coordinates systems available for the points cloud and the tracking estimation/
		 *  \brief you can combine IMAGE/LEFT_HANDED/RIGHT_HANDED/ROBOTIC with APPLY_PATH using the operator | , so that the path is ingest in the point cloud : For example, LEFT_HANDED | APPLY_PATH
		 *  \brief In any case, the tracking is provided regarding the coordinate system.
         */
        typedef enum {
            IMAGE = 1, /*!< standard coordinate system in computer vision. Used in OpenCV : see here : http://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html */
            LEFT_HANDED = 2, /*!< Same as IMAGE but with Y axis inverted (defined by the left-hand rule). Used in Unity or DirectX */
            RIGHT_HANDED = 4, /*!< Same as LEFT_HANDED but with Z axis inverted (defined by the right-hand rule). Used in OpenGL */
            ROBOTIC = 8, /*!< specific ROBOTIC coordinate system : right-hand rule with Z pointing up and Y forward. */
			APPLY_PATH = 16, /*!< Set it to apply the path to the point cloud. */
            LAST_COORDINATE_SYSTEM = 32
        } COORDINATE_SYSTEM;

        inline COORDINATE_SYSTEM operator|(COORDINATE_SYSTEM a, COORDINATE_SYSTEM b) {
            return static_cast<COORDINATE_SYSTEM> (static_cast<int> (a) | static_cast<int> (b));
        }

        /**
         *  \enum SVO_COMPRESSION_MODE
         *  \ingroup Enumerations
         *  \brief Enumerate for available compression mode for SVO recording
         *  \brief LOSSLESS_BASED is an improvment of previous lossless compression (used in ZED Explorer), even if size may be bigger, compression time is much faster.
         */
        typedef enum {
            RAW_BASED, /*!< RAW images, no compression) \ingroup Enumerations*/
            LOSSLESS_BASED, /*!< new Lossless, with png/zstd based compression : avg size = 42% of RAW) \ingroup Enumerations*/
            LOSSY_BASED, /*!< new Lossy, with jpeg based compression : avg size = 22% of RAW) \ingroup Enumerations*/
            LAST_SVO_COMPRESSION_MODE
        } SVO_COMPRESSION_MODE;

        /**
         * \struct recording_state
         * \brief Recording structure that contains information about current svo recording
         *
         */
        struct recording_state {
            bool status; /*!< status of current frame. May be true for success or false if frame could not be written in the svo file  */
            double current_compression_time; /*!< compression time for the current frame in ms */
            double current_compression_ratio; /*!< compression ratio (% of raw size) for the current frame*/
            double average_compression_time; /*!< average compression time in ms since beginning of recording*/
            double average_compression_ratio; /*!< compression ratio (% of raw size) since beginning of recording*/
        };

        /**
         * \struct resolution
         * \brief Width and Height of each left and right image.
         *
         */
        struct resolution {
            int width; /*!< Width of single image in pixels  */
            int height; /*!< Height of single image in pixels*/

            resolution(int w_, int h_) {
                width = w_;
                height = h_;
            }
        };

        /*! Available mode for the ZED camera sensors */
        static std::vector<resolution> zedResolution = [] {
            std::vector<resolution> v;
            v.push_back(resolution(2208, 1242)); // HD2K
            v.push_back(resolution(1920, 1080)); // HD1080
            v.push_back(resolution(1280, 720)); // HD720
            v.push_back(resolution(672, 376)); // Wide VGA
            return v;
        }();

        static inline std::string tracking_state2str(TRACKING_STATE state) {
            std::string output;
            switch (state) {
                case TRACKING_RELOCDB_INIT:
                    output = "TRACKING_RELOC_INIT";
                    break;
                case TRACKING_INIT:
                    output = "TRACKING_INIT";
                    break;
                case TRACKING_RELOC:
                    output = "TRACKING_RELOC";
                    break;
                case TRACKING_LOST:
                    output = "TRACKING_LOST";
                    break;
                case TRACKING_GOOD:
                    output = "TRACKING_GOOD";
                    break;
                case TRACKING_OFF:
                    output = "TRACKING_OFF";
                    break;
                default:
                    output = "";
                    break;
            }
            return output;
        }

        static inline std::string errcode2str(ERRCODE err) {
            std::string output;
            switch (err) {
                case ERRCODE::SUCCESS:
                    output = "SUCCESS";
                    break;
                case ERRCODE::NO_GPU_COMPATIBLE:
                    output = "NO_GPU_COMPATIBLE";
                    break;
                case ERRCODE::NOT_ENOUGH_GPUMEM:
                    output = "NOT_ENOUGH_GPUMEM";
                    break;
                case ERRCODE::ZED_NOT_AVAILABLE:
                    output = "ZED_NOT_AVAILABLE";
                    break;
                case ERRCODE::ZED_RESOLUTION_INVALID:
                    output = "ZED_RESOLUTION_INVALID";
                    break;
                case ERRCODE::ZED_SETTINGS_FILE_NOT_AVAILABLE:
                    output = "ZED_SETTINGS_FILE_NOT_AVAILABLE";
                    break;
                case ERRCODE::INVALID_SVO_FILE:
                    output = "INVALID_SVO_FILE";
                    break;
                case ERRCODE::RECORDER_ERROR:
                    output = "RECORDER_ERROR";
                    break;
                case ERRCODE::INVALID_COORDINATE_SYSTEM:
                    output = "INVALID_COORDINATE_SYSTEM";
                    break;
                case ERRCODE::ZED_WRONG_FIRMWARE:
                    output = "ZED_WRONG_FIRMWARE";
                    break;
                case ERRCODE::NO_NEW_FRAME:
                    output = "NO_NEW_FRAME";
                    break;
                case ERRCODE::CUDA_ERROR_THROWN:
                    output = "CUDA_ERROR_THROWN";
                    break;
                case ERRCODE::ZED_NOT_INITIALIZED:
                    output = "ZED_NOT_INITIALIZED";
                    break;
                case ERRCODE::OUT_OF_DATE_NVIDIA_DRIVER:
                    output = "OUT_OF_DATE_NVIDIA_DRIVER";
                    break;
                case ERRCODE::INVALID_FUNCTION_CALL:
                    output = "INVALID_FUNCTION_CALL";
                    break;
            }
            return output;
        }

        static inline std::string statuscode2str(ZED_SELF_CALIBRATION_STATUS stat) {
            std::string output;
            switch (stat) {
                case ZED_SELF_CALIBRATION_STATUS::SELF_CALIBRATION_NOT_CALLED:
                    output = "SELF_CALIBRATION_NOT_CALLED";
                    break;
                case ZED_SELF_CALIBRATION_STATUS::SELF_CALIBRATION_FAILED:
                    output = "SELF_CALIBRATION_FAILED";
                    break;
                case ZED_SELF_CALIBRATION_STATUS::SELF_CALIBRATION_RUNNING:
                    output = "SELF_CALIBRATION_RUNNING";
                    break;
                case ZED_SELF_CALIBRATION_STATUS::SELF_CALIBRATION_SUCCESS:
                    output = "SELF_CALIBRATION_SUCCESS";
                    break;
                default:
                    output = "Undefined Self calibration status";
                    break;
            }
            return output;
        }

        static inline std::string sensing_mode2str(SENSING_MODE mode) {
            std::string output;
            switch (mode) {
                case SENSING_MODE::STANDARD:
                    output = "STANDARD";
                    break;
                case SENSING_MODE::FILL:
                    output = "FILL";
                    break;

                default: break;

            }
            return output;
        }

        static inline std::string mode2str(MODE mode) {
            std::string output;
            switch (mode) {
                case MODE::NONE:
                    output = "NONE";
                    break;
                case MODE::PERFORMANCE:
                    output = "PERFORMANCE";
                    break;
                case MODE::MEDIUM:
                    output = "MEDIUM";
                    break;
                case MODE::QUALITY:
                    output = "QUALITY";
                    break;
            }
            return output;
        }

        static inline MODE str2mode(std::string mode) {
            MODE output = PERFORMANCE;
            if (!mode.compare("NONE"))
                output = NONE;
            if (!mode.compare("PERFORMANCE"))
                output = PERFORMANCE;
            if (!mode.compare("MEDIUM"))
                output = MEDIUM;
            if (!mode.compare("QUALITY"))
                output = QUALITY;
            return output;
        }

        static inline std::string unit2str(UNIT unit) {
            std::string output;
            switch (unit) {
                case UNIT::MILLIMETER:
                    output = "MILLIMETER";
                    break;
                case UNIT::METER:
                    output = "METER";
                    break;
                case UNIT::INCH:
                    output = "INCH";
                    break;
                case UNIT::FOOT:
                    output = "FEET";
                    break;
            }
            return output;
        }

        static inline UNIT str2unit(std::string unit) {
            UNIT output = MILLIMETER;
            if (!unit.compare("MILLIMETER"))
                output = MILLIMETER;
            if (!unit.compare("METER"))
                output = METER;
            if (!unit.compare("INCH"))
                output = INCH;
            if (!unit.compare("FEET"))
                output = FOOT;
            return output;
        }

        /**
         * \struct CamParameters
         * \brief Intrinsic parameters of one camera
         *
         */
        struct CamParameters {
            float fx; /*!< Focal x */
            float fy; /*!< Focal y */
            float cx; /*!< Optical center x */
            float cy; /*!< Optical center y */

            double disto[5]; /*!< Distortion factor : k1, k2, k3, r1, r2 */

            float vFOV; /*!< Vertical field of view after stereo rectification */
            float hFOV; /*!< Horizontal field of view after stereo rectification */
            float dFOV; /*!< Diagonal field of view after stereo rectification */

            void SetUp(float fx_, float fy_, float cx_, float cy_) {
                fx = fx_;
                fy = fy_;
                cx = cx_;
                cy = cy_;
            }
        };

        /**
         * \struct StereoParameters
         * \brief Intrinsic parameters of each cameras, baseline and convergence (radians)
         *
         */
        struct StereoParameters {
            float baseline; /*!< Distance between the 2 cameras in mm  */
            float Ty;
            float Tz;
            float convergence; /*!< Convergence between the 2 cameras in radians  */
            float Rx; /*!< Rotation around X axis ("tilt"), will be affined by self-calibration - optional*/
            float Rz; /*!< Rotation around Z axis("roll"), will be affined by self-calibration - optional*/
            CamParameters LeftCam; /*!< Intrinsic parameters of the left camera  */
            CamParameters RightCam; /*!< Intrinsic parameters of the right camera  */
        };

        /**
         * \struct InitParams.
         * \brief parameters for ZED initialization.
         * a default constructor is enable.
         */
        struct SLSTEREO_EXPORT_DLL InitParams {
            /**
             *	defines the quality of the disparity map, affects the level of details and also the computation time.
             *	\n default : MODE::PERFORMANCE
             */
            MODE mode;

            /**
             *	define the unit metric for all the depth-distance values.
             *	\n default : UNIT::MILLIMETER
             */
            UNIT unit;

            /**
             *	define the coordinate system, in which the path and the point cloud is referred.
             *	\n default : COORDINATE_SYSTEM::IMAGE
             */
            COORDINATE_SYSTEM coordinate;

            /**
             *	if set to true, it will output some information about the current status of initialization.
             *	\n default : false
             */
            bool verbose;

            /**
             *	defines the graphics card on which the computation will be done. The default value search the more powerful (most CUDA cores) usable GPU.
             *	\n default : -1
             */
            int device;

            /**
             *	specify the minimum depth information that will be computed, in the unit you previously define.
             *	\note By default we compute disparities in a constant interval, defined by a ratio of the image size.
             *	e.g. for HD720 : \f$ W = 1280 \f$, \f$ interval (cst) = \frac{1}{8} \f$ \f$ \rightarrow \f$
             * \f$ DisparityMax = \frac{1280}{8} = 160 \f$ \f$ \rightarrow \f$
             *  \f$ DepthMin  = \frac{focal*baseline}{DisparityMax} = \frac{700*120}{160} = 525mm \f$
             *	\n default : -1
             *  \warning The computation time is affected by the value, exponentially. The closer it gets the longer the disparity search will take.
             * In case of limited computation power, consider increasing the value.
             */
            float minimumDistance;

            /**
             *	if set to true, it will disable self-calibration and take the optional calibration parameters without optimizing them.
             *	It is advised to leave it as false, so that calibration parameters can be optimized.
             *	\n default : false
             */
            bool disableSelfCalib;

            /**
             *	if set to true, it will vertically flipped frames coming from the ZED. (for "Hang out" mode).
             *	\n default : false
             */
            bool vflip;


            // default constructor, set all parameters to their default value.

            InitParams(MODE mode_ = PERFORMANCE, UNIT unit_ = MILLIMETER, COORDINATE_SYSTEM coord_ = IMAGE, bool verbose_ = false, int device_ = -1, float minDist_ = -1., bool disable = false, bool vflip_ = false, bool activRecord_ = false, std::string vidname_ = "") : mode(mode_), unit(unit_), coordinate(coord_), verbose(verbose_), device(device_), minimumDistance(minDist_), disableSelfCalib(disable), vflip(vflip_) {
            }


            /*!
             *  \fn void save(std::string filename)
             *  \brief Save the current bunch of parameters into a file.
             *  \param filename : the name of the file in which the parameters will be stored.
             */
            void save(std::string filename);

            /*!
             *  \fn bool load(std::string filename)
             *  \brief load the values of the parameters contained in a file.
             *  \param filename : the name of the file.
             *  \return Returns if the file was succefully loaded or not.
             */
            bool load(std::string filename);
        };

        /**
         * \struct GrabParams.
         * \brief Contains all the Camera::grab() parameters
         */
        struct SLSTEREO_EXPORT_DLL GrabParams {
            SENSING_MODE dm_type;
            bool computeMeasure;
            bool computeDisparity;
            bool computeXYZ;

            GrabParams(SENSING_MODE dm_type_ = SENSING_MODE::STANDARD,
                    bool computeMeasure_ = 1,
                    bool computeDisparity_ = 1,
                    bool computeXYZ_ = 1) :
            dm_type(dm_type_),
            computeMeasure(computeMeasure_),
            computeDisparity(computeDisparity_),
            computeXYZ(computeXYZ_) {
            }
        };

        /**
         * \struct SLMotionPoseData.
         * \brief Motion tracking data in a structure.
         *  contains quaternion, rotation matrix, translation and other usefull value (timestamp,tracking status, confidence)
         */
        struct SLSTEREO_EXPORT_DLL MotionPoseData {
            /*! boolean that indicates if tracking is activated or not. You should check that first if something wrong.*/
            bool valid;

            /*! Timestamp of the frame that this pose estimate corresponds to.*/
            unsigned long long timestamp;

            /*! Orientation, as a quaternion, of the pose/path of the target frame with
             *  reference to the base frame. Base frame is either the last frame (pose) or the first frame (path).
             *   \n Specified as (x,y,z,w) where RotationAngle is in radians.
             *  @code
             *   x = X * sin(RotationAngle / 2)
             *   y = Y* sin(RotationAngle / 2)
             *   z = Z* sin(RotationAngle / 2)
             *   w = cos(RotationAngle / 2)
             *   // where (X,Y,Z) is the unit length axis of rotation in 3D space
             *  @endcode
             */
            float orientation[4];

            /*! 3x3 Rotation matrix : another way to get the orientation of the camera.
             *  \n Similar to the first 3x3 block of the Eigen Matrix given by #sl::zed::Camera::getPosition().
             */
            float rotationdata[9];

            /*! Translation, ordered x, y, z, of the pose of the target frame with
             * reference to the base frame. Base frame is either the last frame (pose) or the first frame (path).
             */
            float translation[3];

            /*!  Current status of the tracking for the target frame
             * \n see #sl::zed::TRACKING_STATE for more infos
             */
            sl::zed::TRACKING_STATE frame_status;


            /*! Confidence/Quality of the pose estimation for the target frame
             * \n A confidence metric of the tracking [0-100], 0 means that the tracking is lost. similar to value returned by #sl::zed::Camera::getTrackingConfidence() function
             */
            int pose_confidence;

        };

    } // zed namespace
};


#define TIMING
#ifdef TIMING
#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER start = std::chrono::high_resolution_clock::now();
#define DEF_START_TIMER auto start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << name << " " << \
std::chrono::duration_cast<std::chrono::milliseconds>( \
  std::chrono::high_resolution_clock::now()-start \
).count() << " ms "<<std::endl;
#else
#define INIT_TIMER
#define START_TIMER
#define DEF_START_TIMER
#define STOP_TIMER(name)
#endif

#endif /*__GLOBAL_DEFINE__*/
