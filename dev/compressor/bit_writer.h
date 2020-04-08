#ifndef BIT_WRITER
#define BIT_WRITER

#define WRITER_BUFFER 1024

#include <string>
#include <fstream>

class bit_writer
{
private:
	FILE* file;
	uint32_t offset;
	uint8_t* bytes;
public:
	bit_writer(FILE* file);
	~bit_writer();

	uint32_t get_offset();
	void write_bit(uint8_t byte);
	void write_byte(uint8_t byte);
	void force_write();
};

#endif // BIT_WRITER
