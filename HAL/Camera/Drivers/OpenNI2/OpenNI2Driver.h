#pragma once

#include <HAL/Camera/CameraDriverInterface.h>
#include <HAL/Messages/ImageArray.h>
#include <calibu/cam/camera_xml.h>

#include "OniSampleUtilities.h"
#include "OpenNI.h"
#include "imageintrincs.h"
#include "SE3.h"

namespace hal {

class OpenNI2Driver : public CameraDriverInterface
{
    public:
        OpenNI2Driver(unsigned int            nWidth,
                unsigned int            nHeight,
                unsigned int            nFPS,
                bool                    bCaptureRGB,
                bool                    bCaptureDepth,
                bool                    bCaptureIR,
                bool                    bAlignDepth,
                const std::string&      sn,
                const std::string&      scmod);

        virtual ~OpenNI2Driver();

        bool Capture( hal::CameraMsg& vImages );
        std::shared_ptr<CameraDriverInterface> 
          GetInputDevice() { return std::shared_ptr<CameraDriverInterface>(); }

        std::string GetDeviceProperty( const std::string& sProperty );

        size_t NumChannels() const;
        size_t Width( size_t idx = 0 ) const;
        size_t Height( size_t idx = 0 ) const;

    private:

        // NB: this is not a multi-kinect driver.  If you need multi kinects,
        // just use many of these drivers with differnt device URIs.

        void SoftwareAlign( hal::CameraMsg& vImages );
	std::string getSerial(const std::string& Uri) const;
        unsigned int                         m_height;
        unsigned int                         m_width;
        openni::VideoFrameRef                m_depthFrame;
        openni::VideoFrameRef                m_colorFrame;
        openni::VideoFrameRef                m_irFrame;
        openni::Device                       m_device;
        openni::VideoStream                  m_depthStream;
        openni::VideoStream                  m_colorStream;
        openni::VideoStream                  m_irStream;
        openni::VideoMode                    m_depthVideoMode;
        openni::VideoMode                    m_colorVideoMode;
        openni::VideoMode                    m_irVideoMode;
        std::string                          m_sURI;
        ImageIntrinsics                      m_rRGBImgIn;
        ImageIntrinsics                      m_rDepthImgIn;
        std::shared_ptr<calibu::Rig<double>> m_pRig;
        std::vector<uint64_t>                m_SerialNos;
        bool                                 m_bSoftwareAlign;
        double                               m_DepthBaseline;
        double                               m_DepthFocalLength;
        std::vector<openni::VideoStream*>    m_streams;
        std::string                          m_dev_sn;
        std::string                          m_sCameraModel;
};

}
