#!/usr/bin/python2
#  $Id$
#
#  Copyright 2001 Rob Tillotson <rob@pyrite.org>
#  All Rights Reserved
#
#  Permission to use, copy, modify, and distribute this software and
#  its documentation for any purpose and without fee or royalty is
#  hereby granted, provided that the above copyright notice appear in
#  all copies and that both the copyright notice and this permission
#  notice appear in supporting documentation or portions thereof,
#  including modifications, that you you make.
#
#  THE AUTHOR ROB TILLOTSON DISCLAIMS ALL WARRANTIES WITH REGARD TO
#  THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
#  AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
#  SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
#  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
#  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
#  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE!
#
"""Read-only interface to Palm database files.

This module allows Python programs to read Palm database files in the
standard prc/pdb format.  Unlike the 'prc' module it is derived from,
this module does not read the entire database into memory at once,
instead reading each record only as it is needed (and not holding on
to any unnecessary references).  This should make this module more
suitable for applications running on handhelds or other limited
hardware, but it requires that the database file be kept open for
random access.
"""

__version__ = '$Id$'

__author__ = 'Rob Tillotson <rob@pyrite.org>'

__copyright__ = 'Copyright 2001 Rob Tillotson <rob@pyrite.org>'

import sys, string, struct

PI_HDR_SIZE = 78
PI_RESOURCE_ENT_SIZE = 10
PI_RECORD_ENT_SIZE = 8
PILOT_TIME_DELTA = 2082844800L

flagResource = 0x0001
flagReadOnly = 0x0002
flagAppInfoDirty = 0x0004
flagBackup = 0x0008
flagClipping = 0x2000
flagOpen = 0x8000
# 2.x
flagNewer = 0x0010
flagReset = 0x0020
# 3.x
flagCopyPrevention = 0x0040
flagStream = 0x0080
#
flagExcludeFromSync = 0x0080

attrDeleted = 0x80
attrDirty = 0x40
attrBusy = 0x20
attrSecret = 0x10

class Record:
    """A record read from a prc/pdb file.
    This class is meant to be somewhat string-like; len(), [], [:],
    and string methods (in Python >=2.x) work as expected on the
    contents of the record, and the other information is available
    as attributes of the object.

    This class is also meant to be subclassable by programs that
    want a different interface to data read from a prc/pdb file;
    the record_class attribute of the File class can be used to
    control that.
    """
    def __init__(self, attr, uid, data):
        self.attr = attr
        self.deleted = not not attr & attrDeleted
        self.dirty = not not attr & attrDirty
        self.busy = not not attr & attrBusy
        self.secret = not not attr & attrSecret
        self.category = attr & 0x0f
        self.id = uid
        self.data = data
    def __len__(self): return len(self.data)
    def __str__(self): return str(self.data)
    # __cmp__/__rcmp__/__hash__?
    def __getitem__(self,i): return self.data[i]
    def __getslice__(self,i,j): return self.data[i:j]
    def __getattr__(self, k): # pick up 2.0 string methods
        return getattr(self.data, k)

class Resource:
    """A resource read from a prc/pdb file.
    This class is meant to be somewhat string-like; len(), [], [:],
    and string methods (in Python >=2.x) work as expected on the
    contents of the record, and the other information is available
    as attributes of the object.

    This class is also meant to be subclassable by programs that
    want a different interface to data read from a prc/pdb file;
    the resource_class attribute of the File class can be used to
    control that.
    """
    def __init__(self, typ, id, data):
        self.type = typ
        self.id = id
        self.data = data
    def __len__(self): return len(self.data)
    def __str__(self): return str(self.data)
    # __cmp__/__rcmp__/__hash__?
    def __getitem__(self,i): return self.data[i]
    def __getslice__(self,i,j): return self.data[i:j]
    def __getattr__(self, k): # pick up 2.0 string methods
        return getattr(self.data, k)        

