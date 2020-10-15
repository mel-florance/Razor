#pragma once

#include "Razor/Core/Core.h"
#include <glm/glm.hpp>

namespace Razor {

	class RAZOR_API Variant
	{
	public:
		Variant();

		Variant(const Variant& other);
		Variant(int n);
		Variant(unsigned int n);
		Variant(float n);
		Variant(bool b);
		Variant(const char* str);
		Variant(const std::string& str);
		Variant(const std::map<std::string, Variant>& map);
		Variant(const glm::vec2& vec);
		Variant(const glm::vec3& vec);
		Variant(const glm::vec4& vec);
		Variant(const glm::mat2& mat);
		Variant(const glm::mat3& mat);
		Variant(const glm::mat4& mat);

		~Variant();

		enum class Type {
			None,
			Integer,
			UInt,
			Float,
			Bool,
			String, 
			Map,
			Vec2,
			Vec3,
			Vec4,
			Mat2,
			Mat3,
			Mat4
		};

		void setRaw(bool raw = true);
		std::string toString() const;
		int toInt() const;
		int& toInt();
		uint64_t toUInt() const;
		float toFloat() const;
		float& toFloat();
		bool toBool() const;
		const std::map<std::string, Variant>& toMap() const;
		glm::vec2 toVec2() const;
		glm::vec3 toVec3() const;
		glm::vec4 toVec4() const;
		glm::mat2 toMat2() const;
		glm::mat3 toMat3() const;
		glm::mat4 toMat4() const;

		Type type() const;
		std::string typeString() const;

		inline void* getPtr() { return _ptr; }

		inline int& getInt() { return _int; }
		inline float& getFloat() { return _float; }

		Variant& operator = (const Variant& other);
		bool operator == (const Variant& other);

	private:
		Type _type;
		bool _raw;
		union
		{
			int _int;
			unsigned int _uint;
			float _float;
			bool _bool;
			void* _ptr;
		};
	};

}
