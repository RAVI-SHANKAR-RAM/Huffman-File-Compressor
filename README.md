# Huffman-File-Compressor
A high-efficiency, bit-level file compressor and decompressor using Huffman Coding, written in modern C++. This tool provides accurate compression statistics, supports various file types, and demonstrates key data structures like priority queues and binary trees.

Ideal for performance-focused systems, systems programming practice, and academic demonstrations of lossless compression.

Features
Compress and decompress any file using Huffman Coding

Shows compression statistics:

Original file size

Compressed file size

Compression ratio

Implements core data structures:

Min-heap priority queue

Binary Huffman tree

Bit-level encoding with padding for proper byte alignment

Supports multiple formats such as .txt, .csv, .log, etc.

Lightweight and dependency-free

Licensed under the MIT License

How It Works
Reads and loads the input file as text.

Calculates the frequency of each character.

Builds a Huffman Tree using a min-heap (priority queue).

Generates Huffman codes for each character.

Encodes the entire text using those codes.

Bit-packs the binary data and writes it to a compressed .bin file.

Supports full decompression to restore the original file.

Sample Output
Enter filename to compress: input.txt
Compression successful! Compressed file saved as: input.bin
Original: 2170 bytes
Compressed: 1214 bytes
Compression Ratio: 55.94%
Decompression successful! Decompressed file saved as: input_decompressed.txt
Huffman-File-Compressor/
├── main.cpp                 // Core source code
├── input.txt                // Sample input file
├── input.bin                // Compressed binary output
├── input_decompressed.txt   // Output after decompression
└── README.md                // Project documentation
Concepts Used
Huffman Encoding (Greedy algorithm)

Priority Queue with Custom Comparator

Binary Tree Traversal

Bit Manipulation using std::bitset

File I/O (Text and Binary modes)

Padding and Byte Alignment
