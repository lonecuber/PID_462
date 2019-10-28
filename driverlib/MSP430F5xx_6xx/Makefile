#
#  ======== Makefile ========
#

ROOT=..
FAMILYDIR=MSP430F5xx_6xx

######## YOU SHOULDN'T HAVE TO MODIFY ANYTHING BELOW HERE ########

ifndef PART
$(error "PART is not defined!!")
else
include $(ROOT)/makedefs
include $(ROOT)/iar_makedefs
endif

# Add the family path to compiler search path
CFLAGS+=${patsubst %,-I%,${subst :, ,$(FAMILYDIR)}}

# Targets to create directory and the library
all: mklib
all: $(FAMILYDIR)/$(COMPILER)/$(PART)$(AREXT)
all: clean-objects

clean:
	@$(RMDIR) "$(FAMILYDIR)/ccs"
	@$(RMDIR) "$(FAMILYDIR)/iar"

# Find all the source files that need to be compiled into a library
OBJECTS=$(patsubst %.c,%$(OBJEXT),                   \
          $(subst $(FAMILYDIR),$(FAMILYDIR)/$(COMPILER)/$(FAMILYDIR), \
            $(wildcard $(FAMILYDIR)/*.c)))

# Create target directory
mklib:
	@echo "making library directory"
	@$(MKDIR) "$(FAMILYDIR)/$(COMPILER)/$(FAMILYDIR)"

clean-objects:
	@echo "removing the object files"
	@$(RMDIR) "$(FAMILYDIR)/$(COMPILER)/$(FAMILYDIR)"

# Include the list of source (object) files for the library
$(FAMILYDIR)/$(COMPILER)/$(PART)$(AREXT): $(OBJECTS)
