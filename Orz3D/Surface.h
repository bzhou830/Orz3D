#pragma once
#include "opencv2/core.hpp"
#include "opencv2/core/directx.hpp"
#include "opencv2/core/ocl.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

using namespace cv;

class Surface
{
public:
    static VideoCapture m_cap;
    static Mat m_frame_bgr;
    static Surface FromCV(std::string str)
    {
        if (m_cap.isOpened() == false)
            m_cap.open(str);

        //设置视频的获取的位置
        m_cap.set(CAP_PROP_POS_MSEC, static_cast<double>(rand() % 20) * 1000.f);
        if (!m_cap.read(m_frame_bgr))
            assert(0);
        cv::cvtColor(m_frame_bgr, m_frame_bgr, cv::COLOR_RGB2RGBA);
        cv::resize(m_frame_bgr, m_frame_bgr, cv::Size(800, 600));

		return Surface(m_frame_bgr);
    }

    void Update()
    {
        cv::Mat tmp;
        if (!m_cap.read(tmp))
            assert(0);
        cv::cvtColor(tmp, tmp, cv::COLOR_RGB2RGBA);
        cv::resize(tmp, tmp, cv::Size(800, 600));
        //src = tmp.clone();
    }

    Surface(cv::Mat s)
    {
        src = s.clone();
    }

    const uchar* GetBufferPtr() const noexcept
    {
        return src.data;
    }

    unsigned int GetWidth() const noexcept
    {
        return src.cols;
    }
    unsigned int GetHeight() const noexcept
    {
        return src.rows;
    }
private:
    cv::Mat src;
};


