# $Id: Makefile,v 1.11 2012-06-18 18:20:06 sverrehu Exp $
TARGETS		= mancala xmancala
DIST		= mancala
VERMAJ		= 1
VERMIN		= 0
VERPAT		= 3
VERSION		= $(VERMAJ).$(VERMIN).$(VERPAT)

CC		= gcc

# Common directories and libraries
INCDIR		= -I.
LIBDIR		= -L.
LIBS		= 

# Directories and libraries for X, Xpm and XForms.
# If you don't have Xpm, you'll need to link with a static version of XForms.
XINCDIR		= -I/usr/include/X11 -I/opt/local/include
XLIBDIR		= -L/usr/X11R6/lib -L/opt/local/lib
XLIBS		= -lforms

OPTIM		= -O3 -fomit-frame-pointer
CCOPT		= -Wall $(OPTIM) $(INCDIR) $(XINCDIR) -DVERSION=\"$(VERSION)\"
LDOPT		= -s $(LIBDIR)

# Object files common to all programs.
OBJS		= minimax.o mancala.o

# Object files used by xmancala
XSRCS		= xform.c rulestxt.c
XOBJS		= $(XSRCS:.c=.o)


all: $(TARGETS)

%.c %.h: %.fd
	fdesign -convert $<

mancala: textmain.o $(OBJS)
	$(CC) $(CCOPT) -o $@ textmain.o $(OBJS) $(LDOPT) $(LIBS)

xmancala: $(XOBJS) xmain.o $(OBJS)
	$(CC) $(CCOPT) -o $@ xmain.o $(XOBJS) $(OBJS) \
		$(LDOPT) $(XLIBDIR) $(XLIBS)

.c.o:
	$(CC) -o $@ -c $(CCOPT) $<

clean:
	rm -f *.o core depend *~  xform.c xform.h

veryclean: clean
	rm -f $(TARGETS) $(DIST)-$(VERSION).tar.gz

chmod:
	chmod a+r *

depend dep: xform.h
	$(CC) $(INCDIR) $(XINCDIR) -MM *.c >depend

# To let the authors make a distribution. The rest of the Makefile
# should be used by the authors only.
LSMFILE		= $(DIST)-$(VERSION).lsm
DISTDIR		= $(DIST)-$(VERSION)
DISTFILE	= $(DIST)-$(VERSION).tar.gz
DISTFILES	= README INSTALL RULES $(LSMFILE) ChangeLog NEWS \
		  Makefile Makefile.bcc \
		  mancala.c mancala.h minimax.c minimax.h \
		  textmain.c \
		  xform.fd xmain.c rulestxt.c mancala.6 mancala-text

$(LSMFILE): FORCE
	VER=$(VERSION); \
	DATE=`date "+%d%b%y"|tr '[a-z]' '[A-Z]'`; \
	sed -e "s/VER/$$VER/g;s/DATE/$$DATE/g" $(DIST).lsm.in > $(LSMFILE)

FORCE:

# Warning: distclean removes the lsm-file, which can not be
# regenerated without the lsm.in-file, which is not part of the package.
distclean: veryclean
	rm -f $(LSMFILE)

dist: $(LSMFILE) chmod
	mkdir $(DISTDIR)
	chmod a+rx $(DISTDIR)
	ln $(DISTFILES) $(DISTDIR)
	tar -cvzf $(DISTFILE) $(DISTDIR)
	chmod a+r $(DISTFILE)
	rm -rf $(DISTDIR)

ifeq (depend,$(wildcard depend))
include depend
endif
