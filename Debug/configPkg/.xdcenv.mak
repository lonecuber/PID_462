#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/packages;/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages;/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages;/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages
override XDCROOT = /Volumes/SDrive/CCSV6/xdctools_3_32_00_06_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/packages;/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/bios_6_46_00_23/packages;/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/tidrivers_msp43x_2_20_00_08/packages;/Volumes/SDrive/CCSV6/tirtos_msp43x_2_20_00_06/products/uia_2_00_06_52/packages;/Volumes/SDrive/CCSV6/xdctools_3_32_00_06_core/packages;..
HOSTOS = MacOS
endif
