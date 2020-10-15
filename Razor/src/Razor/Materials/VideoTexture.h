#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Razor
{

	class VideoTexture
	{
	public:
		VideoTexture(const std::string& filename);
		~VideoTexture();

		void capture();
		void bind();
		void update();

		inline void switchChannel()
		{
			active_channel = active_channel >= max_channels ? 1 : active_channel++;
		}

		inline cv::Mat& getCapturedImage() { return captured_image; }
		inline unsigned int getId() { return id; }
		inline double getFps() { return fps; }
		inline std::string& getFilename() { return filename; }

	private:
		std::string filename;
		unsigned int id;
		bool thread_enabled;
		bool is_looping;
		cv::Mat captured_image;
		cv::VideoCapture* device;
		unsigned int active_channel;
		unsigned int max_channels;
		double fps;
	};

}
