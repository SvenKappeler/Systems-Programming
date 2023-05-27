# Systems Programming

This repository contains all materials related to the CSC 322 Systems Programming course. The focus of this course is to develop a deep understanding of computer systems, including bits, memory structure, caching, low-level programming, and other essential system-level concepts.

## Projects Overview

This course includes several hands-on projects to apply these concepts and gain practical experience in systems programming. 

### Project 1: Simple Text Encryptor/Decryptor

The first project is a simple text encryption and decryption program in C. The project further familiarizes students with the fundamentals of low-level programming, providing hands-on experience in handling character strings and memory in C.

The text encryption and decryption program accepts commands to encrypt or decrypt strings of text. The encryption/decryption is performed using a simple Caesar cipher method. The program also handles incorrect commands and provides user feedback.

### Project 2: Cache Simulator

The Cache Simulator project involves creating a simple cache simulation program that reads a memory trace and simulates the hit/miss behavior of a cache memory for this trace.

The simulation is performed based on the Least Recently Used (LRU) replacement policy. The main objectives of this project are to understand the concepts of caching, memory hierarchy, and the hit/miss behavior of cache memory.

To run the program, use the following command:

```bash
./cachelab -m [m] -s [s] -e [e] -b [b] -i [tracefile] -r [replacement]
```

Where:

- `-m`: Number of physical address bits.
- `-s`: Number of set index bits (the number of sets is 2^s).
- `-e`: Number of lines per set.
- `-b`: Number of block offset bits (block size is 2^b).
- `-i`: Input memory trace file.
- `-r`: Replacement policy (only "LRU" is implemented in this project).

The program will output the number of cache hits, misses, the miss rate, and the total execution time of the simulation.



### Project 3: Tiny Shell Program with Job Control

The third project involved creating a tiny shell program (`tsh`) with job control. This project aimed to understand process control and job management in Unix-like systems. It involved the use of system calls, signals, and other low-level programming constructs.

This shell program performs various tasks, such as spawning and controlling processes. It utilizes Unix system calls to create and manage processes, handle signals, and interact with the kernel. Moreover, it manages jobs (processes) in the shell, allowing for running processes in both the foreground and the background, stopping and resuming processes, and other related functionalities.
