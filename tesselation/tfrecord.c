#include <PalmOS.h>
#include "tfrecord.h"

static DmOpenRef s_dbFig = 0;
static UInt16 s_recordNum = 0;

UInt16 TFigurerecordGetCount(void)
{
  if (s_dbFig)
    return DmNumRecords(s_dbFig);
  else
    return 0;
}

Boolean TFigurerecordNew(TFigure_type * t1)
{
  MemHandle hFigure;
  UInt16 uIndex = 0;
  UInt32 offset = 0;

  if (!t1)
    return false;
  hFigure = DmNewRecord(s_dbFig, &uIndex, calculateFigureSize(t1));

  if (hFigure) {
    MemHandle p = (MemHandle) MemHandleLock(hFigure);
    writeFigure(t1, p, &offset);
    MemPtrUnlock(p);
    s_recordNum = 0;
    DmReleaseRecord(s_dbFig, uIndex, true);
    return true;
  } else {
    return false;
  }

}

Boolean TFigurerecordChange(UInt16 i, TFigure_type * t1)
{
  MemHandle hFigure;
  UInt32 offset = 0;

  if (!t1)
    return false;
  hFigure = DmResizeRecord(s_dbFig, i, calculateFigureSize(t1));

  if (hFigure) {
    MemHandle p = (MemHandle) MemHandleLock(hFigure);
    writeFigure(t1, p, &offset);
    MemPtrUnlock(p);
    s_recordNum = 0;
    DmReleaseRecord(s_dbFig, i, true);
    return true;
  } else {
    return false;
  }
}

TFigure_type *TFigurerecordGet(UInt16 i)
{
  TFigure_type *t1;
  MemHandle hFigure;
  UInt32 offset = 0;

  if (!s_dbFig)
    return 0;
  hFigure = (MemHandle) DmQueryRecord(s_dbFig, i);
  if (hFigure) {
    MemHandle p = (MemHandle) MemHandleLock(hFigure);
    if (!p)
      ErrFatalDisplay("Could not get handle");
    t1 = readFigure(p, &offset);
    MemPtrUnlock(p);
    return t1;
  } else
    return 0;
}

void TFigurerecordGetName(UInt16 i, char *name)
{
  MemHandle hFigure;
  hFigure = (MemHandle) DmQueryRecord(s_dbFig, i);
  if (hFigure) {
    MemHandle p = (MemHandle) MemHandleLock(hFigure);
    if (!p)
      ErrFatalDisplay("Could not get handle");
    StrNCopy(name, (char *) p, DESCSIZE);
    MemPtrUnlock(p);
    DmReleaseRecord(s_dbFig, i, false);
  } else {
    ErrFatalDisplay("No such record");
  }
}

void TFigureOpen(void)
{
  ExgRegisterData(TABLES_FILE_CREATOR,exgRegExtensionID,'tsl');
  s_dbFig = DmOpenDatabaseByTypeCreator(TABLES_DB_TYPE,
					TABLES_FILE_CREATOR,
					dmModeReadWrite);
  if (!s_dbFig) {
    DmCreateDatabase(0, TABLES_DB_NAME,
		     TABLES_FILE_CREATOR, TABLES_DB_TYPE, false);

    s_dbFig = DmOpenDatabaseByTypeCreator(TABLES_DB_TYPE,
					  TABLES_FILE_CREATOR,
					  dmModeReadWrite);
    if (!s_dbFig) {
      ErrFatalDisplayIf(true, "Could not open new database");
    }
  }
}

void TFigureClose(void)
{
  if (s_dbFig) {
    DmCloseDatabase(s_dbFig);
    s_dbFig = 0;
  }
  return;
}

void TFigurerecordDelete(UInt16 i)
{
  DmRemoveRecord(s_dbFig, i);
}

void writePoint(TPoint_type p1, MemHandle p, UInt32 * offset)
{
  DmWrite(p, (*offset), &p1, sizeof(TPoint_type));
  (*offset) += sizeof(TPoint_type);
}

