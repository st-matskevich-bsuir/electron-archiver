{
    "targets": [
        {
            "target_name": "compressor",
            "sources": [ "./dev/compressor/bit_reader.cpp", "./dev/compressor/bit_writer.cpp", "./dev/compressor/encoding.cpp", 
                         "./dev/compressor/tree_node.cpp", "./dev/compressor/huffman.cpp", "./dev/compressor/node_wrapper.cpp" ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}