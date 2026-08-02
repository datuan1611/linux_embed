# ELDMP — Embedded Linux Device Management Platform
## Complete Development Roadmap (MCU-First Track)

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
competency in embedded systems engineering — spanning MCU bare-metal and
RTOS firmware, embedded GUI, Linux kernel internals, user-space service
architecture, embedded board bring-up, and secure device communication —
while simultaneously producing a public, versioned software portfolio.

The project follows a **project-driven learning model**: every unit of
study is paired with a concrete, shippable piece of the platform. Nothing
is studied in isolation. Progress is tracked through semantic version
releases (V0.5, V1.0, V1.5, V2.0 …), each representing a fully working,
demoable system state.

This edition follows an **MCU-first trajectory**: bare-metal MCU
fundamentals, then FreeRTOS, then embedded GUI (LVGL), then Embedded
Linux (kernel through a custom Buildroot/Yocto image), then the
multi-layer SoC+MCU integration that ties both halves together. A
companion edition of this roadmap follows the reverse order
(Linux-first); the two are equally valid, deliberately different
strategies, not a "correct" and an "incorrect" version.

**Design highlights.** Three choices shape this roadmap more than any
other:

1. **MCU work — bare-metal, then RTOS, then embedded GUI — comes first,
   building upward through three separate sprints (Sprint 3–5) before
   any Linux/kernel work begins.** This ordering treats hardware
   fundamentals as a genuine prerequisite chain, not just a warm-up:
   bare-metal register and interrupt work (Sprint 3) is the foundation
   FreeRTOS task management (Sprint 4) builds on, and FreeRTOS is what
   the embedded GUI (Sprint 5) runs inside. The result is a fully
   realized, market-relevant capability — FreeRTOS-based firmware
   driving a live LVGL interface — demonstrable and tagged as V0.5 by
   roughly month 4, directly matching the FreeRTOS+LVGL skill profile
   most in demand across the broader embedded job market. Because this
   genuine hardware depth is already built by the time kernel work
   starts (Sprint 6), that sprint skips the register-access warm-up lab
   used in a Linux-first ordering of this roadmap — the intuition
   transfers directly from real MCU work, not a short bridge exercise.
   The trade-off: reaching V1.0 (kernel + full service platform) takes
   roughly six weeks longer than a Linux-first ordering, since three
   substantial MCU sprints happen before it rather than one.
2. **Process is kept deliberately light.** Every checklist, template, or
   review step in this document exists only because it demonstrably
   improves learning outcome or portfolio quality for a one-person
   project — nothing is carried over from enterprise practice by
   default.
3. **Learning happens through three channels, not just reading.** Every
   sprint names real source code to read, distinguishes throwaway labs
   from the shipped build, and requires a short written note — reading
   real code, deliberate practice, and teaching-to-learn, alongside
   book chapters mapped precisely to the sprint that needs them, never
   read as a long, undifferentiated stack up front.

The roadmap is split into two tracks:

- **Core Track** (Sprint 0–16, ~16–19 months at a part-time pace):
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
   reading list — book chapters mapped to the sprint that needs them,
   never "read the whole book first."
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
        |  Embedded GUI (LVGL)              |
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
| MCU firmware | Deterministic real-time control, and the embedded GUI | Runs where Linux cannot guarantee real-time behavior, and where the physical display lives |
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
│   ├── driver/             # kernel modules (from Sprint 6)
│   ├── mcu/                 # bare-metal, FreeRTOS, LVGL UI (from Sprint 3)
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
five-minute skim. Four sprints produce work valuable enough to also
exist as **small, standalone, independently browsable repositories**,
later integrated into `eldmp` (as a git submodule or simply documented
and linked):

| Standalone repo | Sprint | Why it deserves its own repo |
|---|---|---|
| `eldmp-mcu-firmware` | Sprint 3–5 | Bare-metal, FreeRTOS, and LVGL work, committed progressively — a focused firmware repo directly showable for roles centered on MCU/RTOS/embedded-GUI work |
| `eldmp-kernel-driver` | Sprint 6 | A focused, ~500-line driver + DT overlay is far more legible on its own than buried inside a large service codebase |
| `eldmp-yocto-layer` | Sprint 12 | A custom Yocto meta-layer is a recognizable, self-contained artifact recruiters in this space specifically look for |
| `eldmp-linux-mcu-protocol` | Sprint 13 | The single strongest piece of work in the whole project; it should be one click away, not several folders deep |

Everything else stays inside the `eldmp` umbrella repo, which functions
as the integration point and the "big picture" story.

---

## 6. Testing Strategy

