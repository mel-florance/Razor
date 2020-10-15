#pragma once

namespace Razor
{

	class EnvironmentTexture
	{
	public:
		EnvironmentTexture(const std::string& filename, bool flipped = false);
		~EnvironmentTexture();

		void load();

		inline unsigned int getId() { return id; }

	private:
		unsigned int id;
		int width;
		int height;
		int nb_components;
		std::string filename;
		bool flipped;
	};

}
