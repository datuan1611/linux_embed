# ELDMP — Embedded Linux Device Management Platform
## Complete Development Roadmap

---

## Table of Contents

1. [Executive Summary](#1-executive-summary)
2. [Project Identity](#2-project-identity)
3. [Guiding Principles](#3-guiding-principles)
4. [System Architecture](#4-system-architecture)
5. [Engineering Standards, Repository & Portfolio Strategy](#5-engineering-standards-repository--portfolio-strategy)
6. [Testing Strategy](#6-testing-strategy)
7. [Hardware Roadmap & Budget](#7-hardware-roadmap--budget)
8. [Study Resources](#8-study-resources)
9. [Core Track — Detailed Sprint Plan](#9-core-track--detailed-sprint-plan)
10. [Release Checklist](#10-release-checklist)
11. [Timeline Summary & Weekly Cadence](#11-timeline-summary--weekly-cadence)
12. [Extension Track](#12-extension-track)
13. [Milestone & Skill Reference](#13-milestone--skill-reference)
14. [Interview & Portfolio Presentation Guide](#14-interview--portfolio-presentation-guide)
15. [Architecture Decision Records (ADR)](#15-architecture-decision-records-adr)
16. [Risks & Pitfalls](#16-risks--pitfalls)
17. [Glossary](#17-glossary)

---

## 1. Executive Summary

**ELDMP (Embedded Linux Device Management Platform)** is a long-term,
self-directed engineering project built to develop deep, verifiable
competency in Embedded Linux system engineering — spanning user-space
service architecture, Linux kernel internals, embedded board bring-up,
MCU/RTOS firmware, and secure device communication — while
simultaneously producing a public, versioned software portfolio.

The project follows a **project-driven learning model**: every unit of
study is paired with a concrete, shippable piece of the platform. Nothing
is studied in isolation. Progress is tracked through semantic version
releases (V1.0, V1.5, V2.0 …), each representing a fully working,
demoable system state.

**Design highlights.** Three choices shape this roadmap more than any
other:

1. **Kernel-space work starts immediately after the foundation sprints
   (Sprint 3), with a short hardware warm-up built directly into it.**
   Before touching any kernel module code, Sprint 3 opens with a brief
   user-space exercise — accessing GPIO registers directly through
   `/dev/mem` and `mmap()` — to build register-level intuition on the
   same board (BeagleBone Black) the kernel driver work uses, without
   duplicating what the kernel API itself teaches and without requiring
   the MCU board any earlier than necessary. The full MCU/FreeRTOS work
   (Sprint 10) is deliberately deferred until after the entire Linux
   track — kernel driver, service platform, and a custom Buildroot/Yocto
   image — is complete. This roadmap treats Embedded Linux as the
   mandatory, senior-differentiating skill and sequences everything to
   protect uninterrupted progress toward it.
2. **Process is kept deliberately light.** Every checklist, template, or
   review step in this document exists only because it demonstrably
   improves learning outcome or portfolio quality for a one-person
   project — nothing is carried over from enterprise practice by
   default.
3. **Learning happens through three channels, not just reading.** Every
   sprint names real source code to read, distinguishes throwaway labs
   from the shipped build, and requires a short written note — reading
   real code, deliberate practice, and teaching-to-learn, alongside
   whatever book or course material is assigned.

The roadmap is split into two tracks:

- **Core Track** (Sprint 0–14, ~15–17 months at a part-time pace):
  produces a complete, defensible portfolio for Senior Embedded Linux
  Engineer / Embedded Systems Engineer roles.
- **Extension Track** (V4.0 and beyond, open-ended): optional
  specialization chosen only after the Core Track is complete.

---

## 2. Project Identity

| Attribute | Value |
|---|---|
| Project name | ELDMP — Embedded Linux Device Management Platform |
| Type | Personal long-term engineering project / public portfolio |
| Primary language (Linux side) | Modern C++ (C++17/20) |
| Primary language (MCU side) | C, bare-metal and FreeRTOS |
| Target OS | Custom Embedded Linux (built via Buildroot and Yocto) |
| Build system | CMake |
| Version control | Git, tagged releases, changelog per version |
| Documentation | Markdown in `docs/`; learning notes in `notes/` |
| Portfolio structure | One umbrella repo (`eldmp`) + selected standalone spin-out repos (see Section 5.3) |

---

## 3. Guiding Principles

1. **Project drives learning**, but every sprint has a fixed, minimal
   reading list — never open-ended "study forever."
2. **Every sprint and every release must build and run.**
3. **Layering is enforced from day one** (Application → Middleware → HAL
   → Driver → Kernel), expressed directly in the repository structure.
4. **Hardware is purchased only when the current sprint needs it.**
5. **Documentation is lightweight per sprint, rigorous per release.** A
   short note and a clean commit is enough between milestones; the full
   checklist (Section 10) applies only at version tags.
6. **Core Track first, always.** The Extension Track only begins after
   V3.5 has been shown to at least one external reviewer.
7. **Every sprint pairs three learning channels, not just reading:**
   studying a resource, reading real production source code, and
   building something (lab and/or shipped feature). A sprint whose only
   input was a book chapter is considered incomplete.
8. **Process exists only if it earns its keep.** For a one-person
   project, any checklist, template, or review step must demonstrably
   improve learning outcome or portfolio quality — otherwise it is
   overhead that doesn't apply here, and it gets cut.

---

## 4. System Architecture

### 4.1 End-state diagram

```
                     Cloud
                       |
              OTA / Monitoring / MQTT
                       |
        +-----------------------------------+
        |             Linux SoC             |
        |  CLI / REST / (WebUI*)            |
        |  Services: Device, Health,        |
        |   Network, OTA, Diagnostic,       |
        |   Watchdog, (AI / ROS2*)          |
        |  Middleware: Config, Logger,      |
        |   IPC, Storage, Security          |
        |  HAL                              |
        |  Driver (char / GPIO / I2C / SPI) |
        |  Linux Kernel + Device Tree       |
        +----------------+------------------+
                          |
              UART / SPI / CAN / Ethernet
                          |
        +-----------------------------------+
        |        MCU (STM32H743ZI)          |
        |  Bare-metal / FreeRTOS            |
        |  Sensors / Motor / Control Loop   |
        |  Embedded GUI (LVGL)*             |
        |  Protocol: heartbeat, CRC,        |
        |   retry, timeout, versioning      |
        +-----------------------------------+

* = Extension Track feature, not part of core scope.
```

### 4.2 Layer responsibilities

| Layer | Responsibility | Why it's separated |
|---|---|---|
| Linux Kernel + Device Tree | Boot, scheduling, memory management, hardware description | Ground truth for the whole system; must be understood, not just consumed |
| Driver | Talks directly to peripherals | Isolates hardware-specific code from everything above it |
| HAL | Stable API to Middleware regardless of board | Makes porting to new hardware (Extension Track) a contained change |
| Middleware (Config, Logger, IPC, Storage, Security) | Cross-cutting infrastructure | Written once, reused by every service, independently testable |
| Services (Device, Health, Network, OTA, Diagnostic, Watchdog) | Business logic | Each service is a small, independently deployable systemd unit |
| CLI / REST / WebUI | External interface | Kept thin; all logic lives in Services |
| MCU firmware | Deterministic, real-time control | Runs where Linux cannot guarantee real-time behavior |
| Linux↔MCU protocol | Reliable inter-processor communication | The most senior-level skill in the project: designing, not just using, a protocol |

---

## 5. Engineering Standards, Repository & Portfolio Strategy

### 5.1 Repository layout (umbrella repo `eldmp`)

```
eldmp/
├── docs/
│   ├── architecture/
│   │   └── decisions/     # the 4 capped ADRs, see Section 15
│   └── release-notes/
├── notes/                  # short technical notes, one per sprint
├── src/
│   ├── logger/
│   ├── config/
│   ├── device/
│   ├── monitor/
│   ├── ipc/
│   ├── driver/             # kernel modules (from Sprint 3)
│   ├── mcu/                 # STM32 firmware (from Sprint 10)
│   ├── watchdog/
│   └── app/
├── tests/
├── scripts/
├── yocto/
└── README.md
```

### 5.2 Standards

- **Build system:** CMake for Linux-side C++; toolchain-specific build for MCU firmware.
- **Version control:** one Git tag per version milestone; commit messages describe the sprint and the change.
- **Configuration over hard-coding:** anything that could differ between boards or deployments lives in `config.json`.
- **Documentation:** `docs/architecture/` for diagrams and the 4 ADRs; `docs/release-notes/` per version tag; `notes/` for the per-sprint learning write-up required by every sprint's Definition of Done (Section 9).
- **Code quality:** builds with `-Wall -Wextra`, zero warnings tolerated by V1.0 — a cheap, concrete signal of care that costs nothing but enabling the flags.
- **Continuous Integration:** a minimal GitHub Actions workflow (or equivalent), set up in Sprint 0, that builds the project and runs unit tests on every push. A green build badge on the README is one of the cheapest, highest-visibility portfolio signals available.
- **License:** the umbrella repo and each spin-out repo carries an open-source license (e.g. MIT) from Sprint 0 — a near-zero-cost signal that the code is meant to be read and reused.

### 5.3 Portfolio repository strategy

A single, ever-growing monorepo is hard for a recruiter to evaluate in a
five-minute skim. Three sprints produce work valuable enough to also
exist as **small, standalone, independently browsable repositories**,
later integrated into `eldmp` (as a git submodule or simply documented
and linked):

| Standalone repo | Sprint | Why it deserves its own repo |
|---|---|---|
| `eldmp-kernel-driver` | Sprint 3 | A focused, ~500-line driver + DT overlay is far more legible on its own than buried inside a large service codebase |
| `eldmp-yocto-layer` | Sprint 9 | A custom Yocto meta-layer is a recognizable, self-contained artifact recruiters in this space specifically look for |
| `eldmp-mcu-firmware` | Sprint 10 | A focused FreeRTOS firmware repo, structured with proper task/queue/semaphore patterns — directly showable for roles centered on MCU/RTOS work |
| `eldmp-linux-mcu-protocol` | Sprint 11 | The single strongest piece of work in the whole project; it should be one click away, not several folders deep |

Everything else stays inside the `eldmp` umbrella repo, which functions
as the integration point and the "big picture" story.

---

## 6. Testing Strategy

| Test level | Scope | Introduced at | Tooling | Notes |
|---|---|---|---|---|
| Unit tests | Single module in isolation | Sprint 1 | GoogleTest or Catch2 | No hardware or live D-Bus dependency |
| Integration tests | Two or more services over IPC | Sprint 6 | Same framework, running D-Bus session | Manual, scripted verification is acceptable here. Building a full automated D-Bus mocking harness is not required for the Core Track — it is a genuinely painful, low-market-value investment at this stage. Automate it later only if it stops being tedious to check by hand. |
| System / boot tests | Full image boots, all services start | Sprint 3, expanded Sprint 9 | Manual boot + log inspection | Formal scripted boot testing is Extension Track (V4.0) material |
| Hardware-in-the-loop (HIL) | Real MCU + real board over the physical link | Sprint 11 | Manual fault injection (cable unplug, corrupted payload) | This is where rigor matters most — see Sprint 11 Definition of Done |
| Regression tests | Re-run of previous sprints' tests together | Every version milestone | Aggregated local run | Required before every tag |

**Convention:** test location mirrors source location
(`tests/<module>/test_<name>.cpp` ↔ `src/<module>/`). A version is not
tagged until the full regression suite passes.

---

## 7. Hardware Roadmap & Budget

| Stage | Hardware | Role | Acquired at | Budget tier |
|---|---|---|---|---|
| Foundation | Ubuntu laptop or VM | Primary dev environment | Sprint 0 | — (existing) |
| Board | BeagleBone Black Rev C | Embedded Linux target | Sprint 3 | $ |
| MCU | STM32 Nucleo-H743ZI | Real-time companion | Sprint 10 | $ |
| Networking | USB-Ethernet adapter or small switch | Test bed | Sprint 12 (only if needed) | $ |
| Industrial SoC | NXP i.MX93 | Extension Track target | V6.0+ | $$–$$$ |

`$` = low-cost hobbyist board, `$$` = mid-range dev kit, `$$$` =
industrial/AI-class eval kit. Prices vary by region and time — treat
tiers as relative planning guidance, not quotes.

**Confirmed hardware path:** BeagleBone Black Rev C for the Core Track's
Linux/kernel work (Sprint 3), then STM32 Nucleo-H743ZI for the MCU work
(Sprint 10), then NXP i.MX93 as the Extension Track industrial SoC
target. Each board is purchased right when its sprint needs it — no
board sits idle waiting to be used (Principle 4). A small LCD display
module is only needed if the optional embedded-GUI (LVGL) extension is
pursued later (Section 12) — it is not part of the Core Track.

**Small components needed alongside the boards above** (low-cost,
usually cheaper as a generic "starter kit" bundle than bought
individually):

| Sprint | Components | Purpose |
|---|---|---|
| 3 | Breadboard, jumper wires (M–M and M–F), a few LEDs, 220–330Ω resistors, a few 10kΩ resistors, a few tactile push buttons, a microSD card (Class 10+, 8–16GB), a **3.3V** USB-to-TTL serial cable | GPIO output/input for Lab 0 and the driver work; the microSD doubles as the backup image from Section 16; the serial cable is the only way to see early boot logs if the board fails to come up over network/USB — a critical safety net for Sprint 3 and again for Sprint 9 |
| 10 | A small DC motor, a motor driver IC/breakout (e.g. L298N or DRV8833), a simple sensor (e.g. a potentiometer, a DS18B20 temperature sensor, or an IR distance sensor) | The sensor read + motor/PWM control loop build |
| 11 | Two CAN transceiver breakout modules (e.g. MCP2551 or SN65HVD230), one per board | Neither the BeagleBone Black nor the STM32 Nucleo exposes a CAN transceiver on-board — only the CAN controller peripheral; a transceiver is required to put signals on a physical CAN bus |

**Important:** the USB-to-TTL adapter must be **3.3V** logic level, not
5V/RS232 — the BeagleBone Black's UART header is not 5V-tolerant, and a
5V adapter can damage the board.

---

## 8. Study Resources

### 8.1 Foundation — precisely mapped, not "read whenever"

| Resource chapters | Author(s) | Read before |
|---|---|---|
| OSTEP — File Systems & I/O | Arpaci-Dusseau | Sprint 1 (Logger does real file I/O and rotation) |
| OSTEP — Address Spaces & Virtual Memory | Arpaci-Dusseau | Sprint 3 (kernel/user space separation is central to driver work) |
| OSTEP — Processes & Process API | Arpaci-Dusseau | Sprint 6 (IPC involves multiple cooperating processes) |
| OSTEP — Threads, Locks, Condition Variables | Arpaci-Dusseau | Sprint 5 (Health Monitor introduces threads/timers), reinforced at Sprint 8 (Watchdog) |
| TLPI — File I/O, Signals, IPC | Michael Kerrisk | Sprint 1–6, directly |
| Effective Modern C++ | Scott Meyers | Ongoing from Sprint 0, RAII/smart pointers used throughout |

### 8.2 Kernel & Embedded Linux

**Reading priority for Sprint 3:** treat *Linux Kernel Labs* as the one
resource to follow sequentially — it paces the hands-on exercises.
Everything else below is a reference to consult for the specific gap it
fills, not a second book to read cover-to-cover in parallel.

| Resource | Source | Used for |
|---|---|---|
| *Exploring BeagleBone* | Derek Molloy | Sprint 3 — specifically covers the progression from userspace `/dev/mem` register access to writing real kernel drivers on this exact board |
| Linux Kernel Labs | Linux Foundation / kernel community | Sprint 3 |
| *Computer Organization and Design: ARM Edition* — cache hierarchy and virtual memory chapters | Patterson & Hennessy | Sprint 3 — the real memory architecture of the BeagleBone Black's Cortex-A8; fills a gap neither the STM32-focused MCU material (Section 8.3) nor OSTEP's OS-level view of virtual memory covers at the hardware level |
| *Mastering Embedded Linux Programming* | Chris Simmonds | Sprint 9 |
| *Embedded Linux Systems with the Yocto Project* + Bootlin Labs | Rudolf J. Streif / Bootlin | Sprint 9 |

### 8.3 MCU & FreeRTOS

| Resource | Source | Used for |
|---|---|---|
| STM32 reference manual and HAL driver source | STMicroelectronics | Sprint 10 |
| FreeRTOS official documentation and kernel source | FreeRTOS.org | Sprint 10 |
| *Computer Organization and Design: ARM Edition* — ISA/assembly chapters only | Patterson & Hennessy | Sprint 10 — light, optional supplement for reading disassembly and compiler output; skip the cache/virtual-memory chapters here, they're mapped to Sprint 3 (Section 8.2) instead since they describe Cortex-A, not Cortex-M, behavior |

### 8.4 Networking & Security

| Resource | Author | Used for |
|---|---|---|
| *Beej's Guide to Network Programming* | Brian "Beej" Hall | Sprint 12 |
| OpenSSL documentation | OpenSSL project | Sprint 14 |

### 8.5 Extension Track only

*Systems Performance* and *BPF Performance Tools* (Brendan Gregg) for
V4.0; OP-TEE and Trusted Firmware-A documentation for V5.0; LVGL
documentation and porting guide for V8.0 (embedded GUI, optional).

**On *Computer Systems: A Programmer's Perspective* (CS:APP, Bryant &
O'Hallaron):** not recommended as Core Track reading — most of its
content (process control, signals, virtual memory from an OS-API
viewpoint) duplicates OSTEP and TLPI, already precisely mapped into the
Core Track, and its primary example architecture is x86-64 rather than
ARM. Two chapters are genuinely unique and not covered anywhere else in
this roadmap: **linking/loading** (how the linker and loader actually
work) and **machine-code-level buffer-overflow/exploitation**. Both are
worth reading for V5.0 (Security) specifically, as a precursor to
*Practical Binary Analysis* (Andriesse) — the exploitation chapter
builds directly toward the binary-analysis skills that book requires.

*C++ Concurrency in Action* (Anthony Williams) as multi-threading grows
more complex.

---

## 9. Core Track — Detailed Sprint Plan

**Universal Definition of Done — applies to every sprint below, in
addition to each sprint's specific criteria:**
- [ ] Code is readable by a stranger in about 10 minutes: header comment stating purpose, meaningful naming, no dead code left in.
- [ ] A technical note (300–600 words: what was learned, what broke, how it was fixed) is written to `notes/`.
- [ ] At least one piece of real, production source code was read, per that sprint's "Reading" line below — not tutorial code.

**Why kernel work comes immediately after the foundation sprints, with a
warm-up built in rather than a separate sprint:** a full MCU/FreeRTOS
sprint before Sprint 3 would either duplicate what the kernel work
itself teaches (register/interrupt intuition) or require buying the MCU
board months before it's needed. Instead, Sprint 3 opens with a short
user-space register-access exercise on the same board already being
used for kernel work, then proceeds directly into the kernel driver
material — no separate sprint, no duplicated content, no early hardware
purchase, and no added time (it fits inside the sprint's existing
buffer). The full MCU/FreeRTOS build is deferred to Sprint 10, after the
entire Linux track is complete.

---

### Sprint 0 — Environment Setup
- **Goal:** reproducible dev environment and repository skeleton.
- **Duration:** ~2 weeks | **Prerequisites:** none
- **Study:** Git, Bash, CMake, Markdown
- **Build:** repository skeleton per Section 5.1, coding style guide, initial README, LICENSE file, and a minimal CI workflow (build + run unit tests on every push)
- **Definition of Done (specific):**
  - [ ] Repository builds an empty CMake project successfully
  - [ ] README explains how to build and run
  - [ ] CI pipeline runs on every push and passes

### Sprint 1 — Logger
- **Goal:** reusable logging library used by every later component.
- **Duration:** ~2 weeks | **Prerequisites:** Sprint 0
- **Study:** TLPI File I/O; OSTEP File Systems
- **Reading:** source of a small open-source C++ logging library (e.g. spdlog) — focus on how log levels and sinks are structured, not on copying it
- **Build:** logger library, log levels, simple file rotation, unit tests
- **Definition of Done (specific):**
  - [ ] Standalone library other modules can link against
  - [ ] File rotation verified with a test exceeding the size threshold

### Sprint 2 — Configuration Manager
- **Goal:** centralized, reloadable configuration.
- **Duration:** ~2 weeks | **Prerequisites:** Sprint 1
- **Study:** JSON parsing and schema design
- **Reading:** a small, well-known JSON library's example usage (e.g. nlohmann/json)
- **Build:** Config Manager, `config.json`, live reload
- **Definition of Done (specific):**
  - [ ] No config value is hard-coded elsewhere in the codebase
  - [ ] Invalid config is rejected with a clear error, not a crash

---

### Sprint 3 — Kernel & Device Tree Fundamentals
- **Goal:** move from user-space-only competency into real kernel-space work, with a short hardware warm-up first to reduce risk.
- **Duration:** ~8–10 weeks (contingency buffer included — kernel debugging is the least predictable work for a self-taught learner; the warm-up lab below fits inside this buffer and adds no extra time)
- **Prerequisites:** Sprint 2; BeagleBone Black Rev C acquired
- **Study:** register-level I/O concepts (memory-mapped I/O, `/dev/mem`, `mmap()`); then Linux Kernel Labs — boot process, character device drivers, interrupt handling, Linux Device Model, kernel synchronization
- **Reading:** the BeagleBone Black's AM335x Technical Reference Manual section on GPIO registers; one simple character driver from `drivers/char/` in the mainline kernel source; the BeagleBone Black's own Device Tree source (`am335x-boneblack.dts`)
- **Lab 0 (throwaway, ~3–5 days, hardware warm-up before any kernel code):** write a small user-space C program that opens `/dev/mem`, `mmap()`s the GPIO register region, and toggles a pin via direct register read/write — no kernel API, no HAL. This builds the bitwise/register intuition the kernel driver needs, on real hardware, without touching kernel code and without needing the MCU board. **Caution:** a pin must be muxed to GPIO mode before raw register writes take effect — use the `config-pin` utility or confirm the pin's mode in the AM335x Technical Reference Manual before assuming a register write has failed.
- **Lab (throwaway, kernel-specific):** follow 2–3 official Linux Kernel Labs exercises exactly as written, on disposable code, before touching the "real" driver — treat these as safe-to-break sandboxes
- **Build (ships in `eldmp-kernel-driver`):** one custom character device driver, one GPIO driver, a Device Tree overlay binding it to real hardware (button/LED)
- **Definition of Done (specific):**
  - [ ] Lab 0: GPIO toggled via `/dev/mem` + `mmap()` in user-space, verified on real hardware
  - [ ] Custom driver loads via `insmod` and is visible in `dmesg` without errors
  - [ ] Device Tree overlay correctly binds the driver to real hardware
  - [ ] Interrupt-driven GPIO input verified (not polling)

### Sprint 4 — Device Manager
- **Goal:** expose device identity and system information.
- **Duration:** ~2–3 weeks | **Prerequisites:** Sprint 3
- **Study:** `/proc` and `/sys` filesystem structure
- **Reading:** how an existing tool (e.g. `lm-sensors` or `htop`) reads `/proc`/`/sys`
- **Build:** `eldmp info` — CPU, RAM, kernel version, hostname; optionally also surfaces data from the Sprint 3 custom driver
- **Definition of Done (specific):**
  - [ ] Output matches actual system state, verified against `uname`/`free`/`lscpu`

### Sprint 5 — Health Monitor
- **Goal:** continuous resource monitoring.
- **Duration:** ~2–3 weeks | **Prerequisites:** Sprint 4
- **Study:** OSTEP Threads, Locks, Condition Variables; timers
- **Build:** `eldmp monitor` — CPU/RAM/disk usage on an interval
- **Definition of Done (specific):**
  - [ ] Runs continuously without memory leak over a multi-hour soak test
  - [ ] Sampling interval configurable via Config Manager

### Sprint 6 — Inter-Process Communication (IPC)
- **Goal:** turn Device, Logger, and Health into independent services communicating only through IPC.
- **Duration:** ~3–4 weeks | **Prerequisites:** Sprint 5
- **Study:** D-Bus concepts and API; OSTEP Processes & Process API
- **Reading:** the D-Bus interface definitions of an existing real service (e.g. NetworkManager) to see how a production D-Bus API is shaped
- **Build:** Device/Logger/Health Services, all D-Bus based
- **Definition of Done (specific):**
  - [ ] No service calls another service's internals directly — only D-Bus
  - [ ] Killing one service does not crash the others
  - [ ] Cross-service behavior verified (manual/scripted — see Section 6)

### Sprint 7 — systemd Integration
- **Goal:** run every service as a managed systemd unit.
- **Duration:** ~2 weeks | **Prerequisites:** Sprint 6
- **Study:** systemd unit files, `journalctl`, `systemctl`
- **Reading:** 3–4 real `.service` files already present on your own machine (`/lib/systemd/system/`) — compare how a mature project structures dependencies and restart policy
- **Build:** `logger.service`, `device.service`, `monitor.service`
- **Definition of Done (specific):**
  - [ ] All services start automatically on boot
  - [ ] Services restart automatically on crash at the systemd level

### Sprint 8 — Watchdog
- **Goal:** application-level supervision on top of systemd.
- **Duration:** ~2–3 weeks | **Prerequisites:** Sprint 7
- **Reading:** how an existing lightweight supervisor (e.g. `runit` or `s6`) implements heartbeat/restart, for design comparison
- **Build:** heartbeat mechanism, automatic restart of unresponsive services, failure logging
- **Definition of Done (specific):**
  - [ ] A deliberately hung service is detected and restarted within a defined timeout

**→ Release V1.0 — "Portfolio-ready kernel-space + user-space platform."**
Full release checklist applies (Section 10): architecture diagram, demo
video, git tag, changelog entry.

---

### Sprint 9 — Buildroot / Yocto
- **Goal:** boot ELDMP as a fully custom embedded Linux image.
- **Duration:** ~6–8 weeks | **Prerequisites:** V1.0
- **Study:** *Mastering Embedded Linux Programming*; Bootlin Labs
- **Reading:** an existing BSP meta-layer for the BeagleBone Black (e.g. `meta-ti` or the community `meta-beagleboard` layer for Yocto, or Buildroot's own `board/beaglebone` configuration)
- **Lab (throwaway):** build one minimal, unrelated Buildroot image first, purely to learn the menuconfig/build loop before committing to the real ELDMP image
- **Build (ships in `eldmp-yocto-layer` + integrated into `eldmp`):** full custom image (U-Boot + kernel + rootfs) via Buildroot, then via Yocto layers/recipes
- **Definition of Done (specific):**
  - [ ] Board boots directly into the ELDMP platform, no manual post-boot setup
  - [ ] Both a Buildroot image and a Yocto image exist and boot successfully
  - [ ] Image build is scripted and reproducible from a clean checkout

**→ Release V1.5.**

---

### Sprint 10 — MCU Bring-up & FreeRTOS
- **Goal:** build the full MCU application layer — sensor/control logic using proper FreeRTOS task, queue, and semaphore patterns.
- **Duration:** ~5–7 weeks | **Prerequisites:** V1.5; STM32 Nucleo-H743ZI acquired
- **Study:** UART, SPI, I2C, CAN, DMA; FreeRTOS tasks, queues, semaphores/mutexes, timing
- **Reading:** the STM32 HAL driver source for the peripherals in use, and the FreeRTOS kernel source for task scheduling and queue implementation
- **Lab (throwaway):** blink/read a single sensor using vendor example code first, to isolate toolchain/board issues from your own logic; separately, a small producer/consumer exercise using a FreeRTOS queue between two tasks, before wiring the real application
- **Build (ships in `eldmp-mcu-firmware`):** sensor read + motor/PWM control loop on the MCU, structured as separate FreeRTOS tasks synchronized via queues/semaphores (not shared global variables), communicating with a PC over UART
- **Definition of Done (specific):**
  - [ ] Sensor readings correct and stable over time
  - [ ] Control loop runs at a deterministic, measured rate
  - [ ] Tasks are synchronized via FreeRTOS queue/semaphore primitives — verified by a deliberately induced race-condition test that the synchronized version passes and an unsynchronized version would fail

### Sprint 11 — Linux ↔ MCU Protocol
- **Goal:** design and implement a real, reliable communication protocol between the two processors.
- **Duration:** ~8–10 weeks, with contingency buffer already included
- **Prerequisites:** Sprint 10
- **Reading:** the source of an existing embedded protocol (e.g. Modbus RTU or MAVLink) to see how a battle-tested design handles CRC, retry, and versioning — not to copy it, but to compare against your own design decisions
- **Build (progressive, ships in `eldmp-linux-mcu-protocol`):**
  1. `Linux → UART → STM32`, then SPI, then CAN
  2. Protocol design: heartbeat, CRC verification, retry logic, timeout handling, version negotiation
  3. Linux sends control commands; MCU returns status
- **Definition of Done (specific):**
  - [ ] Protocol survives a deliberately introduced link failure without corrupting state
  - [ ] CRC correctly rejects at least one deliberately corrupted test message
  - [ ] Protocol version mismatch is detected and handled gracefully

**→ Release V2.0 — the single strongest differentiator in the whole
project. Do not compress this sprint under schedule pressure.**

---

### Sprint 12 — Networking
- **Goal:** connect the platform to a network and a remote broker.
- **Duration:** ~4–6 weeks | **Prerequisites:** V2.0
- **Study:** TCP/IP fundamentals, MQTT
- **Reading:** a lightweight MQTT client library's source (e.g. Paho MQTT C client)
- **Build:** Network Manager service, MQTT client publishing device health/status
- **Definition of Done (specific):**
  - [ ] Device status visible on a remote MQTT broker in real time
  - [ ] Network Manager recovers automatically after a connection drop

**→ Release V2.5.**

### Sprint 13 — OTA (Over-the-Air Updates)
- **Goal:** allow the platform to update itself remotely.
- **Duration:** ~4–6 weeks | **Prerequisites:** V2.5
- **Study:** SWUpdate or RAUC
- **Reading:** the RAUC or SWUpdate source directly — how a real OTA client structures download/verify/install/rollback
- **Build:** download, verify, install, reboot (A/B deferred to Extension Track)
- **Definition of Done (specific):**
  - [ ] A deliberately corrupted update package is rejected before installation
  - [ ] A successful update is verifiable via a version string change after reboot

**→ Release V3.0.**

### Sprint 14 — Security Foundation
- **Goal:** close the most obvious security gaps before calling the Core Track complete.
- **Duration:** ~4–6 weeks | **Prerequisites:** V3.0
- **Study:** OpenSSL, TLS, certificate handling
- **Reading:** a minimal TLS client-server example built directly on OpenSSL
- **Build:** HTTPS for the management endpoint, MQTT over TLS, signed OTA packages
- **Definition of Done (specific):**
  - [ ] An unsigned or tampered OTA package is rejected
  - [ ] All device↔broker/server traffic is encrypted, verified with a packet capture

**→ Release V3.5 — Core Track complete.** Full, defensible portfolio for
Senior Embedded Linux Engineer roles.

---

## 10. Release Checklist

Applies at every major version milestone (V1.0, V1.5, V2.0, V2.5, V3.0, V3.5, and Extension Track versions):

- [ ] Working code with passing unit, integration, and regression tests
- [ ] README updated to reflect current capabilities
- [ ] Architecture diagram updated in `docs/architecture/`
- [ ] Release notes / changelog entry in `docs/release-notes/`
- [ ] Any decision from the capped ADR list (Section 15) logged if it applied this version
- [ ] Git tag created
- [ ] Demo video (2–5 minutes) — required only at V1.0, V2.0, and V3.5, the three milestones with genuinely distinct new capability to show; optional at V1.5/V2.5/V3.0 to avoid turning documentation into a production-quality tax

Sprint-level work only needs a clean commit history and the Universal
Definition of Done from Section 9 — the full checklist above is reserved
for version milestones.

---

## 11. Timeline Summary & Weekly Cadence

### 11.1 Sprint-by-sprint timeline (baseline pace)

| Sprint | Duration | Cumulative | Milestone |
|---|---|---|---|
| 0 | 2 weeks | 2 weeks | — |
| 1 | 2 weeks | 4 weeks | — |
| 2 | 2 weeks | 6 weeks | — |
| 3 (Kernel & DT, incl. warm-up) | 8–10 weeks | ~15 weeks | *(kernel/driver skill demonstrable here, ~3.5 months)* |
| 4 | 2–3 weeks | ~18 weeks | — |
| 5 | 2–3 weeks | ~21 weeks | — |
| 6 | 3–4 weeks | ~25 weeks | — |
| 7 | 2 weeks | ~27 weeks | — |
| 8 | 2–3 weeks | ~30 weeks | **V1.0** |
| 9 | 6–8 weeks | ~38 weeks | **V1.5** |
| 10 (MCU + FreeRTOS) | 5–7 weeks | ~44 weeks | — |
| 11 (Protocol) | 8–10 weeks | ~53 weeks | **V2.0** |
| 12 (Networking) | 4–6 weeks | ~58 weeks | **V2.5** |
| 13 (OTA) | 4–6 weeks | ~63 weeks | **V3.0** |
| 14 (Security) | 4–6 weeks | ~68 weeks | **V3.5 — Core Track complete** |

**Total Core Track duration: approximately 15–17 months** at the
baseline part-time pace. Kernel-space work — the strongest, most
differentiated skill this roadmap protects — is demonstrable by roughly
month ~3.5, and V1.0 (kernel + full user-space platform) is reached by
month ~7. The hardware warm-up lab in Sprint 3 and the added FreeRTOS
synchronization depth in Sprint 10 are absorbed without lengthening the
roadmap versus a plain kernel-first ordering.

### 11.2 Pace scaling by weekly time budget

| Weekly time budget | Typical situation | Approx. Core Track duration |
|---|---|---|
| ~8–10 hrs/week | Part-time, alongside a full-time job | ~15–17 months (baseline) |
| ~15–20 hrs/week | Dedicated evenings and weekends | ~9–12 months |
| ~35+ hrs/week | Full-time focus | ~5–7 months |

Suggested split: roughly 30% reading/study, 70% building/debugging —
study time not followed by an attempt to apply it that same week is a
warning sign, not normal progress.

---

## 12. Extension Track

Optional, chosen only after V3.5, based on career direction:

| Version | Focus | Why you'd pick it |
|---|---|---|
| V4.0 | Performance & Observability (*Systems Performance*, eBPF, `perf`) | Platform/infrastructure-heavy roles |
| V5.0 | Secure Boot & TEE (OP-TEE, TF-A, `dm-verity`, `dm-crypt`, A/B rollback) | Automotive, medical, security-sensitive roles |
| V6.0 | Industrial SoC Port on NXP i.MX93 — RemoteProc, RPMsg, asymmetric multiprocessing, Time-Sensitive Networking, Real-Time Linux | Robotics or industrial automation roles |
| V7.0 | REST API / WebUI / Cloud Dashboard | Product-oriented roles |
| V8.0 | ROS2 / on-device AI inference / embedded GUI (LVGL) | Robotics, edge-AI, or automotive/appliance HMI-focused roles — requires a small LCD module not purchased by default (see Section 7) |

---

## 13. Milestone & Skill Reference

A single reference table covering every release — what it means, why it
matters for a portfolio, and how to talk about it in an interview.

| Milestone | ~Week | Skills gained | Portfolio significance | Interview talking point |
|---|---|---|---|---|
| **V1.0** | 30 | Kernel driver + Device Tree, D-Bus IPC, systemd, watchdog supervision, modern C++, register-level I/O | First release combining kernel-space and user-space work — the strongest possible "early" checkpoint if a job search starts before the roadmap finishes | "I built a multi-service embedded platform where a custom kernel driver exposes real hardware data through D-Bus services, managed by systemd with an additional application-level watchdog." |
| **V1.5** | 38 | Buildroot, Yocto, custom rootfs, cross-compilation | Can build and boot a fully custom embedded Linux image from scratch | "I built and booted a custom embedded Linux image using both Buildroot and Yocto, understanding every layer from bootloader to root filesystem." |
| **V2.0** | 53 | Advanced FreeRTOS firmware (tasks/queues/semaphores), custom Linux↔MCU protocol (CRC/retry/versioning) | The strongest differentiator in the project — an MCU firmware built with proper RTOS synchronization, paired with a protocol that was designed and fault-tested, not just used | "I designed and implemented a fault-tolerant protocol between Linux and an MCU — where the MCU side runs its own FreeRTOS tasks synchronized via queues and semaphores — and validated the link against real failures, not just the happy path." |
| **V2.5** | 58 | TCP/IP, MQTT | Platform is network-connected and remotely observable | "Device health and status are published to a remote broker in real time, with automatic reconnection." |
| **V3.0** | 63 | OTA lifecycle (SWUpdate/RAUC) | Behaves like a real commercial IoT/industrial product | "Implemented an OTA pipeline that rejects corrupted packages before installation." |
| **V3.5** | 68 | TLS, signed updates | Portfolio complete — defensible for Senior Embedded Linux Engineer interviews | "The full platform is encrypted end-to-end and update-signed." |

Each talking point is deliberately a specific, falsifiable claim — the
kind an interviewer can ask a follow-up question about — not a vague
"worked on embedded Linux."

---

## 14. Interview & Portfolio Presentation Guide

### 14.1 Top-level `eldmp` README outline

1. One-line pitch — what ELDMP is and who it's for, with the CI build status badge
2. Architecture diagram (Section 4.1)
3. "What this demonstrates" — a short bullet list drawn from Section 13
4. Current version and what changed most recently
5. Links to the four standalone spin-out repos (Section 5.3)
6. How to build/run, or where to watch the demo video
7. Link to this roadmap document

### 14.2 Standalone repo README outline (for the 4 spin-outs)

Each of `eldmp-kernel-driver`, `eldmp-yocto-layer`, `eldmp-mcu-firmware`,
and `eldmp-linux-mcu-protocol` should be readable with zero context from
the umbrella repo: one-paragraph problem statement, what was built, how
to run/test it, and one link back to `eldmp` for the bigger picture.

---

## 15. Architecture Decision Records (ADR)

**This project intentionally caps Core Track ADRs at four.** If a
decision doesn't belong on this list, a one-line note in the commit
message is enough — writing an ADR for every small choice creates
fatigue and stops being read.

**Location:** `docs/architecture/decisions/ADR-XXXX-short-title.md`

**Template:**

```
# ADR-XXXX: <Title>

Status: Proposed | Accepted | Superseded by ADR-YYYY
Date: <date>

## Context
What problem or decision is being addressed, and what constraints apply.

## Decision
What was actually chosen.

## Consequences
What this makes easier, what this makes harder, what was given up.
```

**The four Core Track ADRs:**

1. **ADR-0001 — Split architecture:** Linux SoC + separate real-time MCU, instead of one powerful SoC handling everything.
2. **ADR-0002 — Both Buildroot and Yocto**, instead of picking just one build system.
3. **ADR-0003 — D-Bus for IPC**, instead of raw Unix sockets or shared memory.
4. **ADR-0004 — A custom, progressively-built protocol (UART→SPI→CAN)** for the Linux↔MCU link, instead of adopting an existing protocol like Modbus directly.

(A fifth, ADR-0005, is added only if and when the i.MX93 port is planned in detail at V6.0. The choice to fold hardware warm-up into Sprint 3 rather than a separate MCU-first sprint is documented in Section 1's Design Highlights rather than as a separate ADR, since it is a roadmap sequencing decision rather than a system architecture decision.)

---

## 16. Risks & Pitfalls

A single practical list — for a solo project, one list covering both
risk and mitigation is enough.

| Risk / Pitfall | Why it happens | How to avoid it |
|---|---|---|
| Buying hardware too early | Excitement to "get started" with hardware | Follow Section 7's acquisition timing — hardware bought before its sprint tends to sit unused |
| Bricking hardware during kernel/driver work | Inevitable when flashing/experimenting at this level | Keep an SD card backup image; treat the board as expendable during Sprint 3 experimentation |
| Compressing Sprint 3 or Sprint 11 | These are the two least predictable, highest-value sprints | Both already carry explicit contingency buffers — protect them, don't reclaim the buffer for other sprints |
| Skipping Lab 0 (the `/dev/mem` warm-up) to "save time" | It's tempting to jump straight to kernel modules | Lab 0 is short (3–5 days) and fits inside Sprint 3's existing buffer — skipping it doesn't save meaningful time but does remove the risk-reduction it provides |
| Scope creep from Extension Track ideas | Secure boot or performance tooling can look more interesting than finishing OTA/TLS | Enforced by Principle 6 — log the idea, don't build it, until V3.5 |
| Over-investing in D-Bus test automation | Mocking D-Bus is genuinely difficult and low market value at this stage | Manual/scripted verification is explicitly sufficient for Core Track (Section 6) |
| Writing an ADR for every small decision | Feels rigorous, but creates fatigue and stops being read | Capped at exactly 4 for the Core Track (Section 15) |
| Documentation debt | Easy to defer "later" | Pre-empted by the Universal Definition of Done (Section 9), which makes the per-sprint note non-optional |
| Job-search timeline pressure vs. full roadmap length | Real life doesn't wait for V3.5 | V1.0 already includes kernel-space work, so it is a strong standalone checkpoint on its own, reached by month ~7 |

---

## 17. Glossary

- **ADR (Architecture Decision Record):** a short, permanent document recording a single non-obvious technical decision, its context, and its consequences.
- **BSP (Board Support Package):** bootloader, kernel configuration, and drivers needed to run Linux on a specific board.
- **Buildroot:** a tool for generating minimal, custom embedded Linux systems through cross-compilation.
- **CRC (Cyclic Redundancy Check):** a checksum algorithm used to detect accidental data corruption.
- **D-Bus:** a message-bus system for inter-process communication on Linux.
- **Device Tree (DT):** a data structure describing hardware layout to the Linux kernel, separate from the kernel binary.
- **HAL (Hardware Abstraction Layer):** a software layer presenting a consistent API regardless of underlying hardware.
- **HIL (Hardware-in-the-Loop) testing:** testing involving real physical hardware rather than simulation.
- **IPC (Inter-Process Communication):** mechanisms allowing separate processes to exchange data.
- **LVGL (Light and Versatile Graphics Library):** an open-source, lightweight embedded GUI library for MCUs and MPUs, used here as an optional Extension Track technology for HMI/display work.
- **MQTT:** a lightweight publish/subscribe messaging protocol common in IoT systems.
- **OTA (Over-the-Air update):** remotely updating device firmware/software without physical access.
- **RAUC / SWUpdate:** open-source frameworks for managing embedded Linux OTA updates, including rollback.
- **RootFS (Root Filesystem):** the filesystem mounted at boot containing everything needed to run the OS.
- **RTOS (Real-Time Operating System):** an OS (e.g. FreeRTOS) guaranteeing response within strict timing constraints.
- **TLS (Transport Layer Security):** the cryptographic protocol securing network communication.
- **U-Boot:** a widely used open-source bootloader for embedded systems.
- **Yocto Project:** tools and metadata for building custom embedded Linux distributions, more flexible and complex than Buildroot.
