# Ziv-Lempel Data Compression Engine

### Project Overview
This project is a C++ implementation of the Ziv-Lempel (LZW) adaptive data compression algorithm. It features a complete system for mapping input strings into numeric codes and a corresponding decompressor to reconstruct original data from those integer sequences.

### Technical Highlights
* Custom Hash Table: Designed and implemented a template-based Hash Table from the ground up, utilizing open addressing and linear probing for collision resolution.
* Adaptive Logic: Implemented the Ziv-Lempel rule to identify the longest string prefixes and dynamically update the dictionary during the compression process.
* Dictionary Management: The system initializes with the standard 256-character ASCII alphabet and manages a dynamic dictionary with a maximum capacity of 4096 codes.
* Efficient Reconstruction: The decompression module rebuilds the code-to-string dictionary in real-time without requiring the original table to be stored in the compressed file.
* Memory Management: Utilized robust C++ data structures and logic to ensure efficient memory usage while handling up to 4095 distinct string entries.

### Implementation Details
* Language: C++
* Core Data Structures: Custom Hash Table for compression and Dynamic Array/Vector for decompression.
* Algorithm: Ziv-Lempel (LZW).
* Collision Resolution: Linear Probing.

### How to Run

1. **Compile the source**:
   
   ```bash
   g++ main.cpp -o Compressor
   ```
   
2. **Run the program**:

    Linux / macOS: ./Compressor

    Windows: Compressor.exe

3. **Usage**:

    Select option 1 to compress an input string into a sequence of integer codes.

    Select option 2 to decompress a sequence of codes back into the original text string.
