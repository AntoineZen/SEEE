/*
 * REPTAR Spartan6 FPGA emulation
 *
 * Copyright (c) 2013 HEIG-VD / REDS
 * Written by Romain Bornet
 *
 * This code is licensed under the GPL.
 */

#ifndef HW_SP6_H
#define HW_SP6_H

#include "hw/sysbus.h"
#include "qemu-common.h"
#include "reptar_sp6_buttons.h"
#include "reptar_sp6_clcd.h"
#include "reptar_sp6_emul.h"

/**
 * Desrcrite an hardware register.
 */
typedef struct
{
	// Register address
	uint32_t addr;
	// Register value
	uint32_t value;
	// Callback that will be called on write
	void (*write_callback)(uint32_t* value_ptr);
}
fake_reg;

typedef struct
{
    SysBusDevice busdev;
    MemoryRegion iomem;
    fake_reg* regs;		/* 1KB (512 * 16bits registers) register map */

    qemu_irq irq;
    int irq_pending;
    int irq_enabled;
} sp6_state_t;


/* Implemented registers addresses */
#define PUSH_BUT_REG	0x0012
#define IRQ_CTL_REG		0x0018
#define DISP_7SEG1_REG	0x0030
#define DISP_7SEG2_REG	0x0032
#define DISP_7SEG3_REG	0x0034
#define LED_REG			0x003A
#define GUARD_REG 		0xFFFFFFFF

/* Constants */
#define SP6_IRQ_CLEAR       0x0001
#define SP6_IRQ_SRC_MASK    0x0060
#define SP6_IRQ_EN          0x0080
#define SP6_IRQ_STATUS      0x0010
#define SP6_IRQ_BTNS_MASK   0x000E

/* Debug output configuration #define or #undef */
#undef REPTAR_SP6_EMUL_DEBUG

#ifdef REPTAR_SP6_EMUL_DEBUG
#define DBG(fmt, ...) \
do { printf("reptar-sp6-emul: " fmt , ## __VA_ARGS__); } while (0)
#else
#define DBG(fmt, ...) \
do { } while (0)
#endif


#endif /* #ifndef HW_SP6_H */
