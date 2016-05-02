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
#include <asm/io.h>
#include <asm/errno.h>
#include "board/ti/reptar/reptar.h"

#define N_CHARS 10
#define N_DELAY 1E8

extern int sevenseg_putc(int index, unsigned char number);

int main(int argc, char *argv[])
{	
	int i = 0;
	int j;

	printf("Start of the 7seg U-boot Standalone Application\n");

	while (1) {
		sevenseg_putc(0, i % N_CHARS);
		sevenseg_putc(1, (i + 1) % N_CHARS);
		sevenseg_putc(2, (i + 2) % N_CHARS);

		j = N_DELAY;
		while (j--);

		i = (i + 1) % N_CHARS;
	}

	printf("Stop of the 7seg U-boot Standalone Application\n");

	return 0;
}
