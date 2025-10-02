# Simple File Compressor
A simple file compression and decompression tool written in C, using **Run-Length Encoding (RLE)** and **Huffman Coding**.  
It allows you to **shrink** files to save space and **unshrink** them back to their original form.

---

## Contents
1. [Motivation](#motivation)  
2. [Features](#features)  
3. [Installation](#installation)  
4. [Usage](#usage)  
5. [Project Structure](#project-structure)  
6. [Future Improvements](#future-improvements)  
7. [License](#license)

---

## Motivation
This project was created as part of a learning exercise to understand and implement **lossless data compression** algorithms.  
I chose RLE and Huffman Coding because together they provide a simple yet effective form of lossless compression. RLE quickly reduces sequences of repeated characters, while Huffman encoding optimizes symbol representation based on frequency. This combination demonstrates how two different techniques can complement each other to achieve better compression.

---

## Features
- Compress (`shrink`) and decompress (`unshrink`) files.  
- Checks file extensions (`.shrink`) before decompression.  
- Simple command-line interface.  
- Implemented fully in C without external libraries.  

---

## Installation
Clone the repository and build the project:
GCC version 14.2.0

```bash
git clone https://github.com/gesinski/simple-file-compressor.git
cd simple-file-compressor
make
```

This will generate the binary:
./shrink.out

---

## Usage
Shrink a file:
./shrink.out example shrink
This creates example.shrink - a compressed file that takes up less space than the original file.

Unshrink a file:
./shrink.out example.shrink unshrink
This creates example.shrink.unshrink - the restored original file.

Other option: Shell Alias (quick and easy)
Open your shell configuration file, e.g., ~/.bashrc or ~/.zshrc.
Add this line:
```
alias shrink='./shrink.out "$1" shrink'
alias unshrink='./shrink.out "$1" unshrink'
```
Reload the shell config:
```
source ~/.bashrc
```
Usage:
```
shrink example
unshrink example.shrink
```

---

## Project Structure
simple-file-compressor/
├── README.md               
├── Makefile               
├── example                
├── include/                
│   ├── huffman-decoding.h
│   ├── huffman-encoding.h
│   ├── rld.h
│   └── rle.h
└── src/                  
    ├── compressor.c        
    ├── huffman-decoding.c
    ├── huffman-encoding.c
    ├── run-length-decoding.c
    └── run-length-encoding.c


---

## Future Improvements
- The program crashes when the file contains numbers due to a flawed RLE implementation - this needs improvement.

- The program sometimes adds extra symbols at the end of the unshrinked file because of byte padding.

- For very small files (under ~300 bytes), the shrunk file may be larger than the original.

- During decompression, the .shrink extension could be removed, and the .unshrink suffix could not be added.

---

## License
This project is licensed under the MIT License - feel free to use and modify.
