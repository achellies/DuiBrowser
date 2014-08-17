#	Description
#		DirtySock Unix Makefile
#
#	Notes
#		This makefile is written to be used with gnumake.
#       Only tested under Linux.
#
#	Usage
#		make				; build debug library
#		make BUILD=final		; build final library
#		make clean			; clean debug build
#		make clean BUILD=final		; clean final build
#		make dist			; build all versions from clean
#
#	Copyright
#		Copyright (c) Electronic Arts 2003. ALL RIGHTS RESERVED.
#

#--------------------------------------------------------------------------
# Archive Target and Objects List
#--------------------------------------------------------------------------

# base of directory structure
BASE = ../../..

BUILD=debug
TARGET_EXT_debug=d
TARGET_EXT_final=f
TARGET_EXT_profile=p
TARGET_EXT=$(TARGET_EXT_$(BUILD))


# source files (VPATH relative)
# The are ordered alphabetically by the directory in source that they
# belong to and then alphabetically within that collection.

SRC = plat-str.c plat-time.c \
	buddyapi.c hlbudapi.c \
	commsrp.c commtcp.c commudp.c \
	cryptarc4.c cryptmd2.c cryptmd5.c cryptrsa.c cryptsha1.c cryptssc2.c cryptstp1.c \
	dirtyaddr.c dirtyerrunix.c dirtylib.c dirtylibunix.c \
	dirtymem.c dirtynames.c dirtynet.c dirtynetunix.c \
	netconn.c netconnunix.c \
	connapi.c netgamedist.c netgamedistserv.c netgamelink.c netgameutil.c \
	dirtygif.c dirtygraph.c dirtyjpg.c \
	lobbylan.c porttestapi.c porttestpkt.c telemetryapi.c tickerapi.c \
	protoadvt.c protoaries.c protofilter.c protohttp.c protomangle.c \
	protoname.c protoping.c protopingmanager.c protossl.c \
	protostream.c prototunnel.c protoudp.c protoupnp.c \
	qosapi.c \
	lobbybase64.c lobbydisplist.c lobbyhasher.c lobbysort.c \
	lobbytagfield.c lobbyutf8.c \
	voipchannel.c voipcodec.c voipconnection.c voipdvi.c voipgroup.c \
	voipmixer.c voippcm.c voiptunnel.c voipstub.c \
	digobjapi.c lockerapi.c netresource.c weboffer.c \
	xmlformat.c xmllist.c xmlparse.c

OBJDIR = $(BUILD)

OBJS = $(SRC:%.c=$(OBJDIR)/%.o)
DEPS = $(SRC:%.c=$(OBJDIR)/%.d)

TARGET = dirtysock

EMPTY:=
SPACE:= $(EMPTY) $(EMPTY)

#--------------------------------------------------------------------------
# Defines
#--------------------------------------------------------------------------

TARGETARCHIVE = $(BASE)/library/unix/lib$(TARGET)$(TARGET_EXT).a

#--------------------------------------------------------------------------
# Command Variables
#--------------------------------------------------------------------------

# which shell for invoke
SHELL       = /bin/sh
# what to use for assembly
# what to use to remove files
RM          = rm -f

#--------------------------------------------------------------------------
# Paths
#--------------------------------------------------------------------------

VPATH = $(BASE)/platform \
	$(BASE)/contrib/dirtystrings/source/common/lobbylogin \
	$(BASE)/core/source/dirtysock/unix \
	$(BASE)/core/source/buddy \
	$(BASE)/core/source/comm \
	$(BASE)/core/source/crypt \
	$(BASE)/core/source/dirtysock \
	$(BASE)/core/source/lobby \
	$(BASE)/core/source/game \
	$(BASE)/core/source/graph \
	$(BASE)/core/source/misc \
	$(BASE)/core/source/pogo \
	$(BASE)/core/source/proto \
	$(BASE)/core/source/util \
	$(BASE)/core/source/voip \
	$(BASE)/core/source/voip/stub \
	$(BASE)/core/source/web \
	$(BASE)/core/source/xml \

