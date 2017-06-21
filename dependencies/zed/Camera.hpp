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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "zed/Mat.hpp"
#include "zed/utils/GlobalDefine.hpp"
#include <cuda/cuda.h>
#include <Eigen/Core>
#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>

namespace sl {
    namespace zed {

        /*!
         *  \brief The function cast a sl::zed::Mat into an openCV cv::Mat
         *
         * \retval Output image as a cv::Mat (opencv format)
         */
        inline cv::Mat slMat2cvMat(sl::zed::Mat slMat) {
            int cvType;
            switch (slMat.data_type) {
                case sl::zed::DATA_TYPE::UCHAR:
                    cvType = CV_MAKETYPE(CV_8U, slMat.channels);
                    break;
                case sl::zed::DATA_TYPE::FLOAT:
                    cvType = CV_MAKETYPE(CV_32F, slMat.channels);
                    break;
                default:
                    std::cerr << "slMat2cvMat: mat type currently not supported " <<
                            slMat.channels << " " << slMat.getDataSize() << std::endl;
            }
            return cv::Mat(slMat.height, slMat.width, cvType, slMat.data);
        }

        /*!
         *  \brief The function cast an openCV cv::Mat into a sl::zed::Mat
         *
         * \retval Output image as a sl::zed::Mat
         */
        inline sl::zed::Mat cvMat2slMat(cv::Mat &cvMat) {
            DATA_TYPE type;
            sl::zed::Mat output;
            switch (cvMat.depth()) {
                case CV_8U:
                    type = DATA_TYPE::UCHAR;
                    break;
                case CV_32F:
                    type = DATA_TYPE::FLOAT;
                    break;
                default:
                    throw std::runtime_error("Cannot cast cv::Mat into sl::zed::Mat, incompatible depth");
                    break;
            }
            output.setUp(cvMat.cols, cvMat.rows, cvMat.step, cvMat.data, cvMat.channels(), type);
            return output;
        }

        /*! \class Camera
         *  \brief The main class to use the ZED camera
         */
        class SLSTEREO_EXPORT_DLL Camera {
        public:
            /*!
             *  \brief Camera constructor. The ZEDResolution_mode sets the sensor
             * resolution and defines the size of the output images, including the
             * measures (disparity map, confidence map..).
             *
             * All computation is done on a CUDA capable device
             * (That means that every CPU computation will need a memory retrieve
             * of the images, which takes some time). If the performance is the main focus,
             * all external computation should run on GPU. The retrieve*_gpu gives
             * directly access to the gpu buffer.
             *
             *  \param zedRes_mode : the chosen ZED resolution
             *  \param fps : a requested fps for this resolution. set as 0.0 will choose the default fps for this resolution ( see User guide)
             *   \param zed_linux_id : ONLY for LINUX : if multiple ZEDs are connected, it will choose the first zed listed (if zed_linux_id=0), the second listed (if zed_linux_id=1), ...
             *  Each ZED will create its own memory (CPU and GPU), therefore the number of ZED available will depend on the configuration of your computer.
             *  Currently not available for Windows
             *
             */
            Camera(ZEDResolution_mode zedRes_mode = ZEDResolution_mode::HD720, float fps = 0.0, int zed_linux_id = 0);


            /*!
             *  \brief Camera constructor, from svo file previously recorded.
             * The size of the output is defined by the recorded images.
             *
             *  \param zed_record_filename : path with filename to the recorded svo file
             *
             */
            Camera(std::string zed_record_filename);


            ~Camera();

            /*!
             *  \brief The init function must be called after the instantiation.
             * The function checks if the ZED camera is plugged and opens it, if the graphics card
             * is compatible, allocates the memory and launches the automatic calibration.
             *  \param parameters : a structure containing all the individual parameters
             *
             * \retval ERRCODE : The error code gives information about the
             * internal process, if SUCCESS is returned, the camera is ready to use.
             * Every other code indicates an error and the program should be stopped.
             * For more details see sl::zed::ERRCODE.
             *
             */
            ERRCODE init(InitParams &parameters);

