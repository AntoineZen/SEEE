Drivers
=======

QEMU Environment and REPTAR platform
------------------------------------

In this section, we will deploy the environment from the MMC cart and we will experiment dynamic module loading. For this we will use the **drivers** project provided in ``~/seee_studend/drivers``. First, we can compile the project:

.. code-block:: console

    redsuser@vm-reds-2015s2:~$ cd seee_student/drivers/
    redsuser@vm-reds-2015s2:~/seee_student/drivers$ make
    make -C ../linux-3.0-reptar M=/home/redsuser/seee_student/drivers ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- 
    make[1]: Entering directory `/home/redsuser/seee_student/linux-3.0-reptar'
      LD      /home/redsuser/seee_student/drivers/built-in.o
      CC [M]  /home/redsuser/seee_student/drivers/reptar_sp6.o
    /home/redsuser/seee_student/drivers/reptar_sp6.c:96:13: warning: 'bitstream_version' defined but not used [-Wunused-variable]
    /home/redsuser/seee_student/drivers/reptar_sp6.c: In function 'fpga_write':
    /home/redsuser/seee_student/drivers/reptar_sp6.c:115:1: warning: control reaches end of non-void function [-Wreturn-type]
    /home/redsuser/seee_student/drivers/reptar_sp6.c: In function 'fpga_read':
    /home/redsuser/seee_student/drivers/reptar_sp6.c:110:1: warning: control reaches end of non-void function [-Wreturn-type]
      CC [M]  /home/redsuser/seee_student/drivers/reptar_sp6_leds.o
    /home/redsuser/seee_student/drivers/reptar_sp6_leds.c: In function 'reptar_sp6_led_set':
    /home/redsuser/seee_student/drivers/reptar_sp6_leds.c:43:34: warning: unused variable 'pd' [-Wunused-variable]
    /home/redsuser/seee_student/drivers/reptar_sp6_leds.c: In function 'reptar_sp6_led_probe':
    /home/redsuser/seee_student/drivers/reptar_sp6_leds.c:69:6: warning: unused variable 'ret' [-Wunused-variable]
      CC [M]  /home/redsuser/seee_student/drivers/reptar_sp6_buttons.o
    /home/redsuser/seee_student/drivers/reptar_sp6_buttons.c: In function 'reptar_sp6_buttons_irq':
    /home/redsuser/seee_student/drivers/reptar_sp6_buttons.c:41:30: warning: unused variable 'dev' [-Wunused-variable]
    /home/redsuser/seee_student/drivers/reptar_sp6_buttons.c: At top level:
    /home/redsuser/seee_student/drivers/reptar_sp6_buttons.c:39:20: warning: 'reptar_sp6_buttons_irq' defined but not used [-Wunused-function]
    /home/redsuser/seee_student/drivers/reptar_sp6_buttons.c:49:20: warning: 'reptar_sp6_buttons_irq_thread' defined but not used [-Wunused-function]
      LD [M]  /home/redsuser/seee_student/drivers/sp6.o
      Building modules, stage 2.
      MODPOST 1 modules
      CC      /home/redsuser/seee_student/drivers/sp6.mod.o
      LD [M]  /home/redsuser/seee_student/drivers/sp6.ko
    make[1]: Leaving directory `/home/redsuser/seee_student/linux-3.0-reptar'
    arm-linux-gnueabihf-gcc -marm -I../linux-3.0-reptar -static buttons_test.c -o buttons_test
    arm-linux-gnueabihf-gcc     usertest.c   -o usertest


We can then deplay the compiled files to the root file-system of the emulated system using the provided srcipt:

.. code-block:: console
    redsuser@vm-reds-2015s2:~/seee_student/drivers$ cd ..
    redsuser@vm-reds-2015s2:~/seee_student$ ./deploy 
    Deploying into reptar rootfs ...
    Mounting filesystem/sd-card.img...
    [sudo] password for redsuser: 
    SD card partitions mounted in 'boot_tmp' and 'filesystem_tmp' directories
    Unmounting SD card image...
      Synchronizing .img file
      Unmounting 'boot_tmp' and 'filesystem_tmp'...
    Done !

This srcipt mount the MMC card image, copy the file to it and unmount it. As our code (driver + app) is deployed, we can test it. For this we will start the emulator then GNU/Linux inside the emulator:


.. code-block:: console

    redsuser@vm-reds-2015s2:~/seee_student$ ./stf
    sp6_class_init()
    WARNING: Image format was not specified for 'filesystem/flash' and probing guessed raw.
             Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
             Specify the 'raw' format explicitly to remove the restrictions.
    WARNING: Image format was not specified for 'filesystem/sd-card.img' and probing guessed raw.
             Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
             Specify the 'raw' format explicitly to remove the restrictions.
    sysbus_create_simple()
    sp6_initfn()
    sp6_emul_init: failed to connect to SP6 server
    sp6_emul_init: terminate thread
    
    
    U-Boot 2011.09-00000-g9af6a15 (Feb 10 2015 - 16:10:59)
    
    U-Boot code: 80008000 -> 80065570  BSS: -> 800F7C68
    OMAP35XX-GP ES3.1, CPU-OPP2, L3-165MHz, Max CPU Clock 600 mHz
    REPTAR Board + LPDDR/NAND
    .....
    Address in environment is  e4:af:a1:40:01:fe
    
    Reptar # boot
    reading uImage
    
    3051480 bytes read
    ## Booting kernel from Legacy Image at 81600000 ...
       Image Name:   Linux-3.0.12-reptar
       Image Type:   ARM Linux Kernel Image (uncompressed)
       Data Size:    3051416 Bytes = 2.9 MiB
       Load Address: 80008000
       Entry Point:  80008000
       Verifying Checksum ... OK
       Loading Kernel Image ... OK
    OK
    Using machid 0x2694 
    
    Starting kernel ...
    
    ......
    
    *** Welcome on REPTAR (HEIG-VD/REDS): use root/root to log in ***
    reptar login:
    
    
Once the GNU/Linux operating system is started, we can test to load & unload the compiled kernel module:

.. code-block:: console

    *** Welcome on REPTAR (HEIG-VD/REDS): use root/root to log in ***
    reptar login: root
    Password: 
    # pwd
    /root
    # cd /
    
    # ls
    bin           home          lost+found    proc          sp6.ko        usr
    buttons_test  ledstest.sh   media         root          sys           var
    dev           lib           mnt           run           tmp
    etc           linuxrc       opt           sbin          usertest
    # insmod sp6.ko 
    reptar_sp6: module starting...
    Probing FPGA driver (device: fpga)
    input: reptar_sp6_buttons as /devices/platform/fpga/reptar_sp6_buttons/input/input1
    reptar_sp6: done.
    # lsmod
    Module                  Size  Used by    Not tainted
    sp6                     4606  0 
    # rmmod sp6
    reptar_sp6: bye bye!
    #

