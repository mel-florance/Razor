#include "rzpch.h"
#include "imgui.h"
#include "ImageEditor.h"
#include "Editor/Editor.h"
#include "Razor/Materials/Texture.h"
#include "Razor/Core/Utils.h"
#include "Razor/Events/Event.h"

#include <glad/glad.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace Razor
{

	ImageEditor::ImageEditor(Editor* editor) :
		EditorComponent(editor),
		images({}), 
		current_image(nullptr),
		current_filter(Filters::BILATERAL),
		show_filter(false),
		can_zoom(false),
		zoom(1.0f)
	{
		filters_parameters[Filters::BILATERAL] = new BilateralParameters { 9, 2.0, 0.5 };
		filters_parameters[Filters::BLUR_GAUSSIAN] = new GaussianBlurParameters{ 9, 2.0, 0.5 };
		filters_parameters[Filters::BLUR_MEDIAN] = new MedianBlurParameters{ 9 };
		filters_parameters[Filters::BLUR_SIMPLE] = new SimpleBlurParameters{ 9 };

		active = false;
	}

	ImageEditor::~ImageEditor()
	{
	}

	void ImageEditor::render(float delta)
	{
		can_zoom = Input::IsKeyPressed(RZ_KEY_LEFT_CONTROL);

		bool open;
		int flags =
			ImGuiWindowFlags_AlwaysHorizontalScrollbar |
			ImGuiWindowFlags_AlwaysVerticalScrollbar |
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoScrollWithMouse;

		if (can_zoom)
			flags |= ImGuiWindowFlags_NoScrollWithMouse;
		else
			flags &= ~ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::Begin("Image editor", &open, flags);

		is_hovered = ImGui::IsWindowHovered();

		auto win_size = ImGui::GetWindowSize();
		ImGui::Dummy(ImVec2(win_size.x, win_size.y - 22.0f));
		auto rect_pos = ImGui::GetItemRectMin();
		auto rect_max = ImGui::GetItemRectMax();
		auto rect_size = ImVec2(rect_pos.x + (win_size.x / 2) + 30.0f, rect_pos.y + (win_size.y / 2));

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Image"))
			{
				ImGui::MenuItem("Open image...");

				if (ImGui::IsItemClicked())
				{
					std::string file = Utils::fileDialog();

					if (file.size() > 0)
						loadImage(file.c_str());
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Filters"))
			{
				for (auto f : filtersStr)
				{
					ImGui::MenuItem(f.c_str());

					if (ImGui::IsItemClicked()) {
						show_filter = true;
						current_filter = strToFilter(f);
					}
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (current_image != nullptr)
		{
			ImGui::SetCursorPos(ImVec2(
				(ImGui::GetWindowWidth() * 0.5f) - ((current_image->size.x * zoom) * 0.5f),
				(ImGui::GetWindowHeight() * 0.5f) - ((current_image->size.y * zoom) * 0.5f)
			));
			ImGui::Image((void*)(intptr_t)current_image->id, ImVec2(current_image->size.x * zoom, current_image->size.y * zoom));
		}
			
		if (can_zoom && is_hovered)
		{
			ImGui::GetWindowDrawList()->AddRectFilled(
				ImVec2(rect_pos.x + (win_size.x * 0.5f) - (30.0f + 25.0f), rect_pos.y + (win_size.y * 0.5f) - (40.0f)),
				rect_size,
				IM_COL32(30, 30, 30, 128)
			);

			std::string str = std::to_string((int)(zoom * 100.0f)) + " %";
			const char* text = str.c_str();
			ImVec2 text_size = ImGui::CalcTextSize(text);
			ImGui::SetCursorPos(ImVec2((win_size.x * 0.5f) - (text_size.x * 0.5f) - 19.0f, (win_size.y * 0.5f) - (text_size.y * 0.5f) + 22.0f));
			ImGui::SetWindowFontScale(1.5f);
			ImGui::Text("%s", text);
			ImGui::SetWindowFontScale(1.0f);
		}

		if (show_filter)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			std::string filter_name = "Filter " + filterToStr(current_filter);
			if (ImGui::Begin(filter_name.c_str(), &show_filter, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse/* | ImGuiWindowFlags_NoResize*/))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 100.0f);
					initial_column_spacing++;
				}

				if (current_filter == Filters::BILATERAL) 
				{
					BilateralParameters* params = (BilateralParameters*)filters_parameters[current_filter];
					ImGui::Text("Kernel size");
					ImGui::NextColumn();
					ImGui::DragInt("##bilateral_kernel_size", &params->kernel_size);

					ImGui::NextColumn();

					ImGui::Text("Sigma color");
					ImGui::NextColumn();
					ImGui::DragFloat("##bilateral_sigma_color", &params->sigma_color);

					ImGui::NextColumn();

					ImGui::Text("Sigma space");
					ImGui::NextColumn();
					ImGui::DragFloat("##bilateral_sigma_space", &params->sigma_space);
				}
				else if (current_filter == Filters::BLUR_GAUSSIAN)
				{
					GaussianBlurParameters* params = (GaussianBlurParameters*)filters_parameters[current_filter];
					ImGui::Text("Kernel size");
					ImGui::NextColumn();
					ImGui::DragInt("##bilateral_kernel_size", &params->kernel_size);

					ImGui::NextColumn();

					ImGui::Text("Sigma color");
					ImGui::NextColumn();
					ImGui::DragFloat("##gaussian_blur_sigma_color", &params->sigma_color);

					ImGui::NextColumn();

					ImGui::Text("Sigma space");
					ImGui::NextColumn();
					ImGui::DragFloat("##gaussian_blur_sigma_space", &params->sigma_space);
				}
				else if (current_filter == Filters::BLUR_MEDIAN)
				{
					MedianBlurParameters* params = (MedianBlurParameters*)filters_parameters[current_filter];
					ImGui::Text("Kernel size");
					ImGui::NextColumn();
					ImGui::DragInt("##median_blur_kernel_size", &params->kernel_size);
				}
				else if (current_filter == Filters::BLUR_SIMPLE)
				{
					SimpleBlurParameters* params = (SimpleBlurParameters*)filters_parameters[current_filter];
					ImGui::Text("Kernel size");
					ImGui::NextColumn();
					ImGui::DragInt("##simple_blur_kernel_size", &params->kernel_size);
				}

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0, 50.0f));

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button(("Apply " + filterToStr(current_filter)).c_str(), ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					show_filter = false;
					cv::Mat mat = cv::Mat();

					if (current_filter == Filters::BILATERAL)
					{
						BilateralParameters* params = (BilateralParameters*)filters_parameters[current_filter];
						int size = params->kernel_size % 2 == 0 ? params->kernel_size + 1 : params->kernel_size;
						cv::bilateralFilter(
							current_image->modified_data,
							mat,
							params->kernel_size,
							(double)(params->kernel_size * params->sigma_color),
							(double)(params->kernel_size * params->sigma_space)
						);
						current_image->modified_data = mat;
						mat.release();
					}
					else if (current_filter == Filters::BLUR_GAUSSIAN)
					{
						GaussianBlurParameters* params = (GaussianBlurParameters*)filters_parameters[current_filter];
						int size = params->kernel_size % 2 == 0 ? params->kernel_size + 1 : params->kernel_size;
						cv::GaussianBlur(
							current_image->modified_data,
							current_image->modified_data,
							cv::Size(params->kernel_size, params->kernel_size),
							params->sigma_color,
							params->sigma_space
						);
					}
					else if (current_filter == Filters::BLUR_MEDIAN)
					{
						MedianBlurParameters* params = (MedianBlurParameters*)filters_parameters[current_filter];
						int size = params->kernel_size % 2 == 0 ? params->kernel_size + 1 : params->kernel_size;
						cv::medianBlur(current_image->modified_data, current_image->modified_data, size);
					}
					else if (current_filter == Filters::BLUR_SIMPLE)
					{
						SimpleBlurParameters* params = (SimpleBlurParameters*)filters_parameters[current_filter];
						int size = params->kernel_size % 2 == 0 ? params->kernel_size + 1 : params->kernel_size;
						cv::blur(current_image->modified_data, current_image->modified_data, cv::Size(params->kernel_size, params->kernel_size));
					}
					else if (current_filter == Filters::BOX)
					{}
					else if (current_filter == Filters::DILATE)
					{}
					else if (current_filter == Filters::ERODE)
					{}
					else if (current_filter == Filters::LAPLACIAN)
					{}
					else if (current_filter == Filters::MORPHOLOGY_EX)
					{}
					else if (current_filter == Filters::SCHARR)
					{}
					else if (current_filter == Filters::SMOOTH)
					{}
					else if (current_filter == Filters::SOBEL)
					{}

					updateImage(current_image);
				}

				ImGui::End();
			}
		}
		
		ImGui::End();
	}

	void ImageEditor::onEvent(Event& event)
	{
		if (event.GetEventType() == EventType::MouseScrolled && can_zoom && is_hovered){
			MouseScrolledEvent& e = (MouseScrolledEvent&)event;

			zoom += e.GetYOffset() / 5.0f;

			if (zoom < EPSILON)
				zoom = EPSILON;
		}
	}

	void ImageEditor::loadImage(const char* path)
	{
		EditorImage* image = new EditorImage();
		image->path = path;

		cv::Mat data = cv::imread(path);
		cv::cvtColor(data, image->original_data, cv::COLOR_BGR2RGB);
		data.release();

		image->size = glm::vec2(image->original_data.cols, image->original_data.rows);
		image->modified_data = image->original_data;

		addImage(image);
		current_image = image;

		glGenTextures(1, &image->id);
		glBindTexture(GL_TEXTURE_2D, image->id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		updateImage(image);
	}

	void ImageEditor::updateImage(EditorImage* image)
	{
		glBindTexture(GL_TEXTURE_2D, image->id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)image->size.x, (GLsizei)image->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, image->modified_data.ptr());
	}

}