void writeDouble(double d1, MemHandle p, UInt32 * offset)
{
  DmWrite(p, (*offset), &d1, sizeof(double));
  (*offset) += sizeof(double);
}

void writeInt(int i1, MemHandle p, UInt32 * offset)
{
  DmWrite(p, (*offset), &i1, sizeof(int));
  (*offset) += sizeof(int);
}

void writeVertex(TVertexnode_type * v1, MemHandle p, UInt32 * offset)
{
  writePoint(v1->p1, p, offset);
  writePoint(v1->p2, p, offset);
}

void writeLine(TLinenode_type * l1, MemHandle p, UInt32 * offset)
{

  TVertexnode_type *v1 = l1->rootnode;
  UInt16 counter = 0;
  UInt32 start = (*offset);

  // skip for the counter
  (*offset) += sizeof(UInt16);
  while (v1) {
    writeVertex(v1, p, offset);
    v1 = v1->next;
    counter++;
  }

  // write counter at the begin
  DmWrite(p, start, &counter, sizeof(UInt16));

  writeDouble(l1->ca, p, offset);
  writeDouble(l1->sa, p, offset);

}

void writeFigure(TFigure_type * t1, MemHandle p, UInt32 * offset)
{

  TLinenode_type *l1 = t1->rootnode;
  UInt16 counter = 0;
  UInt32 counteroffset;

  // to make sure string ends; write zero's
  DmSet(p, (*offset), DESCSIZE, 0);
  DmWrite(p, (*offset), (t1->name), DESCSIZE);
  (*offset) += DESCSIZE;

  counteroffset = (*offset);
  (*offset) += UINT16SIZE;
  while (l1) {
    writeLine(l1, p, offset);
    l1 = l1->next;
    counter++;
  }

  // write counter at the begin
  DmWrite(p, counteroffset, &counter, sizeof(UInt16));
  writeDouble(t1->scale, p, offset);
  writeDouble(t1->rotinc, p, offset);

  writeInt(t1->gridincx, p, offset);
  writeInt(t1->gridincy, p, offset);
  writeInt(t1->shiftx, p, offset);
  writeInt(t1->shifty, p, offset);
  writeInt(t1->offx, p, offset);
  writeInt(t1->offy, p, offset);

}

MemHandle dataRead(MemHandle p, UInt32 offset, UInt32 bytes)
{
  return (MemHandle) ((UInt32) p + offset);
}

int readInt(MemHandle p, UInt32 * offset)
{
  int *i1;
  i1 = (int *) dataRead(p, *offset, sizeof(int));
  (*offset) += sizeof(int);
  return *i1;
}

UInt16 readUInt16(MemHandle p, UInt32 * offset)
{
  UInt16 *i1;
  i1 = (UInt16 *) dataRead(p, *offset, sizeof(UInt16));
  (*offset) += sizeof(UInt16);
  return *i1;
}

double readDouble(MemHandle p, UInt32 * offset)
{
  double *d1;
  d1 = (double *) dataRead(p, *offset, sizeof(double));
  (*offset) += sizeof(double);
  return *d1;
}

TPoint_type readPoint(MemHandle p, UInt32 * offset)
{
  TPoint_type *p1;
  p1 = (TPoint_type *) dataRead(p, *offset, sizeof(TPoint_type));
  (*offset) += sizeof(TPoint_type);
  return *p1;
}

TVertexnode_type *readVertex(MemHandle p, UInt32 * offset)
{
  TVertexnode_type *v1;
  v1 = (TVertexnode_type *) MemPtrNew(sizeof(TVertexnode_type));
  v1->p1 = readPoint(p, offset);
  v1->p2 = readPoint(p, offset);
  v1->next = 0;
  return v1;
}

