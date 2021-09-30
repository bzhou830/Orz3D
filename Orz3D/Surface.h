#pragma once
#include "opencv2/core.hpp"
#include "opencv2/core/directx.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace cv;

class Surface
{
public:
    Surface(std::string str)
    {
        std::string::size_type pos = str.rfind('.');
        std::string ext = str.substr(pos == std::string::npos ? str.length() : pos + 1);
        if (ext == "mp4")
        {
            if (m_cap.isOpened() == false)
                m_cap.open(str);

            //������Ƶ�Ļ�ȡ��λ��
            m_cap.set(CAP_PROP_POS_MSEC, static_cast<double>(rand() % 20) * 1000.f);
            if (!m_cap.read(m_frame))
                assert(0);
            cv::cvtColor(m_frame, m_frame, cv::COLOR_RGB2RGBA);
            cv::resize(m_frame, m_frame, cv::Size(800, 600));
        }
        else if (ext == "png")
        {
            m_frame = cv::imread(str);
            cv::cvtColor(m_frame, m_frame, cv::COLOR_RGB2RGBA);
            cv::resize(m_frame, m_frame, cv::Size(800, 600));
        }
    }
    ~Surface()
    {
        ;
    }

    void Update()
    {
        cv::Mat tmp;
        if (!m_cap.read(tmp))
            assert(0);
        cv::cvtColor(tmp, tmp, cv::COLOR_RGB2RGBA);
        cv::resize(tmp, tmp, cv::Size(800, 600));
        m_frame = tmp.clone();
    }

    const uchar* GetBufferPtr() const noexcept
    {
        return m_frame.data;
    }

    unsigned int GetWidth() const noexcept
    {
        return m_frame.cols;
    }
    unsigned int GetHeight() const noexcept
    {
        return m_frame.rows;
    }
private:
    cv::VideoCapture m_cap;
    cv::Mat          m_frame;
};


