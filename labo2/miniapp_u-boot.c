#include <common.h>
#include <command.h>
#include <asm/arch/mux.h>
#include <asm/io.h>
#include <asm/errno.h>
#include "board/ti/reptar/reptar.h"

#define SW1 (1<<0)
#define SW2 (1<<1)
#define SW3 (1<<2)
#define SW4 (1<<3)
#define SW5 (1<<4)

extern int sevenseg_putc(int index, unsigned char number);

int old_state = 0;
int new_state =0;

int buttons(void);
void sample(void);
void latch(void);
int is_rising(unsigned int bit);

#define handle_counter(i) \
counters[i]++;\
if(counters[i] >= 10)\
{\
	counters[i] = 0;\
}\
sevenseg_putc(i, counters[i]);

int main(int argc, char *argv[])
{	
	int counters[] = {0, 0, 0};

	printf("Start of the Miniapp U-boot Standalone Application\n");

	sevenseg_putc(0, 0);
	sevenseg_putc(1, 0);
	sevenseg_putc(2, 0);

	/* A completer */

	while(1)
	{
		// Sample the button inputs
		sample();

		// Modify internal state
		if( is_falling(SW2))
		{
			handle_counter(0);
		}
		if( is_falling(SW5))
		{
			handle_counter(1);
		}
		if( is_falling(SW4))
		{
			handle_counter(2);
		}
		if (is_falling(SW3))
		{
			break;
		}

		// Save input states
		latch();
		delay(1000);
	}
	printf("Stop of the Miniapp U-boot Standalone Application\n");

	return 0;
}


int buttons()
{
	return readw(SP6_PUSH_BUT);
}

void sample(void)
{
	new_state = buttons();
}

void latch(void)
{
	old_state = new_state;
}

int is_rising(unsigned int bit)
{
	return (new_state & bit) && !(old_state & bit);
}

int is_falling(unsigned int bit)
{
	return !(new_state & bit) && (old_state & bit);
}
