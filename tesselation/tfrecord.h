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

/*!
  \brief size of a integer in bytes
*/
#define INTSIZE    (sizeof(int))
/*!
  \brief size of a unsigned integer 16
*/
#define UINT16SIZE (sizeof(UInt16))
/*!
  \brief size of a double
*/
#define DOUBLESIZE (sizeof(double))
/*!
  \brief size of vertex
*/
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
  \return count
*/
UInt16 TFigurerecordGetCount(void);

/*!
  \brief add a new figure to the database
  \param t1 figure
  \return success false if failed to write figure to the database
  
*/
Boolean TFigurerecordNew(TFigure_type * t1);

/*!
  \brief retrieve figure from the database
  \param i index of record in database
  \return figure
*/
TFigure_type *TFigurerecordGet(UInt16 i);

/*!
  \brief delete a record from the database
  \param i index of record in the database
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
  \param i index of record in the database
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

/*!
  \brief write a double to the record
  \param d1 double to write
  \param p address to write
  \param offset to write
*/
void writeDouble(double d1, MemHandle p, UInt32 * offset);

/*!
  \brief write a integer to the record
  \param i1 integer to write
  \param p address to write
  \param offset to write
*/
void writeInt(int i1, MemHandle p, UInt32 * offset);

/*!
  \brief write a vertex to the record
  \param v1 vertex to write
  \param p address to write
  \param offset to write
*/
void writeVertex(TVertexnode_type * v1, MemHandle p, UInt32 * offset);

/*!
  \brief write a line to the record
  \param l1 line to write
  \param p address to write
  \param offset to write
*/
void writeLine(TLinenode_type * l1, MemHandle p, UInt32 * offset);

/*!
  \brief write a figure to the record
  \param t1 figure to write
  \param p address to write
  \param offset to write
*/
void writeFigure(TFigure_type * t1, MemHandle p, UInt32 * offset);

/*!
  \brief read integer from the record
  \param p address to read
  \param offset to read
  \return integer
*/
int readInt(MemHandle p, UInt32 * offset);

/*!
  \brief read unsigned integer16 from the record
  \param p address to read
  \param offset to read
  \return usigned integer16
*/
UInt16 readUInt16(MemHandle p, UInt32 * offset);

/*!
  \brief read double from the record
  \param p address to read
  \param offset to read
*/
double readDouble(MemHandle p, UInt32 * offset);

/*!
  \brief read point from the record
  \param p address to read
  \param offset to read
  \return double
*/
TPoint_type readPoint(MemHandle p, UInt32 * offset);

/*!
  \brief read vertex from the record
  \param p address to read
  \param offset to read
  \return the point
*/
TVertexnode_type *readVertex(MemHandle p, UInt32 * offset);

/*!
  \brief read line from the record
  \param p address to read
  \param offset to read
  \return the line
*/
TLinenode_type *readLine(MemHandle p, UInt32 * offset);

/*!
  \brief read figure from the record
  \param p address to read
  \param offset to read
  \return a figure
*/
TFigure_type *readFigure(MemHandle p, UInt32 * offset);

/*!
  \brief calucate the memory size of the line
  \param l1 line
  \return size of line in bytes
*/  
UInt32 calculateLineSize(TLinenode_type * l1);

/*!
  \brief calucate the memory size of the figure
  \param t1 figure
  \return size of the figure in bytes
*/ 
UInt32 calculateFigureSize(TFigure_type * t1);

/*!
  \brief receive from infrared receive
  \param socketPtr socket
  \param isAppActive is the application active
*/
Err TFigureReceive(ExgSocketPtr socketPtr, Boolean isAppActive);

/*!
  \brief beam figure with infrared
  \param i index of record in database
*/
void TFigurerecordBeam(UInt16 i);
#endif
