
/*
 * Do not modify this file; it is automatically generated from the template
 * linkcmd.xdt in the ti.platforms.msp430 package and will be overwritten.
 */

/*
 * put '"'s around paths because, without this, the linker
 * considers '-' as minus operator, not a file name character.
 */


-l"/Volumes/SDrive/CCSV6/CapStone2Env/PID_462/Debug/configPkg/package/cfg/PID_RTOS_pe430X.oe430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/mw/wifi/cc3x00/lib/cc3x00_host_driver.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/drivers/lib/drivers_MSP430F5529.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/drivers/lib/drivers_wifi_MSP430F5529.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages/ti/uia/sysbios/lib/release/ti.uia.sysbios.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages/ti/uia/loggers/lib/release/ti.uia.loggers.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/mw/fatfs/lib/release/ti.mw.fatfs.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages/ti/drivers/ports/lib/tirtosport.ae430X"
-l"/Volumes/SDrive/CCSV6/CapStone2Env/PID_462/src/sysbios/sysbios.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages/ti/catalog/msp430/init/lib/ti.catalog.msp430.init.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages/ti/uia/runtime/lib/release/ti.uia.runtime.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages/ti/uia/events/lib/release/ti.uia.events.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages/ti/targets/msp430/rts430/lib/ti.targets.msp430.rts430.ae430X"
-l"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages/ti/targets/msp430/rts430/lib/boot.ae430X"

--retain="*(xdc.meta)"
--retain="*(xdc.noload)"


/* Elf symbols */
--symbol_map __TI_STACK_BASE=_stack
--symbol_map __TI_STACK_SIZE=_STACK_SIZE
--symbol_map __TI_STATIC_BASE=__bss__
--symbol_map __c_int00=_c_int00


--args 0x0
-heap  0x0
-stack 0x300

/*
 * Linker command file contributions from all loaded packages:
 */

/* Content from xdc.services.global (null): */

/* Content from xdc (null): */

/* Content from xdc.corevers (null): */

/* Content from xdc.shelf (null): */

/* Content from xdc.services.spec (null): */

/* Content from xdc.services.intern.xsr (null): */

/* Content from xdc.services.intern.gen (null): */

/* Content from xdc.services.intern.cmd (null): */

/* Content from xdc.bld (null): */

/* Content from ti.targets (null): */

/* Content from ti.targets.msp430.elf (null): */

/* Content from xdc.rov (null): */

/* Content from xdc.runtime (null): */

/* Content from xdc.services.getset (null): */

/* Content from ti.targets.msp430.rts430 (ti/targets/msp430/rts430/link.xdt): */

/* Definitions for ti.targets.msp430.rts430/boot.o* references:             */
/*     - ti_targets_msp430_rts430_noexit__I */
/*           defined in case main() returns in noexit variants              */
/*           of _c_int00.  Even in this case, we should call xdc.runtime    */
/*           exit processing; noexit only means that the RTS library's exit */
/*           handling is unnecessary.                                       */
--symbol_map ti_targets_msp430_rts430_noexit__I=xdc_runtime_System_exit__E

/* Content from ti.sysbios.interfaces (null): */

/* Content from ti.sysbios.family (null): */

/* Content from xdc.rta (null): */

/* Content from ti.uia.events (null): */

/* Content from ti.uia.runtime (null): */

/* Content from xdc.runtime.knl (null): */

/* Content from ti.sysbios.rts (ti/sysbios/rts/linkcmd.xdt): */

/* Content from ti.uia.services (null): */

/* Content from xdc.platform (null): */

/* Content from ti.catalog.msp430 (null): */

/* Content from ti.catalog (null): */

/* Content from ti.catalog.msp430.init (null): */

/* Content from xdc.cfg (null): */

/* Content from ti.catalog.msp430.peripherals.interrupt (null): */

/* Content from ti.catalog.msp430.peripherals.special_function (null): */

/* Content from ti.catalog.msp430.peripherals.timer (null): */

/* Content from ti.catalog.msp430.peripherals.watchdog (null): */

/* Content from ti.catalog.msp430.peripherals.clock (null): */

/* Content from ti.platforms.msp430 (null): */

/* Content from ti.sysbios (null): */

/* Content from ti.drivers.ports (null): */

/* Content from ti.mw.fatfs (null): */

/* Content from ti.uia.loggers (null): */

/* Content from ti.sysbios.hal (null): */

/* Content from ti.sysbios.family.msp430 (null): */

/* Content from ti.sysbios.knl (null): */

/* Content from ti.sysbios.syncs (null): */

/* Content from ti.uia.sysbios (null): */

/* Content from ti.drivers (null): */

/* Content from ti.mw.wifi.cc3x00 (null): */

/* Content from ti.mw (null): */

/* Content from ti.sysbios.gates (null): */

/* Content from ti.sysbios.xdcruntime (null): */

/* Content from ti.sysbios.heaps (null): */

/* Content from ti.sysbios.utils (null): */

/* Content from configPkg (null): */

/* Content from xdc.services.io (null): */



/*
 * symbolic aliases for static instance objects
 */
xdc_runtime_Startup__EXECFXN__C = 1;
xdc_runtime_Startup__RESETFXN__C = 1;


SECTIONS
{



    xdc.meta: type = COPY
    xdc.noload: type = COPY
}