            /*!
             *  \brief The function grabs a new image, rectifies it and computes the
             * disparity map and optionally the depth map.
             * The grabbing function is typically called in the main loop.
             *
             *  \param dm_type : defines the type of disparity map, more info : sl::zed::SENSING_MODE definition
             *  \param computeMeasure : defines if the depth map should be computed.
             * If false, only the disparity map is computed.
             *  \param computeDisparity : defines if the disparity map should be computed.
             * If false, the depth map can't be computed and only the camera images can be retrieved.
             *  \param computeXYZ : defines if the point cloud should be computed (including XYZRGBA).
             *
             *  \retval error : the function returns ERRCODE::SUCCESS if no problem was encountered,
             *  ERRCODE::NO_NEW_FRAME otherwise if something wrong happens
             *
             * \pre Camera::init function must have been called (once for the lifetime of the Camera object)
             *  before calling Camera::grab.
             */
            ERRCODE grab(SENSING_MODE dm_type = SENSING_MODE::STANDARD, bool computeMeasure = 1, bool computeDisparity = 1, bool computeXYZ = 1);

            /*!
             *  \brief cf. Camera::grab()
             *
             */
            ERRCODE grab(GrabParams params);

            /*!
             *  \brief Gets a CPU image to display. Several modes available
             * SidebySide, anaglyph... (for more see sl::zed::VIEW_MODE)
             *
             *  \param view : the wanted mode
             *  \retval View : the return value is a CPU sl::zed::Mat, 4 channels, UCHAR.
             *
             * \warning The buffer must be duplicated if another CPU retrieve has to be done, it will be
             * overwritten otherwise. The buffer must not be freed.
             */
            Mat getView(VIEW_MODE view);

            /*!
             *  \brief Gets a GPU image to display. Several modes available
             * SidebySide, anaglyph... (for more see sl::zed::VIEW_MODE)
             *
             *  \param view : the wanted mode
             *  \retval View : the return value is a GPU sl::zed::Mat, 4 channels, UCHAR.
             *
             * \warning The buffer must be duplicated if another GPU retrieve has to be done, it will be
             * overwritten otherwise. The buffer must not be freed.
             */
            Mat getView_gpu(VIEW_MODE view);

            /*!
             *  \brief Downloads the rectified image from the device and returns the CPU buffer.
             * The retrieve function should be called after the function Camera::grab
             *
             *  \param SIDE  : defines the image side wanted (see sl::zed::SIDE)
             *  \retval Image : the return value is a CPU sl::zed::Mat, 4 channels, UCHAR. The size is given by
             * the input parameters of the constructor Camera.
             *
             * \warning The buffer must be duplicated if another CPU retrieve has to be done, it will be
             * overwritten otherwise. The buffer must not be freed.
             */
            Mat retrieveImage(SIDE side);

            /*!
             *  \brief Downloads the measure (disparity, depth or confidence of disparity)
             * from the device and returns the CPU buffer.
             *  The retrieve function should be called after the function Camera::grab
             *
             *  \param MEASURE : defines the type wanted, such as disparity map,
             * depth map or the confidence (see sl::zed::MEASURE)
             *  \retval Measure : the return value is a CPU sl::zed::Mat, 1 or 4 channels, FLOAT
             *
             * \warning The buffer must be duplicated if another CPU retrieve has to be done, it will be
             * overwritten otherwise. The buffer must not be freed.
             */
            Mat retrieveMeasure(MEASURE measure);

            /*!
             *  \brief Gets the rectified image GPU buffer.
             * The retrieve function should be called after the function Camera::grab
             *
             *  \param SIDE  : defines the image side wanted (see sl::zed::SIDE)
             *  \retval Image : the return value is a GPU sl::zed::Mat, 4 channels, UCHAR. The size is given by
             * the input parameters of the constructor Camera.
             * \warning The buffer must not be freed.
             */
            Mat retrieveImage_gpu(SIDE side);

            /*!
             *  \brief Gets the measure (disparity, depth or certainty of disparity) GPU buffer.
             * The retrieve function should be called after the function Camera::grab
             *
             *  \param MEASURE : defines the data type wanted (see sl::zed::MEASURE)
             *  \retval Measure : the return value is a GPU sl::zed::Mat, 1 or 4 channels, FLOAT. The size is given by
             * the input parameters of the constructor Camera.
             * \warning The buffer must not be freed.
             */
            Mat retrieveMeasure_gpu(MEASURE measure);

