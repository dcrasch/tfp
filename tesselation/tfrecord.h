/*
 * Tesselation 
 *
 * Copyright (c) 2001,2002 David Rasch <drasch@users.sourceforge.net>
 * 
 *
 * Licensed under the GNU GPL, version 2 or later 
 *
 * See file "COPYING"
 * that you should have received with this program 
 * 
 * or visit
 * http://www.gnu.org/copyleft/gpl.html 
 *
 * 
 */
/*! \file tfrecord.h
*/

#ifndef __TFRECORD_H__
#define __TFRECORD_H__

#include "tfigure.h"

#define INTSIZE    (sizeof(int))
#define UINT16SIZE (sizeof(UInt16))
#define DOUBLESIZE (sizeof(double))
#define VERTEXSIZE (sizeof(TPoint_type)*2)

/*!
  \brief open the database
*/
void TFigureOpen(void);

/*!
  \brief close the database
*/
void TFigureClose(void);

/*!
  \brief count of the figures in the database
*/
UInt16 TFigurerecordGetCount(void);

/*!
  \brief add a new figure to the database
  \param t1 figure
*/
Boolean TFigurerecordNew(TFigure_type * t1);

/*!
  \brief retrieve figure from the database
  \param i index of record in database
*/
TFigure_type *TFigurerecordGet(UInt16 i);

/*!
  \brief delete a record from the database
  \param in index of record in the database
*/
void TFigurerecordDelete(UInt16 i);

/*!
  \brief get the name of the record
  \param i index of record in the database
  \param name name of record
*/
void TFigurerecordGetName(UInt16 i, char *name);

/*!
  \brief update the record in the database
  \param i ondex of record in the database
  \param t1 figure
*/
Boolean TFigurerecordChange(UInt16 i, TFigure_type * t1);

/*!
  \brief write a point to the record
  \param p1 point to write
  \param p address to write
  \param offset to write
*/
void writePoint(TPoint_type p1, MemHandle p, UInt32 * offset);
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
