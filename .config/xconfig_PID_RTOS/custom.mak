## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e430X linker.cmd package/cfg/PID_RTOS_pe430X.oe430X

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/PID_RTOS_pe430X.xdl
	$(SED) 's"^\"\(package/cfg/PID_RTOS_pe430Xcfg.cmd\)\"$""\"/Volumes/SDrive/CCSV6/CapStone2Env/PID_462/.config/xconfig_PID_RTOS/\1\""' package/cfg/PID_RTOS_pe430X.xdl > $@
	-$(SETDATE) -r:max package/cfg/PID_RTOS_pe430X.h compiler.opt compiler.opt.defs
