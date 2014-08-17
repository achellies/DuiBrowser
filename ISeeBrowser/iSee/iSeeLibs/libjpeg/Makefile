# Generated automatically from makefile.cfg by configure.
# Makefile for Independent JPEG Group's software

# makefile.cfg is edited by configure to produce a custom Makefile.

# Read installation instructions before saying "make" !!

# For compiling with source and object files in different directories.
srcdir = .
resdir = ./res
VPATH = $(srcdir) $(resdir)

# Where to install the programs and man pages.
prefix = c:/progra~1/Jpeg
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin
libdir = $(exec_prefix)/lib
includedir = $(prefix)/include
binprefix =
manprefix =
manext = 1
mandir = $(prefix)/man/man$(manext)

# The name of your C compiler:
CC= gcc

# You may need to adjust these cc options:
CFLAGS=  -D__GW32__ -D_LARGEFILE_SOURCE=1 -D_LARGEFILE64_SOURCE=1 -D_FILE_OFFSET_BITS=64 \
	-Wall -O3 -mms-bitfields  -fno-exceptions -fomit-frame-pointer -march=i386 -mcpu=i686 \
	-I$(srcdir) -idirafter e:/progra~1/gnuwin32/include -idirafter h:/glibc/include

# Generally, we recommend defining any configuration symbols in jconfig.h,
# NOT via -D switches here.
# However, any special defines for ansi2knr.c may be included here:
ANSI2KNRFLAGS= 

# Link-time cc options:
LDFLAGS=   -Wl,-s -Wl,--enable-auto-import 

# To link any special libraries, add the necessary -l commands here.
LDLIBS=  -Le:/progra~1/gnuwin32/lib -lstdbin -lrle -lintl -liconv -lgw32c -liberty -lwsock32 -lole32 -luuid 

# If using GNU libtool, LIBTOOL references it; if not, LIBTOOL is empty.
LIBTOOL = latool.sh
# $(O) expands to "lo" if using libtool, plain "o" if not.
# Similarly, $(A) expands to "la" or "a".
O = o
A = a
EXE = .exe

# Library version ID; libtool uses this for the shared library version number.
# Note: we suggest this match the macro of the same name in jpeglib.h.
JPEG_LIB_VERSION = 62

# Put here the object file name for the correct system-dependent memory
# manager file.  For Unix this is usually jmemnobs.o, but you may want
# to use jmemansi.o or jmemname.o if you have limited swap space.
SYSDEPMEM= jmemnobs.$(O)

# miscellaneous OS-dependent stuff
SHELL= /bin/sh
# linker
LN= $(CC)
# file deletion command
RM= rm -f
# directory creation command
MKDIR= mkdir
# library (.a) file creation command
AR= ar rc
# second step in .a creation (use "touch" if not needed)
AR2= ranlib
# installation program
INSTALL= /bin/install.exe -cpD
INSTALL_PROGRAM= /bin/install.exe -cpD
INSTALL_LIB= /bin/install.exe -cpD
INSTALL_DATA= /bin/install.exe -cpD

# End of configurable options.


# source files: JPEG library proper
LIBSOURCES= jcapimin.c jcapistd.c jccoefct.c jccolor.c jcdctmgr.c jchuff.c \
        jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c \
        jcphuff.c jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c \
        jdatadst.c jdatasrc.c jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c \
        jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c jdphuff.c \
        jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c \
        jfdctint.c jidctflt.c jidctfst.c jidctint.c jidctred.c jquant1.c \
        jquant2.c jutils.c jmemmgr.c
# memmgr back ends: compile only one of these into a working library
SYSDEPSOURCES= jmemansi.c jmemname.c jmemnobs.c jmemdos.c jmemmac.c
# source files: cjpeg/djpeg/jpegtran applications, also rdjpgcom/wrjpgcom
APPSOURCES= cjpeg.c djpeg.c jpegtran.c rdjpgcom.c wrjpgcom.c cdjpeg.c \
        rdcolmap.c rdswitch.c transupp.c rdppm.c wrppm.c rdgif.c wrgif.c \
        rdtarga.c wrtarga.c rdbmp.c wrbmp.c rdrle.c wrrle.c
