## Makefile for Tesselation
#/*
# * Tesselation 
# *
# * Copyright (c) 2001,2002 David Rasch <drasch@users.sourceforge.net>
# * 
# *
# * Licensed under the GNU GPL, version 2 or later 
# *
# * See file "COPYING"
# * that you should have received with this program 
# * 
# * or visit
# * http://www.gnu.org/copyleft/gpl.html 
# *
# * 
# */

CC	 = m68k-palmos-g++
CFLAGS	 = -Wall -O2 -pipe
PILRC	 = pilrc
OBJRES	 = m68k-palmos-obj-res
NM	 = m68k-palmos-nm
BUILDPRC = build-prc

TARGET  = tesselation
APPNAME = "Tessellation"
APPID   = "DCRP"
RESDIR  = .

SRC = tpoint.c tvertex.c tline.c tfigure.c tfill.c TinyTrig.c tfrecord.c tesselation.c tesselation_main.c tesselation_add.c tesselation_edit.c tesselation_util.c tesselation_rename.c

OBJS = $(SRC:.c=.o)
BITMAPS = $(RESDIR)/$(TARGET).pbm
RESOURCE = $(RESDIR)/$(TARGET).rcp

all: $(TARGET).prc

.c.o:
	$(CC) $(CFLAGS) -c $<

$(TARGET).prc: resources.tag $(TARGET)
	! $(NM) -u $(TARGET) | grep .
	$(OBJRES) $(TARGET)
	$(BUILDPRC) --backup $(TARGET).prc $(APPNAME) $(APPID) *.grc *.bin

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(TARGET)_resource.h: resources.tag

resources.tag: $(RESOURCE) $(BITMAPS)
	$(PILRC) -q -I resource -H $(TARGET)_resource.h $(RESOURCE)
	touch resources.tag

clean:
	rm -rf *.o *~ $(TARGET) *.bin *.grc resources.tag $(TARGET)_resource.h

distclean: clean
	-rm -f $(TARGET).prc

docs:
	doxygen

depends:
	makedepend -Y../common $(SRC) >/dev/null 2>&1