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


#ifndef MAT_HPP
#define	MAT_HPP

#include "zed/utils/GlobalDefine.hpp"

namespace sl {
    namespace zed {

        /*! Enumerate defines the type of the stored elements */
        enum DATA_TYPE {
            FLOAT, /*!< float elements (require to cast the pointer) */
            UCHAR /*!< unsigned char 8 bits elements */
        };

        /*! Enumerate defines the buffer type */
        enum MAT_TYPE {
            CPU, /*!< the buffer is stored in the memory (CPU memory)*/
            GPU /*!<  the buffer is stored in the CUDA device (Npp pointer, GPU memory) */
        };

        /*! \class Mat
         *  \brief This class is used to store a buffer (the pointer) of an image 
         * and the associated metadata (size, type...).
         */
        class SLSTEREO_EXPORT_DLL Mat {
        public:
            /*!
             *  \brief Mat default constructor
             * 
             */
            Mat();

            /*!
             *  \brief Mat constructor
             * 
             * \param width : width of the image in pixels
             * \param height : height of the image in pixels
             * \param channels : number of channels in the image
             * \param data_type : type of stored element (see sl::zed::DATA_TYPE). Can be float or uchar.
             * \param data : the buffer
             * \param mat_type : defines where the buffer is stored (CPU or GPU memory)
             * 
             * \warning The buffer memory is NEITHER automatically allocated NOR 
             * freed and requires an explicit call of the corresponding function
             * (Exception : for some of the sl::zed::Camera functions, the sl::zed::Mat
             * may already be pre-allocated and must not be freed. For more information,
             * refer to the corresponding function documentation)
             * 
             */
            Mat(int width, int height, int channels, DATA_TYPE data_type,
                    sl::uchar* data, MAT_TYPE mat_type = MAT_TYPE::CPU);

            /*!
             *  \brief Gets the value of the image at the given coordinate
             * 
             * \param x coordinate
             * \param y coordinate
             *
             * \retval pixel_value If the Mat has less than 3 channels a uchar3 is still produced 
             * with the value 0 for the missing channels. If the Mat has more than 3 channels
             * the value correspond to the first 3 channels and the others are ignored
             * 
             */
            sl::uchar3 getValue(int x, int y) const;

            /*!
             *  \brief Sets up the buffer metadata
             * 
             * \param width : width of the image in pixels
             * \param height : height of the image in pixels
             * \param step : the step in bytes (essential if the data are aligned)
             * \param data : the buffer
             * \param channels : number of channels in the image
             * \param data_type : type of element stored
             * \param mat_type : define where the buffer is stored (CPU or GPU memory)
             * 
             */
            void setUp(int width, int height, int step, sl::uchar* data, int channels, DATA_TYPE data_type, MAT_TYPE mat_type = CPU);

            /*!
             *  \brief Sets up the buffer metadata
             * 
             * \param width : width of the image in pixels
             * \param height : height of the image in pixels
             * \param channels : number of channels in the image
             * \param data_type : type of element stored
             * \param mat_type : defines where the buffer is stored (CPU or GPU memory)
             * 
             * \note if the mat_type is CPU, the step is automatically set
             * 
             */
            void setUp(int width, int height, int channels, DATA_TYPE data_type, MAT_TYPE mat_type = CPU);

            /*!
             *  \brief Allocates the memory
             *
             * \param width : width of the image in pixels
             * \param height : height of the image in pixels
             * \param channels : number of channels in the image
             * \param data_type : type of element stored
             * 
             * \warning The Mat is required to be a CPU Mat
             * 
             */
            void allocate_cpu(int width, int height, int channels, DATA_TYPE data_type);

            /*!
             *  \brief Allocates the memory with the metadata previously set
             *
             * \pre All the metadata must have been filled before calling allocate
             * to prevent an undefined behavior
             * 
             * \warning The Mat is required to be a CPU Mat
             * 
             */
            void allocate_cpu();

            /*!
             *  \brief Deallocates the memory
             * 
             */
            void deallocate();

            /*!
             *  \brief Gets the width of the image in bytes,
             * useful for the npp function (take into account the number of channels)
             *              
             * \retval width_byte : the width in bytes
             * 
             */
            int getWidthByte() const;

            /*!
             *  \brief Gets the size of the buffer type
             * 
             * \note sizeof() is called on Mat::data using the real type
             * 
             * \retval size in bytes
             * 
             */
            int getDataSize() const;

            /*!
             *  \brief Prints in the standard output information about the Mat
             * 
             */
            void info();

            int width;
            int height;
            int step; // in byte

            /*! The pointer to the data, can also be casted to store float value */
            sl::uchar* data; // uchar* ou float*
            int channels;
            DATA_TYPE data_type;
            MAT_TYPE type;

        };
    }
}
#endif	/* MAT_HPP */