SOURCES= $(LIBSOURCES) $(SYSDEPSOURCES) $(APPSOURCES)
# files included by source files
INCLUDES= jchuff.h jdhuff.h jdct.h jerror.h jinclude.h jmemsys.h jmorecfg.h \
        jpegint.h jpeglib.h jversion.h cdjpeg.h cderror.h transupp.h
# documentation, test, and support files
DOCS= README install.doc usage.doc cjpeg.1 djpeg.1 jpegtran.1 rdjpgcom.1 \
        wrjpgcom.1 wizard.doc example.c libjpeg.doc structure.doc \
        coderules.doc filelist.doc change.log
MKFILES= configure makefile.cfg makefile.ansi makefile.unix makefile.bcc \
        makefile.mc6 makefile.dj makefile.wat makefile.vc makelib.ds \
        makeapps.ds makeproj.mac makcjpeg.st makdjpeg.st makljpeg.st \
        maktjpeg.st makefile.manx makefile.sas makefile.mms makefile.vms \
        makvms.opt
CONFIGFILES= jconfig.cfg jconfig.bcc jconfig.mc6 jconfig.dj jconfig.wat \
        jconfig.vc jconfig.mac jconfig.st jconfig.manx jconfig.sas \
        jconfig.vms
CONFIGUREFILES= config.guess config.sub install-sh ltconfig ltmain.sh
OTHERFILES= jconfig.doc ckconfig.c ansi2knr.c ansi2knr.1 jmemdosa.asm
TESTFILES= testorig.jpg testimg.ppm testimg.bmp testimg.jpg testprog.jpg \
        testimgp.jpg
DISTFILES= $(DOCS) $(MKFILES) $(CONFIGFILES) $(SOURCES) $(INCLUDES) \
        $(CONFIGUREFILES) $(OTHERFILES) $(TESTFILES)
# library object files common to compression and decompression
COMOBJECTS= jcomapi.$(O) jutils.$(O) jerror.$(O) jmemmgr.$(O) $(SYSDEPMEM)
# compression library object files
CLIBOBJECTS= jcapimin.$(O) jcapistd.$(O) jctrans.$(O) jcparam.$(O) \
        jdatadst.$(O) jcinit.$(O) jcmaster.$(O) jcmarker.$(O) jcmainct.$(O) \
        jcprepct.$(O) jccoefct.$(O) jccolor.$(O) jcsample.$(O) jchuff.$(O) \
        jcphuff.$(O) jcdctmgr.$(O) jfdctfst.$(O) jfdctflt.$(O) \
        jfdctint.$(O)
# decompression library object files
DLIBOBJECTS= jdapimin.$(O) jdapistd.$(O) jdtrans.$(O) jdatasrc.$(O) \
        jdmaster.$(O) jdinput.$(O) jdmarker.$(O) jdhuff.$(O) jdphuff.$(O) \
        jdmainct.$(O) jdcoefct.$(O) jdpostct.$(O) jddctmgr.$(O) \
        jidctfst.$(O) jidctflt.$(O) jidctint.$(O) jidctred.$(O) \
        jdsample.$(O) jdcolor.$(O) jquant1.$(O) jquant2.$(O) jdmerge.$(O)
# These objectfiles are included in libjpeg.a
LIBOBJECTS= $(CLIBOBJECTS) $(DLIBOBJECTS) $(COMOBJECTS)
DLLOBJECTS= $(LIBOBJECTS) jpeg-dllversion.$(O)
# object files for sample applications (excluding library files)
COBJECTS= cjpeg.$(O) cjpeg-res.$(O) rdppm.$(O) rdgif.$(O) rdtarga.$(O) rdrle.$(O) \
        rdbmp.$(O) rdswitch.$(O) cdjpeg.$(O)
DOBJECTS= djpeg.$(O) djpeg-res.$(O) wrppm.$(O) wrgif.$(O) wrtarga.$(O) wrrle.$(O) \
        wrbmp.$(O) rdcolmap.$(O) cdjpeg.$(O)
