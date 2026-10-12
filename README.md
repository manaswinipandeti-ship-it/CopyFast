cat > README.md <<'EOF'
# CopyFast – Multithreaded File Copy Utility

## Overview

CopyFast is a command-line file-copying utility developed in C. It supports buffered file copying, concurrent directory copying, and SHA-256-based file integrity verification.

## Features

- **Buffered File Copy:** Copies files using a fixed-size buffer.
- **Concurrent Directory Copy:** Uses multiple POSIX threads to copy files from a directory.
- **File Integrity Verification:** Compares SHA-256 hashes to verify whether source and destination files have identical content.
- **Error Handling:** Reports errors such as missing source files and file access failures.
- **Performance Measurement:** Displays the time taken for copying operations.

## Technologies Used

- C programming language
- POSIX file operations
- POSIX threads (pthreads)
- OpenSSL SHA-256
- GCC and Make

## Project Structure

- `src/main.c` – Program entry point and workflow
- `src/copier.c` – File-copying logic
- `src/copier.h` – File-copying function declarations
- `src/directory.c` – Directory-copying logic and worker threads
- `src/directory.h` – Directory-copying function declarations
- `src/verify.c` – File integrity verification
- `src/verify.h` – Verification function declarations
- `Makefile` – Build configuration
- `test_folder/` – Sample files for testing

## Build Instructions

Ensure GCC, Make, pthreads, and OpenSSL development libraries are installed.

Compile the project using:

```bash
make
## Build Instructions

Compile the project using Make:

```bash
make
```

This builds the executable named `copyfast`.

## Usage

### Copy a File

```bash
./copyfast test_folder/file1.txt copied_file1.txt
```

### Copy a Directory

```bash
./copyfast test_folder destination_folder
```

The program detects whether the source is a file or directory and selects the appropriate copying method.

## System Architecture

- **File Copying:** Uses buffered I/O with `open()`, `read()`, and `write()`.
- **Directory Copying:** Uses POSIX threads to copy files concurrently.
- **Integrity Verification:** Uses SHA-256 hashing to compare source and destination file contents.
- **Performance Measurement:** Records the elapsed time for copying operations.
- **Error Handling:** Reports file access and copying failures.

## Testing

The program was tested for:
- Successful file copying.
- SHA-256 verification when files are identical.
- Detection of differences between files.
- Handling of missing source files.

## Future Improvements

- Replace deprecated OpenSSL SHA-256 functions with the modern EVP API.
- Improve handling of partial writes and interrupted system calls.
- Expand automated testing for edge cases.