            /*!
             *  \brief Performs a GPU normalization of the measure value and download the result as a CPU image
             *  \param MEASURE : defines the requested output (see sl::zed::MEASURE)
             *  \param min : defines the lower bound of the normalization, default : automatically found
             *  \param max : defines the upper bound of the normalization, default : automatically found
             *
             *  \retval normalized_measure :  the return value is a CPU sl::zed::Mat, 4 channels, UCHAR.
             *  \warning The buffer must be duplicated if another CPU retrieve has to be done, it'll be
             * overwritten otherwise. The buffer must not be freed.
             *
             */
            Mat normalizeMeasure(MEASURE measure, float min = 0, float max = 0);

            /*!
             *  \brief GPU Normalization of the measure value and get the result as a GPU image
             *  \param MEASURE : defines the requested output (see sl::zed::MEASURE)
             *  \param min : defines the lower bound of the normalization, default : automatically found
             *  \param max : defines the upper bound of the normalization, default : automatically found
             *
             *  \retval normalized_measure :  the return value is a GPU sl::zed::Mat, 4 channels, UCHAR.
             *  \warning The buffer must be duplicated if another GPU retrieve has to be done, it'll be
             * overwritten otherwise. The buffer must not be freed.
             *
             */
            Mat normalizeMeasure_gpu(MEASURE measure, float min = 0, float max = 0);


            /*!
             *  \brief Sets a filtering value for the disparity map
             * (and by extension the depth map). The function should be called before
             * Camera::grab to be taken into account.
             * \param ThresholdIdx : a value in [1,100]. A lower value means more confidence and precision
             * (but less density), an upper value reduces the filtering (more density, less certainty).
             * Other value means no filtering.
             *
             */
            void setConfidenceThreshold(int ThresholdIdx);

            /*!
             *  \brief Gets the current confidence threshold value for the disparity map
             * (and by extension the depth map).
             * \retval : current filtering value between 0 and 100.
             *
             */
            int getConfidenceThreshold();

            /*!
             *  \brief Gets the CUDA context used for all the computation
             *  \retval CUDA_context : the CUcontext
             */
            CUcontext getCUDAContext();

            /*!
             *  \brief Gets the image size
             *  \retval resolution : the image size
             */
            resolution getImageSize();

            /*!
             *  \brief Set the maximum distance of depth/disparity estimation (all values after this limit will be reported as TOO_FAR value)
             *  \param distanceMax : maximum distanec in the defined UNIT
             *
             */
            void setDepthClampValue(float distanceMax);

            /*!
             *  \brief Get the current maximum distance of depth/disparity estimation
             *  \retval : current maximum distance in the defined UNIT
             *
             */
            float getDepthClampValue();


            /*!
             *  \brief Get the closest measurable distance by the camera, according to the camera and the depth map parameters
             *  \retval float : the distance in the defined UNIT
             *
             */
            float getClosestDepthValue();


            //@{
            /**  @name Camera infos */
            /*!
             *  \brief Gets the ZED Serial Number
             *  \retval Returns the ZED Serial Number (as uint) (Live or SVO).
             */
            unsigned int getZEDSerial();

            /*!
             *  \brief Gets the ZED Current Firmware version
             *  \retval Returns the ZED Firmware version (as uint), 0 if the ZED is not connected.
             */
            unsigned int getZEDFirmware();

            /*!
             *  \brief Sets the position of the SVO file to a desired frame.
             * \param frame : the number of the desired frame to be decoded.
             * \retval true if succes, false if failed (i.e. if the ZED is currently used in live and not playing a SVO file)
             *
             */
            bool setSVOPosition(int frame);

            /*!
             *  \brief Get the current position of the SVO file
             * \retval SVO Style Only : the current position in the SVO file as int (-1 if the SDK is not reading a SVO)
             *
             */
            int getSVOPosition();

            /*!
             *  \brief Get the number of frames in the SVO file
             * \retval SVO Style Only : the total number of frames in the SVO file (-1 if the SDK is not reading a SVO)
             *
             */
            int getSVONumberOfFrames();

            /*!
             *  \brief Set the value to the corresponding Camera Settings mode (Gain, brightness, hue, exposure...)
             *  \param ZEDCamera_settings : enum for the control mode
             *  \param value : value to set for the corresponding control
             *  \param usedefault : will set default (or automatic) value if set to true (value (int) will not be taken into account)
             *
             */
            void setCameraSettingsValue(ZEDCamera_settings mode, int value, bool usedefault = false);