| Test level | Scope | Introduced at | Tooling | Notes |
|---|---|---|---|---|
| Unit tests | Single module in isolation | Sprint 1 | GoogleTest or Catch2 | No hardware or live D-Bus dependency |
| Integration tests | Two or more services over IPC | Sprint 9 | Same framework, running D-Bus session | Manual, scripted verification is acceptable here. Building a full automated D-Bus mocking harness is not required for the Core Track — it is a genuinely painful, low-market-value investment at this stage. |
| System / boot tests | Full image boots, all services start | Sprint 6, expanded Sprint 12 | Manual boot + log inspection | Formal scripted boot testing is Extension Track (V4.0) material |
| Hardware-in-the-loop (HIL) | Real MCU + real board over the physical link | Sprint 13 | Manual fault injection (cable unplug, corrupted payload) | This is where rigor matters most — see Sprint 13 Definition of Done |
| Regression tests | Re-run of previous sprints' tests together | Every version milestone | Aggregated local run | Required before every tag |

**Convention:** test location mirrors source location
(`tests/<module>/test_<name>.cpp` ↔ `src/<module>/`). From Sprint 0
onward, unit tests also run automatically in CI on every push (Section
5.2). A version is not tagged until the full regression suite passes.

---

## 7. Hardware Roadmap & Budget

| Stage | Hardware | Role | Acquired at | Budget tier |
|---|---|---|---|---|
| Foundation | Ubuntu laptop or VM | Primary dev environment | Sprint 0 | — (existing) |
| MCU | STM32 Nucleo-H743ZI | Real-time companion, first hardware needed | Sprint 3 | $ |
| Display | Small SPI/parallel LCD module (LVGL-compatible) | Embedded GUI target | Sprint 5 | $ |
| Board | BeagleBone Black Rev C | Embedded Linux target | Sprint 6 | $ |
| Networking | USB-Ethernet adapter or small switch | Test bed | Sprint 14 (only if needed) | $ |
| Industrial SoC | NXP i.MX93 | Extension Track target | V6.0+ | $$–$$$ |

`$` = low-cost hobbyist board, `$$` = mid-range dev kit, `$$$` =
industrial/AI-class eval kit. Prices vary by region and time — treat
tiers as relative planning guidance, not quotes.

**Confirmed hardware path:** STM32 Nucleo-H743ZI first (Sprint 3, for
bare-metal and FreeRTOS work), a small LCD module next (Sprint 5, for
LVGL), then BeagleBone Black Rev C (Sprint 6, for the Linux/kernel
track), then NXP i.MX93 as the Extension Track industrial SoC target.
Each board is purchased right when its sprint needs it — no board sits
idle waiting to be used (Principle 4).

**Small components needed alongside the boards above** (low-cost,
usually cheaper as a generic "starter kit" bundle than bought
individually):

| Sprint | Components | Purpose |
|---|---|---|
| 3 | Breadboard, jumper wires (M–M and M–F), a few LEDs, 220–330Ω resistors, a few 10kΩ resistors, a few tactile push buttons | GPIO output/input for bare-metal register work; this same kit is reused for the BeagleBone Black in Sprint 6 |
| 4 | A small DC motor, a motor driver IC/breakout (e.g. L298N or DRV8833), a simple sensor (e.g. a potentiometer, a DS18B20 temperature sensor, or an IR distance sensor) | The sensor read + motor/PWM control loop build |
| 6 | A microSD card (Class 10+, 8–16GB), a **3.3V** USB-to-TTL serial cable | The microSD doubles as the backup image recommended in Section 16; the serial cable is the only way to see early boot logs if the board fails to come up over network/USB — a critical safety net for Sprint 6 and again for Sprint 12 |
| 13 | Two CAN transceiver breakout modules (e.g. MCP2551 or SN65HVD230), one per board | Neither the BeagleBone Black nor the STM32 Nucleo exposes a CAN transceiver on-board — only the CAN controller peripheral |

**Important:** the USB-to-TTL adapter for the BeagleBone Black must be
**3.3V** logic level, not 5V/RS232 — its UART header is not
5V-tolerant, and a 5V adapter can damage the board.

---

## 8. Study Resources

Book chapters are mapped precisely to the sprint that needs them — none
of these are meant to be read cover-to-cover before starting to build.
Where a resource list below differs from a strict single-book-per-topic
approach, it's because the most authoritative source for a given sprint
sometimes comes from more than one book.

### 8.1 MCU Foundations — Bare-Metal (Sprint 3)