TROBJECTS= jpegtran.$(O) jpegtran-res.$(O) rdswitch.$(O) cdjpeg.$(O) transupp.$(O)

$(LIBOBJECTS) : CFLAGS += -DJPEG_DLL

.SUFFIXES:
.SUFFIXES: .lo .la .rc .o .exe .c

all:  libjpeg.la cjpeg djpeg jpegtran rdjpgcom wrjpgcom jpegexiforient

# Rule to make compiled resource (Windows)
.rc.o:
	windres --include-dir $(resdir) -i $< -o $@

# Special compilation rules to support ansi2knr and libtool.

# How to compile with libtool.
.c.lo:
 	$(LIBTOOL) --mode=compile $(CC) $(CFLAGS) -c $(srcdir)/$*.c

.c.o:
	$(CC) $(CFLAGS) -c $(srcdir)/$<

# How to use ansi2knr, when not using libtool.
# .c.o:
# 	./ansi2knr $(srcdir)/$*.c knr/$*.c
# 	$(CC) $(CFLAGS) -c knr/$*.c
# 	$(RM) knr/$*.c

# How to use ansi2knr AND libtool.
# .c.lo:
# 	./ansi2knr $(srcdir)/$*.c knr/$*.c
# 	$(LIBTOOL) --mode=compile $(CC) $(CFLAGS) -c knr/$*.c
# 	$(RM) knr/$*.c

ansi2knr: ansi2knr.c
	$(CC) $(CFLAGS) $(ANSI2KNRFLAGS) -o ansi2knr $(srcdir)/ansi2knr.c
	$(MKDIR) knr

# the library:

# without libtool:
libjpeg.a:  $(LIBOBJECTS)
	$(RM) libjpeg.a
	$(AR) libjpeg.a  $(LIBOBJECTS)
	$(AR2) libjpeg.a

# with libtool:
libjpeg.la:  $(DLLOBJECTS) jpeg-dll-res.rc
	$(LIBTOOL) --mode=link $(CC) -o libjpeg.la $(DLLOBJECTS) \
		-rpath $(libdir) -version-info $(JPEG_LIB_VERSION):0:0
	rm -f jpeg.def
	o2dll -o jpeg.dll -l libjpeg -x $(DLLOBJECTS) -version-info $(JPEG_LIB_VERSION):0:0

# sample programs:

cjpeg: $(COBJECTS) libjpeg.la
	$(LN) $(LDFLAGS) -o cjpeg $(COBJECTS) -L. -ljpeg $(LDLIBS)

djpeg: $(DOBJECTS) libjpeg.la
	$(LN) $(LDFLAGS) -o djpeg $(DOBJECTS) -L. -ljpeg $(LDLIBS)

jpegtran: $(TROBJECTS) libjpeg.la
	$(LN) $(LDFLAGS) -o jpegtran $(TROBJECTS) -L. -ljpeg $(LDLIBS)

rdjpgcom: rdjpgcom.$(O) rdjpgcom-res.$(O)
	$(LN) $(LDFLAGS) -o rdjpgcom rdjpgcom.$(O) rdjpgcom-res.$(O) $(LDLIBS)

wrjpgcom: wrjpgcom.$(O) wrjpgcom-res.$(O)
	$(LN) $(LDFLAGS) -o wrjpgcom wrjpgcom.$(O) wrjpgcom-res.$(O) $(LDLIBS)

jpegexiforient: jpegexiforient.$(O)	jpegexiforient-res.$(O)
	$(LN) $(LDFLAGS) -o jpegexiforient jpegexiforient.$(O) jpegexiforient-res.$(O) $(LDLIBS)

# Installation rules:

install: install-bin install-lib

