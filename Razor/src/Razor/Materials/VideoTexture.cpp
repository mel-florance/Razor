#include "rzpch.h"
#include "VideoTexture.h"

#include <glad/glad.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace Razor
{

	VideoTexture::VideoTexture(const std::string& filename) :
		filename(filename),
		id(0),
		active_channel(0),
		max_channels(2),
		device(nullptr),
		thread_enabled(true),
		is_looping(true),
		fps(0.0)
	{
		device = new cv::VideoCapture();
		device->open(filename);
		fps = device->get(cv::CAP_PROP_FPS);

		glGenTextures(1, &id);
		bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	VideoTexture::~VideoTexture()
	{

	}

	void VideoTexture::capture()
	{
		if (device->isOpened())
		{
			*device >> captured_image;

			if (captured_image.empty() && is_looping)
				device->set(cv::CAP_PROP_POS_AVI_RATIO, 0);
		}
	}

	void VideoTexture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void VideoTexture::update()
	{
		bind();

		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			GL_RGB,
			captured_image.cols,
			captured_image.rows, 
			0,
			GL_BGR,
			GL_UNSIGNED_BYTE, 
			captured_image.ptr()
		);
	}

}
