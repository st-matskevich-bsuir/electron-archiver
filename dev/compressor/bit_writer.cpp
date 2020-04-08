#include "bit_writer.h"

bit_writer::bit_writer(FILE * file)
{
	this->file = file;
	this->bytes = new uint8_t[WRITER_BUFFER];
	this->offset = 0;
}

bit_writer::~bit_writer()
{
	delete[] this->bytes;
}

uint32_t bit_writer::get_offset()
{
	return this->offset;
}

void bit_writer::write_bit(uint8_t byte)
{
	if (this->offset % 8 == 0)
		this->bytes[this->offset / 8] = 0;

	this->bytes[this->offset / 8] |= (byte & 1) << (this->offset % 8);
	this->offset++;

	if (this->offset == WRITER_BUFFER * 8)
	{
		fwrite(&this->bytes[0], sizeof(uint8_t), WRITER_BUFFER, this->file);
		this->offset = 0;
	}
}

void bit_writer::write_byte(uint8_t byte)
{
	for (int i = 0; i < 8; i++)
		this->write_bit(byte >> i);
}

void bit_writer::force_write()
{
	this->offset--;
	fwrite(&this->bytes[0], sizeof(uint8_t), this->offset / 8 + 1, this->file);
	this->offset = 0;
}