install-bin: cjpeg djpeg jpegtran rdjpgcom wrjpgcom 
	$(INSTALL_PROGRAM) cjpeg$(EXE) $(bindir)/$(binprefix)cjpeg$(EXE)
	$(INSTALL_PROGRAM) djpeg$(EXE) $(bindir)/$(binprefix)djpeg$(EXE)
	$(INSTALL_PROGRAM) jpegtran$(EXE) $(bindir)/$(binprefix)jpegtran$(EXE)
	$(INSTALL_PROGRAM) rdjpgcom$(EXE) $(bindir)/$(binprefix)rdjpgcom$(EXE)
	$(INSTALL_PROGRAM) wrjpgcom$(EXE) $(bindir)/$(binprefix)wrjpgcom$(EXE)
	$(INSTALL_PROGRAM) exifautotran $(bindir)/$(binprefix)exifautotran
	$(INSTALL_PROGRAM) jpegexiforient$(EXE) $(bindir)/$(binprefix)jpegexiforient$(EXE)
	$(INSTALL_DATA) $(srcdir)/cjpeg.1 $(mandir)/$(manprefix)cjpeg.$(manext)
	$(INSTALL_DATA) $(srcdir)/djpeg.1 $(mandir)/$(manprefix)djpeg.$(manext)
	$(INSTALL_DATA) $(srcdir)/jpegtran.1 $(mandir)/$(manprefix)jpegtran.$(manext)
	$(INSTALL_DATA) $(srcdir)/rdjpgcom.1 $(mandir)/$(manprefix)rdjpgcom.$(manext)
	$(INSTALL_DATA) $(srcdir)/wrjpgcom.1 $(mandir)/$(manprefix)wrjpgcom.$(manext)
	$(INSTALL_DATA) $(srcdir)/jpegexiforient.1 $(mandir)/$(manprefix)jpegexiforient.$(manext)
	$(INSTALL_DATA) $(srcdir)/exifautotran.1 $(mandir)/$(manprefix)exifautotran.$(manext)

install-lib: libjpeg.la install-headers
	$(INSTALL_PROGRAM) jpeg*.dll $(bindir)/$(binprefix)
	$(INSTALL_LIB) libjpeg.la $(libdir)/$(binprefix)libjpeg.la
	$(INSTALL_LIB) libjpeg.dll.a $(libdir)/$(binprefix)libjpeg.dll.a
	$(INSTALL_LIB) jpeg.lib $(libdir)/$(binprefix)jpeg.lib
	$(INSTALL_LIB) jpeg-bcc.lib $(libdir)/$(binprefix)jpeg-bcc.lib
	$(INSTALL_LIB) jpeg.def $(libdir)/$(binprefix)jpeg.def

install-headers: jconfig.h
	$(INSTALL_DATA) jconfig.h $(includedir)/jconfig.h
	$(INSTALL_DATA) $(srcdir)/jpeglib.h $(includedir)/jpeglib.h
	$(INSTALL_DATA) $(srcdir)/jmorecfg.h $(includedir)/jmorecfg.h
	$(INSTALL_DATA) $(srcdir)/jerror.h $(includedir)/jerror.h

clean:
	$(RM) *.o *.lo libjpeg.a libjpeg.la libjpeg.dll.a *.dll *.exe jpeg*.lib
	$(RM) cjpeg$(EXE) djpeg$(EXE) jpegtran$(EXE) rdjpgcom$(EXE) wrjpgcom$(EXE) jpegexiforient$(EXE)
	$(RM) ansi2knr core testout* config.log config.status
	$(RM) -r knr .libs _libs

distclean: clean
	$(RM) Makefile jconfig.h libtool config.cache

test: cjpeg djpeg jpegtran
	$(RM) testout*
	./djpeg -dct int -ppm -outfile testout.ppm  $(srcdir)/testorig.jpg
	./djpeg -dct int -bmp -colors 256 -outfile testout.bmp  $(srcdir)/testorig.jpg
	./cjpeg -dct int -outfile testout.jpg  $(srcdir)/testimg.ppm
	./djpeg -dct int -ppm -outfile testoutp.ppm $(srcdir)/testprog.jpg
	./cjpeg -dct int -progressive -opt -outfile testoutp.jpg $(srcdir)/testimg.ppm
	./jpegtran -outfile testoutt.jpg $(srcdir)/testprog.jpg
	cmp $(srcdir)/testimg.ppm testout.ppm
	cmp $(srcdir)/testimg.bmp testout.bmp
	cmp $(srcdir)/testimg.jpg testout.jpg
	cmp $(srcdir)/testimg.ppm testoutp.ppm
	cmp $(srcdir)/testimgp.jpg testoutp.jpg
	cmp $(srcdir)/testorig.jpg testoutt.jpg

