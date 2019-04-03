#include "rzpch.h"
#include "Variant.h"

namespace Razor {

	Variant::Variant() :
		_type(Type::None),
		_raw(false)
	{
	}

	Variant::Variant(const Variant& other)
		: _type(other._type),
		_raw(other._raw)
	{
		if (_type == Type::Integer)
			_int = other._int;
		else if (_type == Type::UInt)
			_uint = other._uint;
		else if (_type == Type::Float)
			_float = other._float;
		else if (_type == Type::Bool)
			_bool = other._bool;
		else if (_type == Type::String)
			_ptr = new std::string(*(reinterpret_cast<std::string*>(other._ptr)));
		else if (_type == Type::Map)
			_ptr = new std::map<std::string, Variant>(*(reinterpret_cast<std::map<std::string, Variant>*>(other._ptr)));
	}

	Variant::Variant(int n)
		: _type(Type::Integer),
		_raw(false),
		_int(n)
	{}

	Variant::Variant(unsigned int n)
		: _type(Type::UInt),
		_raw(false),
		_uint(n)
	{}

	Variant::Variant(float n)
		: _type(Type::Float),
		_raw(false),
		_float(n)
	{}

	Variant::Variant(bool b)
		: _type(Type::Bool),
		_raw(false),
		_bool(b)
	{}

	Variant::Variant(const char* str)
		: _type(Type::String),
		_raw(false),
		_ptr(new std::string(str))
	{}

	Variant::Variant(const std::string& str)
		: _type(Type::String),
		_raw(false),
		_ptr(new std::string(str))
	{}

	Variant::Variant(const std::map<std::string, Variant>& map)
		: _type(Type::Map),
		_raw(false),
		_ptr(new std::map<std::string, Variant>(map))
	{}

	Variant::~Variant()
	{
		if (_type == Type::String)
			delete reinterpret_cast<std::string*>(_ptr);
		else if (_type == Type::Map)
			delete reinterpret_cast<std::map<std::string, Variant>*>(_ptr);
	}

	std::string Variant::toString() const
	{
		if (_type == Type::Integer)
			return std::to_string(_int);
		else if (_type == Type::Float)
			return std::to_string(_float);
		else if (_type == Type::Bool)
			return _bool ? "true" : "false";
		else if (_type == Type::String)
			return *(reinterpret_cast<std::string*>(_ptr));

		return std::string();
	}

	int Variant::toInt() const
	{
		if (_type == Type::Integer)
			return _int;
		else if (_type == Type::UInt)
			return static_cast<int>(_uint);
		else if (_type == Type::Float)
			return static_cast<int>(_float);
		else if (_type == Type::Bool)
			return static_cast<int>(_bool);
		else if (_type == Type::String)
			return std::stoi(*reinterpret_cast<std::string*>(_ptr));
		else
			throw "Can't convert object to int.";
	}

	uint64_t Variant::toUInt() const
	{
		if (_type == Type::UInt)
			return _uint;
		else if (_type == Type::Integer)
			return static_cast<uint64_t>(_int);
		else if (_type == Type::Float)
			return static_cast<uint64_t>(_float);
		else if (_type == Type::Bool)
			return static_cast<uint64_t>(_bool);
		else if (_type == Type::String)
			return std::stoul(*reinterpret_cast<std::string*>(_ptr));
		else
			throw "Can't convert object to unsigned int.";
	}

	float Variant::toFloat() const
	{
		if (_type == Type::Integer)
			return static_cast<float>(_int);
		else if (_type == Type::UInt)
			return static_cast<float>(_uint);
		else if (_type == Type::Float)
			return static_cast<float>(_float);
		else if (_type == Type::Bool)
			return static_cast<float>(_bool);
		else if (_type == Type::String)
			return std::stof(*reinterpret_cast<std::string*>(_ptr));
		else
			throw "Can't convert object to float.";
	}

	bool Variant::toBool() const
	{
		if (_type == Type::Bool)
			return _bool;
		else if (_type == Type::Integer)
			return static_cast<bool>(_int);
		else if (_type == Type::UInt)
			return static_cast<bool>(_uint);
		else if (_type == Type::Float)
			return static_cast<bool>(_float);
		else
			throw "Can't convert object to bool.";
	}

	const std::map<std::string, Variant>& Variant::toMap() const
	{
		if (_type != Type::Map)
			throw "Can't convert object to map.";

		return *reinterpret_cast<std::map<std::string, Variant>*>(_ptr);
	}

	Variant& Variant::operator = (const Variant& other)
	{
		_type = other._type;
		_raw = other._raw;

		if (_type == Type::Integer)
			_int = other._int;
		else if (_type == Type::UInt)
			_uint = other._uint;
		else if (_type == Type::Float)
			_float = other._float;
		else if (_type == Type::Bool)
			_bool = other._bool;
		else if (_type == Type::String)
			_ptr = new std::string(*(reinterpret_cast<std::string*>(other._ptr)));
		else if (_type == Type::Map)
			_ptr = new std::map<std::string, Variant>(*(reinterpret_cast<std::map<std::string, Variant>*>(
				other._ptr)));

		return *this;
	}

	bool Variant::operator==(const Variant& other)
	{
		_type = other._type;
		_raw = other._raw;

		if (_type == Type::Integer)
			return _int == other._int;
		else if (_type == Type::UInt)
			return _uint == other._uint;
		else if (_type == Type::Float)
			return _float == other._float;
		else if (_type == Type::Bool)
			return _bool == other._bool;
		else if (_type == Type::String)
			return _ptr == new std::string(*(reinterpret_cast<std::string*>(other._ptr)));
		else if (_type == Type::Map)
			return _ptr == new std::map<std::string, Variant>(*(reinterpret_cast<std::map<std::string, Variant>*>(
				other._ptr)));

		return false;
	}

	Variant::Type Variant::type() const
	{
		return _type;
	}

	void Variant::setRaw(bool raw)
	{
		_raw = raw;
	}

	std::string Variant::typeString() const
	{
		static std::map<int, std::string> types;
		static bool init = false;

		if (!init)
		{
			types[static_cast<int>(Type::Integer)] = "Integer";
			types[static_cast<int>(Type::UInt)]    = "UInt";
			types[static_cast<int>(Type::Float)]   = "Float";
			types[static_cast<int>(Type::Bool)]    = "Bool";
			types[static_cast<int>(Type::String)]  = "String";
			types[static_cast<int>(Type::Map)]     = "Map";
			init = true;
		}

		return types[static_cast<int>(_type)];
	}

}