| Resource | Author(s) | Used for |
|---|---|---|
| *The C Programming Language* | Kernighan & Ritchie | Optional refresher on pointers, bitwise operations, and `volatile` — skip if already comfortable with C from prior programming work |
| *The Definitive Guide to ARM Cortex-M3/M4* | Joseph Yiu | Exception model, NVIC, memory map — the chapters directly relevant to bare-metal bring-up |
| *Mastering STM32* | Carmine Noviello | Peripheral setup chapters matching whatever is in use (GPIO, UART, SPI, I2C, CAN, DMA) |
| STM32H743 reference manual | STMicroelectronics | Register-level detail, used as-needed rather than read start to end |

**Note on *Computer Organization and Design: ARM Edition*:** this is
placed in Section 8.6 (Extension) rather than as a Sprint 3
prerequisite. Its most valuable content for this project — cache
hierarchies, virtual memory, out-of-order pipelining — describes
Cortex-A-class behavior (relevant to Sprint 6's SoC) far more than
Cortex-M, which has neither cache nor an MMU in most parts. Reading it
in full before Sprint 3 would front-load hundreds of pages that don't
apply yet.

### 8.2 MCU + RTOS + Embedded GUI (Sprint 4–5)

| Resource | Author(s) | Used for |
|---|---|---|
| *Mastering the FreeRTOS Real Time Kernel* | Richard Barry (official FreeRTOS book) | Sprint 4 — tasks, queues, semaphores, timing |
| FreeRTOS kernel source | FreeRTOS.org | Sprint 4 — scheduler and queue implementation, read directly |
| LVGL documentation, porting guide, and example widgets | LVGL project | Sprint 5 |

### 8.3 Kernel & Embedded Linux (Sprint 6, 12)

| Resource | Author(s) | Used for |
|---|---|---|
| OSTEP — Address Spaces & Virtual Memory | Arpaci-Dusseau | Sprint 6 (kernel/user space separation is central to driver work) |
| Linux Kernel Labs | Linux Foundation / kernel community | Sprint 6 — hands-on exercises |
| *Linux Kernel Development* | Robert Love | Sprint 6 — the conceptual companion to Kernel Labs' hands-on exercises: why the scheduler and memory manager are structured the way they are |
| *Linux Device Drivers* (3rd ed., "LDD3") | Corbet, Rubini, Kroah-Hartman | Sprint 6 — the classic structural reference for driver design, **with a caveat:** written for kernel 2.6.10; many specific API calls it describes have since changed or been removed. Use it for concepts and structure, not copy-paste API calls — cross-check anything version-specific against current kernel documentation (`Documentation/driver-api/` in the kernel source tree) or Linux Kernel Labs. |
| the BeagleBone Black's own Device Tree source (`am335x-boneblack.dts`) | — | Sprint 6 |
| *Mastering Embedded Linux Programming* | Chris Simmonds | Sprint 12 |
| *Embedded Linux Systems with the Yocto Project* + Bootlin Labs | Rudolf J. Streif / Bootlin | Sprint 12 |

### 8.4 Foundation reading for the rest of the Linux track

| Resource chapters | Author(s) | Read before |
|---|---|---|
| OSTEP — File Systems & I/O | Arpaci-Dusseau | Sprint 1 (Logger does real file I/O and rotation) |
| OSTEP — Processes & Process API | Arpaci-Dusseau | Sprint 9 (IPC involves multiple cooperating processes) |
| OSTEP — Threads, Locks, Condition Variables | Arpaci-Dusseau | Sprint 8 (Health Monitor introduces threads/timers), reinforced at Sprint 11 (Watchdog) |
| TLPI — File I/O, Signals, IPC | Michael Kerrisk | Sprint 1–2 and 7–10, directly |
| Effective Modern C++ | Scott Meyers | Ongoing from Sprint 0, RAII/smart pointers used throughout the Linux-side C++ |

### 8.5 Networking & Security

| Resource | Author | Used for |
|---|---|---|
| *Beej's Guide to Network Programming* | Brian "Beej" Hall | Sprint 14 |
| OpenSSL documentation | OpenSSL project | Sprint 16 |

### 8.6 Extension Track only

*Systems Performance* and *BPF Performance Tools* (Brendan Gregg) for
V4.0; *Computer Organization and Design: ARM Edition* (Patterson &
Hennessy) as an optional deep-dive into cache/virtual-memory/pipelining
concepts once the SoC side is running (see note in 8.1); OP-TEE and
Trusted Firmware-A documentation for V5.0; *Practical Binary Analysis*
(Andriesse) for V5.0 if pursuing the security direction specifically;
*C++ Concurrency in Action* (Anthony Williams) as multi-threading grows
more complex.

---

## 9. Core Track — Detailed Sprint Plan

**Universal Definition of Done — applies to every sprint below, in
addition to each sprint's specific criteria:**
- [ ] Code is readable by a stranger in about 10 minutes: header comment stating purpose, meaningful naming, no dead code left in.
- [ ] A technical note (300–600 words: what was learned, what broke, how it was fixed) is written to `notes/`.
- [ ] At least one piece of real, production source code was read, per that sprint's "Reading" line below — not tutorial code.

**Why MCU work (bare-metal → RTOS → GUI) comes before Linux/kernel
work:** this ordering treats hardware fundamentals as a genuine
prerequisite chain rather than a warm-up exercise. Sprint 3's bare-metal
register and interrupt work is what Sprint 4's FreeRTOS task model is
built on top of; FreeRTOS is what Sprint 5's embedded GUI runs inside.
By Sprint 6, real register-level intuition already exists from actual
MCU hardware — so unlike a Linux-first ordering of this roadmap, Sprint
6 does not need a separate warm-up lab; the transfer is direct.

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

### Sprint 3 — Bare-Metal MCU Fundamentals
- **Goal:** bring up the MCU from bare-metal — registers, interrupts, memory-mapped I/O, and core peripherals — before any RTOS or GUI layer.
- **Duration:** ~4–5 weeks | **Prerequisites:** Sprint 2; STM32 Nucleo-H743ZI acquired
- **Study:** ARM Cortex-M4 exception model and NVIC (Yiu); register-level GPIO/UART/SPI/I2C/CAN/DMA (STM32 reference manual, *Mastering STM32*)
- **Reading:** the peripheral chapters of *Mastering STM32* matching what's in use; the STM32 reference manual sections for GPIO/UART/DMA registers
- **Lab (throwaway):** blink an LED via direct register writes — no HAL — before moving to HAL-based peripheral setup for the rest of the sprint, to build genuine register comfort
- **Build (ships in `eldmp-mcu-firmware`):** polling-based sensor read over UART/I2C/SPI, interrupt-driven GPIO input for a button, one basic DMA transfer — all bare-metal or thin HAL, no RTOS yet
- **Definition of Done (specific):**
  - [ ] Sensor read via polling is correct and stable
  - [ ] Interrupt-driven GPIO input verified (not polling)
  - [ ] At least one peripheral interaction implemented via direct register access (not HAL), documented in the sprint's technical note

### Sprint 4 — FreeRTOS Integration
- **Goal:** restructure the bare-metal work into a properly synchronized FreeRTOS application.
- **Duration:** ~3–4 weeks | **Prerequisites:** Sprint 3
- **Study:** FreeRTOS tasks, queues, semaphores/mutexes, timing
- **Reading:** the FreeRTOS kernel source for task scheduling and queue implementation
- **Lab (throwaway):** a small producer/consumer exercise using a FreeRTOS queue between two tasks, before wiring the real application
- **Build:** sensor read + motor/PWM control loop, restructured as separate FreeRTOS tasks synchronized via queues/semaphores (not shared global variables)
- **Definition of Done (specific):**
  - [ ] Control loop runs at a deterministic, measured rate
  - [ ] Tasks are synchronized via FreeRTOS queue/semaphore primitives — verified by a deliberately induced race-condition test that the synchronized version passes and an unsynchronized version would fail

### Sprint 5 — Embedded GUI (LVGL)
- **Goal:** add a live embedded GUI rendering real sensor data, running as its own FreeRTOS task — the same stack used for automotive instrument-cluster/infotainment and appliance HMI panels.
- **Duration:** ~2–3 weeks | **Prerequisites:** Sprint 4; small SPI/parallel LCD module acquired
- **Study:** LVGL widgets, event loop, display/input driver interface
- **Reading:** LVGL's own display/input driver porting guide, to see how a production-grade embedded GUI library expects to be integrated
- **Lab (throwaway):** run one of LVGL's built-in demo widgets on the display before wiring in real data
- **Build (ships in `eldmp-mcu-firmware`):** sensor data rendered live on an LVGL-based UI, running as an isolated FreeRTOS task that does not block the sensor/control tasks from Sprint 4
- **Definition of Done (specific):**
  - [ ] LVGL UI updates live from real sensor data without visible tearing or frame drops
  - [ ] The UI task is verified not to block or delay the control loop's deterministic timing (measured, not assumed)

**→ Release V0.5 — first full demoable capability.** FreeRTOS-based
firmware driving a live embedded GUI — directly aligned with
FreeRTOS+LVGL-focused roles. A lightweight tag: README, git tag, and a
short demo video are worthwhile here.

---

### Sprint 6 — Kernel & Device Tree Fundamentals
- **Goal:** move from MCU-only competency into real Linux kernel-space work.
- **Duration:** ~8–10 weeks (contingency buffer included — kernel debugging is the least predictable work for a self-taught learner, independent of prior MCU experience)
- **Prerequisites:** V0.5; BeagleBone Black Rev C acquired
- **Study:** Linux Kernel Labs — boot process, character device drivers, interrupt handling, Linux Device Model, kernel synchronization
- **Reading:** one simple character driver from `drivers/char/` in the mainline kernel source; the BeagleBone Black's own Device Tree source (`am335x-boneblack.dts`)
- **Note:** unlike a Linux-first ordering of this roadmap, this sprint does not need a separate register-access warm-up lab — Sprint 3's bare-metal work already built that intuition on real hardware. Only the kernel-specific tooling (`insmod`, `dmesg`, the Device Tree compiler, the kernel module build system) is genuinely new here.
- **Lab (throwaway, not shipped):** follow 2–3 official Linux Kernel Labs exercises exactly as written, on disposable code, before touching the "real" driver
- **Build (ships in `eldmp-kernel-driver`):** one custom character device driver, one GPIO driver, a Device Tree overlay binding it to real hardware (button/LED)
- **Definition of Done (specific):**
  - [ ] Custom driver loads via `insmod` and is visible in `dmesg` without errors
  - [ ] Device Tree overlay correctly binds the driver to real hardware
  - [ ] Interrupt-driven GPIO input verified (not polling)

### Sprint 7 — Device Manager
- **Goal:** expose device identity and system information.
- **Duration:** ~2–3 weeks | **Prerequisites:** Sprint 6
- **Study:** `/proc` and `/sys` filesystem structure
- **Reading:** how an existing tool (e.g. `lm-sensors` or `htop`) reads `/proc`/`/sys`
- **Build:** `eldmp info` — CPU, RAM, kernel version, hostname; optionally also surfaces data from the Sprint 6 custom driver
- **Definition of Done (specific):**
  - [ ] Output matches actual system state, verified against `uname`/`free`/`lscpu`

### Sprint 8 — Health Monitor
- **Goal:** continuous resource monitoring.
- **Duration:** ~2–3 weeks | **Prerequisites:** Sprint 7
- **Study:** OSTEP Threads, Locks, Condition Variables; timers
- **Build:** `eldmp monitor` — CPU/RAM/disk usage on an interval
- **Definition of Done (specific):**
  - [ ] Runs continuously without memory leak over a multi-hour soak test
  - [ ] Sampling interval configurable via Config Manager

### Sprint 9 — Inter-Process Communication (IPC)
- **Goal:** turn Device, Logger, and Health into independent services communicating only through IPC.
- **Duration:** ~3–4 weeks | **Prerequisites:** Sprint 8
- **Study:** D-Bus concepts and API; OSTEP Processes & Process API
- **Reading:** the D-Bus interface definitions of an existing real service (e.g. NetworkManager) to see how a production D-Bus API is shaped
- **Build:** Device/Logger/Health Services, all D-Bus based
- **Definition of Done (specific):**
  - [ ] No service calls another service's internals directly — only D-Bus
  - [ ] Killing one service does not crash the others
  - [ ] Cross-service behavior verified (manual/scripted — see Section 6)

### Sprint 10 — systemd Integration
- **Goal:** run every service as a managed systemd unit.
- **Duration:** ~2 weeks | **Prerequisites:** Sprint 9
- **Study:** systemd unit files, `journalctl`, `systemctl`
- **Reading:** 3–4 real `.service` files already present on your own machine (`/lib/systemd/system/`)
- **Build:** `logger.service`, `device.service`, `monitor.service`
- **Definition of Done (specific):**
  - [ ] All services start automatically on boot
  - [ ] Services restart automatically on crash at the systemd level

### Sprint 11 — Watchdog
- **Goal:** application-level supervision on top of systemd.
- **Duration:** ~2–3 weeks | **Prerequisites:** Sprint 10
- **Reading:** how an existing lightweight supervisor (e.g. `runit` or `s6`) implements heartbeat/restart, for design comparison
- **Build:** heartbeat mechanism, automatic restart of unresponsive services, failure logging
- **Definition of Done (specific):**
  - [ ] A deliberately hung service is detected and restarted within a defined timeout

**→ Release V1.0 — "Kernel-space + user-space platform."** This release
includes both real driver/Device Tree work and the full user-space
service architecture (the MCU/RTOS/GUI work was already demonstrated
earlier at V0.5). Full release checklist applies (Section 10).

---

### Sprint 12 — Buildroot / Yocto
- **Goal:** boot ELDMP as a fully custom embedded Linux image.
- **Duration:** ~6–8 weeks | **Prerequisites:** V1.0
- **Study:** *Mastering Embedded Linux Programming*; Bootlin Labs
- **Reading:** an existing BSP meta-layer for the BeagleBone Black (e.g. `meta-ti` or the community `meta-beagleboard` layer for Yocto, or Buildroot's own `board/beaglebone` configuration)
- **Lab (throwaway):** build one minimal, unrelated Buildroot image first, purely to learn the menuconfig/build loop
- **Build (ships in `eldmp-yocto-layer` + integrated into `eldmp`):** full custom image (U-Boot + kernel + rootfs) via Buildroot, then via Yocto layers/recipes
- **Definition of Done (specific):**
  - [ ] Board boots directly into the ELDMP platform, no manual post-boot setup
  - [ ] Both a Buildroot image and a Yocto image exist and boot successfully
  - [ ] Image build is scripted and reproducible from a clean checkout

**→ Release V1.5.**

---

### Sprint 13 — Linux ↔ MCU Protocol
- **Goal:** design and implement a real, reliable communication protocol between the two processors — the point where the MCU-first and Linux-first halves of this project finally meet.
- **Duration:** ~8–10 weeks, with contingency buffer already included
- **Prerequisites:** V1.5 (the MCU side has been ready since Sprint 5/V0.5)
- **Reading:** the source of an existing embedded protocol (e.g. Modbus RTU or MAVLink) to see how a battle-tested design handles CRC, retry, and versioning
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

### Sprint 14 — Networking
- **Goal:** connect the platform to a network and a remote broker.
- **Duration:** ~4–6 weeks | **Prerequisites:** V2.0
- **Study:** TCP/IP fundamentals, MQTT
- **Reading:** a lightweight MQTT client library's source (e.g. Paho MQTT C client)
- **Build:** Network Manager service, MQTT client publishing device health/status
- **Definition of Done (specific):**
  - [ ] Device status visible on a remote MQTT broker in real time
  - [ ] Network Manager recovers automatically after a connection drop

**→ Release V2.5.**

### Sprint 15 — OTA (Over-the-Air Updates)
- **Goal:** allow the platform to update itself remotely.
- **Duration:** ~4–6 weeks | **Prerequisites:** V2.5
- **Study:** SWUpdate or RAUC
- **Reading:** the RAUC or SWUpdate source directly — how a real OTA client structures download/verify/install/rollback
- **Build:** download, verify, install, reboot (A/B deferred to Extension Track)
- **Definition of Done (specific):**
  - [ ] A deliberately corrupted update package is rejected before installation
  - [ ] A successful update is verifiable via a version string change after reboot

**→ Release V3.0.**

### Sprint 16 — Security Foundation
- **Goal:** close the most obvious security gaps before calling the Core Track complete.
- **Duration:** ~4–6 weeks | **Prerequisites:** V3.0
- **Study:** OpenSSL, TLS, certificate handling
- **Reading:** a minimal TLS client-server example built directly on OpenSSL
- **Build:** HTTPS for the management endpoint, MQTT over TLS, signed OTA packages
- **Definition of Done (specific):**
  - [ ] An unsigned or tampered OTA package is rejected
  - [ ] All device↔broker/server traffic is encrypted, verified with a packet capture

**→ Release V3.5 — Core Track complete.** Full, defensible portfolio for
Senior Embedded Linux Engineer / Embedded Systems Engineer roles.

---

## 10. Release Checklist

Applies at every major version milestone (V0.5, V1.0, V1.5, V2.0, V2.5, V3.0, V3.5, and Extension Track versions):

- [ ] Working code with passing unit, integration, and regression tests
- [ ] README updated to reflect current capabilities
- [ ] Architecture diagram updated in `docs/architecture/`
- [ ] Release notes / changelog entry in `docs/release-notes/`
- [ ] Any decision from the capped ADR list (Section 15) logged if it applied this version
- [ ] Git tag created
- [ ] Demo video (2–5 minutes) — required only at V1.0, V2.0, and V3.5, the three milestones with genuinely distinct new capability to show; optional at V0.5/V1.5/V2.5/V3.0 to avoid turning documentation into a production-quality tax

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
| 3 (Bare-metal MCU) | 4–5 weeks | ~11 weeks | — |
| 4 (FreeRTOS) | 3–4 weeks | ~15 weeks | — |
| 5 (LVGL) | 2–3 weeks | ~18 weeks | **V0.5** |
| 6 (Kernel & DT) | 8–10 weeks | ~27 weeks | — |
| 7 | 2–3 weeks | ~30 weeks | — |
| 8 | 2–3 weeks | ~33 weeks | — |
| 9 | 3–4 weeks | ~37 weeks | — |
| 10 | 2 weeks | ~39 weeks | — |
| 11 | 2–3 weeks | ~42 weeks | **V1.0** |
| 12 (Buildroot/Yocto) | 6–8 weeks | ~50 weeks | **V1.5** |
| 13 (Protocol) | 8–10 weeks | ~59 weeks | **V2.0** |
| 14 (Networking) | 4–6 weeks | ~64 weeks | **V2.5** |
| 15 (OTA) | 4–6 weeks | ~69 weeks | **V3.0** |
| 16 (Security) | 4–6 weeks | ~74 weeks | **V3.5 — Core Track complete** |

**Total Core Track duration: approximately 16–19 months** at the
baseline part-time pace — roughly six weeks (~1.5 months) longer than a
Linux-first ordering of this same roadmap, because three substantial MCU
sprints (bare-metal, FreeRTOS, LVGL) happen before any Linux work rather
than being compressed into one. In exchange, the first fully realized,
market-relevant capability (V0.5: FreeRTOS + live embedded GUI) is
demonstrable by month ~4.2 — directly matching the skill profile most in
demand for MCU/RTOS-focused roles, well before V1.0's kernel work is
reached at month ~9.7.

### 11.2 Pace scaling by weekly time budget

| Weekly time budget | Typical situation | Approx. Core Track duration |
|---|---|---|
| ~8–10 hrs/week | Part-time, alongside a full-time job | ~16–19 months (baseline) |
| ~15–20 hrs/week | Dedicated evenings and weekends | ~10–13 months |
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
| V5.0 | Secure Boot & TEE (OP-TEE, TF-A, `dm-verity`, `dm-crypt`, A/B rollback); optionally *Practical Binary Analysis* if pursuing security/reverse-engineering depth | Automotive, medical, security-sensitive roles |
| V6.0 | Industrial SoC Port on NXP i.MX93 — RemoteProc, RPMsg, asymmetric multiprocessing, Time-Sensitive Networking, Real-Time Linux | Robotics or industrial automation roles |
| V7.0 | REST API / WebUI / Cloud Dashboard | Product-oriented roles |
| V8.0 | ROS2 / on-device AI inference / deeper computer-architecture study (*Computer Organization and Design: ARM Edition*) | Robotics or edge-AI specialization |

---

## 13. Milestone & Skill Reference

A single reference table covering every release — what it means, why it
matters for a portfolio, and how to talk about it in an interview.

| Milestone | ~Week | Skills gained | Portfolio significance | Interview talking point |
|---|---|---|---|---|
| **V0.5** | 18 | Bare-metal MCU (registers, interrupts), FreeRTOS (tasks/queues/semaphores), embedded GUI (LVGL) | First full demoable capability — directly matches the FreeRTOS+LVGL stack used in automotive HMI/infotainment and appliance display work | "I built FreeRTOS-based firmware for an STM32 MCU, from bare-metal register access up through a live LVGL user interface rendering real sensor data — the same stack used for automotive instrument clusters and appliance control panels." |
| **V1.0** | 42 | Kernel driver + Device Tree, D-Bus IPC, systemd, watchdog supervision, modern C++ | First release combining kernel-space and user-space Linux work, on top of the earlier MCU/GUI milestone | "I built a multi-service embedded Linux platform where a custom kernel driver exposes real hardware data through D-Bus services, managed by systemd with an additional application-level watchdog." |
| **V1.5** | 50 | Buildroot, Yocto, custom rootfs, cross-compilation | Can build and boot a fully custom embedded Linux image from scratch | "I built and booted a custom embedded Linux image using both Buildroot and Yocto, understanding every layer from bootloader to root filesystem." |
| **V2.0** | 59 | Custom Linux↔MCU protocol (CRC/retry/versioning) | The strongest differentiator in the project — where the MCU work from V0.5 and the Linux platform from V1.0/V1.5 are tied together by a protocol designed and fault-tested, not just used | "I designed and implemented a fault-tolerant protocol connecting Linux to the FreeRTOS-based MCU firmware, and validated it against real link failures, not just the happy path." |
| **V2.5** | 64 | TCP/IP, MQTT | Platform is network-connected and remotely observable | "Device health and status are published to a remote broker in real time, with automatic reconnection." |
| **V3.0** | 69 | OTA lifecycle (SWUpdate/RAUC) | Behaves like a real commercial IoT/industrial product | "Implemented an OTA pipeline that rejects corrupted packages before installation." |
| **V3.5** | 74 | TLS, signed updates | Portfolio complete — defensible for Senior Embedded Linux Engineer / Embedded Systems Engineer interviews | "The full platform is encrypted end-to-end and update-signed." |

Each talking point is deliberately a specific, falsifiable claim — the
kind an interviewer can ask a follow-up question about — not a vague
"worked on embedded systems."

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

Each of `eldmp-mcu-firmware`, `eldmp-kernel-driver`, `eldmp-yocto-layer`,
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

(A fifth, ADR-0005, is added only if and when the i.MX93 port is planned in detail at V6.0. The choice of an MCU-first sprint order — bare-metal, then RTOS, then GUI, then Linux — is documented in Section 1's Design Highlights rather than as a separate ADR, since it is a roadmap sequencing decision rather than a system architecture decision.)

---

## 16. Risks & Pitfalls

A single practical list — for a solo project, one list covering both
risk and mitigation is enough.

| Risk / Pitfall | Why it happens | How to avoid it |
|---|---|---|
| Buying hardware too early | Excitement to "get started" with hardware | Follow Section 7's acquisition timing — hardware bought before its sprint tends to sit unused |
| Getting stuck polishing the LVGL UI before moving to Linux | GUI work is visually rewarding and open-ended, easy to keep tweaking | Sprint 5's Definition of Done is deliberately narrow (live update, no tearing, no blocking) — treat it as closed once met, and move to Sprint 6 on schedule |
| Bricking hardware during kernel/driver work | Inevitable when flashing/experimenting at this level | Keep an SD card backup image; treat the board as expendable during Sprint 6 experimentation |
| Compressing Sprint 6 (Kernel) or Sprint 13 (Protocol) | These are the two least predictable, highest-value sprints | Both already carry explicit contingency buffers — protect them, don't reclaim the buffer for other sprints |
| Reading MCU/Linux books cover-to-cover before building anything | The provided reading list looks like a strict prerequisite chain if taken literally | Every resource in Section 8 is mapped to a specific sprint by chapter — read only what that sprint needs, when it needs it |
| Treating *Linux Device Drivers* (LDD3) API examples as current | The book is widely recommended but written for kernel 2.6.10 | Use it for structure and concepts only; verify any specific API call against current kernel documentation before using it (Section 8.3) |
| Scope creep from Extension Track ideas | Secure boot or performance tooling can look more interesting than finishing OTA/TLS | Enforced by Principle 6 — log the idea, don't build it, until V3.5 |
| Over-investing in D-Bus test automation | Mocking D-Bus is genuinely difficult and low market value at this stage | Manual/scripted verification is explicitly sufficient for Core Track (Section 6) |
| Writing an ADR for every small decision | Feels rigorous, but creates fatigue and stops being read | Capped at exactly 4 for the Core Track (Section 15) |
| Documentation debt | Easy to defer "later" | Pre-empted by the Universal Definition of Done (Section 9), which makes the per-sprint note non-optional |
| Job-search timeline pressure vs. full roadmap length | Real life doesn't wait for V3.5 | V0.5 (MCU + RTOS + GUI, ~month 4.2) and V1.0 (~month 9.7) are each independently strong, presentable checkpoints reachable well before the full roadmap is done |

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
- **LVGL (Light and Versatile Graphics Library):** an open-source, lightweight embedded GUI library for MCUs and MPUs, widely used for HMI/display work in automotive, industrial, and consumer devices.
- **MQTT:** a lightweight publish/subscribe messaging protocol common in IoT systems.
- **NVIC (Nested Vectored Interrupt Controller):** the Cortex-M peripheral that manages interrupt priority and dispatch.
- **OTA (Over-the-Air update):** remotely updating device firmware/software without physical access.
- **RAUC / SWUpdate:** open-source frameworks for managing embedded Linux OTA updates, including rollback.
- **RootFS (Root Filesystem):** the filesystem mounted at boot containing everything needed to run the OS.
- **RTOS (Real-Time Operating System):** an OS (e.g. FreeRTOS) guaranteeing response within strict timing constraints.
- **TLS (Transport Layer Security):** the cryptographic protocol securing network communication.
- **U-Boot:** a widely used open-source bootloader for embedded systems.
- **Yocto Project:** tools and metadata for building custom embedded Linux distributions, more flexible and complex than Buildroot.