TLinenode_type *readLine(MemHandle p, UInt32 * offset)
{
  TLinenode_type *l1;
  TVertexnode_type *v1;
  int counter, i;
  l1 = (TLinenode_type *) MemPtrNew(sizeof(TLinenode_type));

  counter = readUInt16(p, offset);
  v1 = readVertex(p, offset);
  l1->rootnode = v1;

  for (i = 1; i < counter; i++) {
    v1->next = readVertex(p, offset);
    v1 = v1->next;
  }

  l1->next = 0;
  l1->ca = readDouble(p, offset);
  l1->sa = readDouble(p, offset);
  return l1;

}

TFigure_type *readFigure(MemHandle p, UInt32 * offset)
{
  TFigure_type *t1;
  TLinenode_type *l1;
  int counter = 0, i;

  t1 = (TFigure_type *) MemPtrNew(sizeof(TFigure_type));

  StrNCopy(t1->name, (char *) ((UInt32) p + (*offset)), DESCSIZE);
  (*offset) += DESCSIZE;

  counter = readUInt16(p, offset);

  l1 = readLine(p, offset);
  t1->rootnode = l1;
  for (i = 1; i < counter; i++) {
    l1->next = readLine(p, offset);
    l1 = l1->next;
  }

  t1->scale = readDouble(p, offset);
  t1->rotinc = readDouble(p, offset);

  t1->gridincx = readInt(p, offset);
  t1->gridincy = readInt(p, offset);
  t1->shiftx = readInt(p, offset);
  t1->shifty = readInt(p, offset);

  t1->offx = readInt(p, offset);
  t1->offy = readInt(p, offset);

  // initialize standard values
  t1->selvertex = NULL;
  t1->sellast = NULL;
  t1->selline = NULL;
  t1->oldp.x = 0;
  t1->oldp.y = 0;

  return t1;
}

UInt32 calculateLineSize(TLinenode_type * l1)
{
  TVertexnode_type *v1 = l1->rootnode;
  UInt32 size = 0;
  while (v1) {
    size += VERTEXSIZE;
    v1 = v1->next;
  }
  size += DOUBLESIZE * 2;	// sa en ca
  size += UINT16SIZE;		// counter
  return size;
}

UInt32 calculateFigureSize(TFigure_type * t1)
{
  TLinenode_type *l1 = t1->rootnode;
  UInt32 size = 0;
  while (l1) {
    size += calculateLineSize(l1);
    l1 = l1->next;
  }

  size += UINT16SIZE;		// counter
  size += DOUBLESIZE * 2;	// scale, rotinc
  size += INTSIZE * 6;		// gridincx,gridincy,shiftx,shifty,offx,offy

  size += DESCSIZE;		// description string
  return size;
}

static Err SendBytes(ExgSocketPtr s, void *buffer, UInt32 bytesToSend)
{
  Err err = errNone;
  
  while (err == errNone && bytesToSend > 0) {
    UInt32 bytesSent = ExgSend(s, buffer, bytesToSend, &err);
    bytesToSend -= bytesSent;
    buffer = ((char *) buffer) + bytesSent;
  }
  return err;
}


void TFigurerecordBeam(UInt16 i)
{
  MemHandle hFigure;
  Char filename[50];
  if (!s_dbFig)
    return 0;
  hFigure = (MemHandle) DmQueryRecord(s_dbFig, i);
  PackedFigure  *thePackedFigure;
  Err       err;
  Char      name[50];
  
  thePackedCustomer = (PackedFigure *) MemHandleLock(theRecord);
  MemSet(&s, sizeof(s), 0);
  StrNCopy(s.description, (char *) p, DESCSIZE);
  StrPrintF(name, "%s%s", exgBeamPrefix, "figure.tsl");
  s.name = name;
  
  err = ExgPut(&s);
  if (err == errNone)
    err = SendBytes(&s, thePackedFigure, MemHandleSize(theRecord));
  MemHandleUnlock(theRecord);
  err = ExgDisconnect(&s, err);
}
