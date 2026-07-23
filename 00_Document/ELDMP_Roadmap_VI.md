# ELDMP — Embedded Linux Device Management Platform
## Lộ Trình Phát Triển Hoàn Chỉnh

---

## Mục Lục

1. [Tóm Tắt Tổng Quan](#1-tom-tat-tong-quan)
2. [Định Danh Dự Án](#2-dinh-danh-du-an)
3. [Nguyên Tắc Chỉ Đạo](#3-nguyen-tac-chi-dao)
4. [Kiến Trúc Hệ Thống](#4-kien-truc-he-thong)
5. [Tiêu Chuẩn Kỹ Thuật, Repository & Chiến Lược Portfolio](#5-tieu-chuan-ky-thuat-repository--chien-luoc-portfolio)
6. [Chiến Lược Kiểm Thử](#6-chien-luoc-kiem-thu)
7. [Lộ Trình Phần Cứng & Ngân Sách](#7-lo-trinh-phan-cung--ngan-sach)
8. [Tài Liệu Học Tập](#8-tai-lieu-hoc-tap)
9. [Core Track — Kế Hoạch Sprint Chi Tiết](#9-core-track--ke-hoach-sprint-chi-tiet)
10. [Checklist Phát Hành](#10-checklist-phat-hanh)
11. [Tổng Quan Timeline & Nhịp Độ Hàng Tuần](#11-tong-quan-timeline--nhip-do-hang-tuan)
12. [Extension Track](#12-extension-track)
13. [Bảng Tham Chiếu Milestone & Kỹ Năng](#13-bang-tham-chieu-milestone--ky-nang)
14. [Hướng Dẫn Phỏng Vấn & Trình Bày Portfolio](#14-huong-dan-phong-van--trinh-bay-portfolio)
15. [Architecture Decision Records (ADR)](#15-architecture-decision-records-adr)
16. [Rủi Ro & Cạm Bẫy](#16-rui-ro--cam-bay)
17. [Bảng Thuật Ngữ](#17-bang-thuat-ngu)

---

## 1. Tóm Tắt Tổng Quan

**ELDMP (Embedded Linux Device Management Platform)** là một dự án kỹ
thuật cá nhân dài hạn, tự định hướng, được xây dựng nhằm phát triển năng
lực sâu, có thể kiểm chứng được trong lĩnh vực kỹ thuật hệ thống Embedded
Linux — trải dài từ kiến trúc dịch vụ user-space, kiến thức nội tại của
Linux kernel, bring-up board nhúng, firmware MCU, cho đến giao tiếp
thiết bị bảo mật — đồng thời tạo ra một portfolio phần mềm công khai, có
versioning rõ ràng.

Dự án đi theo **mô hình học tập dẫn dắt bởi project (project-driven
learning)**: mỗi đơn vị kiến thức học được đều gắn với một phần cụ thể,
có thể release được của platform. Không có gì được học một cách tách
biệt. Tiến độ được theo dõi qua các bản release semantic version (V1.0,
V1.5, V2.0 …), mỗi bản đại diện cho một trạng thái hệ thống hoạt động
đầy đủ, có thể demo được.

**Những điểm thiết kế nổi bật.** Ba lựa chọn định hình roadmap này rõ nhất:

1. **Công việc kernel-space bắt đầu sớm (Sprint 3).** Một driver tuỳ
   chỉnh và Device Tree overlay — kỹ năng khác biệt đầu tiên có giá trị
   thật sự — có thể chứng minh được sau khoảng tháng thứ 3–4, và đã là
   một phần của bản release V1.0 thay vì xuất hiện muộn hơn. Kỹ năng
   kernel-level càng được thể hiện sớm, thì việc tìm việc bị gián đoạn
   hay biến cố cuộc sống càng ít có khả năng làm mất giá trị của
   portfolio đã xây dựng được.
2. **Quy trình được giữ ở mức tối giản có chủ đích.** Mọi checklist,
   template, hay bước review trong tài liệu này chỉ tồn tại vì nó thực
   sự cải thiện hiệu quả học tập hoặc chất lượng portfolio cho một dự án
   một người — không có gì được kế thừa mặc định từ quy trình doanh
   nghiệp.
3. **Việc học diễn ra qua ba kênh, không chỉ đọc sách.** Mỗi sprint đều
   chỉ rõ source code thật cần đọc, phân biệt lab thử nghiệm (bỏ đi) với
   phần build sẽ được ship, và yêu cầu viết một ghi chú ngắn — đọc code
   thật, thực hành có chủ đích, và dạy lại để học sâu hơn — song song
   với bất kỳ tài liệu sách/khoá học nào được chỉ định.

Roadmap được chia thành hai track:

- **Core Track** (Sprint 0–14, ~15–17 tháng với nhịp độ bán thời gian):
  tạo ra một portfolio hoàn chỉnh, đủ sức thuyết phục cho vị trí Senior
  Embedded Linux Engineer / Embedded Systems Engineer.
- **Extension Track** (V4.0 trở đi, mở, không giới hạn): các hướng
  chuyên sâu tuỳ chọn, chỉ bắt đầu sau khi hoàn thành Core Track.

---

## 2. Định Danh Dự Án

| Thuộc tính | Giá trị |
|---|---|
| Tên dự án | ELDMP — Embedded Linux Device Management Platform |
| Loại hình | Dự án kỹ thuật cá nhân dài hạn / portfolio công khai |
| Ngôn ngữ chính (phía Linux) | Modern C++ (C++17/20) |
| Ngôn ngữ chính (phía MCU) | C, bare-metal và FreeRTOS |
| Hệ điều hành mục tiêu | Embedded Linux tuỳ chỉnh (build bằng Buildroot và Yocto) |
| Build system | CMake |
| Version control | Git, release được gắn tag, có changelog cho mỗi version |
| Tài liệu | Markdown trong `docs/`; ghi chú học tập trong `notes/` |
| Cấu trúc portfolio | Một repo umbrella (`eldmp`) + các repo spin-out độc lập được chọn lọc (xem mục 5.3) |

---

## 3. Nguyên Tắc Chỉ Đạo

1. **Project dẫn dắt việc học**, nhưng mỗi sprint có một danh sách tài
   liệu đọc cố định, tối thiểu — không bao giờ là "học lan man không
   giới hạn."
2. **Mỗi sprint và mỗi release đều phải build và chạy được.**
3. **Việc phân lớp được áp dụng ngay từ ngày đầu** (Application →
   Middleware → HAL → Driver → Kernel), thể hiện trực tiếp trong cấu
   trúc repository.
4. **Hardware chỉ được mua khi sprint hiện tại thực sự cần đến.**
5. **Tài liệu hoá nhẹ nhàng ở cấp sprint, nghiêm ngặt ở cấp release.**
   Một ghi chú ngắn và một commit sạch là đủ giữa các milestone;
   checklist đầy đủ (mục 10) chỉ áp dụng tại các version tag.
6. **Core Track luôn được ưu tiên trước.** Extension Track chỉ bắt đầu
   sau khi V3.5 đã được cho ít nhất một người đánh giá bên ngoài xem qua.
7. **Mỗi sprint kết hợp ba kênh học tập, không chỉ đọc:** nghiên cứu tài
   liệu, đọc source code sản xuất thật, và xây dựng một thứ gì đó (lab
   và/hoặc tính năng được ship). Một sprint chỉ có đầu vào là một chương
   sách được xem là chưa hoàn thành.
8. **Quy trình chỉ tồn tại nếu nó xứng đáng.** Với một dự án một người,
   bất kỳ checklist, template, hay bước review nào cũng phải chứng minh
   được rằng nó cải thiện kết quả học tập hoặc chất lượng portfolio —
   nếu không, đó là overhead không phù hợp ở đây, và sẽ bị loại bỏ.

---

## 4. Kiến Trúc Hệ Thống

### 4.1 Sơ Đồ Trạng Thái Cuối Cùng

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

* = Tính năng thuộc Extension Track, không nằm trong phạm vi Core Track.
```

### 4.2 Trách Nhiệm Của Từng Lớp

| Lớp | Trách nhiệm | Vì sao được tách riêng |
|---|---|---|
| Linux Kernel + Device Tree | Boot, lập lịch (scheduling), quản lý bộ nhớ, mô tả phần cứng | Nền tảng gốc của toàn hệ thống; phải được hiểu, không chỉ sử dụng |
| Driver | Giao tiếp trực tiếp với thiết bị ngoại vi | Cô lập code phụ thuộc phần cứng khỏi mọi lớp phía trên |
| HAL | API ổn định cho Middleware bất kể board nào | Giúp việc port sang phần cứng mới (Extension Track) trở thành một thay đổi có giới hạn |
| Middleware (Config, Logger, IPC, Storage, Security) | Hạ tầng dùng chung xuyên suốt | Viết một lần, tái sử dụng cho mọi service, kiểm thử độc lập được |
| Services (Device, Health, Network, OTA, Diagnostic, Watchdog) | Logic nghiệp vụ | Mỗi service là một systemd unit nhỏ, triển khai độc lập |
| CLI / REST / WebUI | Giao diện bên ngoài | Giữ mỏng; toàn bộ logic thật nằm ở Services |
| MCU firmware | Điều khiển thời gian thực, xác định, và giao diện đồ hoạ nhúng | Chạy ở nơi Linux không thể đảm bảo hành vi thời gian thực, và nơi màn hình vật lý được gắn |
| Linux↔MCU protocol | Giao tiếp đáng tin cậy giữa hai bộ xử lý | Kỹ năng cấp senior nhất trong dự án: thiết kế protocol, không chỉ sử dụng nó |

---

## 5. Tiêu Chuẩn Kỹ Thuật, Repository & Chiến Lược Portfolio

### 5.1 Cấu Trúc Repository (repo umbrella `eldmp`)

```
eldmp/
├── docs/
│   ├── architecture/
│   │   └── decisions/     # 4 ADR đã giới hạn, xem mục 15
│   └── release-notes/
├── notes/                  # ghi chú kỹ thuật ngắn, mỗi sprint một file
├── src/
│   ├── logger/
│   ├── config/
│   ├── device/
│   ├── monitor/
│   ├── ipc/
│   ├── driver/             # kernel module (từ Sprint 3)
│   ├── mcu/                 # firmware STM32 + UI LVGL (từ Sprint 10)
│   ├── watchdog/
│   └── app/
├── tests/
├── scripts/
├── yocto/
└── README.md
```

### 5.2 Tiêu Chuẩn

- **Build system:** CMake cho toàn bộ C++ phía Linux; build riêng theo toolchain cho firmware MCU.
- **Version control:** một Git tag cho mỗi version milestone; commit message mô tả rõ sprint và thay đổi.
- **Config thay vì hard-code:** bất kỳ giá trị nào có thể khác nhau giữa các board hoặc deployment đều nằm trong `config.json`.
- **Tài liệu:** `docs/architecture/` cho sơ đồ và 4 ADR; `docs/release-notes/` cho mỗi version tag; `notes/` cho ghi chú học tập mỗi sprint, được yêu cầu bởi Definition of Done (mục 9).

### 5.3 Chiến Lược Repository Cho Portfolio

Một monorepo duy nhất, ngày càng lớn, sẽ khó để nhà tuyển dụng đánh giá
trong 5 phút lướt qua. Ba sprint tạo ra công việc đủ giá trị để tồn tại
như **các repository nhỏ, độc lập, dễ xem riêng lẻ**, sau đó được tích
hợp vào `eldmp` (dưới dạng git submodule hoặc chỉ đơn giản là liên kết
và ghi chú lại):

| Repo độc lập | Sprint | Vì sao xứng đáng có repo riêng |
|---|---|---|
| `eldmp-kernel-driver` | Sprint 3 | Một driver + DT overlay tập trung, khoảng 500 dòng, dễ đọc hơn nhiều khi đứng riêng so với bị chôn trong một codebase service lớn |
| `eldmp-yocto-layer` | Sprint 9 | Một meta-layer Yocto tuỳ chỉnh là một artifact dễ nhận diện, độc lập, mà nhà tuyển dụng trong lĩnh vực này thường tìm kiếm |
| `eldmp-linux-mcu-protocol` | Sprint 10–11 | Phần việc mạnh nhất trong toàn bộ dự án; nên chỉ cách một cú click, không phải nằm sâu trong nhiều thư mục |

Mọi phần còn lại nằm trong repo umbrella `eldmp`, đóng vai trò là điểm
tích hợp và câu chuyện "bức tranh tổng thể".

---

## 6. Chiến Lược Kiểm Thử

| Cấp độ test | Phạm vi | Bắt đầu từ | Công cụ | Ghi chú |
|---|---|---|---|---|
| Unit test | Một module đơn lẻ, độc lập | Sprint 1 | GoogleTest hoặc Catch2 | Không phụ thuộc hardware hay phiên D-Bus thật |
| Integration test | Từ hai service trở lên giao tiếp qua IPC | Sprint 6 | Cùng framework, có phiên D-Bus đang chạy | Kiểm tra thủ công/có kịch bản là đủ ở đây. Việc xây dựng hạ tầng mock D-Bus tự động hoàn chỉnh không bắt buộc cho Core Track — đây là khoản đầu tư thực sự khó khăn và có giá trị thị trường thấp ở giai đoạn này. Chỉ tự động hoá sau này nếu việc kiểm tra thủ công trở nên quá tốn công. |
| System / boot test | Toàn bộ image boot, mọi service khởi động | Sprint 3, mở rộng ở Sprint 9 | Boot thủ công + kiểm tra log | Boot test có kịch bản chính thức thuộc Extension Track (V4.0) |
| Hardware-in-the-loop (HIL) | MCU thật + board thật giao tiếp qua kết nối vật lý | Sprint 10–11 | Fault injection thủ công (rút/cắm cáp, làm hỏng payload) | Đây là nơi cần sự nghiêm ngặt nhất — xem Definition of Done của Sprint 11 |
| Regression test | Chạy lại toàn bộ test của các sprint trước cùng nhau | Mỗi version milestone | Chạy tổng hợp cục bộ | Bắt buộc trước mỗi lần tag |

**Quy ước:** vị trí file test phản ánh vị trí source
(`tests/<module>/test_<name>.cpp` ↔ `src/<module>/`). Một version không
được tag cho đến khi toàn bộ regression suite pass.

---

## 7. Lộ Trình Phần Cứng & Ngân Sách

| Giai đoạn | Phần cứng | Vai trò | Mua tại | Mức ngân sách |
|---|---|---|---|---|
| Nền tảng | Laptop Ubuntu hoặc VM | Môi trường phát triển chính | Sprint 0 | — (đã có sẵn) |
| Board | BeagleBone Black Rev C | Mục tiêu Embedded Linux | Sprint 3 | $ |
| MCU | STM32 Nucleo-H743ZI | Bộ đồng hành thời gian thực | Sprint 10–11 | $ |
| Màn hình | Module LCD SPI/parallel nhỏ (tương thích LVGL) | Mục tiêu GUI nhúng | Sprint 10 | $ |
| Networking | Adapter USB-Ethernet hoặc switch nhỏ | Môi trường test mạng | Sprint 12 (chỉ khi cần) | $ |
| SoC công nghiệp | NXP i.MX93 | Mục tiêu của Extension Track | V6.0+ | $$–$$$ |

`$` = board giá rẻ dành cho hobbyist, `$$` = kit phát triển tầm trung,
`$$$` = kit đánh giá cấp công nghiệp/AI. Giá thay đổi theo khu vực và
thời gian — xem các mức này là định hướng tương đối, không phải báo giá.

**Lộ trình phần cứng đã xác định:** BeagleBone Black Rev C cho Core
Track, STM32 Nucleo-H743ZI cùng một module LCD nhỏ cho phía MCU và GUI
nhúng, sau đó NXP i.MX93 làm mục tiêu SoC công nghiệp cho Extension
Track. Không cần board thay thế nào ở bất kỳ giai đoạn nào — mỗi loại
chỉ được mua khi đến đúng sprint cần nó (Nguyên tắc 4).

---

## 8. Tài Liệu Học Tập

### 8.1 Nền Tảng — Gắn Chính Xác, Không Phải "Đọc Lúc Nào Cũng Được"

| Chương tài liệu | Tác giả | Đọc trước |
|---|---|---|
| OSTEP — File Systems & I/O | Arpaci-Dusseau | Sprint 1 (Logger thao tác file I/O và xoay vòng file thật) |
| OSTEP — Address Spaces & Virtual Memory | Arpaci-Dusseau | Sprint 3 (ranh giới kernel/user space là trọng tâm của việc viết driver) |
| OSTEP — Processes & Process API | Arpaci-Dusseau | Sprint 6 (IPC liên quan đến nhiều process phối hợp với nhau) |
| OSTEP — Threads, Locks, Condition Variables | Arpaci-Dusseau | Sprint 5 (Health Monitor bắt đầu dùng thread/timer), củng cố lại ở Sprint 8 (Watchdog) |
| TLPI — File I/O, Signals, IPC | Michael Kerrisk | Sprint 1–6, dùng trực tiếp |
| Effective Modern C++ | Scott Meyers | Liên tục từ Sprint 0, RAII/smart pointer dùng xuyên suốt |

### 8.2 Kernel & Embedded Linux

| Tài liệu | Nguồn | Dùng cho |
|---|---|---|
| Linux Kernel Labs | Linux Foundation / cộng đồng kernel | Sprint 3 |
| *Mastering Embedded Linux Programming* | Chris Simmonds | Sprint 9 |
| *Embedded Linux Systems with the Yocto Project* + Bootlin Labs | Rudolf J. Streif / Bootlin | Sprint 9 |

### 8.3 MCU, FreeRTOS & GUI Nhúng

| Tài liệu | Nguồn | Dùng cho |
|---|---|---|
| Tài liệu chính thức và source kernel của FreeRTOS | FreeRTOS.org | Sprint 10 |
| Tài liệu, hướng dẫn porting, và ví dụ widget của LVGL | Dự án LVGL | Sprint 10 |

### 8.4 Networking & Security

| Tài liệu | Tác giả | Dùng cho |
|---|---|---|
| *Beej's Guide to Network Programming* | Brian "Beej" Hall | Sprint 12 |
| Tài liệu OpenSSL | Dự án OpenSSL | Sprint 14 |

### 8.5 Chỉ Dành Cho Extension Track

*Systems Performance* và *BPF Performance Tools* (Brendan Gregg) cho
V4.0; tài liệu OP-TEE và Trusted Firmware-A cho V5.0;
*C++ Concurrency in Action* (Anthony Williams) khi phần đa luồng trở nên
phức tạp hơn.

---

## 9. Core Track — Kế Hoạch Sprint Chi Tiết

**Definition of Done chung — áp dụng cho mọi sprint bên dưới, cùng với
tiêu chí riêng của từng sprint:**
- [ ] Code có thể đọc hiểu bởi người lạ trong khoảng 10 phút: có comment đầu file nêu rõ mục đích, đặt tên rõ nghĩa, không để lại dead code.
- [ ] Một ghi chú kỹ thuật (300–600 từ: học được gì, cái gì bị lỗi, cách sửa) được viết vào `notes/`.
- [ ] Ít nhất một đoạn source code thật, đang chạy trong sản phẩm thực tế đã được đọc, theo dòng "Reading" của sprint đó bên dưới — không phải code tutorial.

**Vì sao công việc kernel lại đứng trước D-Bus/systemd/Watchdog:** viết
một character driver và một Device Tree overlay không hề phụ thuộc kỹ
thuật vào IPC hay service management — nó chỉ cần Logger và Config tồn
tại để ghi lại kết quả. Sắp xếp nó ở Sprint 3 giúp bạn tiếp cận hardware
thật và bước vào kernel-space ngay trong vài tháng đầu, mà không phá vỡ
bất kỳ chuỗi prerequisite thực sự nào.

---

### Sprint 0 — Thiết Lập Môi Trường
- **Mục tiêu:** môi trường phát triển có thể tái lập và bộ khung repository.
- **Thời lượng:** ~2 tuần | **Yêu cầu trước:** không có
- **Study:** Git, Bash, CMake, Markdown
- **Build:** bộ khung repository theo mục 5.1, coding style guide, README ban đầu
- **Definition of Done (riêng):**
  - [ ] Repository build thành công một project CMake rỗng
  - [ ] README giải thích cách build và chạy

### Sprint 1 — Logger
- **Mục tiêu:** thư viện logging tái sử dụng được, dùng cho mọi thành phần sau này.
- **Thời lượng:** ~2 tuần | **Yêu cầu trước:** Sprint 0
- **Study:** TLPI File I/O; OSTEP File Systems
- **Reading:** source của một thư viện C++ logging mã nguồn mở nhỏ gọn (ví dụ spdlog) — tập trung vào cách log level và sink được cấu trúc, không phải để copy nguyên xi
- **Build:** thư viện logger, log level, xoay vòng file (rotation) đơn giản, unit test
- **Definition of Done (riêng):**
  - [ ] Là thư viện độc lập, các module khác có thể link vào
  - [ ] Xoay vòng file được xác minh bằng một test vượt ngưỡng kích thước

### Sprint 2 — Configuration Manager
- **Mục tiêu:** cấu hình tập trung, có thể reload.
- **Thời lượng:** ~2 tuần | **Yêu cầu trước:** Sprint 1
- **Study:** JSON parsing và thiết kế schema
- **Reading:** cách dùng của một thư viện JSON nhỏ, phổ biến (ví dụ nlohmann/json)
- **Build:** Config Manager, `config.json`, hỗ trợ reload trực tiếp
- **Definition of Done (riêng):**
  - [ ] Không có giá trị config nào bị hard-code ở nơi khác trong codebase
  - [ ] Config sai được từ chối với thông báo lỗi rõ ràng, không crash

---

### Sprint 3 — Kernel & Device Tree Fundamentals
- **Mục tiêu:** chuyển từ năng lực chỉ ở user-space sang làm việc thật với kernel-space, ngay từ sớm.
- **Thời lượng:** ~8–10 tuần (đã tính buffer dự phòng — debug kernel là công việc khó đoán nhất với người tự học, nên được xử lý cẩn trọng tương đương sprint Linux↔MCU Protocol)
- **Yêu cầu trước:** Sprint 2; đã có BeagleBone Black Rev C
- **Study:** Linux Kernel Labs — boot process, character device driver, xử lý interrupt, Linux Device Model, kernel synchronization
- **Reading:** một character driver đơn giản trong `drivers/char/` của mainline kernel, và chính Device Tree source của BeagleBone Black (`am335x-boneblack.dts`) để xem cách một board description thật được cấu trúc
- **Lab (thử nghiệm, không ship):** làm theo đúng 2–3 bài lab chính thức của Linux Kernel Labs, trên code có thể bỏ đi, trước khi đụng vào driver "thật" — coi đây là sandbox an toàn để phá hỏng
- **Build (ship trong `eldmp-kernel-driver`):** một character device driver tuỳ chỉnh, một GPIO driver, một Device Tree overlay gắn vào hardware thật (nút bấm/LED)
- **Definition of Done (riêng):**
  - [ ] Driver tuỳ chỉnh load được qua `insmod` và hiện trong `dmesg` không lỗi
  - [ ] Device Tree overlay gắn đúng driver vào hardware thật
  - [ ] GPIO input theo interrupt được xác minh (không phải polling)

### Sprint 4 — Device Manager
- **Mục tiêu:** hiển thị thông tin định danh thiết bị và hệ thống.
- **Thời lượng:** ~2–3 tuần | **Yêu cầu trước:** Sprint 3
- **Study:** cấu trúc filesystem `/proc` và `/sys`
- **Reading:** cách một công cụ có sẵn (ví dụ `lm-sensors` hoặc `htop`) đọc `/proc`/`/sys`
- **Build:** lệnh `eldmp info` — CPU, RAM, kernel version, hostname; tuỳ chọn hiển thị thêm dữ liệu từ custom driver ở Sprint 3
- **Definition of Done (riêng):**
  - [ ] Output khớp với trạng thái hệ thống thật, xác minh bằng `uname`/`free`/`lscpu`

### Sprint 5 — Health Monitor
- **Mục tiêu:** giám sát tài nguyên liên tục.
- **Thời lượng:** ~2–3 tuần | **Yêu cầu trước:** Sprint 4
- **Study:** OSTEP Threads, Locks, Condition Variables; timer
- **Build:** lệnh `eldmp monitor` — mức sử dụng CPU/RAM/disk theo chu kỳ
- **Definition of Done (riêng):**
  - [ ] Chạy liên tục không rò rỉ bộ nhớ qua một soak test nhiều giờ
  - [ ] Chu kỳ lấy mẫu có thể cấu hình qua Config Manager

### Sprint 6 — Inter-Process Communication (IPC)
- **Mục tiêu:** biến Device, Logger, và Health thành các service độc lập, chỉ giao tiếp qua IPC.
- **Thời lượng:** ~3–4 tuần | **Yêu cầu trước:** Sprint 5
- **Study:** khái niệm và API của D-Bus; OSTEP Processes & Process API
- **Reading:** định nghĩa interface D-Bus của một service thật đang tồn tại (ví dụ NetworkManager) để xem một D-Bus API sản xuất thật được thiết kế ra sao
- **Build:** Device/Logger/Health Service, đều dựa trên D-Bus
- **Definition of Done (riêng):**
  - [ ] Không service nào gọi trực tiếp hàm nội bộ của service khác — chỉ qua D-Bus
  - [ ] Tắt một service không làm sập các service còn lại
  - [ ] Hành vi giữa các service đã được xác minh (thủ công/có kịch bản — xem mục 6)

### Sprint 7 — systemd Integration
- **Mục tiêu:** chạy mọi service dưới dạng systemd unit được quản lý.
- **Thời lượng:** ~2 tuần | **Yêu cầu trước:** Sprint 6
- **Study:** systemd unit file, `journalctl`, `systemctl`
- **Reading:** 3–4 file `.service` thật đang có sẵn trên máy của bạn (`/lib/systemd/system/`) — so sánh cách một dự án trưởng thành cấu trúc dependency và restart policy
- **Build:** `logger.service`, `device.service`, `monitor.service`
- **Definition of Done (riêng):**
  - [ ] Mọi service tự khởi động khi boot
  - [ ] Service tự restart khi crash ở cấp systemd

### Sprint 8 — Watchdog
- **Mục tiêu:** giám sát ở cấp ứng dụng, phía trên systemd.
- **Thời lượng:** ~2–3 tuần | **Yêu cầu trước:** Sprint 7
- **Reading:** cách một supervisor nhẹ có sẵn (ví dụ `runit` hoặc `s6`) triển khai heartbeat/restart, để so sánh thiết kế
- **Build:** cơ chế heartbeat, tự động restart service không phản hồi, ghi log lỗi
- **Definition of Done (riêng):**
  - [ ] Một service bị treo có chủ đích được phát hiện và restart trong khoảng thời gian đã định

**→ Release V1.0 — "Nền tảng kernel-space + user-space."** Bản release
này đã bao gồm cả công việc driver/Device Tree thật lẫn kiến trúc service
user-space đầy đủ. Checklist phát hành đầy đủ được áp dụng (mục 10).

---

### Sprint 9 — Buildroot / Yocto
- **Mục tiêu:** boot ELDMP như một image embedded Linux hoàn toàn tuỳ chỉnh.
- **Thời lượng:** ~6–8 tuần | **Yêu cầu trước:** V1.0
- **Study:** *Mastering Embedded Linux Programming*; Bootlin Labs
- **Reading:** một BSP meta-layer có sẵn cho BeagleBone Black (ví dụ `meta-ti` hoặc layer cộng đồng `meta-beagleboard` cho Yocto, hoặc cấu hình `board/beaglebone` của chính Buildroot)
- **Lab (thử nghiệm):** build một image Buildroot tối giản, không liên quan trước, chỉ để học vòng lặp menuconfig/build, trước khi bắt tay vào image ELDMP thật
- **Build (ship trong `eldmp-yocto-layer` + tích hợp vào `eldmp`):** image tuỳ chỉnh hoàn chỉnh (U-Boot + kernel + rootfs) qua Buildroot, sau đó qua Yocto layer/recipe
- **Definition of Done (riêng):**
  - [ ] Board boot thẳng vào platform ELDMP, không cần setup thủ công sau boot
  - [ ] Cả image Buildroot lẫn image Yocto đều tồn tại và boot thành công
  - [ ] Build image được script hoá và tái lập được từ một checkout sạch

**→ Release V1.5.**

---

### Sprint 10 — MCU Bring-up & GUI Nhúng (LVGL)
- **Mục tiêu:** thiết lập phía MCU độc lập — logic cảm biến/điều khiển cộng với một GUI nhúng cơ bản — trước khi kết nối với Linux.
- **Thời lượng:** ~5–7 tuần | **Yêu cầu trước:** V1.5; đã có STM32 Nucleo-H743ZI và một module LCD SPI/parallel nhỏ
- **Study:** UART, SPI, I2C, CAN, DMA, kiến thức nền FreeRTOS; kiến thức nền LVGL (widget, event loop, interface driver màn hình/input)
- **Reading:** source của STM32 HAL driver cho các peripheral đang dùng, source scheduler của FreeRTOS, và hướng dẫn porting driver màn hình/input của chính LVGL để xem một thư viện GUI nhúng cấp sản xuất mong đợi được tích hợp ra sao
- **Lab (thử nghiệm):** chớp đèn/đọc một cảm biến bằng code ví dụ của vendor trước, để tách riêng vấn đề toolchain/board khỏi logic của chính bạn; riêng biệt, chạy thử một demo widget có sẵn của LVGL trên màn hình trước khi gắn dữ liệu thật vào
- **Build:** đọc cảm biến + vòng lặp điều khiển motor/PWM trên MCU, giao tiếp với PC qua UART; dữ liệu cảm biến cũng được hiển thị trực tiếp lên một UI dựa trên LVGL trên màn hình — đúng tech stack thường dùng cho cụm đồng hồ/infotainment ô tô và panel HMI thiết bị gia dụng
- **Definition of Done (riêng):**
  - [ ] Dữ liệu cảm biến đúng và ổn định theo thời gian
  - [ ] Vòng lặp điều khiển chạy ở tốc độ xác định, đã đo được
  - [ ] UI LVGL cập nhật trực tiếp từ dữ liệu cảm biến thật, không bị xé hình (tearing) hay giật khung hình rõ rệt

### Sprint 11 — Linux ↔ MCU Protocol
- **Mục tiêu:** thiết kế và triển khai một protocol giao tiếp thật, đáng tin cậy giữa hai bộ xử lý.
- **Thời lượng:** ~8–10 tuần, đã bao gồm buffer dự phòng
- **Yêu cầu trước:** Sprint 10
- **Reading:** source của một protocol nhúng có sẵn (ví dụ Modbus RTU hoặc MAVLink) để xem một thiết kế đã được kiểm chứng thực tế xử lý CRC, retry, và versioning ra sao — không phải để copy, mà để đối chiếu với các quyết định thiết kế của chính bạn
- **Build (tiến dần, ship trong `eldmp-linux-mcu-protocol`):**
  1. `Linux → UART → STM32`, rồi SPI, rồi CAN
  2. Thiết kế protocol: heartbeat, xác minh CRC, retry logic, xử lý timeout, thương lượng version
  3. Linux gửi lệnh điều khiển; MCU trả về trạng thái
- **Definition of Done (riêng):**
  - [ ] Protocol vẫn giữ đúng trạng thái khi gặp sự cố kết nối có chủ đích, không bị hỏng dữ liệu
  - [ ] CRC từ chối đúng ít nhất một test message bị làm hỏng có chủ đích
  - [ ] Trường hợp lệch version protocol được phát hiện và xử lý một cách nhẹ nhàng (không crash)

**→ Release V2.0 — phần khác biệt mạnh nhất trong toàn bộ dự án. Không
được rút gọn sprint này dù có áp lực tiến độ.**

---

### Sprint 12 — Networking
- **Mục tiêu:** kết nối platform với mạng và một broker từ xa.
- **Thời lượng:** ~4–6 tuần | **Yêu cầu trước:** V2.0
- **Study:** kiến thức nền TCP/IP, MQTT
- **Reading:** source của một thư viện MQTT client nhẹ (ví dụ Paho MQTT C client)
- **Build:** Network Manager service, MQTT client publish trạng thái/health của thiết bị
- **Definition of Done (riêng):**
  - [ ] Trạng thái thiết bị hiển thị real-time trên một MQTT broker từ xa
  - [ ] Network Manager tự phục hồi sau khi mất kết nối

**→ Release V2.5.**

### Sprint 13 — OTA (Over-the-Air Updates)
- **Mục tiêu:** cho phép platform tự cập nhật từ xa.
- **Thời lượng:** ~4–6 tuần | **Yêu cầu trước:** V2.5
- **Study:** SWUpdate hoặc RAUC
- **Reading:** source của RAUC hoặc SWUpdate trực tiếp — cách một OTA client thật cấu trúc download/verify/install/rollback
- **Build:** download, verify, install, reboot (A/B partitioning để lại cho Extension Track)
- **Definition of Done (riêng):**
  - [ ] Một update package bị làm hỏng có chủ đích bị từ chối trước khi cài đặt
  - [ ] Một update thành công được xác minh qua thay đổi version string sau khi reboot

**→ Release V3.0.**

### Sprint 14 — Security Foundation
- **Mục tiêu:** khắc phục những lỗ hổng bảo mật rõ ràng nhất trước khi coi Core Track hoàn thành.
- **Thời lượng:** ~4–6 tuần | **Yêu cầu trước:** V3.0
- **Study:** OpenSSL, TLS, quản lý certificate
- **Reading:** một ví dụ TLS client-server tối giản, xây trực tiếp trên OpenSSL
- **Build:** HTTPS cho management endpoint, MQTT qua TLS, OTA package đã ký (signed)
- **Definition of Done (riêng):**
  - [ ] Một OTA package chưa ký hoặc bị can thiệp bị từ chối
  - [ ] Toàn bộ traffic giữa thiết bị và broker/server được mã hoá, xác minh bằng packet capture

**→ Release V3.5 — Core Track hoàn thành.** Portfolio đầy đủ, đủ sức
thuyết phục cho các vị trí Senior Embedded Linux Engineer.

---

## 10. Checklist Phát Hành

Áp dụng tại mỗi version milestone lớn (V1.0, V1.5, V2.0, V2.5, V3.0, V3.5, và các version thuộc Extension Track):

- [ ] Code hoạt động, pass unit test, integration test, và regression test
- [ ] README được cập nhật theo đúng năng lực hiện tại
- [ ] Sơ đồ kiến trúc được cập nhật trong `docs/architecture/`
- [ ] Có entry release notes / changelog trong `docs/release-notes/`
- [ ] Bất kỳ quyết định nào thuộc danh sách ADR giới hạn (mục 15) đã được ghi lại nếu áp dụng ở version này
- [ ] Đã tạo Git tag
- [ ] Video demo (2–5 phút) — chỉ bắt buộc tại V1.0, V2.0, và V3.5, ba milestone thực sự có năng lực mới khác biệt để show; tuỳ chọn ở V1.5/V2.5/V3.0 để tránh biến tài liệu hoá thành một khoản "thuế" chất lượng sản xuất

Công việc ở cấp sprint chỉ cần lịch sử commit sạch và Definition of Done
chung ở mục 9 — checklist đầy đủ bên trên chỉ dành cho các version
milestone.

---

## 11. Tổng Quan Timeline & Nhịp Độ Hàng Tuần

### 11.1 Timeline Theo Từng Sprint (Nhịp Độ Cơ Sở)

| Sprint | Thời lượng | Tích luỹ | Milestone |
|---|---|---|---|
| 0 | 2 tuần | 2 tuần | — |
| 1 | 2 tuần | 4 tuần | — |
| 2 | 2 tuần | 6 tuần | — |
| 3 | 8–10 tuần | ~15 tuần | *(kỹ năng kernel/driver có thể chứng minh được ở đây, ~3,5 tháng)* |
| 4 | 2–3 tuần | ~18 tuần | — |
| 5 | 2–3 tuần | ~21 tuần | — |
| 6 | 3–4 tuần | ~25 tuần | — |
| 7 | 2 tuần | ~27 tuần | — |
| 8 | 2–3 tuần | ~30 tuần | **V1.0** |
| 9 | 6–8 tuần | ~38 tuần | **V1.5** |
| 10 | 5–7 tuần | ~44 tuần | — |
| 11 | 8–10 tuần | ~53 tuần | **V2.0** |
| 12 | 4–6 tuần | ~58 tuần | **V2.5** |
| 13 | 4–6 tuần | ~63 tuần | **V3.0** |
| 14 | 4–6 tuần | ~68 tuần | **V3.5 — Core Track hoàn thành** |

**Tổng thời lượng Core Track: khoảng 15–17 tháng** với nhịp độ bán thời
gian cơ sở. Kỹ năng khác biệt, giá trị cao nhất — công việc kernel/driver
— có thể chứng minh được sớm, khoảng tháng thứ 3,5, rất lâu trước khi có
version tag đầu tiên.

### 11.2 Điều Chỉnh Nhịp Độ Theo Ngân Sách Thời Gian Hàng Tuần

| Ngân sách thời gian/tuần | Tình huống điển hình | Thời lượng Core Track ước tính |
|---|---|---|
| ~8–10 giờ/tuần | Bán thời gian, song song với công việc toàn thời gian | ~15–17 tháng (cơ sở) |
| ~15–20 giờ/tuần | Dành riêng buổi tối và cuối tuần | ~9–12 tháng |
| ~35+ giờ/tuần | Tập trung toàn thời gian | ~5–7 tháng |

Gợi ý phân bổ: khoảng 30% đọc/học, 70% xây dựng/debug — thời gian học mà
không kèm theo việc áp dụng ngay trong cùng tuần đó là một dấu hiệu cảnh
báo, không phải tiến độ bình thường.

---

## 12. Extension Track

Tuỳ chọn, chỉ chọn sau khi hoàn thành V3.5, dựa theo định hướng sự nghiệp:

| Version | Trọng tâm | Vì sao nên chọn |
|---|---|---|
| V4.0 | Performance & Observability (*Systems Performance*, eBPF, `perf`) | Các vị trí thiên về platform/hạ tầng |
| V5.0 | Secure Boot & TEE (OP-TEE, TF-A, `dm-verity`, `dm-crypt`, A/B rollback) | Automotive, y tế, các vị trí nhạy cảm về bảo mật |
| V6.0 | Port sang SoC công nghiệp trên nền NXP i.MX93 — RemoteProc, RPMsg, asymmetric multiprocessing, Time-Sensitive Networking, Real-Time Linux | Robotics hoặc tự động hoá công nghiệp |
| V7.0 | REST API / WebUI / Cloud Dashboard | Các vị trí thiên về product |
| V8.0 | ROS2 / suy luận AI on-device / GUI | Chuyên sâu robotics |

---

## 13. Bảng Tham Chiếu Milestone & Kỹ Năng

Một bảng tham chiếu duy nhất bao quát mọi bản release — ý nghĩa của nó,
vì sao nó quan trọng với portfolio, và cách trình bày nó trong phỏng vấn.

| Milestone | ~Tuần | Kỹ năng đạt được | Ý nghĩa với portfolio | Điểm nói trong phỏng vấn |
|---|---|---|---|---|
| **V1.0** | 30 | Kernel driver + Device Tree, D-Bus IPC, systemd, giám sát bằng watchdog, modern C++ | Bản release đầu tiên kết hợp cả kernel-space lẫn user-space — checkpoint "sớm" mạnh nhất nếu việc tìm việc bắt đầu trước khi roadmap hoàn tất | "Tôi đã xây dựng một platform nhúng nhiều service, trong đó một kernel driver tuỳ chỉnh expose dữ liệu hardware thật qua các D-Bus service, được quản lý bởi systemd cùng một watchdog ở cấp ứng dụng." |
| **V1.5** | 38 | Buildroot, Yocto, rootfs tuỳ chỉnh, cross-compilation | Có thể build và boot một image embedded Linux hoàn toàn tuỳ chỉnh từ đầu | "Tôi đã build và boot một image embedded Linux tuỳ chỉnh bằng cả Buildroot lẫn Yocto, hiểu rõ từng lớp từ bootloader đến root filesystem." |
| **V2.0** | 53 | Firmware MCU (FreeRTOS), GUI nhúng (LVGL), protocol Linux↔MCU tuỳ chỉnh (CRC/retry/versioning) | Điểm khác biệt mạnh nhất trong dự án — một protocol được thiết kế và kiểm thử lỗi thật, không chỉ đơn thuần sử dụng, cộng thêm một GUI nhúng thật ở phía MCU | "Tôi đã thiết kế và triển khai một protocol chịu lỗi giữa Linux và MCU, kiểm chứng nó với các lỗi kết nối thật, và phía MCU chạy một UI dựa trên LVGL hiển thị dữ liệu cảm biến theo thời gian thực — đúng tech stack dùng cho cụm đồng hồ ô tô và panel HMI thiết bị gia dụng." |
| **V2.5** | 58 | TCP/IP, MQTT | Platform đã kết nối mạng và giám sát được từ xa | "Trạng thái/health của thiết bị được publish tới một broker từ xa theo thời gian thực, với khả năng tự kết nối lại." |
| **V3.0** | 63 | Vòng đời OTA (SWUpdate/RAUC) | Hoạt động như một sản phẩm IoT/công nghiệp thương mại thật | "Đã triển khai một pipeline OTA từ chối các package bị hỏng trước khi cài đặt." |
| **V3.5** | 68 | TLS, update đã ký | Portfolio hoàn chỉnh — đủ sức thuyết phục cho phỏng vấn Senior Embedded Linux Engineer | "Toàn bộ platform được mã hoá đầu-cuối và update được ký số." |

Mỗi điểm nói trong phỏng vấn đều được cố tình diễn đạt thành một tuyên bố
cụ thể, có thể kiểm chứng — dạng câu mà người phỏng vấn có thể hỏi tiếp —
thay vì một câu mơ hồ kiểu "đã từng làm về embedded Linux."

---

## 14. Hướng Dẫn Phỏng Vấn & Trình Bày Portfolio

### 14.1 Cấu Trúc README Cấp Cao Nhất Của `eldmp`

1. Pitch một dòng — ELDMP là gì và dành cho ai
2. Sơ đồ kiến trúc (mục 4.1)
3. "Điều dự án này chứng minh" — danh sách gạch đầu dòng ngắn, lấy từ mục 13
4. Version hiện tại và thay đổi gần nhất
5. Link tới 3 repo spin-out độc lập (mục 5.3)
6. Cách build/chạy, hoặc nơi xem video demo
7. Link tới tài liệu roadmap này

### 14.2 Cấu Trúc README Cho Từng Repo Độc Lập (3 Repo Spin-out)

Mỗi repo `eldmp-kernel-driver`, `eldmp-yocto-layer`, và
`eldmp-linux-mcu-protocol` cần đọc hiểu được mà không cần bối cảnh từ
repo umbrella: một đoạn mô tả vấn đề, những gì đã xây dựng, cách
chạy/test, và một link quay lại `eldmp` để thấy bức tranh tổng thể.

---

## 15. Architecture Decision Records (ADR)

**Dự án này cố tình giới hạn số ADR của Core Track ở đúng bốn cái.** Nếu
một quyết định không thuộc danh sách này, một dòng ghi chú ngắn trong
commit message là đủ — viết ADR cho mọi lựa chọn nhỏ sẽ gây mệt mỏi và
khiến nó không còn được ai đọc nữa.

**Vị trí lưu:** `docs/architecture/decisions/ADR-XXXX-short-title.md`

**Template:**

```
# ADR-XXXX: <Tiêu đề>

Status: Proposed | Accepted | Superseded by ADR-YYYY
Date: <ngày>

## Context
Vấn đề hoặc quyết định đang được giải quyết, và các ràng buộc liên quan.

## Decision
Lựa chọn thực tế đã được đưa ra.

## Consequences
Điều gì trở nên dễ hơn, điều gì trở nên khó hơn, điều gì đã phải đánh đổi.
```

**Bốn ADR của Core Track:**

1. **ADR-0001 — Kiến trúc tách rời:** Linux SoC + MCU thời gian thực riêng biệt, thay vì dùng một SoC mạnh duy nhất xử lý mọi thứ.
2. **ADR-0002 — Dùng cả Buildroot lẫn Yocto**, thay vì chỉ chọn một build system.
3. **ADR-0003 — Dùng D-Bus cho IPC**, thay vì Unix socket thô hoặc shared memory.
4. **ADR-0004 — Một protocol tự thiết kế, xây dựng dần dần (UART→SPI→CAN)** cho kết nối Linux↔MCU, thay vì áp dụng thẳng một protocol có sẵn như Modbus.

(ADR thứ năm, ADR-0005, chỉ được thêm khi việc port sang i.MX93 được lên kế hoạch chi tiết ở V6.0.)

---

## 16. Rủi Ro & Cạm Bẫy

Một danh sách thực dụng duy nhất — với một dự án một người, một danh
sách bao gồm cả rủi ro lẫn cách khắc phục là đủ.

| Rủi ro / Cạm bẫy | Vì sao nó xảy ra | Cách tránh |
|---|---|---|
| Mua hardware quá sớm | Hào hứng muốn "bắt đầu ngay" với hardware | Tuân theo thời điểm mua ở mục 7 — hardware mua trước sprint cần nó thường bị bỏ không dùng |
| Làm hỏng hardware khi làm việc với kernel/driver | Khó tránh khỏi khi flash/thử nghiệm ở mức độ này | Luôn giữ bản backup SD card; coi board là "có thể hy sinh" trong quá trình thử nghiệm ở Sprint 3 |
| Rút gọn Sprint 3 hoặc Sprint 11 | Đây là hai sprint khó đoán nhất, giá trị cao nhất | Cả hai đã có buffer dự phòng rõ ràng — bảo vệ nó, không lấy lại buffer này cho sprint khác |
| Scope creep từ ý tưởng của Extension Track | Secure boot hay performance tooling có thể trông hấp dẫn hơn việc hoàn thành OTA/TLS | Được kiểm soát bởi Nguyên tắc 6 — ghi lại ý tưởng, đừng xây dựng nó, cho đến khi hoàn thành V3.5 |
| Đầu tư quá mức vào test tự động cho D-Bus | Mock D-Bus thực sự khó và có giá trị thị trường thấp ở giai đoạn này | Kiểm tra thủ công/có kịch bản là đủ cho Core Track (mục 6) |
| Viết ADR cho mọi quyết định nhỏ | Cảm giác nghiêm túc, nhưng gây mệt mỏi và khiến nó không còn được đọc | Giới hạn đúng 4 ADR cho Core Track (mục 15) |
| Nợ tài liệu hoá | Dễ bị trì hoãn "để sau" | Đã được ngăn chặn từ trước bởi Definition of Done chung (mục 9), buộc ghi chú mỗi sprint phải có, không phải việc tuỳ chọn |
| Áp lực thời gian tìm việc so với độ dài roadmap | Cuộc sống thật không chờ đến V3.5 | V1.0 đã bao gồm sẵn công việc kernel-space, nên đây là một checkpoint độc lập, đủ mạnh để dùng riêng |

---

## 17. Bảng Thuật Ngữ

- **ADR (Architecture Decision Record):** một tài liệu ngắn, lưu trữ vĩnh viễn, ghi lại một quyết định kỹ thuật không hiển nhiên, bối cảnh của nó, và hệ quả của nó.
- **BSP (Board Support Package):** bootloader, cấu hình kernel, và driver cần thiết để chạy Linux trên một board cụ thể.
- **Buildroot:** một công cụ để tạo ra các hệ thống embedded Linux tối giản, tuỳ chỉnh, thông qua cross-compilation.
- **CRC (Cyclic Redundancy Check):** một thuật toán checksum dùng để phát hiện lỗi hỏng dữ liệu ngoài ý muốn.
- **D-Bus:** một hệ thống message-bus dùng cho giao tiếp liên tiến trình (IPC) trên Linux.
- **Device Tree (DT):** một cấu trúc dữ liệu mô tả bố cục hardware cho Linux kernel, tách biệt khỏi binary của kernel.
- **HAL (Hardware Abstraction Layer):** một lớp phần mềm cung cấp API nhất quán bất kể hardware bên dưới là gì.
- **HIL (Hardware-in-the-Loop) testing:** kiểm thử có sự tham gia của hardware vật lý thật, không phải mô phỏng.
- **IPC (Inter-Process Communication):** các cơ chế cho phép các process riêng biệt trao đổi dữ liệu.
- **LVGL (Light and Versatile Graphics Library):** một thư viện GUI nhúng mã nguồn mở, nhẹ, dùng cho MCU và MPU, được dùng rộng rãi cho các ứng dụng HMI/màn hình trong automotive, công nghiệp, và thiết bị tiêu dùng.
- **MQTT:** một giao thức messaging publish/subscribe nhẹ, phổ biến trong hệ thống IoT.
- **OTA (Over-the-Air update):** cập nhật firmware/phần mềm thiết bị từ xa mà không cần truy cập vật lý.
- **RAUC / SWUpdate:** các framework mã nguồn mở để quản lý update OTA cho embedded Linux, bao gồm cả rollback.
- **RootFS (Root Filesystem):** filesystem được mount khi boot, chứa mọi thứ cần thiết để chạy hệ điều hành.
- **RTOS (Real-Time Operating System):** một hệ điều hành (ví dụ FreeRTOS) đảm bảo phản hồi trong giới hạn thời gian nghiêm ngặt.
- **TLS (Transport Layer Security):** giao thức mã hoá dùng để bảo mật giao tiếp mạng.
- **U-Boot:** một bootloader mã nguồn mở phổ biến, dùng cho hệ thống nhúng.
- **Yocto Project:** bộ công cụ và metadata để build các bản phân phối embedded Linux tuỳ chỉnh, linh hoạt và phức tạp hơn Buildroot.