check: test

# Mistake catcher:

jconfig.h: jconfig.doc
	echo You must prepare a system-dependent jconfig.h file.
	echo Please read the installation directions in install.doc.
	exit 1

# GNU Make likes to know which target names are not really files to be made:
.PHONY: all install install-lib install-headers clean distclean test check


jcapimin.$(O): jcapimin.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcapistd.$(O): jcapistd.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jccoefct.$(O): jccoefct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jccolor.$(O): jccolor.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcdctmgr.$(O): jcdctmgr.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jchuff.$(O): jchuff.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jchuff.h
jcinit.$(O): jcinit.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcmainct.$(O): jcmainct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcmarker.$(O): jcmarker.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcmaster.$(O): jcmaster.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcomapi.$(O): jcomapi.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcparam.$(O): jcparam.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcphuff.$(O): jcphuff.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jchuff.h
jcprepct.$(O): jcprepct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jcsample.$(O): jcsample.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jctrans.$(O): jctrans.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdapimin.$(O): jdapimin.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdapistd.$(O): jdapistd.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdatadst.$(O): jdatadst.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h
jdatasrc.$(O): jdatasrc.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h
jdcoefct.$(O): jdcoefct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdcolor.$(O): jdcolor.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jddctmgr.$(O): jddctmgr.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jdhuff.$(O): jdhuff.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdhuff.h
jdinput.$(O): jdinput.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmainct.$(O): jdmainct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmarker.$(O): jdmarker.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmaster.$(O): jdmaster.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdmerge.$(O): jdmerge.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdphuff.$(O): jdphuff.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdhuff.h
jdpostct.$(O): jdpostct.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdsample.$(O): jdsample.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jdtrans.$(O): jdtrans.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jerror.$(O): jerror.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jversion.h jerror.h
jfdctflt.$(O): jfdctflt.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jfdctfst.$(O): jfdctfst.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jfdctint.$(O): jfdctint.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctflt.$(O): jidctflt.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctfst.$(O): jidctfst.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctint.$(O): jidctint.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jidctred.$(O): jidctred.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jdct.h
jquant1.$(O): jquant1.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jquant2.$(O): jquant2.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jutils.$(O): jutils.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h
jmemmgr.$(O): jmemmgr.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemansi.$(O): jmemansi.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemname.$(O): jmemname.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemnobs.$(O): jmemnobs.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemdos.$(O): jmemdos.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
jmemmac.$(O): jmemmac.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h jmemsys.h
cjpeg.$(O): cjpeg.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h jversion.h
djpeg.$(O): djpeg.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h jversion.h
jpegtran.$(O): jpegtran.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h transupp.h jversion.h
rdjpgcom.$(O): rdjpgcom.c jinclude.h jconfig.h
wrjpgcom.$(O): wrjpgcom.c jinclude.h jconfig.h
cdjpeg.$(O): cdjpeg.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdcolmap.$(O): rdcolmap.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdswitch.$(O): rdswitch.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
transupp.$(O): transupp.c jinclude.h jconfig.h jpeglib.h jmorecfg.h jpegint.h jerror.h transupp.h
rdppm.$(O): rdppm.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrppm.$(O): wrppm.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdgif.$(O): rdgif.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrgif.$(O): wrgif.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdtarga.$(O): rdtarga.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrtarga.$(O): wrtarga.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdbmp.$(O): rdbmp.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrbmp.$(O): wrbmp.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
rdrle.$(O): rdrle.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
wrrle.$(O): wrrle.c cdjpeg.h jinclude.h jconfig.h jpeglib.h jmorecfg.h jerror.h cderror.h
