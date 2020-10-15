#pragma once

#include "Razor/Core/Core.h"

namespace Razor {

	class RAZOR_API Texture
	{
	public:
		enum ChannelType 
		{
			GREY = 1,
			GREY_ALPHA = 2,
			RGB = 3,
			RGB_ALPHA = 4
		};

		enum class Type
		{
			Diffuse,
			Specular,
			Normal
		};

		enum Filter
		{
			NEAREST = 0x2600,
			LINEAR = 0x2601,
			NEAREST_MIPMAP_NEAREST = 0x2700,
			LINEAR_MIPMAP_NEAREST = 0x2701,
			NEAREST_MIPMAP_LINEAR = 0x2702,
			LINEAR_MIPMAP_LINEAR = 0x2703
		};

		enum WrapType
		{
			REPEAT = 0x2901,
			MIRRORED_REPEAT = 0x8370,
			CLAMP_TO_EDGE = 0x812F
		};

		Texture(
			const std::string& filename,
			bool mimaps = false,
			bool flipped = true, 
			ChannelType type = ChannelType::RGB_ALPHA,
			bool free_after_load = true
		);
		virtual ~Texture();

		Texture* load();
		void bind(unsigned int unit);
		void unbind();

		inline unsigned int getId() { return id; }
		inline float& getLodBias() { return lodBias; }
		inline std::string getFilename() { return filename; }
		inline bool isFlipped() { return flipped; }
		inline unsigned char* getData() { return data; }
		inline bool hasMipmaps() { return mipmaps; }
		inline int getWidth() { return width; }
		inline int getHeight() { return height; }
		inline int getComponentsCount() { return components_count; }
		inline ChannelType getChannelType() { return channel_type; }
		inline Filter& getMinFilter() { return min_filter; }
		inline Filter& getMaxFilter() { return mag_filter; }
		inline WrapType& getWrapS() { return wrap_s; }
		inline WrapType& getWrapT() { return wrap_t; }

		inline void setId(unsigned int id) { this->id = id; }
		inline void setLoadBias(float& value) { lodBias = value; }
		inline void setFilename(const std::string& name) { filename = name; }
		inline void setFlipped(bool value) { flipped = value; }
		inline void setdata(unsigned char* data) { this->data = data; }
		inline void setHavingMipmaps(bool value) { mipmaps = value; }
		inline void setWidth(int width) { this->width = width; }
		inline void setHeight(int height) { this->height = height; }
		inline void setComponentsCount(int count) { components_count = count; }
		inline void setChannelType(ChannelType type) { channel_type = type; }
		inline void setMinFilter(Filter filter) { min_filter = filter; }
		inline void setMagFilter(Filter filter) { mag_filter = filter; }
		inline void setWrapS(WrapType wrap) { wrap_s = wrap; }
		inline void setWrapT(WrapType wrap) { wrap_t = wrap; }

	private:
		unsigned char* data;
		unsigned int id;
		std::string filename;
		bool mipmaps;
		float lodBias;
		bool flipped;
		int components_count;
		int width;
		int height;
		ChannelType channel_type;
		bool free_after_load;
		Filter min_filter;
		Filter mag_filter;
		WrapType wrap_s;
		WrapType wrap_t;
	};

}
