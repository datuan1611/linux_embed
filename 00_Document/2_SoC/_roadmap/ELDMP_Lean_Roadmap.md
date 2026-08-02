# ELDMP (Embedded Linux Device Management Platform)
## Lean Long-term Roadmap (Portfolio Edition)

> **Purpose**
>
> Build one practical Embedded Linux platform while learning. The project is designed to become a long-term portfolio rather than a collection of small demos.

---

# 1. Development Philosophy

## Goal

- Learn Embedded Linux deeply.
- Build a reusable platform.
- Create a portfolio for Senior Embedded Linux roles.
- Extend naturally toward Embedded Security.

## Rules

1. Project drives learning.
2. Only learn knowledge required for the current sprint.
3. Every sprint produces working software.
4. Documentation is lightweight but complete.
5. Keep architecture extensible.

---

# 2. Technology Scope

## Included in V1

- Linux System Programming
- Modern C++
- CMake
- Logger
- Configuration
- Device Information
- Health Monitor
- D-Bus IPC
- systemd
- Watchdog
- Yocto (basic)

## Deferred

- Web UI
- Cloud dashboard
- Docker deployment
- Complex CI/CD
- Database
- Performance optimization
- Advanced Security

---

# 3. Hardware Roadmap

| Stage | Hardware |
|-------|----------|
| Foundation | Ubuntu Laptop |
| V0.2 | BeagleBone Black Rev C |
| V2 | STM32H743 |
| V3 | STM32MP257 or i.MX93 |

Optional:
- USB-UART
- Logic Analyzer
- Ethernet Switch

---

# 4. Book Roadmap

## Foundation

- The Linux Programming Interface
- Effective Modern C++

## Linux

- OSTEP

## Embedded Linux

- Mastering Embedded Linux Programming

## Yocto

- Embedded Linux System with the Yocto Project
- Bootlin Labs

## Networking

- Beej's Guide to Network Programming

## Security (later)

- OpenSSL Documentation
- OP-TEE Documentation

---

# 5. Sprint-based Roadmap

## Sprint 0 - Environment

Learn:
- Git
- Bash
- CMake
- Markdown

Deliverables:
- Repository
- README
- Build system

---

## Sprint 1 - Logger

Study:
- TLPI File I/O

Implement:
- Logger library
- Log level
- File rotation (simple)

Output:
- logger library
- Unit test

---

## Sprint 2 - Config

Study:
- JSON

Implement:
- Config Manager

Output:
- config.json
- Reload support

---

## Sprint 3 - Device Manager

Study:
- /proc
- /sys

Implement:
- CPU
- RAM
- Kernel Version
- Hostname

Command:

```
eldmp info
```

---

## Sprint 4 - Health Monitor

Study:
- Thread
- Timer

Implement:
- CPU usage
- RAM usage
- Disk usage

Command:

```
eldmp monitor
```

---

## Sprint 5 - IPC

Study:
- D-Bus

Implement:
- Device Service
- Logger Service
- Health Service

All communication through IPC.

---

## Sprint 6 - systemd

Implement:
- logger.service
- device.service
- monitor.service

Learn:
- journalctl
- systemctl

---

## Sprint 7 - Watchdog

Implement:
- heartbeat
- restart failed service

Release:

# V1.0

Portfolio-ready Embedded Linux Platform

---

## Sprint 8 - Yocto

Learn:
- Layers
- Recipes

Deliverable:
- Build image with Yocto

Release:

# V1.5

---

## Sprint 9 - Networking

Learn:
- TCP/IP
- MQTT

Implement:
- Network Manager
- MQTT Client

Release:

# V2.0

---

## Sprint 10 - OTA

Study:
- SWUpdate or RAUC

Implement:
- Download
- Verify
- Install
- Reboot

(No A/B yet)

Release:

# V2.5

---

## Sprint 11 - Security Foundation

Learn:
- OpenSSL
- TLS
- Certificates

Implement:
- HTTPS
- MQTT TLS
- Signed package

Release:

# V3.0

---

# 6. Repository Layout

```text
eldmp/
├── docs/
├── src/
│   ├── logger/
│   ├── config/
│   ├── device/
│   ├── monitor/
│   ├── ipc/
│   ├── watchdog/
│   └── app/
├── tests/
├── scripts/
├── yocto/
└── README.md
```

# 7. Every Sprint Must Deliver

- Working code
- Clean commit history
- README update
- Architecture diagram
- One demo video
- Git tag

# 8. Success Metrics

By V1.0:
- Comfortable with Linux user-space.
- Confident with CMake and Modern C++.
- Able to explain architecture.

By V2.0:
- Embedded Linux platform running on board.
- Yocto image.
- MQTT communication.

By V3.0:
- Foundation for Embedded Linux Security.
- Strong public portfolio.
