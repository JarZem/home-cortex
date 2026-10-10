# MIC CORE v2 ↔ ESP32-C6: 1Wire BOOT and diagnostics (2026-10-10)

## Wire contract
Physical link: CH32V003 MIC ↔ ESP32-C6, single half-duplex open-drain data wire plus GND, 19200 baud, 8N1. Existing CORE v2 transport is in `src/v2/core_v2.[ch]`. First byte is always interpreted within the existing protocol, **not** as arbitrary ASCII. One-byte control tokens: `0x7E` KEEPALIVE and `0x7F` NACK. The reserved BOOT opcode is `0x80` (control class, high bits `10`). MIC transmits **exactly three bytes** on every MCU start: `[0x80, previous_status, CRC8]`. CRC8 uses polynomial 0x07, init 0x00, calculated over the first two bytes via `v2_crc8`. `previous_status=0xFF` means unknown/invalid. ESP must recognize 0x80 as a three-byte BOOT control message rather than a one-byte ACK/NAK token, verify CRC8 and record status.

## Startup order
1. Initialize MCU clocks, USART/DMA/1Wire hardware.
2. Read retained operation marker BEFORE overwriting it. Build BOOT frame.
3. Mark current operation IDLE, initialize heartbeat, initialize transport.
4. Queue BOOT as first outgoing frame and start transmission immediately via existing DMA TX. Regular keepalive follows; BOOT is not emitted as a SENSOR packet.
5. ESP receives BOOT for **every start**, including power-on and software restart. No confirmation/retry is currently implemented; therefore delivery to ESP is not yet guaranteed if the bus is busy or the receiver is offline.

## Retained operation status
`src/v2/boot_status.[ch]` defines a two-byte `.noinit` pair: `status` and `~status`. Read validity requires XOR == 0xFF. Values: `0x00` IDLE; `0x01` SENSOR; `0xF0` OTA_ACTIVE; `0xF1` OTA_FLASH_WRITE; `0xF2` OTA_VERIFY. Additional sensor operation codes are to be allocated by the SENSOR state machines. The state machine writes status **at operation start**, and increments heartbeat after completing its dispatch pass. UART RX, DMA and 1Wire transport **must not** feed the sensor heartbeat or overwrite sensor diagnostic status. When OTA begins, set OTA_ACTIVE; mark write and verify stages accordingly. The OTA state machine is not yet integrated in this experimental firmware; these codes are reserved.

A retained byte is not a hardware reset-cause flag. The BOOT frame currently contains the previous operation marker, not an RCC reset-reason byte. The complement detects most corrupt/uninitialized pairs, but cannot prove a reset was caused by a watchdog. On a power-on reset the old SRAM contents must not be trusted; adding RCC reset-cause gating is a pending improvement.

## Watchdog
`src/v2/heartbeat.[ch]`: heartbeat is checked every 600000 SysTick ticks (~100 ms at 6 MHz). Four unchanged checks trigger `NVIC_SystemReset()`. The watchdog is enabled by the SENSOR dispatcher only once running. The current experimental transport-only `sensor_stub` intentionally does not enable/feed it. OTA needs its own timeout policy so sensor inactivity during Flash programming does not cause false resets. Hardware IWDG is not yet implemented.

## Build and validation (branch `bench/reginit16`)
```sh
git pull --ff-only
pio run -e HOST_CORE_V2_TEST && .pio/build/HOST_CORE_V2_TEST/program
pio run -e HOST_V2_HEARTBEAT_TEST && .pio/build/HOST_V2_HEARTBEAT_TEST/program
pio run -e HOST_V2_BOOT_STATUS_TEST && .pio/build/HOST_V2_BOOT_STATUS_TEST/program
pio run -e CH32V003_CORE_V2
~/.platformio/packages/toolchain-riscv/bin/riscv-wch-elf-size -A .pio/build/CH32V003_CORE_V2/firmware.elf
```
User-reported successful target build: .init 160 B + .text 2468 B = **2628 B Flash**; .bss 296 B + .noinit 2 B = **298 B RAM**. The linker locates .noinit at 0x20000128 after .bss. A real-board soft-reset retention test, verification that startup does not clear .noinit, and actual ESP receive test remain outstanding. Do not mistake successful compilation for hardware verification.


## Agreed minimal A/B OTA handshake (protocol specification; implementation pending)
SENSOR images are **linked separately** for fixed slot A and slot B. No PIC or relocation support in MIC. ESP owns both build variants and selects the image **for the inactive slot** based on the MIC response. The one-byte logical active-slot selector is stored in Flash; its physical page programming and power-fail behavior still require implementation and validation.

