#ifndef __TFRECORD_H__
#define __TFRECORD_H__

#include "tfigure.h"

#define INTSIZE    (sizeof(int))
#define UINT16SIZE (sizeof(UInt16))
#define DOUBLESIZE (sizeof(double))
#define VERTEXSIZE (sizeof(TPoint_type)*2)

void TFigureOpen(void);
void TFigureClose(void);

UInt16 TFigurerecordGetCount(void);
Boolean TFigurerecordNew(TFigure_type * t1);
TFigure_type *TFigurerecordGet(UInt16 i);
void TFigurerecordDelete(UInt16 i);
void TFigurerecordGetName(UInt16 i, char *name);
Boolean TFigurerecordChange(UInt16 i, TFigure_type * t1);

void writePoint(TPoint_type t1, MemHandle p, UInt32 * offset);
void writeDouble(double d1, MemHandle p, UInt32 * offset);
void writeInt(int i1, MemHandle p, UInt32 * offset);
void writeVertex(TVertexnode_type * v1, MemHandle, UInt32 * offset);
void writeLine(TLinenode_type * l1, MemHandle p, UInt32 * offset);
void writeFigure(TFigure_type * t1, MemHandle p, UInt32 * offset);

int readInt(MemHandle p, UInt32 * offset);
UInt16 readUInt16(MemHandle p, UInt32 * offset);
double readDouble(MemHandle p, UInt32 * offset);
TPoint_type readPoint(MemHandle p, UInt32 * offset);
TVertexnode_type *readVertex(MemHandle p, UInt32 * offset);
TLinenode_type *readLine(MemHandle p, UInt32 * offset);
TFigure_type *readFigure(MemHandle p, UInt32 * offset);

UInt32 calculateLineSize(TLinenode_type * l1);
UInt32 calculateFigureSize(TFigure_type * t1);

Err TFigureReceive(ExgSocketPtr socketPtr, Boolean isAppActive);
void TFigurerecordBeam(UInt16 i);
#endif
