#pragma once

#include "Editor/EditorComponent.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace Razor
{
	class Editor;
	class Texture;
	class Event;

	class ImageEditor : public EditorComponent
	{
	public:
		ImageEditor(Editor* editor);
		~ImageEditor();

		enum class BlurType
		{
			SIMPLE,
			GAUSSIAN,
			MEDIAN,
			BILATERAL
		};

		std::vector<std::string> filtersStr = {
			 "Bilateral",
			 "Gaussian blur",
			 "Median blur",
			 "Simple blur",
			 "Box",
			 "Dilate",
			 "Erode",
			 "Laplacian",
			 "Morphology ex",
			 "Scharr",
			 "Smooth",
			 "Sobel"
		};

		enum class Filters
		{
			BILATERAL,
			BLUR_SIMPLE,
			BLUR_GAUSSIAN,
			BLUR_MEDIAN,
			BOX,
			DILATE,
			ERODE,
			LAPLACIAN,
			MORPHOLOGY_EX,
			SCHARR,
			SMOOTH,
			SOBEL
		};

		inline std::string filterToStr(Filters filter)
		{
			switch (filter)
			{
			case Filters::BILATERAL:     return "Bilateral";     break;
			case Filters::BLUR_GAUSSIAN: return "Gaussian blur"; break;
			case Filters::BLUR_MEDIAN:   return "Median blur";   break;
			case Filters::BLUR_SIMPLE:   return "Simple blur";   break;
			case Filters::BOX:           return "Box";           break;
			case Filters::DILATE:        return "Dilate";        break;
			case Filters::ERODE:         return "Erode";         break;
			case Filters::LAPLACIAN:     return "Laplacian";     break;
			case Filters::MORPHOLOGY_EX: return "Morphology ex"; break;
			case Filters::SCHARR:        return "Scharr";        break;
			case Filters::SMOOTH:        return "Smooth";        break;
			case Filters::SOBEL:         return "Sobel";         break;
			}

			return std::string();
		}

		inline Filters strToFilter(const std::string& str)
		{
			if (str == "Bilateral")          return Filters::BILATERAL;
			else if (str == "Gaussian blur") return Filters::BLUR_GAUSSIAN;
			else if (str == "Median blur")   return Filters::BLUR_MEDIAN;
			else if (str == "Simple blur")   return Filters::BLUR_SIMPLE;
			else if (str == "Box")           return Filters::BOX;
			else if (str == "Dilate")        return Filters::DILATE;
			else if (str == "Erode")         return Filters::ERODE;
			else if (str == "Laplacian")     return Filters::LAPLACIAN;
			else if (str == "Morphology ex") return Filters::MORPHOLOGY_EX;
			else if (str == "Scharr")        return Filters::SCHARR;
			else if (str == "Smooth")        return Filters::SMOOTH;
			else if (str == "Sobel")         return Filters::SOBEL;

			return Filters::BILATERAL;
		}

		struct EditorImage
		{
			EditorImage() : 
				path(NULL),
				id(0),
				size(glm::vec2(0.0f))
				{}

			const char* path;
			unsigned int id;
			cv::Mat original_data;
			cv::Mat modified_data;
			glm::vec2 size;
		};

		struct FilterParameter {};

		struct BilateralParameters : public FilterParameter 
		{
			BilateralParameters(int size, float color, float space) :
				kernel_size(size), sigma_color(color), sigma_space(space) {}

			int kernel_size;
			float sigma_color;
			float sigma_space;
		};

		struct GaussianBlurParameters : public FilterParameter
		{
			GaussianBlurParameters(int size, float color, float space) :
				kernel_size(size), sigma_color(color), sigma_space(space) {}

			int kernel_size;
			float sigma_color;
			float sigma_space;
		};

		struct MedianBlurParameters : public FilterParameter
		{
			MedianBlurParameters(int size) :
				kernel_size(size) {}

			int kernel_size;
		};

		struct SimpleBlurParameters : public FilterParameter
		{
			SimpleBlurParameters(int size) :
				kernel_size(size) {}

			int kernel_size;
		};

		void render(float delta) override;
		void onEvent(Event& event) override;

		void loadImage(const char* path);
		void updateImage(EditorImage* image);
		inline void addImage(EditorImage* image) { images.push_back(image); }

	private:
		Filters current_filter;
		EditorImage* current_image;
		std::vector<EditorImage*> images;
		float zoom;
		bool can_zoom;
		bool is_hovered;
		bool show_filter;

		std::map<Filters, FilterParameter*> filters_parameters;
	};

}