            /*!
             *  \brief Get the current value to the corresponding Camera Settings mode (Gain, brightness, hue, exposure...)
             *  \param ZEDCamera_settings : enum for the control mode
             *  \retval Current value for the corresponding control (-1 if something wrong happened).
             *
             */
            int getCameraSettingsValue(ZEDCamera_settings mode);

            /*!
             *  \brief Get the current fps of the camera.
             *  \retval Current FPS (or recorded FPS for SVO). Return -1.f if something goes wrong.
             *
             */
            float getCurrentFPS();

            /*!
             *  \brief Set a new frame rate for the camera, or the closest avaliable frame rate.
             *  \param desiredFPS : the new desired frame rate
             *  \retval bool : return an error if something wrong happen
             *
             */
            bool setFPS(int desiredFPS);

            /*!
             *  \brief Get the timestamp at the time the frame has been extracted from USB stream. (should be called after a grab())
             *  \retval Current Timestamp in ns. -1 is not available (SVO file without compression).
             *  \n Note that new SVO file from SDK 1.0.0 (with compression) contains the camera timestamp for each frame.
             *
             */
            unsigned long long getCameraTimestamp();

            /*!
             *  \brief Get the current timestamp at the time the function is called. Can be compared to the camera timestamp for synchronization.
             *  \n Use this function to compare the current timestamp and the camera timestamp, since they have the same reference (Computer start time).
             *
             *  \retval Current Timestamp in ns.
             *
             */
            unsigned long long getCurrentTimestamp();


            /*!
             *  \brief Get the number of frame dropped since grab() has been called for the first time
             *  \n Based on camera timestamp and fps comparison.
             *
             *  \retval number of frame dropped since first grab() call.
             *
             */
            unsigned int getFrameDroppedCount();



            /*!
             *  \brief Useful if you use the camera upside down, this will flip the images so you can get the images in a normal way.
             *  \param flip : apply or not the vertical flip
             *
             */
            void setFlip(bool flip);
            //@}


            //@{
            /**  @name Self calibration */

            /*!
             *  \brief Gets the ZED parameters
             *  \retval StereoParameters pointer containing the intrinsic parameters of each camera
             * and the baseline (UNIT defined by user durint Init()) and convergence (CV==Ry) (radian) of the ZED.
             */
            StereoParameters* getParameters();

            /*!
             *  \brief Get the current status of the self-calibration.
             *  \retval ZED_SELF_CALIBRATION_STATUS : The status code gives information about the
             * self calibration status.
             * For more details see sl::zed::ZED_SELF_CALIBRATION_STATUS.
             */
            ZED_SELF_CALIBRATION_STATUS getSelfCalibrationStatus();

            /*!
             *  \brief Get the estimated rotation from the self-calibration.
             *  \retval double3 (structure of 3 double values) of the Rotation (one component for each axis)
             *  <br>- d1 will return the \f$ Rx \f$ component ("Tilt" or "Pitch")
             *  <br>- d2 will return the \f$ Ry \f$ component ("Convergence")
             *  <br>- d3 will return the \f$ Rz \f$ component ("Roll")
             *  All values are in radians.
             */
            sl::double3 getSelfCalibrationRotation();

            /*!
             *  \brief The reset function can be called at any time AFTER the Camera::init function has been called.
             *  It will reset and calculate again correction for misalignment, convergence and color mismatch.
             *  It can be called after changing camera parameters without needing to restart your executable.
             *
             * \retval error boolean value : the function returns false if no problem was encountered,
             * true otherwise.
             * if no problem was encountered, the camera will use new parameters. Otherwise, it will be the old ones
             */
            bool resetSelfCalibration();
            //@}


            // -----------------------------------------------------------------
            //                         Tracking functions:
            // -----------------------------------------------------------------
            //@{
            /**  @name Tracking */
            /*!
             *  \brief Initialize and Start the tracking functions
             *  \param initPosition : position of the first camera, used as reference. By default it should be identity.
             *  \param enableAreaLearning : define if the relocalization (when tracking is lost) and loop closure is enable or not.
             *  \param areaDBpath : define if and where a relocalization database from a previous run on the same scene has to be loaded.
             *  \retval bool : return false if the area database file wasn't found, true otherwise
             * 
             *  \warning The relocalization database can only be used with the same depth performance mode as the one used for the recording
             */
            bool enableTracking(Eigen::Matrix4f &initPosition, bool enableAreaLearning = false, std::string areaDBpath = "");

