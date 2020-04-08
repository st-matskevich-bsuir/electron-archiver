#ifndef ENCODING
#define ENCODING

#include <vector>
#include <string.h>

#include "bit_writer.h"
#include "bit_reader.h"
#include "tree_node.h"

#define ALPHABET_SIZE 256

class encoding{
public:
	uint8_t* bytes;
	uint8_t size, end;

	~encoding();
};


std::vector<encoding> encode(const tree_node& item);
tree_node decode(const std::vector<encoding>& enc);
void write_encoding(bit_writer& wrt, const std::vector<encoding>& enc);
std::vector<encoding> read_encoding(bit_reader& rdr);


#endif // ENCODING
