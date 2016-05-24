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

Character driver
----------------

In this section we will implement the ``read()`` and ``write()`` callback inside the kernel module to enable to access and modify the content of a static buffer owned by the kernel module. The function are implemented this way:

.. code-block:: c

    ssize_t fpga_read(struct file *filp, char *buffer, size_t length, loff_t *offset) 
    {
      printk("read(len=%d, offset=%d)\n", length, *offset);
      // if offset is not zero, we have readen everithing, return that there is zero byte to read
      if (*offset != 0)
        return 0;
    
      // Trip the size
      if (length > strlen(bitstream_version))
      {
    	  length = strlen(bitstream_version);
      }
      // Avoid overflows
      if (length > sizeof(bitstream_version))
      {
        length = sizeof(bitstream_version);
      }
    
      // Copy to the userspace
      if(copy_to_user(buffer, bitstream_version, length))
      {
        printk("copy_to_user failed!\n");
        return -EFAULT;
      }
    
      return length;
    }

    ssize_t fpga_write(struct file *filp, const char *buff, size_t len, loff_t *off) 
    {
      // Avoid overflows
      if (len > sizeof(bitstream_version))
      {
          len = sizeof(bitstream_version);
      }
    
      printk("compying %d bytes\n	", len);
      if(copy_from_user(bitstream_version, buff, len))
        return -EFAULT;
    
      return len;
    }
    
    struct file_operations fpga_fops = {
      .read = fpga_read,
      .write = fpga_write,
    };
    
    
The node file that serve a driver entry point is generated by the following instruction:

.. code-block::c

    static int fpga_probe(struct platform_device *pdev) {
      ...
      device_create(pdata->fpga_class, NULL, pdev->dev.devt, NULL, "sp6");
      ...
      return 0;
    }


The device name will then be **/dev/sp6**. This is the file we will open for reading and wirting.
    
We can then write a user space application that can write this buffer using the provided character based interface:

.. code-block:: c

    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/fcntl.h>
    
    char buffer[80];
    
    int main(int argc, char **argv)
    {
    	int ret;
    
    	int f = open("/dev/sp6", O_RDWR);
    
    	// Read initial value
    	ret = read(f, buffer, sizeof(buffer));
    	printf("Read \"%s\" (%d bytes)\n", buffer, ret);
    
    	// modify the kernel value
    	lseek(f, 0, SEEK_SET);
    	sprintf(buffer, "Hello kernel!");
    	ret = write(f, buffer, strlen(buffer));
    	printf("Write \"%s\" (%d bytes)\n", buffer, ret);
    
    	// Read modified value
    	lseek(f, 0, SEEK_SET);
    	ret = read(f, buffer, sizeof(buffer));
    	printf("Read \"%s\" (%d bytes)\n", buffer, ret);
    
    	close(f);
    }
    
    
This code first read the buffer, then modify it, and read it again to see the modifications.

We can install the kernel module and then test the user application:

.. code-block:: console

    # insmod /sp6.ko 
    reptar_sp6: module starting...
    Probing FPGA driver (device: fpga)
    input: reptar_sp6_buttons as /devices/platform/fpga/reptar_sp6_buttons/input/input1
    reptar_sp6: done.
    # /usertest 
    read(len=80, offset=0)
    Read "FPGA bitstream VERSION" (22 bytes)
    compying 13 bytes
    	Write "Hello kernel!" (13 bytes)
    read(len=80, offset=0)
    Read "Hello kernel!m VERSION" (22 bytes)
    # 
    
We can alos see which major and minor number have been given to our driver by looking at the file attribute of the device file ``/dev/sp6``:

.. code-block:: console

    # ls -l /dev/sp6
    crw-rw----    1 root     root      252,   0 May  3 17:23 /dev/sp6
    
The numbers after the owner & group are the major and minor mumbers, so for our device the **major number is 252** and the **minor number is 0**. Those number where generated by the ``alloc_chrdev_region()`` call in the ``fpga_probe()`` function:

.. code-block:: c

    static int fpga_probe(struct platform_device *pdev) {
      ...
      ret = alloc_chrdev_region(&pdev->dev.devt, 0, 1, "fpga");
      
      ...
    }

LEDs Driver
-----------

In this part, we will add the LED driver functionality to our driver. For this, we first need to map the LEDS register to our driver address space. This is done using ``ioremap()`` from the probe function. We must also create a new driver of the class "LED":

.. code-block:: c

    static int reptar_sp6_led_probe(struct platform_device *pdev)
    {
    	struct reptar_sp6_led_platdata *pdata = pdev->dev.platform_data;
    	struct reptar_sp6_led *led;
    	struct platform_device *fpga_pdev;
    	struct resource *fpga_resource;
    	int ret;
    
    
    	....
    
        /* Map the LED register */
    	led->reg = ioremap(FPGA_BASE + LED_OFFSET, 4);
    
    	/* Register our new led device into led class */
    
    	printk("Alocating the LEDs\n");
    	ret = led_classdev_register(pdev->dev.parent, &(pdev_to_sp6_led(pdev)->cdev));
    	if(ret)
    	{
    	  dev_err(&pdev->dev, "can't allocate led driver\n");
    	  return -ENOENT;
    	}
    
    
    	return 0;
    }


It is alos a good practice to reserve the I/O memory using ``request_mem_region()`` and to release it with ``release_mem_region()``:

