Periferal Emulations
====================

In this lab, we study how to intergrate new perfiferal into the QEMU emulator. We will register a new periferal that emulate part of the FPGA that is present on the readl Reptar hardware.


1) QEMU environment and repatar machine
---------------------------------------

This part is an handover of the QEMU envionment provided for the lab. First part try to run the *qtemu* witch is the frontend to the QEMU and reptar emulation. To start *QEMU* and the *qtemu* togeter, we can can run the ``stq`` script::

    redsuser@vm-reds-2015s2:~/seee_student$ ./stq
    libGL error: pci id for fd 12: 80ee:beef, driver (null)
    libGL error: core dri or dri2 extension not found
    libGL error: failed to load driver: vboxvideo
    Running QEMU
    WARNING: Image format was not specified for 'filesystem/flash' and probing guessed raw.
             Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
             Specify the 'raw' format explicitly to remove the restrictions.
    WARNING: Image format was not specified for 'filesystem/sd-card.img' and probing guessed raw.
             Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
             Specify the 'raw' format explicitly to remove the restrictions.

    reptar-sp6-emul: sp6_emul_init
    
    
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
    
We can see that we reach the *U-Boot* prompt. We end up with the following environement:

    .. image:: stq_result.png
   
The QEMU windows (black container) represents the Reptar LCD screen. On the right, the frontend that represent the Reptar hardware. We can see that we have U-Boot prompt ready in the terminal window.

We can re-compile by hand the QUEM::

    redsuser@vm-reds-2015s2:~/seee_student$ cd qemu-reds/
    redsuser@vm-reds-2015s2:~/seee_student/qemu-reds$ ./configure --target-list=arm-softmmu --enable-debug --disable-attr --disable-docs; make -j 3
    
    redsuser@vm-reds-2015s2:~/seee_student/qemu-reds$ make
    ...
    ...
      LINK  arm-softmmu/qemu-system-arm
    make[1]: Leaving directory `/home/redsuser/seee_student/qemu-reds'




2) Spartan 6 FPGA Emulation
---------------------------

In this part, we will implement the Spartan 6 FPGA handler in QEMU. We will map its physical address space (but still emulated) to callback functions. There will be a callback for the read and one for the write. 

The program flow is the following:

    1) Register the periferal type
    2) In the board init function, create the periferal and connect it to the owner (SYS bus) and set the base address.
    3) In the periferal initialisation function, register the callbacks and declare the registers.
    4) Attache the register to the SYS bus.
   
   
   
The declaration of the periferal is the following:

.. code-block:: c

    // Declare the Spartan 6 periferal
    static const TypeInfo reptar_sp6_info = {
    		.name = "reptar_sp6",					// Name use for reference/creation
    		.parent = TYPE_SYS_BUS_DEVICE,			// This device belong to the SYS BUS
    		.instance_size = sizeof(sp6_state_t),	// Declare the memory needed (for dynamic alocation ?)
    		.instance_init = sp6_init,				// Declare instance intialization callback (instance constructor)
    		.class_init = sp6_class_init,			// Declare class(static) initialization callback (class constructor)
    };
    
    static void sp6_register_types(void)
    {
    	// Register the periferal type
    	type_register(&reptar_sp6_info);
    }
    
    
    // Magic tricks that will make sp6_reister_types() to be called.
    type_init(sp6_register_types)
    
The ``reptar_sp6_info`` structure names the periferal. This name will be used in the Reptar initialization function to intialize the periferal:

.. code-block:: c 

    static void reptar_init(MachineState *machine)
    {
      
        ...
        // Addd the Spartan 6 to the SYS BUS
        s->sp6 = sysbus_create_simple("reptar_sp6", 0x18000000, NULL);
    }
    
The above code adds the Spartan 6 periferal of type *"reptar_sp6"* to the system bus at base address **0x18000000**. 

The ``reptar_sp6_info`` structure also poins to two functions, ``sp6_init()`` and ``sp6_class_init()``.

The ``sp6_init()`` is the instance initialization function. For now, we keep it empty:

.. code-block:: c

    static void sp6_init(Object *obj)
    {
    	printf("sp6_init()\n");
    
    }  
    
The ``sp6_class_init()`` is the class initialization function. All it does is to reference another callback function, ``sp6_initfn()`` that will be called when the class is first referenced:

.. code-block:: c

    static void sp6_class_init(ObjectClass* this, void* data)
    {
    	SysBusDeviceClass* k = SYS_BUS_DEVICE_CLASS(this);
    	printf("sp6_class_init()\n");
    
    	k->init = sp6_initfn;
    }

To be clear, the ``sp6_class_init()` function will be called at the QEMU startup.  ``sp6_init()`` and ``sp6_initfn()`` will be called when adding the periferal to the system bus after the ``sysbus_create_simple()`` call.

The ``sp6_initfn()`` will initialze the communiction with the front-end, intialize the memory region and the callbacks and map the memory region to the device:

.. code-block:: c

    static int sp6_initfn(SysBusDevice *dev)
    {
    	printf("sp6_initfn()\n");
    	// Initialze the comunication with the graphics front-end
    	sp6_emul_init();
    
    	// Initialize an IO memeory range, and affect call back functions
    	memory_region_init_io(&sp6_mem_reg, OBJECT(dev), &sp6_ops, sp6_reg_state, "SP6", 16*1024*1024);
    	// Declare the memory on the system bus.
    	sysbus_init_mmio(dev, &sp6_mem_reg);
    
    	return 0;
    }
    
