# Project Title

File Compression and Decompression Tool 

## Description

This project's scope is to develop a standalone file compression and decompression tool. It will not be designed to function as a plugin or to be embedded into existing systems at this stage. The tool's capabilities will be limited to specific file formats and will not cover every possible file type. The tool will primarily focus on implementing algorithms for compressing and decompressing text, image and zip files. While it aims to provide efficient compression and decompression for these file formats, it may not support every variation or proprietary format within each category. The tool can help compress files before sending via email or WhatsApp, which only transfers smaller files. It will still be limited because it will only reduce the file sizes to a certain level depending on the original file, which might exceed the benchmark for these social media applications. 

## Getting Started
* This project is designed to run on Linux environments.
* To start the program, you'll need to install 
    1. Flask 
    2. python3 

### Dependencies
* We implemented the following algorithms

* C programming language will be used to implement the algorithms because of the wide range of libraries for encoding and decoding videos and audio it has. Also, it is a low-level language that provides direct control over hardware resources. 

* Huffman Algorithm
The Huffman algorithm is a popular method used for lossless data compression. It works by assigning variable-length codes to input characters based on their frequencies of occurrence in the input data. Characters that appear more frequently are assigned shorter codes, while less frequent characters are assigned longer codes. This algorithm is particularly efficient for compressing text files and is widely used in file compression utilities.

In this project, we employed the Huffman algorithm to compress and decompress all text files, such as .doc files.

* Future Works
Our future goal is to extend the functionality of the compression algorithm to support compression of PDF files.

Addressing this enhancement will enable our tool to handle a wider range of file types, accommodating users who need to compress PDF documents efficiently.

* Runlength  (RLE)
Run-Length Encoding (RLE) is a simple form of lossless data compression that replaces sequences of repeating data with a single data value and the number of times it repeats. It is particularly effective for compressing data with long consecutive sequences of identical values, such as images with large areas of uniform color or text files with repeated characters.

In this project we used Runlength to compress and decompress images 

* Zlib Algorithm
Zlib is a widely-used data compression library that implements a combination of several compression algorithms, including the DEFLATE algorithm, which is based on LZ77 and Huffman coding. Zlib provides a balance between compression ratio and speed, making it suitable for a wide range of applications, from file compression utilities to network protocols.

* Future Works
One of the future improvements we aim to implement is ensuring that the compressed files retain the original file format, including their file extensions. Currently, the compression method only returns files in text document format.

By addressing this issue, users will benefit from a more versatile compression tool that maintains the integrity of the original files, allowing for seamless integration with various file types and applications.

Purpose in Compression and Decompression

Compression: The Zlib algorithm compresses input data using a combination of LZ77 and Huffman coding, resulting in a compressed output with a good balance between size reduction and processing speed.
Decompression: During decompression, the Zlib algorithm reverses the compression process, reconstructing the original input data from the compressed representation using LZ77 decoding and Huffman decoding.


### Installing

* Install the following dependencies using BASH

# Install Flask
pip install flask

# Install Python 3
sudo apt-get install python3

### Executing program

1. Navigate to the <FILE_COMPRESSION_TOOL> directory in your terminal.

2. Run the following command 
    - python3 app.py

3. Make sure that app.py is available in the directory.

4. After running the command, a link will appear in your terminal e.g - http://127.0.0.1:5000. Ctrl + click on the link to open it in your default browser. This will launch the application.

5. Once the application is open in your browser, you can navigate through the site to perform file compression tasks.

* available options for navigation

UNDER COMPRESSION THE USER WILL BE ABLE TO DO THE FOLLOWING OPERATIONS 
compress image
compress documents 
compress tiff
create a zip file

UNDER DECOMPRESSION THE USER WILL BE ABLE TO DO THE FOLLOWING OPERATIONS 
decompress all images
decompress documents
unzip a zip file

## Shutting Down the Program
* To shut down the program, simply press Ctrl + C  in the terminal where the program is running or kill -9 <pid> in a different terminal.

## Authors

Contributors names and contact info

1. Bethel Panashe Choto
2. Tinotenda Rodney Alfaneti

## Version History
* V.0.1 - Initial Release