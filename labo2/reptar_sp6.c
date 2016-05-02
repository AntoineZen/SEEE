/*
 * REPTAR Spartan6 FPGA emulation (reptar_sp6)
 *
 * Copyright (c) 2013-2014 HEIG-VD / REDS
 * Written by Romain Bornet
 *
 * This code is licensed under the GPL.
 */

#include "qemu-common.h"
#include "reptar_sp6.h"
#include "hw/sysbus.h"

#include "cJSON.h"

static void leds_write(uint32_t* value_ptr);
static void irq_ctl_reg_write(uint32_t* value_ptr);
static void d7seg1_reg_write(uint32_t* value_ptr);
static void d7seg2_reg_write(uint32_t* value_ptr);
static void d7seg3_reg_write(uint32_t* value_ptr);

static fake_reg sp6_reg_state[] =
{
		// {addr, value, write_callback}
		{PUSH_BUT_REG, 0, NULL},
		{IRQ_CTL_REG, 0, irq_ctl_reg_write},
		{DISP_7SEG1_REG, 0, d7seg1_reg_write},
		{DISP_7SEG2_REG, 0, d7seg2_reg_write},
		{DISP_7SEG3_REG, 0, d7seg3_reg_write},
		{LED_REG, 0, leds_write},
		{GUARD_REG, 0, 0},
};

static sp6_state_t sp6_state = {.regs=sp6_reg_state};


/**
 * Call back for when the led are written
 */
static void leds_write(uint32_t* value_ptr)
{
	//printf("Led write");

	// Create the JSON object containing the data
	cJSON* root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "perif", "led");
	cJSON_AddNumberToObject(root, "value", *value_ptr);

	// Pass it to the front-end
	sp6_emul_cmd_post(root);
}

static void irq_ctl_reg_write(uint32_t* value_ptr)
{
	// Manage IRQ Clear
	if (*value_ptr & SP6_IRQ_CLEAR)
	{
		if(sp6_state.irq_pending)
		{
			sp6_state.irq_pending = false;
			qemu_irq_lower(sp6_state.irq);
		}
		//printf("Clearing IRQ\n");

		// We don't wan to store the clear bit, so clear it as well as the whole register except the enable bit
		*value_ptr &= ~SP6_IRQ_CLEAR;

	}
	// Mangae the enable
	if (*value_ptr & SP6_IRQ_EN)
	{
		sp6_state.irq_enabled = true;
	}
	else
	{
		sp6_state.irq_enabled = false;
	}
}


static void set_7seg(int n, int value)
{
	//printf("Writing 7seg %d to %d\n", n, value);
	// Create the JSON object containing the data
	cJSON* root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "perif", "7seg");
	cJSON_AddNumberToObject(root, "digit", n);
	cJSON_AddNumberToObject(root, "value", value);

	// Pass it to the front-end
	sp6_emul_cmd_post(root);
}

static void d7seg1_reg_write(uint32_t* value_ptr)
{
	set_7seg(1, *value_ptr);
}


static void d7seg2_reg_write(uint32_t* value_ptr)
{
	set_7seg(2, *value_ptr);
}

static void d7seg3_reg_write(uint32_t* value_ptr)
{
	set_7seg(3, *value_ptr);
}

static uint64_t sp6_read(void *opaque, hwaddr addr, unsigned size)
{
	fake_reg* reg = ((sp6_state_t*)opaque)->regs;
	//printf("sp6_read(%x, %x, %x)\n", (int)opaque, (int)addr, size);

	// Look for the address by its address, until we reach it or the guard register
	while(reg->addr != addr && reg->addr != GUARD_REG)
	{
		reg++;
	}
	// If we matched the readen address to an existing register, return its value
	if( reg->addr == addr)
	{
		return reg->value;
	}
	// Else, we read 0
	else
	{
		// For debug
		printf("ERROR: sp6_read: Register 0x%lx not found!\n", (uint64_t)addr);
	}
	return 0;
}

static void sp6_write(void *opaque,  hwaddr addr, uint64_t data, unsigned size)
{
	//printf("sp6_write(%x, %x, %x, %x)\n", (int)opaque, (uint64_t)addr, (int)data, size);

	fake_reg* reg = ((sp6_state_t*)opaque)->regs;

	// Look for the register by its address, until we reach it or the guard register
	while(reg->addr != addr && reg->addr != GUARD_REG)
	{
		reg++;
	}

	// If we matched the readen address to an existing register, modify it
	if( reg->addr == addr)
	{
		reg->value = (uint32_t)data;

		// If a callback is defined, call it passing the written data
		if(reg->write_callback != NULL)
		{
			reg->write_callback(&reg->value);
		}
	}
	else
	{
		// For debug
		printf("ERROR: sp6_write: Register 0x%lx not found!\n", (uint64_t)addr);
	}
}

// Declare read & write call-back for the Spartan6 Register address
static const MemoryRegionOps sp6_ops = {
    .read = sp6_read,
    .write = sp6_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

/*
static void sp6_init(Object *obj)
{
	printf("sp6_init()\n");
}
*/

static int sp6_initfn(SysBusDevice *dev)
{
	printf("sp6_initfn()\n");
	// Initialze the comunication with the graphics front-end
	sp6_emul_init();

	// Initialize an IO memeory range, and affect call back functions
	memory_region_init_io(&sp6_state.iomem, OBJECT(dev), &sp6_ops, &sp6_state, "SP6", 16*1024*1024);
	// Declare the memory on the system bus.
	sysbus_init_mmio(dev, &sp6_state.iomem);

	// Tell the button driver where to find the device state.
	reptar_sp6_btns_init(&sp6_state);

	// map the IRQ
	sysbus_init_irq(dev, &(sp6_state.irq));

	return 0;
}

static void sp6_class_init(ObjectClass* this, void* data)
{
	SysBusDeviceClass* k = SYS_BUS_DEVICE_CLASS(this);
	printf("sp6_class_init()\n");

	k->init = sp6_initfn;
}

// Declare the Spartan 6 periferal
static const TypeInfo reptar_sp6_info = {
		.name = "reptar_sp6",					// Name use for reference/creation
		.parent = TYPE_SYS_BUS_DEVICE,			// This device belong to the SYS BUS
		.instance_size = sizeof(sp6_state_t),	// Declare the memory needed (for dynamic alocation ?)
		//.instance_init = sp6_init,				// Declare instance intialization callback (instance constructor)
		.class_init = sp6_class_init,			// Declare class(static) initialization callback (class constructor)
};

static void sp6_register_types(void)
{
	// Register the periferal type
	type_register(&reptar_sp6_info);
}


// Magic tricks that will make sp6_reister_types() to be called.
type_init(sp6_register_types)
