# ELDMP – Embedded Linux Device Management Platform

> **Long-term Master Plan (v0.1)**

## 1. Vision

ELDMP là một **nền tảng (platform)** dành cho Embedded Linux, được phát
triển trong nhiều năm nhằm:

- Học Embedded Linux từ Foundation → Security.
- Xây dựng portfolio có chiều sâu.
- Áp dụng Software Engineering chuyên nghiệp.
- Có thể tái sử dụng trên nhiều board và nhiều domain.

------------------------------------------------------------------------

# 2. Design Principles

1.  Product-first, not demo-first.
2.  Mỗi release luôn build và chạy được.
3.  Chỉ học 5–10 kiến thức mới mỗi version.
4.  Documentation quan trọng ngang source code.
5.  Mọi module đều có API rõ ràng.
6.  Luôn có benchmark, test và release note.

------------------------------------------------------------------------

# 3. Final Product Architecture

    Hardware
       │
    Bootloader
       │
    Linux Kernel
       │
    HAL
       │
    Middleware
       ├── Config
       ├── Logger
       ├── IPC
       ├── Storage
       └── Security
       │
    Services
       ├── Device
       ├── Health
       ├── Network
       ├── OTA
       ├── Cloud
       ├── MQTT
       ├── Diagnostic
       └── Watchdog
       │
    CLI / REST / WebUI

------------------------------------------------------------------------

# 4. Long-term Hardware Roadmap

| Giai đoạn | Thiết bị                 | Mục đích               |
|-----------|--------------------------|------------------------|
| Stage 0   | Ubuntu Laptop            | Phát triển nền tảng    |
| V0.2      | BeagleBone Black Rev C   | Embedded Linux thực tế |
| V0.5      | STM32H743                | MCU đồng hành          |
| V1.5      | Logic Analyzer, USB-UART | Debug                  |
| V2.x      | Router/Switch            | Networking             |
| V3.x      | SD/USB                   | OTA                    |
| V4.x      | STM32MP257 hoặc i.MX93   | Security               |

------------------------------------------------------------------------

# 5. Learning Roadmap

## Stage 0

### Books

- Linux Command Line
- Git Book
- CMake Tutorial

### Skills

- Git
- Bash
- Markdown
- PlantUML
- Docker
- CMake

### Deliverables

- Repository
- Coding style
- README
- Documentation template

------------------------------------------------------------------------

## Version 0.1 – Core Framework

### Books

- TLPI
- Effective Modern C++

### Skills

- Process
- Thread
- File I/O
- Signal
- Timer
- JSON
- Logger
- RAII
- Smart Pointer

### Modules

- Config
- Logger
- CLI
- DeviceInfo

### Deliverables

- `eldmp info`
- Device information
- Unit tests
- Architecture document

------------------------------------------------------------------------

## Version 0.2 – Device Management

### Hardware

- BBB

### Books

- OSTEP
- TLPI (/proc, /sys)

### Skills

- procfs
- sysfs
- Filesystem
- Monitoring

### Modules

- Device Manager
- Health Monitor

### Deliverables

- CPU/RAM/Disk monitor
- Temperature
- Health daemon

------------------------------------------------------------------------

## Version 0.3 – IPC

### Skills

- D-Bus
- Unix Socket
- IPC
- State Machine

### Deliverables

- Service communication via IPC only

------------------------------------------------------------------------

## Version 0.4 – systemd

### Skills

- systemd
- journalctl
- service
- timer

### Deliverables

- logger.service
- monitor.service
- device.service

------------------------------------------------------------------------

## Version 0.5 – Watchdog

### Hardware

- STM32H743

### Skills

- Supervisor
- Heartbeat
- Restart strategy

### Deliverables

- Auto restart
- Failure log

------------------------------------------------------------------------

## Version 1.0 – Production Foundation

### Books

- Mastering Embedded Linux Programming

### Skills

- RootFS
- BusyBox
- Cross compile
- Device Tree
- HAL

### Deliverables

- Platform chạy trên BBB

------------------------------------------------------------------------

## Version 1.5 – Yocto

### Books

- Embedded Linux System with the Yocto Project
- Bootlin Yocto Labs

### Deliverables

- Build hoàn toàn bằng Yocto

------------------------------------------------------------------------

## Version 2.x

### Skills

- TCP/IP
- REST
- MQTT
- DHCP
- NTP

### Deliverables

- Network + Cloud

------------------------------------------------------------------------

## Version 3.x

### Study

- RAUC
- SWUpdate
- Mender

### Deliverables

- OTA
- Rollback
- A/B update

------------------------------------------------------------------------

## Version 3.5

### Books

- Systems Performance
- BPF Performance Tools

### Deliverables

- Benchmark
- perf
- eBPF

------------------------------------------------------------------------

## Version 4.x

### Books

- OpenSSL

### Skills

- TLS
- PKI
- Signed firmware
- Encrypted config

------------------------------------------------------------------------

## Version 5.x

### Hardware

- STM32MP257 / i.MX93

### Study

- OP-TEE
- TF-A
- dm-verity
- dm-crypt

### Deliverables

- Secure Boot
- Verified Boot
- Secure OTA

------------------------------------------------------------------------

# 6. Core Book List

## Linux

- TLPI
- OSTEP
- APUE

## Embedded Linux

- Mastering Embedded Linux Programming
- Embedded Linux System with the Yocto Project
- Bootlin Labs

## C++

- Effective Modern C++
- C++ Concurrency in Action

## Performance

- Systems Performance
- BPF Performance Tools

## Security

- OpenSSL
- OP-TEE
- Trusted Firmware-A

------------------------------------------------------------------------

# 7. Mandatory Engineering Checklist

Mỗi release phải có:

- README
- Requirements
- Architecture
- UML
- API
- Unit Test
- Integration Test
- Benchmark
- Release Notes
- Changelog
- Git Tag
- Demo Video

------------------------------------------------------------------------

# 8. Success Criteria

- Có platform chạy được.
- Có tài liệu đầy đủ.
- Có kiến trúc mở rộng.
- Có khả năng chuyển sang Security.
- Có giá trị như portfolio chuyên nghiệp.
