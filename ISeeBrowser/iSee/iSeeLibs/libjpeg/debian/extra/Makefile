CFLAGS = -O2 -Wall -g
CC = cc
INSTALL = install -m755 -o root -g root
DESTDIR =
prefix  = /usr/local
bindir  = $(prefix)/bin
mandir  = $(prefix)/share/man/man1

all: jpegexiforient

jpegexiforient: jpegexiforient.c
	$(CC) $(CFLAGS) -o jpegexiforient jpegexiforient.c
clean:
	-rm -f jpegexiforient
install:
	$(INSTALL) jpegexiforient    $(DESTDIR)$(bindir)
	$(INSTALL) jpegexiforient.1  $(DESTDIR)$(mandir)
	$(INSTALL) exifautotran      $(DESTDIR)$(bindir)
	$(INSTALL) exifautotran.1    $(DESTDIR)$(mandir)
