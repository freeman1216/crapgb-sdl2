#pragma once
#ifndef BADDEFINES_H
#define BADDEFINES_H



// Memory region base addresses
#define ROM0_START      (0x0000) // 16KB ROM Bank 00
#define ROMX_START      (0x4000) // 16KB Switchable ROM bank
#define VRAM_START      (0x8000) // 8KB Video RAM
#define EXTRAM_START    (0xA000) // 8KB External RAM
#define WRAM0_START     (0xC000) // 4KB Work RAM Bank 0
#define WRAMX_START     (0xD000) // 4KB Work RAM Bank 1 (CGB only)
#define ECHO_START      (0xE000) // Echo of (0xC000–(0xDDFF (do not use)
#define OAM_START       (0xFE00) // Sprite Attribute Table (OAM)
#define UNUSED_START    (0xFEA0) // Not usable
#define IOREGS_START    (0xFF00) // I/O Registers
#define HRAM_START      (0xFF80) // High RAM (HRAM)
#define IE_REG_ADDR     (0xFFFF)  // Interrupt Enable Register

// Memory region sizes
#define ROM0_SIZE       (0x4000)
#define ROMX_SIZE       (0x4000)
#define VRAM_SIZE       (0x2000)
#define EXTRAM_SIZE     (0x2000)
#define WRAM0_SIZE      (0x1000)
#define WRAMX_SIZE      (0x1000)
#define ECHO_SIZE       (0x1E00) // Mirrors (0xC000–(0xDDFF
#define OAM_SIZE        (0x00A0)
#define UNUSED_SIZE     (0x0060)
#define IOREGS_SIZE     (0x0080)
#define HRAM_SIZE       (0x007F)
#define IE_REG_SIZE     (0x0001)

//LCDC bit masks
#define LCDC_PPU_ENABLE_MASK        (0x80)
#define LCDC_WINDOW_TILE_MAP_MASK   (0x40)
#define LCDC_WINDOW_ENABLE_MASK     (0x20)
#define LCDC_TILESET_MASK           (0x10)
#define LCDC_BG_TILE_MAP_MASK       (0x08)
#define LCDC_OBJ_SIZE_MASK          (0x04)
#define LCDC_OBJ_ENABLE_MASK        (0x02)
#define LCDC_BG_ENABLE_MASK         (0x01)
#define BG_WIN_ENABLE_MASK          (LCDC_BG_ENABLE_MASK | LCDC_WINDOW_ENABLE_MASK)

//STAT bit masks
#define STAT_LYC_MASK            (0x04)
#define STAT_LYC_INT_ENABLE_MASK (0x40)
#define STAT_MODE_MASK           (0x03)

//TIMA bit masks
#define TIMA_ENABLE_MASK        (0x04)
#define TIMA_CLOCK_SELECT_MASK  (0x03)

//PPU timing defines
#define CYCLES_PER_SCANLINE (456)
#define MODE2_OAM_CYCLES    (80)
#define MODE3_DRAW_CYCLES   (204)
#define MODE0_HBLANK_CYCLES (CYCLES_PER_SCANLINE - MODE2_OAM_CYCLES - MODE3_DRAW_CYCLES)
#define TOTAL_SCANLINES     (154)
#define VISIBLE_SCANLINES   (144)
#define FAKE_VBLANK_CYCLES  (4) 
#define PIXELS_PER_SCANLINE (160)

//PPU tiles defines
#define TILEMAP1_ADDR       (0x9800)
#define TILEMAP2_ADDR       (0x9C00)
#define TILESET_UNSIGNED    (0x8000)
#define TILESET_SIGNED      (0x9000)

//Sprite defines
#define NUM_OF_SPRITES          (40)
#define SPRITES_PER_SCANLINE    (10)
#define ATTR_PRIORITY_MASK      (0x80)
#define ATTR_YFLIP_MASK         (0x40)
#define ATTR_XFLIP_MASK         (0x20)
#define ATTR_PALLETE_MASK       (0x10)


//interrupt defines
#define REQUEST_INTERRUPT(mask)  badstate.io.if_reg |= mask 
#define INTERRUPT_VBLANK   (0x01)  // Bit 0
#define INTERRUPT_STAT     (0x02)  // Bit 1
#define INTERRUPT_TIMER    (0x04)  // Bit 2
#define INTERRUPT_SERIAL   (0x08)  // Bit 3
#define INTERRUPT_JOYPAD   (0x10)  // Bit 4

//Interrupt Vector Addresses 
#define VBLANK_VECTOR      (0x0040)
#define STAT_VECTOR        (0x0048)
#define TIMER_VECTOR       (0x0050)
#define SERIAL_VECTOR      (0x0058)
#define JOYPAD_VECTOR      (0x0060)
//Logging macro
#define BADLOG(fmt, ...) \
    do { \
        fprintf(stderr, "[LOG] "); \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
    } while (0)

#endif 