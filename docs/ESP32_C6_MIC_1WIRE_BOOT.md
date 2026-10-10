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
