#!/usr/bin/python2.2

__doc__ = """ Pytessel, Copyright 2002, David Rasch, drasch@users.sourceforge.net, Licensed under the GPL V2. """

import prcreader
import string
import struct
import Image,ImageDraw
import math
import whrandom
import sys

def readUInt16(raw,pos):
    tmp=struct.unpack(">H",raw[pos:pos+2])[0]
    return (tmp,pos+2)

def readInt(raw,pos):
    tmp=struct.unpack(">h",raw[pos:pos+2])[0]
    return (tmp,pos+2)

def readString(raw,pos,n):
    tmp=string.split(raw[pos:pos+n],'\0',1)[0]
    return (tmp,pos+n)

def readDouble(raw,pos):
    return (struct.unpack(">d",raw[pos:pos+8])[0],pos+8)

def readPoint(raw,pos):
    (x,pos)=readDouble(raw,pos)
    (y,pos)=readDouble(raw,pos)
    return ((x,y),pos)

class Figure:
    def read(self,raw,pos):
        (self.name,pos)=readString(raw,pos,32)
        (self.linescount,pos)=readUInt16(raw,pos)
        self.lines=[]
        for i in xrange(self.linescount):
            line=Line()
            pos=line.read(raw,pos)
            self.lines.append(line)
        (self.scale,pos)=readDouble(raw,pos)
        (self.rotinc,pos)=readDouble(raw,pos)

        (self.gridincx,pos)=readInt(raw,pos)
        (self.gridincy,pos)=readInt(raw,pos)
        (self.shiftx,pos)=readInt(raw,pos)
        (self.shifty,pos)=readInt(raw,pos)
        (self.offx,pos)=readInt(raw,pos)
        (self.offy,pos)=readInt(raw,pos)

        return pos

    def fit(self):
        pass
        

    def draw(self,imdraw):
        minx=int(-self.scale * (self.gridincx / 2.0) + self.offx)
        maxx=int(imdraw.im.size[0] + self.scale * (self.gridincx / 2.0))
        miny=int(-self.scale * (self.gridincy / 2.0 + self.offy))
        maxy=int(imdraw.im.size[1] + self.scale * (self.gridincy / 2.0 + self.offy))

        while (miny <= maxy):
            sx = minx;
            sy = miny;

            while (sx <=maxx):
                rot = 0
                while (rot < 2*math.pi):
                    ca=math.cos(rot)*self.scale
                    sa=math.sin(rot)*self.scale

                    for l in self.lines:
                        points=     map(lambda (x,y):((x*ca - y*sa)+sx,(x*sa + y*ca)+sy),l.points)
                        extrapoints=map(lambda (x,y):((x*ca - y*sa)+sx,(x*sa + y*ca)+sy),l.extrapoints)
                        imdraw.point(points,fill=0) # different line algorithm as palm!
                        #imdraw.point(extrapoints,fill=0)
                        imdraw.line(points,fill=0)
                        #imdraw.line(extrapoints,fill=0)

                    rot = rot + self.rotinc

                sx = sx + int(self.scale * self.gridincx)
                sy = sy + int(self.scale * self.shifty)
            minx = minx + int(self.scale * self.shiftx)
            miny = miny + int(self.scale * self.gridincy)

            if (minx >0):
                minx = minx - (self.scale * self.gridincx)
                maxy = maxy - (self.scale * self.shifty)
                    
class Line:
    def read(self,raw,pos):
        (self.vertexcount,pos)=readUInt16(raw,pos)
        self.points=[]
        self.extrapoints=[]
        for i in xrange(self.vertexcount):
             (p1,pos)=readPoint(raw,pos)
             (p2,pos)=readPoint(raw,pos)
             self.points.append(p1)
             self.extrapoints.append(p2)

        (self.ca,pos)=readDouble(raw,pos)
        (self.sa,pos)=readDouble(raw,pos)
        return pos


def pointwhite(im,x,y):
    pixel=im.getpixel((x,y))
    return pixel==(255,255,255)

def floodfill(im,x,y,color):

    ## SLOOOOOOWWW
    (maxx,maxy)=im.size
    pointer=0
    stack=[]

    stack.append((x,y))
    while 1:
        (x,y)=currentPixel=stack[-1]
        del stack[-1]

        im.putpixel(currentPixel,color)
        if x<maxx-1 and pointwhite(im,x+1,y):
            stack.append((x+1,y))

        if y<maxy-1 and pointwhite(im,x,y+1):
            stack.append((x,y+1))

        if x>0 and pointwhite(im,x-1,y):
            stack.append((x-1,y))

        if y>0 and pointwhite(im,x,y-1):
            stack.append((x,y-1))

        if stack==[]:
            return

def fillscreen(im):
    (maxx,maxy)=im.size
    for y in xrange(maxy):
        print y
        for x in xrange(maxx):            
            if pointwhite(im,x,y):
                color=(whrandom.randint(1,255),whrandom.randint(1,255),whrandom.randint(1,255))
                floodfill(im,x,y,color)

if __name__ == '__main__':
    figuredb=prcreader.File("FigureDB.pdb")

    startrec=0
    endrec=len(figuredb)
    if len(sys.argv)>1:        
        items=map(int,string.split(sys.argv[1],'-'))
        if len(items)==1:
            startrec=items[0]
            endrec=startrec+1
        elif len(items)==2:
            (startrec,endrec)=items
            endrec=endrec+1
        else:
            pass
                
    for recordnum in xrange(startrec,endrec):
        
        raw=figuredb.read_record(recordnum)

        figure=Figure()
        pos=figure.read(raw,0)
        #figure.scale=1

        filename= "image%03d.png"%recordnum
        print "Tessellating figure :%s as %s"%(figure.name,filename)

        im=Image.new('RGB',(160,160),(255,255,255))
        imdraw=ImageDraw.Draw(im)

        print "Tessellate"
        figure.draw(imdraw)
        del imdraw

        print "Floodfill"
        fillscreen(im)
        im.save(filename)
        print "OK"
        


    
    

    