class File:
    """A prc/pdb file.

    This class provides read-only access to a Palm database in prc/pdb
    format.  The single argument to the constructor is either a filename
    or a file-like object, which must be seekable.

    Reading of record contents will be deferred as long as possible to
    save memory.  The only data retained by the File object is the
    database header information (so it can be available as attributes)
    and the offset/attribute table (which is stored raw, to avoid creating
    lots of objects when opening a database).  Thus, if the application
    does not hold references to records it is not actually using, memory
    usage should be fairly minimal even for very large databases.
    """
    record_class = Record
    resource_class = Resource
    def __init__(self, name):
        if type(name) == type(''): self.f = open(name,'rb')
        else: self.f = name

        self.read_header()

    def read_header(self, seek=None):
        if seek is not None: self.f.seek(0)
        data = self.f.read(PI_HDR_SIZE)
        if len(data) < PI_HDR_SIZE: raise IOError, "file too short"
        (name, flags, ver, ctime, mtime, btime, mnum, appinfo, sortinfo,
	 typ, creator, uid, nextrec, numrec) \
	 = struct.unpack('>32shhLLLlll4s4sllh', data[:PI_HDR_SIZE])

	if nextrec or appinfo < 0 or sortinfo < 0 or numrec < 0:
	    raise IOError, "invalid database header"

        self.name = string.split(name,'\0')[0],
        self.type = typ
        self.creator = creator
        self.create_date = ctime - PILOT_TIME_DELTA
        self.modify_date = mtime - PILOT_TIME_DELTA
        self.backup_date = btime - PILOT_TIME_DELTA
        self.modnum = mnum
        self.version = ver
        self.f_reset = flags & flagReset
        self.f_resource = flags & flagResource
        self.f_newer = flags & flagNewer
        self.f_appinfo_dirty = flags & flagAppInfoDirty
        self.f_read_only = flags & flagReadOnly
        self.f_backup = flags & flagBackup
        self.f_clipping = flags & flagClipping
        self.f_copy_prevent = flags & flagCopyPrevention
        self.f_stream = flags & flagStream
        self.f_open = flags & flagOpen

        if self.f_resource: s = PI_RESOURCE_ENT_SIZE
        else: s = PI_RECORD_ENT_SIZE

        # Rather than decoding the entries into a list of tuples
        # here, we will do that at the time a record is read.
        # This means that memory consumption will be only
        # (records*8) or (records*10) bytes per record, instead
        # of the size of four Python objects.  On huge databases
        # (multiple megabytes) this could make a significant
        # difference.
        self.__entries = self.f.read(s*numrec)
        self.__numrec = numrec
        if len(self.__entries) < s*numrec:
            raise IOError, "bad database header"

        self.f.seek(0,2)
        self.__filelen = self.f.tell()
        self.__appinfo = appinfo
        self.__sortinfo = sortinfo
        if self.__numrec:
            if self.f_resource: self.__recbegin = self.get_resource_entry(0)[2]
            else: self.__recbegin = self.get_record_entry(0)[0]

    def get_resource_entry(self, i):
        o = i*PI_RESOURCE_ENT_SIZE
        return struct.unpack('>4shl', self.__entries[o:o+PI_RESOURCE_ENT_SIZE])
    def get_record_entry(self, i):
        o = i*PI_RECORD_ENT_SIZE
        return struct.unpack('>ll', self.__entries[o:o+PI_RECORD_ENT_SIZE])
    
    def read_record(self, i):
        if i >= self.__numrec or i < 0: raise IndexError,i
        if self.f_resource:
            typ, id, offset = self.get_resource_entry(i)
            if i+1 >= self.__numrec: rlen = self.__filelen - offset
            else: rlen = self.get_resource_entry(i+1)[2] - offset
            self.f.seek(offset)
            return self.resource_class(typ, id, self.f.read(rlen))
        else:
            offset, auid = self.get_record_entry(i)
            if i+1 >= self.__numrec: rlen = self.__filelen - offset
            else: rlen = self.get_record_entry(i+1)[0] - offset
            attr = (auid & 0xff000000) >> 24
            uid = auid & 0x00ffffff
            self.f.seek(offset)
            return self.record_class(attr, uid, self.f.read(rlen))

    def read_appinfo(self):
        if not self.__appinfo: return ''
        if self.__sortinfo: rlen = self.__sortinfo - self.__appinfo
        elif self.__numrec: rlen = self.__recbegin - self.__appinfo
        else: rlen = self.__filelen - self.__appinfo
        self.f.seek(self.__appinfo)
        return self.f.read(rlen)

    def read_sortinfo(self):
        if not self.__sortinfo: return ''
        if self.__numrec: rlen = self.__recbegin - self.__sortinfo
        else: rlen = self.__filelen - self.__sortinfo
        self.f.seek(self.__sortinfo)
        return self.f.read(rlen)
    
    def __len__(self): return self.__numrec
    def __getitem__(self, i):
        if i < 0: i = self.__numrec + i
        return self.read_record(i)
    def __getattr__(self, k):
        if k == 'appinfo': return self.read_appinfo()
        elif k == 'sortinfo': return self.read_sortinfo()
        else: raise AttributeError, k
    def __getslice__(self, low, high):
        if high == sys.maxint: high = self.__numrec  # empty end index
        if low >= self.__numrec: raise IndexError, low
        if high > self.__numrec: raise IndexError, high
        return Slice(self, low, high)

class Slice:
    """A slice of a prc/pdb database.
    Applications should never create one of these objects directly;
    they should only be created by slicing a File object.
    """
    def __init__(self, db, low, high):
        self.db = db
        self.low = low
        self.high = high

    def __len__(self): return high-low
    def __getitem__(self, i):
        if i >= self.high-self.low: raise IndexError, i
        return self.db[i+self.low]
    def __getslice__(self, low, high):
        if high == sys.maxint: high = self.low+self.high
        return self.__class__(self.db, self.low+low, self.low+high)
