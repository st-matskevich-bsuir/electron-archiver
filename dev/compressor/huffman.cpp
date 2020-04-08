#include "huffman.h"

huffman::huffman(const char* filename)
{
	_input = fopen(filename, "rb");
	_filename = filename;
}

huffman::~huffman()
{
	if (_input != NULL)
		fclose(_input);
}

int huffman::compress()
{
	if (_input == NULL)
		return -1;

	std::vector<tree_node> chars;
	chars.resize(ALPHABET_SIZE);
	for (size_t i = 0; i < chars.size(); i++)
	{
		chars[i].character = i;
	}

	uint8_t line[LINE_BUFFER];
	while (!feof(_input))
	{
		size_t count = fread(line, sizeof(uint8_t), LINE_BUFFER, _input);
		for (size_t i = 0; i < count; i++)
		{
			chars[line[i]].cost++;
		}
	}

	tree_node tree = build_tree(chars);
	std::vector<encoding> table = encode(tree);

	std::string compressed_name = _filename + ".compressed";
	FILE* compressed_file = fopen(compressed_name.c_str(), "wb");
	if (compressed_file == NULL)
		return -2;

	bit_writer wrt(compressed_file);
	write_encoding(wrt, table);

	rewind(_input);
	size_t size = 0;
	while (!feof(_input))
	{
		size_t count = fread(line, sizeof(uint8_t), LINE_BUFFER, _input);
		for (size_t i = 0; i < count; i++)
		{
			size += (table[line[i]].size - 1) * 8;
			size += table[line[i]].end + 1;
		}
	}
	uint8_t end = (size + wrt.get_offset()) % 8;
	wrt.write_byte(end);

	rewind(_input);
	while (!feof(_input))
	{
		size_t count = fread(line, sizeof(uint8_t), LINE_BUFFER, _input);
		for (size_t i = 0; i < count; i++)
		{
			for (uint8_t j = 0; j < table[line[i]].size - 1; j++)
				wrt.write_byte(table[line[i]].bytes[j]);

			int last = table[line[i]].size - 1;
			for (uint8_t k = 0; k <= table[line[i]].end; k++)
				wrt.write_bit(table[line[i]].bytes[last] >> k);
		}
	}
	wrt.force_write();

	tree.free_tree();
	fclose(compressed_file);

	return 0;
}

int huffman::decompress()
{
	if (_input == NULL)
		return -1;

	bit_reader rdr(_input);
	std::vector<encoding> table = read_encoding(rdr);
	tree_node tree = decode(table);
	uint8_t padding = rdr.read_byte();

	std::string decompressed_name = _filename + ".decompressed";
	FILE* decompressed_file = fopen(decompressed_name.c_str(), "wb");

	uint8_t line[LINE_BUFFER];
	size_t position = 0;
	tree_node* current = &tree;
	while (feof(_input) == 0 || rdr.get_buffered() != rdr.get_offset() / 8 + 1 || rdr.get_offset() % 8 != padding)
	{
		uint8_t bit = rdr.read_bit();
		if (bit > 0)
			current = current->right;
		else
			current = current->left;

		if (current->left == 0 && current->right == 0)
		{
			line[position] = current->character;
			position++;
			if (position == LINE_BUFFER)
			{
				fwrite(line, sizeof(uint8_t), LINE_BUFFER, decompressed_file);
				position = 0;
			}
			current = &tree;
		}
	}
	fwrite(line, sizeof(uint8_t), position, decompressed_file);

	tree.free_tree();
	fclose(decompressed_file);
	return 0;
}

huffman& huffman::operator=(const huffman& other)
{
	_input = fopen(other._filename.c_str(), "rb");
	_filename = other._filename;
	return *this;
}
