#include "bit_reader.h"

bit_reader::bit_reader(FILE * file)
{
	this->file = file;
	this->bytes = new uint8_t[READER_BUFFER];
	this->offset = 0;
	this->buffered = 0;
}

bit_reader::~bit_reader()
{
	delete[] this->bytes;
}

uint32_t bit_reader::get_offset()
{
	return this->offset;
}

uint32_t bit_reader::get_buffered()
{
	return this->buffered;
}

uint8_t bit_reader::read_bit()
{
	if (this->offset == 0 || this->offset >= this->buffered * 8)
		this->buffered = fread(&this->bytes[0], sizeof(uint8_t), READER_BUFFER, this->file);

	uint8_t res = (this->bytes[this->offset / 8] & (1 << (this->offset % 8))) >> (this->offset % 8);
	this->offset++;
	this->offset %= READER_BUFFER * 8;
	return res;
}

uint8_t bit_reader::read_byte()
{
	uint8_t res = 0;
	for (int i = 0; i < 8; i++)
		res |= this->read_bit() << i;

	return res;
}
