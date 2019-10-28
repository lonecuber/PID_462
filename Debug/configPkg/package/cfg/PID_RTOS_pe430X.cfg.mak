# invoke SourceDir generated makefile for PID_RTOS.pe430X
PID_RTOS.pe430X: .libraries,PID_RTOS.pe430X
.libraries,PID_RTOS.pe430X: package/cfg/PID_RTOS_pe430X.xdl
	$(MAKE) -f /Volumes/SDrive/CCSV6/CapStone2Env/PID_462/src/makefile.libs

clean::
	$(MAKE) -f /Volumes/SDrive/CCSV6/CapStone2Env/PID_462/src/makefile.libs clean

