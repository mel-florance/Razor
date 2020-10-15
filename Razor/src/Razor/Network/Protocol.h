#pragma once

#define BIG_ENDIAN (((union { unsigned x; unsigned char c; }){1}).c)

class Protocol
{
public:
	struct Buffer
	{
		uint8_t* data;
		int size;
		int index;
	};

	inline static uint8_t swapByteOrder(uint8_t value) {
		return (value >> 4);
	}

	inline static uint16_t swapByteOrder(uint16_t value) {
		return (value >> 8) | (value << 8);
	}

	inline static uint32_t swapByteOrder(uint32_t ui) {
		return (ui >> 24) 
			| ((ui << 8) & 0x00ff0000) 
			| ((ui >> 8) & 0x0000ff00) 
			| (ui << 24);
	}

	inline static uint64_t swapByteOrder(uint64_t value) {
		return (value >> 56) 
			| ((value << 40) & 0x00ff000000000000) 
			| ((value << 24) & 0x0000ff0000000000) 
			| ((value << 8)  & 0x000000ff00000000) 
			| ((value >> 8)  & 0x00000000ff000000) 
			| ((value >> 24) & 0x0000000000ff0000) 
			| ((value >> 40) & 0x000000000000ff00) 
			| (value << 56);
	}

	static void write_byte(Buffer& buffer, uint8_t value);
	static void write_short(Buffer& buffer, uint16_t value);
	static void write_int(Buffer& buffer, uint32_t value);
	static void write_string(Buffer& buffer, const std::string& value);

	static void read_byte(Buffer& buffer, uint8_t* value);
	static void read_short(Buffer& buffer, uint16_t* value);
	static void read_int(Buffer& buffer, uint32_t* value);
	static void read_string(Buffer& buffer, std::string* value);
};

