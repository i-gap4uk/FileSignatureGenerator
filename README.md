# File Signature Generator

The main purpose of the program is to generate a signature of a specified file.
The signature will be generated in the next way:

1. The source file will be split into blocks of the same size (fixed size in bytes).
   In the case when the source file size is not multiple by specified block size, the last block will be completed by zero.

2. Each block will be generated a hash value (by MD5 standard) and append it into the output file (signature file).

### Dependencies

The program is developed for running under Linux.
OpenSSL's library is used for hash generating. So you need to install the  `libssl1.0-dev` library:
* `sudo apt install libssl1.0-dev`.
~~~~
For building the project is used `cmake` and `make`:
* `sudo apt install cmake`
* `sudo apt install build-essential`

### Program interface

The program could take in 4 arguments. Some arguments could be omitted. In this case, values omitted arguments will be used as default. The default values of the arguments:

* Output File - "file_signature.txt".
* Block size - 1048576 (1 Mb).
~~~~
The 1-st argument (the source file path) couldn't be omitted.

### Multithreading

The program on startup checks max available cores of PC on which it runs and uses less by one thread count.