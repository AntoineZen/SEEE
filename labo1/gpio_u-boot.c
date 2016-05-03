/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <asm/arch/mux.h>
#include <asm/arch/gpio.h>
#include <asm/io.h>
#include <asm/errno.h>
#include "board/ti/reptar/reptar.h"
#include <asm/gpio.h>

#define NB_SWITCHS 	5
#define NB_LEDS 	4

const int SWITCH_LIST[] = { GPIO_SW_0, GPIO_SW_1, GPIO_SW_2, GPIO_SW_3, GPIO_SW_4};
const int LED_LIST[] = { GPIO_LED_0, GPIO_LED_1, GPIO_LED_2, GPIO_LED_3 };

int main(int argc, char *argv[])
{	
	int i;

	printf ("Start of the GPIO U-boot Standalone Application\n");

	for(i=0; i < NB_LEDS; i++)
	{
		gpio_request(LED_LIST[i], "");
	}
	for(i=0; i < NB_SWITCHS; i++)
	{
		gpio_request(SWITCH_LIST[i], "SW0");
	}

	while( !gpio_get_value(GPIO_SW_4) )
	{
		for( i=0; i< NB_LEDS;i++)
		{
			gpio_set_value(LED_LIST[i], gpio_get_value(SWITCH_LIST[i]));
		}
	}


	for(i=0; i < NB_LEDS; i++)
	{
	  gpio_free(LED_LIST[i]);
	}
	for(i=0; i < NB_SWITCHS; i++)
	{
	  gpio_free(SWITCH_LIST[i]);
	}


	printf ("Stop of the GPIO U-boot Standalone Application\n");

	return (0);
}
