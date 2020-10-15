#pragma once

#include "Razor/Filesystem/Serializer.h"

namespace Razor
{

	class Asset : public Serializer
	{
	public:
		Asset(const std::string& path);
		virtual ~Asset();

		inline std::string& getPath() { return path; }
		inline void setPath(const std::string& str) { path = str; }

		virtual void load() {}
		virtual void serialize() {}
		virtual void unserialize() {}

	protected:
		std::string path;
	};

}
