#
#  ======== Makefile ========
#

#
# $(PARTS) contains a list of defined part numbers that for which libs should be
# build for
#

all: $(PART_LIST)

clean:
	@ $(MAKE) -f ./MSP430F5xx_6xx/Makefile clean PART=$@
	@ $(MAKE) -f ./MSP430FR5xx_6xx/Makefile clean PART=$@

#
#  Rules to call the individual family Makefiles
#
######## MSP430F5xx_6xx ########
MSP430F5%:
	@ echo "Building for $@ for the MSP430F5xx_6xx Family"
	@ $(MAKE) -f ./MSP430F5xx_6xx/Makefile PART=$@

MSP430F6%:
	@ echo "Building for $@ for the MSP430F5xx_6xx Family"
	@ $(MAKE) -f ./MSP430F5xx_6xx/Makefile PART=$@

######## MSP430FR5xx_6xx ########
MSP430FR5%:
	@ echo "Building for $@ for the MSP430FR5xx_6xx Family"
	@ $(MAKE) -f ./MSP430FR5xx_6xx/Makefile PART=$@

MSP430FR6%:
	@ echo "Building for $@ for the MSP430FR5xx_6xx Family"
	@ $(MAKE) -f ./MSP430FR5xx_6xx/Makefile PART=$@