At 19200 baud ESP sends a single OTA request: `[0x40, COUNT_LO, COUNT_HI, CRC32_LE(4), CRC8]` (8 bytes). COUNT is the number of 64-byte data frames, including any final 0xFF padding; CRC32 covers **exactly COUNT*64 bytes** of the padded, slot-specific binary image. CRC8 polynomial 0x07/init 0 covers the first seven bytes. Zero count and any count exceeding the target slot capacity are rejected. All multi-byte fields are little-endian.

MIC reads its one-byte active-slot selector and replies **OTA_READY** with the **currently active slot** so ESP can select the opposite build: `[0x81, ACTIVE_SLOT, CRC8]` (3 bytes), where ACTIVE_SLOT 0x00=A or 0x01=B. `0x81` is a proposed reserved control opcode; the ESP must wait for OTA_READY before sending data. MIC automatically selects TARGET_SLOT = ACTIVE_SLOT XOR 1; ESP must send the binary linked for that target. If selector is invalid, MIC must NACK and remain in CORE (no writes). The READY response must be transmitted before arming the first OTA RX frame, avoiding half-duplex collisions.

Each frame: `[0x41, 64 bytes, CRC8]` (66 bytes); frame index is implicit and increments **only after Flash write and read-back verification**. On each frame: receive into RAM; CRC8; compute address TARGET_BASE + index*64; erase/program Flash as supported by device; compare programmed bytes; ACK; increment heartbeat; await next frame. On error NACK and keep the same index. ESP retries the same frame, restarting the entire OTA session after four NACKs. The MIC must also handle repeated frames after a lost ACK (index-less frames require a reliable ACK/retransmission synchronization rule; this is not yet solved by CRC alone). OTA heartbeat timeout is 4 times the complete frame budget including receive, erase, program and verify.

After COUNT accepted frames, MIC calculates CRC32 over exactly COUNT*64 bytes of target Flash. If it matches the request, MIC writes the active-slot selector to the target slot **only then**, and immediately resets. After reset, the existing BOOT message reports the prior OTA status and CORE starts the newly active, correctly linked SENSOR. A CRC32 mismatch leaves the old slot active and reports failure. No separate ERASE, WRITE, VERIFY or RUN commands.

CRC8 for packets and CRC32 for the image are logically distinct; implementations may share a bitwise kernel only if that saves compiled Flash. Compare both build sizes. The OTA control decoder, flash writer, OTA_READY and selector storage are **not yet implemented** in CORE v2. The protocol's active-slot selector needs a power-loss-safe commit mechanism despite its one-byte logical representation.

## OTA frame index — agreed correction
Every data frame contains a **one-byte index**: `[0x41, INDEX8, DATA64, CRC8]` (67 bytes, within current RX buffer). Before **any** erase/write, MIC compares INDEX8 against the expected index (starting at 0, incremented after ACK). On mismatch, **abort the OTA session immediately**; do not NACK-and-retry and do not write Flash. Existing active slot remains unchanged. With a one-byte index, a session supports at most 256 frames (16 KiB); the smaller SENSOR slot limit applies. On valid index, continue CRC8 → erase/write/read-back → ACK; ordinary CRC/write failures may NACK and be retried. An unexpected index is terminal, as explicitly requested.

## OTA V2 implementation status and corrected handshake (2026-10-10)
The earlier single `OTA_START(count,crc32)` followed by `OTA_READY` has an ordering error: ESP cannot calculate the correct A/B-specific image CRC before learning the active slot. **Normative order:** (1) ESP requests OTA mode (opcode to be finalized in transport), (2) MIC responds `[0x81, active_slot, crc8]`, (3) ESP selects binary for inactive slot and sends `[0x40,count_lo,count_hi,crc32_le4,crc8]`, (4) MIC receives indexed frames `[0x41,index8,data64,crc8]`. The 67-byte frame exactly fits V2_MAX_RX. Index mismatch aborts immediately before writing; four consecutive CRC/write failures abort; valid frames write the inactive slot, then full-image CRC32, commit active slot, reset. The image CRC32 is CRC-32/ISO-HDLC (reflected polynomial 0xEDB88320, init/final XOR 0xFFFFFFFF) over count*64 bytes including padding.

The CORE repository now includes portable `src/v2/ota_v2.[ch]` with this indexed transfer logic and `test/v2_ota_test.c`, built using `pio run -e HOST_V2_OTA_TEST && .pio/build/HOST_V2_OTA_TEST/program`. The callbacks for reading/writing the active-slot byte, real CH32V003 Flash page erase/program/readback, restart, and physical 1Wire control dispatch **are not yet wired to MCU hardware**. Tests are prepared but their execution has not yet been confirmed. The physical flash selector is one logical byte; safe physical storage across power loss remains an implementation requirement.