.. code-block:: c

    int __init reptar_sp6_leds_init(struct platform_device *parent_fpga)
    {
    	int i;
    
    	request_mem_region(FPGA_BASE + LED_OFFSET, 4, "sp6_leds");
    
    	... // Rest of function
    }
    
    void __exit reptar_sp6_leds_exit(void)
    {
    	int i;
    
    	... // Rest of function
    
    	release_mem_region(FPGA_BASE + LED_OFFSET, 4);
    }

Button driver
-------------

In this task, we will implement the button driver using the IRQ. The interrupt to map is the vector **10** as seen in the previous lab.  this can be done from the module probe function as following:

.. code-block:: c

    static int reptar_sp6_buttons_probe(struct platform_device *pdev)
    {
    	struct reptar_sp6_buttons_platdata *pdata = pdev->dev.platform_data;
    	struct reptar_sp6_buttons *btns;
    	struct input_dev *input;
    	struct platform_device *fpga_pdev;
    	struct resource *res;
    	int ret;
    	int gpio;
    	int i;
    
    	//... more code
    
    	/* Register 2 interrupt handlers (top, bottom) */
    	ret = request_threaded_irq( btns->irq,
    				    reptar_sp6_buttons_irq,
    				    reptar_sp6_buttons_irq_thread,
    				    IRQF_TRIGGER_RISING,
    				    "sp6_buttons",
    				    btns);
    	if(ret)
    	{
    	  dev_err(&pdev->dev, "Failed request IRQ\n");
    		      return -1;
    	}
    
    	// Enable IRQ at FPGA level
    	*(btns->irq_reg) |= 0x0080;
    	//printk("IRQ_CTR_REG = 0x%04x\n", *(btns->irq_reg));
    
    	platform_set_drvdata(pdev, btns);
    
    	/* Registration as input device */
    
    	// ...
    	
    	return 0;
    }

**Note** that we also need to enable the interrupt at FPGA level. This is done by the ``*(btns->irq_reg) |= 0x0080`` instruction!

We need then to get the button number and clear the IRQ from the imediate IRQ handler call-back (not from the thread):

.. code-block:: c

    static irqreturn_t reptar_sp6_buttons_irq(int irq, void *dev_id)
    {
      struct reptar_sp6_buttons *dev = dev_id;
    
    
    
      if (!(*(dev->irq_reg) & 0x10))
        {
          printk("Button IRQ triggered & IRQ_STATUS not set!\n");
        }
    
      /* take the button number from the IRQ CTRL reg */
      dev->current_button = *(dev->btns_reg);
    
      /* Clear the IRQ   (clear bit is the last bit)*/
      *(dev->irq_reg) |= 1;
    
      //printk("reptar_sp6_buttons_irq(%d)\n", dev->current_button);
    
      return IRQ_WAKE_THREAD;
    }
    
    
We can then the module using the povided utility:

.. code-block:: console

    # insmod /sp6.ko 
    reptar_sp6: module starting...
    [DOM-0] <4>Probing FPGA driver (device: fpga)
    [DOM-0] <7>Registered led device: sp6_led0
    [DOM-0] <7>Registered led device: sp6_led1
    [DOM-0] <7>Registered led device: sp6_led2
    [DOM-0] <7>Registered led device: sp6_led3
    [DOM-0] <7>Registered led device: sp6_led4
    [DOM-0] <7>Registered led device: sp6_led5
    [DOM-0] <4>IRQ_CTR_REG = 0x0080
    [DOM-0] <6>input: reptar_sp6_buttons as /devices/platform/fpga/reptar_sp6_buttons/input/input1
    [DOM-0] <4>reptar_sp6: done.
    # /buttons_test 
    Please start /buttons_test with argument -e<x> where <x> means that used device is /dev/input/event<x>
    # QObject: Cannot create children for a parent that is in a different thread.
    (Parent is QNativeSocketEngine(0xe6eb30), parent's thread is QThread(0xc9d290), current thread is EvtThread(0xea7990)
    
    # /buttons_test -e1
    Input device name: "reptar_sp6_buttons"
    Supported events:
      Event type 0 (Sync)
      Event type 1 (Key)
        Event code 1 (Esc)
        Event code 14 (Backspace)
        Event code 28 (Enter)
        Event code 57 (Space)
        Event code 103 (Up)
        Event code 105 (Left)
        Event code 106 (Right)
        Event code 108 (Down)
    Testing ... (will exit after 32 input events)
    Type: (Key), code 103 (Up), DOWN
    Event sync
    Type: (Key), code 103 (Up), UP
    Event sync
    Type: (Key), code 108 (Down), DOWN
    Event sync
    Type: (Key), code 108 (Down), UP
    Event sync
    Type: (Key), code 106 (Right), DOWN
    Event sync
    Type: (Key), code 106 (Right), UP
    Event sync
    Type: (Key), code 105 (Left), DOWN
    Event sync
    Type: (Key), code 105 (Left), UP
    Event sync
    Type: (Key), code 28 (Enter), DOWN
    Event sync
    Type: (Key), code 28 (Enter), UP
    Event sync
    Type: (Key), code 1 (Esc), DOWN
    Event sync
    Type: (Key), code 1 (Esc), UP
    Event sync
    Type: (Key), code 57 (Space), DOWN
    Event sync
    Type: (Key), code 57 (Space), UP
    Event sync
    Type: (Key), code 14 (Backspace), DOWN
    Event sync
    Type: (Key), code 14 (Backspace), UP
    Event sync
    # 



