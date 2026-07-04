# Embedded Linux System Engineer/Architect

**Embbedded Linux System**
```text
Cloud
   │
OTA / Monitoring
   │
Linux SoC
├── GUI
├── AI
├── ROS2
├── Middleware
├── Device Manager
├── Network
├── Security
└── Linux Kernel
        │
 CAN / Ethernet / SPI / UART
        │
 ├── MCU Motion
 ├── MCU Sensor
 ├── MCU Safety
 └── MCU IO
```
```text
+-------------------------------------------------------+
| Raspberry Pi 4B                                       |
| Linux                                                 |
| Buildroot / Yocto                                     |
| Driver                                                |
| Middleware                                            |
| Application                                           |
+---------------------+---------------------------------+
                      |
        UART / SPI / CAN / Ethernet
                      |
+---------------------v---------------------------------+
| STM32 Nucleo-H753ZI                                   |
| Bare-metal / FreeRTOS / micro ROS                     |
| Sensors                                               |
| Motor                                                 |
| Control Loop                                          |
| Protocol                                              |
+-------------------------------------------------------+
```

```text
This architecture is widely used in:
- Industrial Automation
- Robotics
- Automotive Electronics
- Smart IoT Devices
- Medical Equipment
```

## Learning Path

**Phase 1 — Operating System Fundamentals**
```text
Duration: 1–2 Months

Objective:
- Build a solid understanding of how Linux works as an operating system.

Study Topics:
- Operating Systems: Three Easy Pieces
- Linux command line
- Bash scripting
- GCC
- Make
- CMake
- GDB
- ELF executable format
- Processes
- Threads
- CPU scheduling
- Virtual memory
- mmap
- fork() / exec()
- Signals
- Pipes
- Shared memory
- Mutexes
- Condition variables
- File systems
- Virtual File System (VFS)

Practice Environment:
- Ubuntu Virtual Machine
- No hardware is required at this stage.
```

**Phase 2 — Linux Kernel Fundamentals**
```text
Duration: 2–3 Months

Study Material:
- Linux Kernel Labs

Complete as many labs as possible, including:
- Boot process
- Scheduler
- Process management
- Memory management
- Virtual File System
- Character device drivers
- Interrupt handling
- Kernel threads
- Linux Device Model
- Debugging techniques
- Kernel synchronization

Practice Environment:
- Ubuntu -> QEMU -> Linux Kernel
- A Raspberry Pi is still unnecessary at this point.
```

**Phase 3 — Embedded Linux System Development**
```text
Duration: 2–3 Months

Study Material
- Mastering Embedded Linux Programming

Study Topics
BootROM
SPL
U-Boot
Linux Kernel
Init system
Root filesystem
BusyBox
Buildroot
Yocto Project (fundamentals)
Cross-compilation
Device Tree
Kernel configuration
Package management
OTA update mechanisms
Basic embedded security

Practice Environment
- Using Buildroot: QEMU -> Buildroot -> Embedded Linux
- Or using Yocto: QEMU -> Yocto -> Embedded Linux
- Most exercises can be completed without purchasing any hardware.
```

```text
When Should You Buy Hardware?
Only after completing the first three phases.
By then, you should already understand:
- Linux boot sequence
- Kernel architecture
- Buildroot and Yocto
- Device Tree
- Cross-compilation
- Bootloader concepts
At this point, physical hardware becomes truly valuable.
```

**Phase 4 — Linux Development Board**
```text
Duration: 2 Months

Recommended Purchase
Choose one Linux ARM board:
- Raspberry Pi 5
- Orange Pi 5

Objectives
- Boot Linux
- Build and customize the kernel
- Build U-Boot
- Flash firmware
- Modify Device Trees
- GPIO programming
- UART
- SPI
- I²C
- PWM
```

**Phase 5 — MCU Development**
```text
Duration: 2 Months

Now purchase an MCU development board.
Recommended Boards
- STM32 Nucleo (F4 or H7 series)

Study Topics
- UART
- SPI
- I²C
- CAN
- Ethernet (if supported)
- DMA
- FreeRTOS (optional but recommended)

Initially, communication between your PC and the MCU is sufficient.
```

**Phase 6 — Linux ↔ MCU Communication**
```text
Duration: 3–4 Months

This is one of the most important stages.
Start with:
   Linux -> UART -> STM32
Then progress to:
   Linux -> SPI -> STM32
Next:
   Linux -> CAN -> STM32
Finally:
   Linux -> Ethernet -> STM32

Suggested Projects:
- Linux sends control commands
- MCU returns status information
- Design a communication protocol
- Heartbeat mechanism
- CRC verification
- Retry logic
- Timeout handling
- Protocol version management

These projects closely resemble real industrial products.
```

**Phase 7 — Multi-Layer Software Architecture**
```text
Duration: 4–6 Months

Separate the Linux software into layers:
Application
↓
Middleware
↓
HAL
↓
Driver
↓
Kernel

Similarly, organize the MCU firmware:
Application
↓
Service Layer
↓
HAL
↓
Driver

Study and implement:
- Inter-Process Communication (IPC)
- Message Queues
- Service Discovery
- State Machines
- Logging systems
- Configuration management
- Diagnostics
- OTA updates
```

**Phase 8 — Industrial Embedded Platforms**
```text
Only after mastering the previous phases should you consider investing in industrial-grade SoCs.

Recommended Platforms
- STM32MP157
- NXP i.MX8M
- TI AM62x
- BeagleBone Black (excellent for real-time and PRU learning)
- NVIDIA Jetson (if focusing on AI and computer vision)

Advanced Topics
- RemoteProc
- RPMsg
- Asymmetric Multi-Processing (AMP)
- Heterogeneous Computing
- Secure Boot
- A/B System Updates
- Docker
- ROS2
- Time-Sensitive Networking (TSN)
- Real-Time Linux
```

**Reference**
```text
A/Computer Science Foundation
- Operating Systems: Three Easy Pieces (CSAPP)
    Process,Thread,Virtual Memory,Paging,
    Context Switch,Scheduling,Synchronization,File System ,IPC
- Computer Systems: A Programmer's Perspective (OSTEP)
    CPU pipeline,Cache,TLB,MMU,DMA,Interrupt,ARM architecture
- Networking
    TCP/IP,Ethernet,ARP,ICMP,UDP,TCP,Routing,NAT
 
B/Linux Internals
- Linux Memory Management
    .text,.data,.bss,heap,stack,mmap
- ELF + Dynamic Linker + Dynamic Loader
- Linux Boot Flow
    Rom,Bootloader,U-Boot,Kernel,Device Tree,Init,Systemd,Application
 
C/Embedded Linux Platform
- Device Tree
- Buildroot
- Yocto
- U-Boot
 
D/Kernel Space
- Kernel Driver: Character,GPIO,I2C,SPI
- Interrupt,DMA
 
E/System Architecture
- Linux + MCU
- IPC framework
- OTA
- Security
 
==================
 
6 months
- OSTEP (Operating System)
- CSAPP (Computer Architecture)
- Networking
- ELF
- Memory
 
6~18 months (Raspberry Pi 4 + NUCLEO-F446RE)
- Buildroot
- Yocto
- U-Boot
- Driver
- Device Tree
 
18~36 months (STM32MP157 / i.MX8M Plus EVK)
- Linux + MCU
- Remote update
- Industrial network
- CAN
- Ethernet

```