The ``memory_region_init_io()`` reference the ``sp6_ops`` structure. This is a global static structure containing the pointer to the read & write callbacks (It also define device endianness):

.. code-block:: c

    // Declare read & write call-back for the Spartan6 Register address
    static const MemoryRegionOps sp6_ops = {
        .read = sp6_read,
        .write = sp6_write,
        .endianness = DEVICE_NATIVE_ENDIAN,
    };

The read & write callback functions should be written, so they can be referenced by the above structure. For now, they just print that they are called so we can test our empty periferal:

.. code-block:: c

    static uint64_t sp6_read(void *opaque, hwaddr addr, unsigned size)
    {
    	printf("sp6_read(%x, %x, %x)\n", (int)opaque, (uint64_t)addr, size);
    	return 0;
    }
    
    static void sp6_write(void *opaque,  hwaddr addr, uint64_t data, unsigned size)
    {
    	printf("sp6_write(%x, %x, %x, %x)\n", (int)opaque, (uint64_t)addr, (int)data, size);
    }
    
We can re-comile QEMU and run it. We can see during the startup that the defined callback function are called before entering U-Boot::

    Reptar # redsuser@vm-reds-2015s2:~/seee_student$ ./stq
    libGL error: pci id for fd 12: 80ee:beef, driver (null)
    libGL error: core dri or dri2 extension not found
    libGL error: failed to load driver: vboxvideo
    Running QEMU
    sp6_class_init()
    WARNING: Image format was not specified for 'filesystem/flash' and probing guessed raw.
             Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
             Specify the 'raw' format explicitly to remove the restrictions.
    WARNING: Image format was not specified for 'filesystem/sd-card.img' and probing guessed raw.
             Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted.
             Specify the 'raw' format explicitly to remove the restrictions.
    sysbus_create_simple()
    sp6_initfn()
    reptar-sp6-emul: sp6_emul_init
    
    
    U-Boot 2011.09-00000-g9af6a15 (Feb 10 2015 - 16:10:59)


We can test the read & write callback using the ``md.w`` and ``mw.w`` utility from UBoot. Those function repectively read and write a 32 bit word from the memory. 

Test Read of read at 0x18000000 ::

    Reptar # md.w 0x18000000 1
    18000000:sp6_read(881920, 0, 2)
     0000    ..

We see the output of the ``printf()`` stament implemented in the ``sp6_read()`` callback.

Test Write of value "35" at address 0x18000000::

    Reptar # mw.w 0x18000000 35
    sp6_write(881920, 0, 35, 2)

We see the output of the ``printf()`` stament implemented in the ``sp6_write()`` callback.

    
3) LED devices emulation
------------------------

Once the Spartan 6 periferal is created and reachable from the emulated CPU, we need to implement the periferal behavior. The first part will be the LED output. The LEDS are driven by a single register. Each bit represent a LED. The offset for this register is **0x003a**.

To emulate the registers, we create a data structre as following:

.. code-block:: c

    typedef struct
    {
    	// Register address
    	uint32_t addr;
    	// Register value
    	uint32_t value;
    	// Callback that will be called on write
    	void (*write_callback)(uint32_t value);
    }
    fake_reg;
    
This structure contains the address of the register, its address, and a pointer to callback function that will be used to do some action when the register is written. This structure is then used to construct the register map as a static array of those structure:

.. code-block:: c

    #define LED_REG	0x003A
    #define GUARD_REG 0xFFFFFFFF
    
    static fake_reg sp6_reg_state[] =
    {
    		// {addr, value, write_callback}
    		{LED_REG, 0, leds_write},
    		{GUARD_REG, 0, NULL},
    };

The ``GUARD_RED`` will be used to break the loop when the last element is reached. We can modify the ``sp6_read()`` so that the value of the register value is return when reading it: 


.. code-block:: c

    static uint64_t sp6_read(void *opaque, hwaddr addr, unsigned size)
    {
    	fake_reg* reg = (fake_reg*)opaque;
    	printf("sp6_read(%x, %x, %x)\n", (int)opaque, (uint64_t)addr, size);
    
    	// Look for the address, until we reach it or the guard register
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
    	return 0;
    }


The ``sp6_write()``function is modified in a similar way. In addition, it call the callback function if its defined:

.. code-block:: c

    static void sp6_write(void *opaque,  hwaddr addr, uint64_t data, unsigned size)
    {
    	printf("sp6_write(%x, %x, %x, %x)\n", (int)opaque, (uint64_t)addr, (int)data, size);
    
    	fake_reg* reg = (fake_reg*)opaque;
    
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
    			reg->write_callback((uint32_t)data);
    		}
    	}
    	else
    	{
    		// For debug
    		printf("ERROR: Register 0x%x not found!\n");
    	}
    }

Then we need to implement the callback for the LED register. This function simply pass the value to the fontend:

.. code-block:: c

    void leds_write(uint32_t value)
    {
    	printf("Led write");
    
    	// Create the JSON object containing the data
    	cJSON* root = cJSON_CreateObject();
    	cJSON_AddStringToObject(root, "perif", "led");
    	cJSON_AddNumberToObject(root, "value", value);
    
    	// Pass it to the front-end
    	sp6_emul_cmd_post(root);
    }
    
We can then test by writing any value to the address 0x1800003A from the U-Boot prompt::

    TBD
    





