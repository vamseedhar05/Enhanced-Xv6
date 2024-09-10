# Introduction to xv6 and Enhancements

xv6 is a modern re-implementation of Unix Version 6 (v6), originally developed by Dennis Ritchie and Ken Thompson. It retains the structure and simplicity of the original v6 but is designed for the RISC-V multiprocessor architecture using ANSI C. xv6 provides a foundation for understanding basic operating system concepts like process management, scheduling, and system calls.

## Enhancements in Our Project

This project extends xv6 by implementing additional system calls and scheduling policies. Below are the key enhancements:

### 1. System Call Implementations

- **Get Read Count**: This system call keeps track of the number of read operations that have been performed by a process.
- **sigalarm**: Implements an alarm mechanism where a user can specify a timer, and when the time expires, a user-defined function is triggered.
- **sigreturn**: Restores the previous state of the process after an alarm handler has finished execution.

These system calls add significant functionality for managing process interactions with the operating system.

### 2. Scheduling Policy Implementations

#### **First Come First-Served (FCFS)**

FCFS is a straightforward scheduling algorithm where processes are executed in the order they arrive in the ready queue. The process that arrives first will be executed first, regardless of its execution time or priority. This method is easy to implement but can lead to issues like the "convoy effect," where short processes may be stuck waiting behind long-running ones.

#### **Multilevel Feedback Queue (MLFQ)**

MLFQ is a more advanced scheduling algorithm designed to improve process management efficiency by using multiple queues with different priority levels. In our implementation:
- Processes start in the highest-priority queue (0) and are moved to lower-priority queues (up to 3) based on their behavior and time slices.
- If a process uses up its entire time slice, it is demoted to a lower-priority queue.
- Within the same queue, processes are scheduled using Round-Robin, ensuring fairness.
- This method allows shorter processes or I/O-bound tasks to be executed sooner, while longer processes are gradually deprioritized.

MLFQ offers better performance for a mix of short and long-running processes by dynamically adjusting their priority.

---

## Networking in xv6

Apart from system-level improvements, the project includes networking features implemented in two parts:

### Part A: Basic UDP and TCP Functionality

- **UDP and TCP Server/Client Communication**: 
  - This subpart handles basic server and client interaction using both TCP and UDP protocols. The server listens for incoming connections, while clients can send and receive messages using these protocols.
  
- **Rock, Paper, Scissors Implementation**: 
  - A simple multiplayer game of Rock, Paper, Scissors is implemented over both TCP and UDP connections, showcasing how both protocols handle communication.

### Part B: TCP over UDP Implementation

In this part, we implemented a TCP-like server and client communication using the UDP protocol. Since UDP is connectionless, we added custom functionality to simulate TCP features:
- **Acknowledgments**: Ensures that packets are delivered and received correctly by sending acknowledgment messages for each packet.
- **Flow Control**: Regulates the rate of data transmission between the sender and receiver to prevent buffer overflow and data loss.

This part demonstrates how reliable communication can be built over an unreliable protocol like UDP by adding additional layers of control and error-checking.

---

## How to Use

1. Navigate to the main directory of the project.
2. Use the `make` command to compile the system.
3. Run the system using `qemu` to launch xv6 with the implemented features.
4. Test the new system calls and scheduling policies by running provided test scripts or writing custom tests.
5. For networking, start the server and client programs to establish connections and transfer data via UDP and TCP protocols.