            /*!
             *  \brief fill the position of the camera and return the current state of the Tracker
             *  \param position : the matrix containing the position of the camera (path or position)
             *  \n 4x4 Matrix position/path :
             *  \n\n
             * \f$ \left( \begin{array}{cccc}
             * R11 & R21 & R31 & Tx \\
             * R12 & R22 & R32 & Ty \\
             * R13 & R23 & R33 & Tz \\
             *  0 & 0 & 0 & 1 \\
             * \end{array} \right) \f$
             *
             *  \n  --> \a Rnm is the XYZ 3x3 rotation matrix
             *  \n  --> \a Tx/y/z is the XYZ translation vector.
             *
             * \n\n
             * \n Extract Rotation Matrix --> Eigen::Matrix3f rot_ = position.block(0,0,3,3);
             * \n Extract Translation Vector --> Eigen::Vector3f trans_ = position.block(0,3,3,1);
             * \n Convert to Quaternion --> Eigen::Quaternionf quat(position.block(0,0,3,3));
             * \n \n
             *
             *  \param mat_type : define if the function return the path (the cumulate displacement of the camera) or juste the pose (the displacement from the previous position).
             */
            sl::zed::TRACKING_STATE getPosition(Eigen::Matrix4f &position, MAT_TRACKING_TYPE mat_type = MAT_TRACKING_TYPE::PATH);

            /*!
             *  \brief fill the #sl::zed::MotionPoseData for the current frame (see the definition of #sl::zed::MotionPoseData to know how to extract the pose estimation) and return the current state of the Tracker
             *  \retval #sl::zed::TRACKING_STATE that indicates the current state of pose estimation. This value is also available in the #sl::zed::MotionPoseData structure
             *  \param struct_position_io : handle the #sl::zed::MotionPoseData structure that contains the position, orientation and other values about global pose estimation for the current frame.
             *  \param mat_type : define if the function handles the path (the cumulate displacement of the camera) or juste the pose (the displacement from the previous position).
             */
            sl::zed::TRACKING_STATE getPosition(sl::zed::MotionPoseData& struct_position_io, MAT_TRACKING_TYPE mat_type = MAT_TRACKING_TYPE::PATH);

            /*!
             *  \brief Return a confidence metric of the tracking [0-100], 0 means that the tracking is lost
             */
            int getTrackingConfidence();


            /*!
             *  \brief return the state of the current tracked frame
             */
            TRACKING_FRAME_STATE getTrackingFrameState();


            /*!
             *  \brief Save the area learning information in a file.
             *  \param areaDBpath : the name and path of the database
             * 
             *  \warning This feature is still in beta, you might encounter reloading issues. 
             * Please also note that the reloc database depends on the depth map performance mode chosen during the recording. The same mode must be used to reload the database.
             */
            bool saveAreaLearningDB(std::string areaDBpath);


            /*!
             *  \brief stop the tracker, if you want to restart, call enableTracking().
             */
            void stopTracking();

            // -----------------------------------------------------------------
            //                         IMU DATA INTEGRATION :
            // -----------------------------------------------------------------
            /*!
             *  \brief if you have an external sensor (IMU) you can use its data to set a prior for the tracking.
             *  \param prior : the pose (rotations and translations) that will be use as prior for tracking operation.
             *  \retval false if the tracking is not enabled, 1 otherwise.
             */
            bool setTrackingPrior(Eigen::Matrix4f &priorRt);

            /*!
             *  \brief if you have an external sensor you can use its data to set a prior for the tracking.
             *  \param prior : the pose (rotations only) that will be use as prior for tracking operation.
             *  \retval false if the tracking is not enabled, 1 otherwise.
             */
            bool setTrackingPrior(Eigen::Matrix3f &priorR);

            /*!
             *  \brief if you have an external sensor you can use its data to set a prior for the tracking.
             *  \param prior : the pose (translations only) that will be use as prior for tracking operation.
             *  \retval false if the tracking is not enabled, 1 otherwise.
             */
            bool setTrackingPrior(Eigen::Vector3f &priorT);
            //@}

            // -----------------------------------------------------------------
            //                         Specific When Recording mode is activated
            // -----------------------------------------------------------------
            //@{
            /**  @name Recorder */

