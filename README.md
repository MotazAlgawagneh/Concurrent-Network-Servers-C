#  Concurrent Client-Server Network Architecture

##  Overview
This repository contains a comprehensive, multi-phase network programming project built entirely in **C** using **POSIX Sockets**. The system implements a client-server architecture designed to process string validations (Palindrome checking), progressively scaling from a simple local program to a fully concurrent server handling multiple clients simultaneously via TCP and UDP protocols.

##  Development Phases

### Phase 1: Local String Validation
* Implemented the core string processing and validation logic (handling spaces, cases, and input limits) locally without networking.

### Phase 2: Multi-processing TCP Server (`fork()`)
* Developed a connection-oriented **TCP Server**.
* Achieved concurrency by utilizing the `fork()` system call, creating a dedicated child process for every new client connection to ensure non-blocking operations.

### Phase 3: I/O Multiplexing TCP Server (`select()`)
* Optimized server resources by replacing `fork()` with the `select()` system call.
* This phase handles multiple concurrent client connections within a single thread/process, significantly reducing CPU and memory overhead.

### Phase 4: UDP Protocol Transition
* Transitioned the architecture to a connectionless **UDP Server** (`SOCK_DGRAM`).
* Handled the unreliability of UDP by adjusting the client-server communication logic using `sendto()` and `recvfrom()`.

##  Tech Stack & System Calls
* **Language:** C
* **Networking:** POSIX Sockets API, TCP/IP, UDP.
* **Concurrency:** Multi-processing (`fork()`), I/O Multiplexing (`select()`).
* **OS Concepts:** File Descriptors, Inter-process Communication (IPC), Socket Binding & Listening.

##  How to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/MotazAlgawagneh/Concurrent-Network-Servers-C.git
   ```

2. To run a specific phase (e.g., Phase 2), compile the server and client files:
   ```bash
   gcc "Phase_2_TCP_Fork_(Server).c" -o server
   gcc "Phase_2_TCP_Fork_(Client).c" -o client
   ```
3. Run the server on a specific terminal:
   ```bash
   ./server
   ```
   
4. Run the client on another terminal (connecting to localhost):
   ```bash
   ./client 127.0.0.1
   ```     
