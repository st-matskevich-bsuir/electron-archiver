#include "encoding.h"

void traverse_tree(const tree_node& item, std::vector<encoding>& enc, uint8_t* cur_encoding)
{
    static size_t count = -1;
    count++;
    if(item.left != 0 && item.right != 0)
    {
        cur_encoding[count / 8] &= ((1 << (count % 8)) - 1);
        traverse_tree(*item.left, enc, cur_encoding);

        cur_encoding[count / 8] |= (1 << (count % 8));
        traverse_tree(*item.right, enc, cur_encoding);
        count--;
    }
    else
    {
        count--;
        cur_encoding[count / 8] &= ((1 << ((count % 8) + 1)) - 1);
        memcpy(enc[item.character].bytes, cur_encoding, count / 8 + 1);
        enc[item.character].size = count / 8 + 1;
        enc[item.character].end = count % 8;
    }

}

std::vector<encoding> encode(const tree_node& item)
{
	std::vector<encoding> enc;
	enc.resize(ALPHABET_SIZE);
    for(size_t i = 0; i < ALPHABET_SIZE; i++)
    {
        enc[i].bytes = new uint8_t[(2 * ALPHABET_SIZE - 1) / 8];
        enc[i].size = 0;
        enc[i].end = 0;
    }

	uint8_t cur_encoding[(2 * ALPHABET_SIZE - 1) / 8];
    traverse_tree(item, enc, cur_encoding);

    return enc;
}

tree_node decode(const std::vector<encoding>& enc)
{
    tree_node res, *current;
    res.left = 0;
    res.right = 0;
    for(size_t i = 0; i < ALPHABET_SIZE; i++)
    {
        current = &res;
        for(uint32_t j = 0; j <= (enc[i].size - 1) * 8 + enc[i].end; j++)
        {
            if(((enc[i].bytes[j / 8] >> (j % 8)) & 1) > 0)
            {
                if(current->right == 0)
                {
                    current->right = new tree_node();
                }
                current = current->right;
            }
            else
            {
                if(current->left == 0)
                {
                    current->left = new tree_node();
                }
                current = current->left;
            }
        }
        current->character = i;
    }

    return res;
}

void write_encoding(bit_writer& wrt, const std::vector<encoding>& enc)
{
    //int size = 0; //counts header size for debugging purposes
    for(size_t i = 0; i < ALPHABET_SIZE; i++)
    {
        wrt.write_byte(enc[i].size);
        wrt.write_byte(enc[i].end);

        //size += (enc[i].size + 1) * 8;
        //size += enc[i].end + 1;

        for(uint8_t j = 0; j < enc[i].size - 1; j++)
            wrt.write_byte(enc[i].bytes[j]);

        int last = enc[i].size - 1;
        for(uint8_t k = 0; k <= enc[i].end; k++)
            wrt.write_bit(enc[i].bytes[last] >> k);
    }

}

std::vector<encoding> read_encoding(bit_reader& rdr)
{
	std::vector<encoding> enc;
	enc.resize(ALPHABET_SIZE);
    for(size_t i = 0; i < ALPHABET_SIZE; i++)
    {
        enc[i].size = rdr.read_byte();
        enc[i].end = rdr.read_byte();
        enc[i].bytes = new uint8_t[enc[i].size];

        for(int j = 0; j < enc[i].size - 1; j++)
            enc[i].bytes[j] = rdr.read_byte();

		uint8_t last = enc[i].size - 1;
        enc[i].bytes[last] = 0;
        for(int k = 0; k <= enc[i].end; k++)
            enc[i].bytes[last] |= rdr.read_bit() << k;
    }
    return enc;
}

encoding::~encoding()
{
	delete[] bytes;
}