            /*!
             *  \brief Create a file for Recording. Must be called after Init().
             *  \param video_filename can be a *.svo file or a *.avi file (detected by the suffix name provided).
             *  \param compression_mode can be one of the SVO_COMPRESSION_MODE enum
             *  \warning This function can be called multiple times during ZED lifetime, but if video_filename is already existing, the file will be erased.
             *  \retval ERRCODE that defines if file was successfully created and can be filled with images.
             *  \n * SUCCESS if file can be filled
             *  \n * RECORDER_ERROR if something wrong happens.
             *
             */

            ERRCODE enableRecording(std::string video_filename, SVO_COMPRESSION_MODE compression_mode = SVO_COMPRESSION_MODE::LOSSLESS_BASED);


            /*!
             *  \brief Get the current side by side YUV 4:2:2 frame, CPU buffer
             *  \warning The buffer must be duplicated if record() is called, it will be
             *   overwritten otherwise. The buffer must not be freed.
             *   Undefined behavior if called outside recorder mode.
             */
            sl::zed::Mat getCurrentRawRecordedFrame();


            /*!
             *  \brief Ingest the current frame provided by grab() into the file
             *  \warning grab() must be called before record() to take the last frame available. Otherwise, it will be the last grabbed frame.
             *  \retval return recording state structure. For more details see sl::zed::recording_state.
             */
            sl::zed::recording_state record();



            /*!
             *  \brief Stops the recording and closes the file
             */
            void stopRecording();

            /*!
             *  \brief Displays the current recorded frame
             *  \warning Might reduce the recording framerate
             */
            void displayRecorded();
            //@}

            /*!
             *  \brief The function return the version of the currently installed ZED SDK
             *  \retval ZED SDK version as a string with the following format : MAJOR.MINOR.PATCH
             */
            static std::string getSDKVersion();

            /*!
             *  \brief The function checks if ZED cameras are connected, can be called before instantiating a Camera object
             *  \retval the number of connected ZED
             *  \warning On Windows, only one ZED is accessible so this function will return 1 even if multiple ZED are connected.
             */
            static int isZEDconnected();

            /*!
             *  \brief The function stick the calling thread to a specific CPU core. This function is only available for Jetson TK1 and TX1.
             *  \param cpu_core : int that defines the core the thread must be run on. could be between 0 and 3. (cpu0,cpu1,cpu2,cpu3).
             *  \retval 0 is stick is OK, otherwise status error.
             *  \warning Function only available for Jetson. On other platform, result will be always 0 and no operations are performed.
             */
            static int sticktoCPUCore(int cpu_core);




        private:
            ERRCODE openCamera(bool);
            bool nextImage(bool);
            void computeDepth();
            void computeTriangulation(int, int);
            int initMemory();
            bool initRectifier();
            bool loadParameters();
            int* members;
        };
    }

    /*!
     *  \brief write the current depth map into a file
     *  \param DEPTH_FORMAT : for file format
     *  \param name : the name (path) in which the depth will be saved
     *  \param factor : only for PNG and PGM, apply a gain to the depth value (default 1.)
     *  The maximum value is 65536, so you can set the Camera::setDepthClampValue to 20000 and give a factor to 3, Do not forget to scale (by 1./factor) the pixel value to get the real depth.
     *  The occlusions are represented by 0.
     *  \retval return false if something wrong happen, else return true
     *  \note factor : only for PNG and PGM
     */
    extern "C" SLSTEREO_EXPORT_DLL bool writeDepthAs(sl::zed::Camera *zed, sl::DEPTH_FORMAT format, std::string name, float factor = 1.);

    /*!
     *  \brief write the current point cloud into a file
     *  \param POINT_CLOUD_FORMAT : for file format
     *  \param name : the name (path) in which the point cloud will be saved
     *  \param withColor : indicates if the color must be saved (default false). Not avaliable for XYZ and VTK
     *  \param keepOccludedPoint : indicates if the non avaliable data should be saved and set to 0 (default false), if set to true this give a Point Cloud with a size = height * width
     *  \retval return false if something wrong happen, else return true
     *  \note The color is not saved for XYZ and VTK files
     */
    extern "C" SLSTEREO_EXPORT_DLL bool writePointCloudAs(sl::zed::Camera *zed, sl::POINT_CLOUD_FORMAT format, std::string name, bool withColor = false, bool keepOccludedPoint = false);
}

#endif      /* CAMERA_HPP */