## OTA mandatory ACK after image header
Handshake is strictly: ESP requests OTA mode → MIC replies OTA_READY(active A/B) → ESP sends image header (frame count + CRC32 + CRC8) for inactive-slot-specific build → MIC validates header and replies **ACK (0x7E)** or **NACK (0x7F)** → ESP sends DATA index 0 **only after ACK** → MIC ACK/NACK after each frame → next DATA. Invalid header must never arm data reception. Header ACK is separate from OTA_READY. No data frames may be sent before it.

## OTA V2 actual firmware wiring — experimental, not hardware-verified
Current files: `src/v2/ota_v2.[ch]` (indexed protocol), `src/v2/ota_flash_ch32.c` (real 64-byte Flash erase/program/readback, CRC32/ISO-HDLC, bounded A/B slot addresses, redundant one-byte active selector), `src/v2/mic_hw.c` (deferred OTA processing in main loop), `src/v2/core_v2.[ch]` (DMA RX decoding), `scripts/ota_selector_init.py` (one-time selector provisioning). Build target `CH32V003_CORE_V2` now includes OTA. This is experimental code: compile and hardware tests have NOT been reported yet.

Actual wire sequence:
- ESP -> MIC: `[0x42,CRC8]` (OTA request; CRC8 over 0x42).
- MIC -> ESP: `[0x81,ACTIVE_SLOT,CRC8]`; invalid/unprovisioned selector returns NACK.
- ESP chooses opposite slot-specific binary and sends `[0x40,COUNT_LO,COUNT_HI,CRC32_LE4,CRC8]`.
- MIC validates COUNT 1..55 and header CRC8, then sends **single-byte ACK 0x7E** (or NACK 0x7F). No data before ACK.
- ESP -> MIC: `[0x41,INDEX8,DATA64,CRC8]`; MIC verifies index before Flash, checks CRC8, erases/writes/verifies 64B page, responds ACK/NACK. Four consecutive CRC/write errors terminate the session. Index mismatch immediately terminates without writing. CRC32 of count*64 Flash bytes checked after last accepted page.
- MIC commits the one-byte active selector only after full CRC32 success, then resets. The current implementation resets immediately on successful last frame; ESP should treat subsequent BOOT as confirmation, **not** expect a final ACK.

Memory layout per current SENSOR linkers: A `0x08002400` 3520B (55 pages), B `0x080031C0` 3520B (55 pages). Metadata A `0x08003F80`, metadata B `0x08003FC0` (each 64B). No OTA operation may erase CORE, the active SENSOR or either metadata page except selector commit. **On a factory-blank MCU, OTA refuses to start until the selector is provisioned via SWIO**: `python3 scripts/ota_selector_init.py --slot A` produces a 64B image for address `0x08003F80` (this script does not flash hardware). Provision the corresponding A-linked SENSOR separately. The last Flash page access and write-protection settings must be verified against the exact chip/revision before flashing.

Hardware checks required before deployment: compile Flash/RAM/linker map and check for overlap; actual Flash programming and readback on spare MCU; power loss during page programming and during selector commit; SWIO recovery; 1Wire timing and USART DMA, ACK timing; activation and execution of separately linked SENSOR A/B (the experimental CORE V2 still has a SENSOR stub rather than the final SENSOR dispatcher). Do not claim fully operational end-to-end OTA until these pass.

Community research: WCH reference manual documents fast 64B erase/write and separate unlock of LOCK/FLOCK. The factory ROM UART loader erases *all* user Flash regardless of requested sector count (basilhussain/ch32v003-bootloader-docs), therefore cannot preserve CORE/A-B. OpenServoCore/tinyboot demonstrates a 1-wire loader fitting the 1920B system-flash area, but moving our CORE into system Flash requires option-byte/boot-path work and carries significant risk. mikrocontroller.net CH32V003 serial bootloader discussion describes a severe case of corrupted page count overwriting bootloader: strict COUNT<=55 validation is mandatory. Russian ELECTRONIX thread reports RAM-executed Flash routines freezing on CH32V003; keep the current Flash routine in CORE Flash and verify hardware rather than assuming RAM execution is safer.

## Final frame ACK correction
After the last valid DATA frame and full-image CRC32, MIC commits the new selector, sends a **final ACK (0x7E)**, waits for USART transmission-complete (TC), then resets. ESP must read this final ACK, then wait for the BOOT frame. If CRC32 or commit fails, MIC sends NACK and retains old active slot. The ESP reference sender in `home-cortex/firmware/esp32_c6/mic_ota_client.[ch]` implements request → READY → header ACK → indexed DATA/ACK → final ACK → BOOT. The sender's `send/recv` callbacks still need binding to the actual ESP32-C6 UART/1Wire bus; no actual ESP32 firmware repository was identified.
