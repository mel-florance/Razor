#pragma once

#include "Razor/Core/Core.h"

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

		~Variant();

		enum class Type {
			None, Integer, UInt, Float, Bool, String, Map
		};

		void setRaw(bool raw = true);
		std::string toString() const;
		int toInt() const;
		uint64_t toUInt() const;
		float toFloat() const;
		bool toBool() const;
		const std::map<std::string, Variant>& toMap() const;

		Type type() const;
		std::string typeString() const;

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
