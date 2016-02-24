
===================
SEEE lab notes
===================


REPTAR Introduction
===================

This part has the goal to setup and to make a first contact with the environment used for the SEEE hand-on work (laboratory).


1) Environment setup
--------------------

The project files are provided as a tarball. The tarball was downloaded in the `~/Downloads` directory. We can exctract the repository with the following commands:

	redsuser@vm-reds-2015s2:~$ tar xf Downloads/seee_student.tar.gz 
	redsuser@vm-reds-2015s2:~$ cd seee_student/


We can now open Ecplise and select `/home/redsuser/seee_student` as workspace.

Next step is to build the QEMU emulator. The project downloaded is not clean, so we need to clean it first. Then we can build the emulator::
	
	redsuser@vm-reds-2015s2:~/seee_student$ cd qemu-reds
	redsuser@vm-reds-2015s2:~/seee_student/qemu-red$ make distclean
	redsuser@vm-reds-2015s2:~/seee_student/qemu-red$ cd..
        redsuser@vm-reds-2015s2:~/seee_student$ make qemu

2) QEMU startup
---------------

We can then run the Emulator direcly from Eclipse. For that select *Run -> Debug Configuration*. Then select the *gemu-reds Debug* configuration and click *Debug*. A black Windows apears, this is the emulator windows. 

We can pause the Emulator exectution:

	.. image::qemu_debugger.png

We can see that the emulator is stopped int the __GI_pppoll() function. We can then stop the execution.

From the command line we can start the QEMU as well at we arrive in the U-Boot environment::

	redsuser@vm-reds-2015s2:~/seee_student$ ../stf
	bash: ../stf: No such file or directory
	redsuser@vm-reds-2015s2:~/seee_student$ ./stf
	WARNING: Image format was not specified for 'filesystem/flash' and probing guessed raw.
		 Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
		 Specify the 'raw' format explicitly to remove the restrictions.
	WARNING: Image format was not specified for 'filesystem/sd-card.img' and probing guessed raw.
		 Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
		 Specify the 'raw' format explicitly to remove the restrictions.
	reptar-sp6-emul: sp6_emul_init
	sp6_emul_init: failed to connect to SP6 server
	sp6_emul_init: terminate thread


	U-Boot 2011.09-00000-g9af6a15 (Feb 10 2015 - 16:10:59)

	U-Boot code: 80008000 -> 80065570  BSS: -> 800F7C68
	OMAP35XX-GP ES3.1, CPU-OPP2, L3-165MHz, Max CPU Clock 600 mHz
	REPTAR Board + LPDDR/NAND
	I2C:   ready
	monitor len: 000EFC68
	ramsize: 10000000
	TLB table at: 8fff0000
	Top of RAM usable for U-Boot at: 8fff0000
	Reserving 959k for U-Boot at: 8ff00000
	Reserving 136k for malloc() at: 8fede000
	Reserving 32 Bytes for Board Info at: 8feddfe0
	Reserving 128 Bytes for Global Data at: 8feddf60
	New Stack Pointer is: 8feddf50
	RAM Configuration:
	Bank #0: 80000000 256 MiB
	Bank #1: 90000000 0 Bytes
	relocation Offset is: 0fef8000
	WARNING: Caches not enabled
	monitor flash len: 00066970
	Now running in RAM - U-Boot at: 8ff00000
	Flash: 16 KiB
	Flash pflash NOR detected.
	MMC:   OMAP SD/MMC: 0
	In:    serial
	Out:   serial
	Err:   serial
	fpga_init: CONFIG_FPGA = 0x1
	Net:   smc911x-0
	Warning: smc911x-0 MAC addresses don't match:
	Address in SROM is         52:54:00:12:34:56
	Address in environment is  e4:af:a1:40:01:fe

	Reptar # 


3) U-Boot test
--------------

For the U-Boot command prompt we can enter command like printing the environment::

	Reptar # printenv
	baudrate=115200
	boot_linux=mmc rescan; fatload mmc 0 ${loadaddr} uImage; setenv bootargs console=ttyO2,115200n8 noinitrd rw consoleblank=0 root=/dev/mmcblk0p2 rootwait ethaddr=${ethaddr} ip=${ipaddr}:${serverip}:${gatewayip}:${netmask}:::off; bootm ${loadaddr}
	bootargs=console=ttyO2,115200n8 noinitrd rw consoleblank=0 root=/dev/mmcblk0p2 rootwait bootm 0x81600000
	bootcmd=mmc rescan; fatload mmc 0 0x81600000 uImage; setenv bootargs console=ttyO2,115200n8 noinitrd rw consoleblank=0 root=/dev/mmcblk0p2 rootwait; bootm 0x81600000
	bootdelay=-1
	ethact=smc911x-0
	ethaddr=E4:AF:A1:40:01:FE
	fileaddr=81600000
	filesize=4AC808
	goapp=go 0x81600000
	ipaddr=10.0.2.10
	loadaddr=0x81600000
	netmask=255.255.255.0
	serverip=10.0.2.2
	stderr=serial
	stdin=serial
	stdout=serial
	test=test string
	tftp1=tftp helloworld_u-boot/helloworld.bin
	tftp2=tftp gpio_u-boot/gpio_u-boot.bin
	tftp3=tftp sp6_buttons_u-boot/sp6_buttons.bin

	Environment size: 949/4092 bytes


We can then buid the U-Boot demo executable::

	redsuser@vm-reds-2015s2:~/seee_student$ cd helloworld_u-boot/
	redsuser@vm-reds-2015s2:~/seee_student/helloworld_u-boot$ make
	arm-linux-gnueabihf-gcc -g  -O0  -fno-common -ffixed-r8 -msoft-float  -D__KERNEL__ -Iinclude -fno-builtin -ffreestanding -nostdinc -isystem /opt/linaro-arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/4.7.3/include -pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux -mno-thumb-interwork  -Wall -Wstrict-prototypes -c -o stubs.o stubs.c
	arm-linux-gnueabihf-ar crv libstubs.a stubs.o
	a - stubs.o
	arm-linux-gnueabihf-gcc -g  -O0  -fno-common -ffixed-r8 -msoft-float  -D__KERNEL__ -Iinclude -fno-builtin -ffreestanding -nostdinc -isystem /opt/linaro-arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/4.7.3/include -pipe  -DCONFIG_ARM -D__ARM__ -marm  -mabi=aapcs-linux -mno-thumb-interwork  -Wall -Wstrict-prototypes -c -o helloworld.o helloworld.c
	arm-linux-gnueabihf-gcc  -D__ASSEMBLY__ -g  -O0  -fno-common -ffixed-r8 -msoft-float  -D__KERNEL__ -Iinclude -fno-builtin -ffreestanding -nostdinc -isystem /opt/linaro-arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-gnueabihf/4.7.3/include -pipe  -c -o crt0.o crt0.S
	arm-linux-gnueabihf-ld -g  -Ttext 0x81600000 \
				-o helloworld crt0.o helloworld.o stubs.o \
				-L/opt/linaro-arm-linux-gnueabihf/bin/../lib/gcc/arm-linux-


	
