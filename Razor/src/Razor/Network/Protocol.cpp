#include "rzpch.h"
#include "Protocol.h"

#include "Razor/Filesystem/BinaryReader.h"

void Protocol::write_byte(Buffer& buffer, uint8_t value)
{
	assert(buffer.index + 4 <= buffer.size);

	if (Razor::BinaryReader::isBigEndian())
		*((uint8_t*)(buffer.data + buffer.index)) = Protocol::swapByteOrder(value);
	else
		*((uint8_t*)(buffer.data + buffer.index)) = value;

	buffer.index += 4;
}

void Protocol::write_short(Buffer& buffer, uint16_t value)
{
	assert(buffer.index + 4 <= buffer.size);

	if (Razor::BinaryReader::isBigEndian())
		*((uint16_t*)(buffer.data + buffer.index)) = Protocol::swapByteOrder(value);
	else
		*((uint16_t*)(buffer.data + buffer.index)) = value;

	buffer.index += 4;
}

void Protocol::write_int(Buffer& buffer, uint32_t value)
{
	assert(buffer.index + 4 <= buffer.size);

	if (Razor::BinaryReader::isBigEndian())
		*((uint32_t*)(buffer.data + buffer.index)) = Protocol::swapByteOrder(value);
	else
		*((uint32_t*)(buffer.data + buffer.index)) = value;

	buffer.index += 4;
}

void Protocol::write_string(Buffer& buffer, const std::string& value)
{
	assert(buffer.index + 4 <= buffer.size);

	*((char*)(buffer.data + buffer.index)) = *value.c_str();

	buffer.index += 4;
}

void Protocol::read_byte(Buffer& buffer, uint8_t* value)
{
	assert(buffer.index + 4 <= buffer.size);
	uint8_t out;

	if (Razor::BinaryReader::isBigEndian())
		out = Protocol::swapByteOrder(*((uint8_t*)(buffer.data + buffer.index)));
	else
		out = *((uint8_t*)(buffer.data + buffer.index));

	buffer.index += 4;
	*value = out;
}

void Protocol::read_short(Buffer& buffer, uint16_t* value)
{
	assert(buffer.index + 4 <= buffer.size);
	uint16_t out;

	if (Razor::BinaryReader::isBigEndian())
		out = Protocol::swapByteOrder(*((uint16_t*)(buffer.data + buffer.index)));
	else
		out = *((uint16_t*)(buffer.data + buffer.index));

	buffer.index += 4;
	*value = out;
}

void Protocol::read_int(Buffer& buffer, uint32_t* value)
{
	assert(buffer.index + 4 <= buffer.size);
	uint32_t out;

	if (Razor::BinaryReader::isBigEndian())
		out = Protocol::swapByteOrder(*((uint32_t*)(buffer.data + buffer.index)));
	else
		out = *((uint32_t*)(buffer.data + buffer.index));
	
	buffer.index += 4;
	*value = out;
}

void Protocol::read_string(Buffer& buffer, std::string* value)
{
	assert(buffer.index + 4 <= buffer.size);
	std::string out;

	out = *((char*)(buffer.data + buffer.index));

	buffer.index += 4;
	*value = out;
}
