#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

/*
 STANDALONE FLASH LAYOUT (NO BOOTLOADER):

   name    |          range          |  size   |     function
-----------+-------------------------+---------+------------------
 Sector  0 | 0x08000000 - 0x08003FFF |  16 KiB | firmware (vectors + code)
 Sector  1 | 0x08004000 - 0x08007FFF |  16 KiB | firmware
 Sector  2 | 0x08008000 - 0x0800BFFF |  16 KiB | firmware / storage
 Sector  3 | 0x0800C000 - 0x0800FFFF |  16 KiB | firmware / storage
 Sector  4 | 0x08010000 - 0x0801FFFF |  64 KiB | firmware
 Sector  5 | 0x08020000 - 0x0803FFFF | 128 KiB | firmware
 Sector  6 | 0x08040000 - 0x0805FFFF | 128 KiB | firmware
 Sector  7 | 0x08060000 - 0x0807FFFF | 128 KiB | firmware
 Sector  8 | 0x08080000 - 0x0809FFFF | 128 KiB | firmware
 Sector  9 | 0x080A0000 - 0x080BFFFF | 128 KiB | firmware
 Sector 10 | 0x080C0000 - 0x080DFFFF | 128 KiB | firmware
 Sector 11 | 0x080E0000 - 0x080FFFFF | 128 KiB | firmware
*/

#define FLASH_ORIGIN 0x08000000
#define FLASH_TOTAL_SIZE (1024 * 1024)

#if EMULATOR
extern uint8_t *emulator_flash_base;
#define FLASH_PTR(x) (emulator_flash_base + ((x) - FLASH_ORIGIN))
#else
#define FLASH_PTR(x) ((const uint8_t *)(x))
#endif

/* -------------------------------------------------
 * STANDALONE FIRMWARE START
 * ------------------------------------------------- */

#define FLASH_APP_START 0x08000000
#define FLASH_APP_LEN   FLASH_TOTAL_SIZE

/* Safety guard: NEVER allow bootloader offsets again */
#if FLASH_APP_START != 0x08000000
#error "FLASH_APP_START MUST be 0x08000000 for standalone firmware"
#endif

/* -------------------------------------------------
 * Legacy definitions retained for compatibility
 * (no longer used for boot flow)
 * ------------------------------------------------- */

#define FLASH_BOOT_START FLASH_ORIGIN
#define FLASH_BOOT_LEN   0x0000

#define FLASH_STORAGE_START FLASH_ORIGIN
#define FLASH_STORAGE_LEN   0x0000

#define FLASH_FWHEADER_START FLASH_APP_START
#define FLASH_FWHEADER_LEN   0x0000

#define FLASH_BOOT_SECTOR_FIRST 0
#define FLASH_BOOT_SECTOR_LAST  0

#define FLASH_STORAGE_SECTOR_FIRST 0
#define FLASH_STORAGE_SECTOR_LAST  0

#define FLASH_CODE_SECTOR_FIRST 0
#define FLASH_CODE_SECTOR_LAST  11

/* -------------------------------------------------
 * API
 * ------------------------------------------------- */

void memory_protect(void);
void memory_write_unlock(void);

int memory_bootloader_hash(uint8_t *hash);
int memory_firmware_hash(const uint8_t *challenge,
                          uint32_t challenge_size,
                          void (*progress_callback)(uint32_t, uint32_t),
                          uint8_t hash[32]);

static inline void flash_write32(uint32_t addr, uint32_t word) {
    *(volatile uint32_t *)FLASH_PTR(addr) = word;
}

static inline void flash_write8(uint32_t addr, uint8_t byte) {
    *(volatile uint8_t *)FLASH_PTR(addr) = byte;
}

#endif /* __MEMORY_H__ */
