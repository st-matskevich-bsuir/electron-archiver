#ifndef HUFFMAN
#define HUFFMAN

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#include "tree_node.h"
#include "encoding.h"
#include "bit_writer.h"
#include "bit_reader.h"

#define LINE_BUFFER 256
#define FILENAME_BUFFER 256

class huffman {
private:
	std::string _filename;
	FILE* _input;
public:
	huffman(const char* filename);
	~huffman();

	int compress();
	int decompress();

	huffman& operator=(const huffman& other);
};

#endif