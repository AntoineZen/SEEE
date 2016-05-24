/*
 * REPTAR FPGA buttons emulation
 *
 * Copyright (c) 2013 Reconfigurable Embedded Digital Systems (REDS) Institute at HEIG-VD, Switzerland
 * Written by Romain Bornet <romain.bornet@heig-vd.ch>
 *
 * This module provides a basic emulation for the 8 buttons of REPTAR's FPGA board.
 *
 */

#include "reptar_sp6.h"

sp6_state_t *sp6_state;

int reptar_sp6_btns_event_process(cJSON * object)
{
	//printf("reptar_sp6_btns_event_process()\n");
	int i;
	char* perif_name = cJSON_GetObjectItem(object, "perif")->valuestring;
	int button_index = 1;


	if (perif_name == NULL)
	{
		printf("Unable to have perif\n");
		cJSON_Delete(object);
		return 0;
	}

	if(strcmp(perif_name, "btn") == 0)
	{
		int button_bits = cJSON_GetObjectItem(object, "status")->valueint;

		for(i=0; i < 16; i++)
		{
		  if( button_bits & (1 << i))
		    break;
		  button_index++;
		}
		if(i>=16)
		  button_index=0;
		//printf("QEMU: button %d pressed\n", button);

		// Find the button register
		fake_reg* button_req = sp6_state->regs;
		while(button_req->addr != PUSH_BUT_REG && button_req->addr != GUARD_REG)
		{
			button_req++;
		}
		// Modify it if it was found
		if(button_req->addr == PUSH_BUT_REG)
		{
			button_req->value = button_bits;
		}


		// Find the IRQ CTR register
		fake_reg* irq_reg = sp6_state->regs;
		while(irq_reg->addr != IRQ_CTL_REG && irq_reg->addr != GUARD_REG)
		{
			irq_reg++;
		}
		// Modify it if it was found
		if(irq_reg->addr == IRQ_CTL_REG)
		{
			if(sp6_state->irq_enabled && !sp6_state->irq_pending)
			{
				sp6_state->irq_pending = true;
				qemu_irq_raise(sp6_state->irq);
				//printf("Raising IRQ\n");

				irq_reg->value &= ~SP6_IRQ_BTNS_MASK;
				irq_reg->value |= SP6_IRQ_STATUS;
				irq_reg->value |= (button_index << 1 ) & SP6_IRQ_BTNS_MASK;
				//irq_reg->value &= ~SP6_IRQ_SRC_MASK;
			}
		}
	}

	cJSON_Delete(object);
	return 0;

}

int reptar_sp6_btns_init(void *opaque)
{
    sp6_state = (sp6_state_t *) opaque;

    return 0;
}
