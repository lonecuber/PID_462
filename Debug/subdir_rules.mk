################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
MAIN.obj: ../MAIN.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"/Volumes/SDrive/CCSV6/ccsv6/tools/compiler/ti-cgt-msp430_15.12.1.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=globals --opt_for_speed=5 --use_hw_mpy=F5 --include_path="/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430F5xx_6xx" --include_path="/Volumes/SDrive/CCSV6/ccsv6/ccs_base/msp430/include" --include_path="/Volumes/SDrive/CCSV6/CapStone2Env/PID_462" --include_path="/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430F5xx_6xx" --include_path="/Volumes/SDrive/CCSV6/ccsv6/tools/compiler/ti-cgt-msp430_15.12.1.LTS/include" --advice:power_severity=suppress -g --define=__MSP430F5529__ --define=ccs --define=MSP430WARE --diag_warning=225 --diag_warning=255 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --abi=eabi --printf_support=minimal --preproc_with_compile --preproc_dependency="MAIN.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

MSP_EXP430F5529LP.obj: ../MSP_EXP430F5529LP.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"/Volumes/SDrive/CCSV6/ccsv6/tools/compiler/ti-cgt-msp430_15.12.1.LTS/bin/cl430" -vmspx --data_model=restricted --near_data=globals --opt_for_speed=5 --use_hw_mpy=F5 --include_path="/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430F5xx_6xx" --include_path="/Volumes/SDrive/CCSV6/ccsv6/ccs_base/msp430/include" --include_path="/Volumes/SDrive/CCSV6/CapStone2Env/PID_462" --include_path="/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430F5xx_6xx" --include_path="/Volumes/SDrive/CCSV6/ccsv6/tools/compiler/ti-cgt-msp430_15.12.1.LTS/include" --advice:power_severity=suppress -g --define=__MSP430F5529__ --define=ccs --define=MSP430WARE --diag_warning=225 --diag_warning=255 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --abi=eabi --printf_support=minimal --preproc_with_compile --preproc_dependency="MSP_EXP430F5529LP.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: ../PID_RTOS.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"/Volumes/SDrive/CCSV6/xdctools_3_32_00_06_core/xs" --xdcpath="/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/packages;/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages;/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages;/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages;" xdc.tools.configuro -o configPkg -t ti.targets.msp430.elf.MSP430X -p ti.platforms.msp430:MSP430F5529 -r release -c "/Volumes/SDrive/CCSV6/ccsv6/tools/compiler/ti-cgt-msp430_15.12.1.LTS" --compileOptions "-vmspx --data_model=restricted --near_data=globals --opt_for_speed=5 --use_hw_mpy=F5 --include_path=\"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430F5xx_6xx\" --include_path=\"/Volumes/SDrive/CCSV6/ccsv6/ccs_base/msp430/include\" --include_path=\"/Volumes/SDrive/CCSV6/CapStone2Env/PID_462\" --include_path=\"/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/msp430_driverlib_2_70_01_01a/driverlib/MSP430F5xx_6xx\" --include_path=\"/Volumes/SDrive/CCSV6/ccsv6/tools/compiler/ti-cgt-msp430_15.12.1.LTS/include\" --advice:power_severity=suppress -g --define=__MSP430F5529__ --define=ccs --define=MSP430WARE --diag_warning=225 --diag_warning=255 --display_error_number --diag_wrap=off --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --abi=eabi --printf_support=minimal  " "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/compiler.opt: | configPkg/linker.cmd
configPkg/: | configPkg/linker.cmd


