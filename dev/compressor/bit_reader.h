#ifndef BIT_READER
#define BIT_READER

#define READER_BUFFER 1024

#include <string>
#include <fstream>

class bit_reader
{
    private:
        FILE* file;
        uint32_t offset, buffered;
        uint8_t* bytes;
    public:
		bit_reader(FILE* file);
		~bit_reader();

		uint32_t get_offset();
		uint32_t get_buffered();
		uint8_t read_bit();
		uint8_t read_byte();
};

#endif // BIT_READER