#--------------------------------------------------------------------------
# Compiler flags, defines, and includes
#--------------------------------------------------------------------------

#
# dirtynet.h was written assuming only two targets: PS2 and Windows.
# If _WINSOCKAPI_ is not defined then things like sockaddr_in are
# defined inline.  The correct thing to do is wrap them in 
# #ifndef HAVE_SOCKADDR or put them in a separate header file.  However,
# to minimise changes we'll like and say that Unix follows _WINSOCKAPI_
#
CFLAGS = -std=gnu99 -Dstricmp=strcasecmp -D_GNU_SOURCE -D_WINSOCKAPI_ -DDIRTYSOCK_CORE_ONLY -DLINUX -DDS_PLATFORM

GCC_MAJOR_VERSION := $(shell gcc -v 2>&1 | tail -1 | awk -F' ' '{print $$3}' | awk -F. '{print $$1}')

ifeq ($(GCC_MAJOR_VERSION),4)
    EXTRA_WARNFLAGS := -Wno-pointer-sign
endif

WARNFLAGS = -Wall -Wsign-compare -Wpointer-arith -Werror -Wno-multichar $(EXTRA_WARNFLAGS)

CODEDEFINES_COMMON  = -DDIRTY_HAVE_SOCKET_HEADER -DS_PLATFORM -DDIRTYCODE_PLATFORM=11
CODEDEFINES_DEBUG   = -DDIRTYCODE_DEBUG=1 $(CODEDEFINES_COMMON)
CODEDEFINES_FINAL   = -DDIRTYCODE_DEBUG=0 $(CODEDEFINES_COMMON)

CFLAGS_debug    = -g -O0 $(CFLAGS) $(WARNFLAGS) $(CODEDEFINES_DEBUG)
CFLAGS_final    = -g -O2 $(CFLAGS) $(WARNFLAGS) $(CODEDEFINES_FINAL)
CFLAGS_profile	= -pg $(CFLAGS_final)

INCLUDE_DIRS= $(BASE)/platform $(BASE)/core/include $(BASE)/core/include/unix $(BASE)/contrib/dirtystrings/include/unix
INCLUDES	= $(INCLUDE_DIRS:%=-I%)


#--------------------------------------------------------------------------
# Rules Section
#--------------------------------------------------------------------------

.PHONY: all build

all: build

build: $(OBJDIR) $(TARGETARCHIVE)

$(TARGETARCHIVE): $(OBJS)
	@echo Building archive $(TARGETARCHIVE)
	@echo Building archive $(TARGETARCHIVE) >>$(TARGET).err
	@test -f $(TARGETARCHIVE) || mkdir -p `dirname $(TARGETARCHIVE)`
	@$(AR) r $(TARGETARCHIVE) $^

$(OBJDIR)/%.o: %.c
	@echo Compiling file $<
	@echo Compiling file $< >> $(TARGET).err
	@$(CC) -c $(CFLAGS_$(BUILD)) $(INCLUDES) $< -o $@ >>$(TARGET).err

$(OBJDIR)/%.d: %.c
	@echo Generating dependencies for $<
	@test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	@awk -f mkdepend.awk input=$< dependency_file=$@ object_file=$(OBJDIR)/`basename $< .c`.o include_path=.:$(subst $(SPACE),:,$(INCLUDE_DIRS)) < /dev/null > $@


.PHONY: clean dist
clean:
	@echo Deleting object files and target library
	@echo Deleting object files and target library >>$(TARGET).err
	@$(RM) core *.err $(OBJDIR)/*.o $(TARGETARCHIVE)

dist:
	@$(MAKE) BUILD=debug clean 
	@$(MAKE) BUILD=debug
	@$(MAKE) BUILD=final clean
	@$(MAKE) BUILD=final
	@$(MAKE) BUILD=profile clean
	@$(MAKE) BUILD=profile

-include $(DEPS)
