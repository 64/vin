#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "gapbuffer.h"

GapBuffer::GapBuffer(int gsize) : GAP_SIZE(gsize)
{
    InitBuffer(GAP_SIZE);
};


GapBuffer::GapBuffer(FILE *file, int gsize) : GAP_SIZE(gsize)
{
    struct stat buf;
    
    fstat(fileno(file), &buf);
    long file_size = buf.st_size;
    InitBuffer(file_size + GAP_SIZE);    
    MoveGapToPoint();
    ExpandGap( (int)file_size );
    unsigned int amount = fread(gapstart, 1, file_size, file);

    gapstart += amount;
};

GapBuffer::GapBuffer(const GapBuffer& tb)
{
     GAP_SIZE = tb.GAP_SIZE;
     
     buffer = (char *) malloc(tb.bufend - tb.buffer);
     
     strcpy(buffer, tb.buffer);

     bufend = buffer + (tb.bufend - tb.buffer);
     gapstart = buffer + (tb.gapstart - tb.buffer);
     gapend = gapstart + (tb.gapend - tb.gapstart);
     point = buffer + (tb.point - tb.buffer);
}


GapBuffer::~GapBuffer()
{

    if (buffer)
        free(buffer);
};

int GapBuffer::CopyBytes(char *destination, char *source, unsigned int length)
{

    if ( (destination == source) || (length == 0) )
        return 1;

    // if we're moving the character toward the front of the buffer
    if (source > destination)
    {
        // check to make sure that we don't go beyond the buffer
        if ( (source + length) >= bufend ) {
            return 0;
        }

        for (; length > 0; length--)
            *(destination++) = *(source++);

    }
    else
    {
        // To prevent overwriting characters we still
        // need to move, go to the back and copy forward.
        source += length;
        destination += length;

        for (; length > 0; length--)
            // decrement first 'cause we start one byte beyond where we want
            *(--destination) = *(--source); 
    }

    return 1;
}

void GapBuffer::ExpandBuffer(unsigned int size)
{   


    if (((bufend - buffer) + size) > BufferSize())
    {

        char *origbuffer = buffer;

        int NewBufferSize = (bufend - buffer) + size  + GAP_SIZE;
        
        buffer = (char *) realloc(buffer, NewBufferSize);

        point += buffer - origbuffer;
        bufend += buffer - origbuffer;
        gapstart += buffer - origbuffer;
        gapend += buffer - origbuffer;
    }

}


void GapBuffer::MoveGapToPoint()
{
    if (point == gapstart) {
        return;
    }

    if (point == gapend) {
        point = gapstart;
        return;
    }

    // Move gap towards the left 
    if (point < gapstart) {
        // Move the point over by gapsize.        
        CopyBytes(point + (gapend-gapstart), point, gapstart - point);
        gapend -= (gapstart - point);
        gapstart = point;
    } else {
        // Since point is after the gap, find distance
        // between gapend and point and that's how
        // much we move from gapend to gapstart.
        CopyBytes(gapstart, gapend, point - gapend);
        gapstart += point - gapend;
        gapend = point;
        point = gapstart;
    }
}

void GapBuffer::ExpandGap(unsigned int size)
{
    if (size > SizeOfGap()) {
        size += GAP_SIZE;
        ExpandBuffer(size);
        CopyBytes(gapend+size, gapend, bufend - gapend);

        gapend += size;
        bufend += size;
    }
}

void GapBuffer::SetPoint(unsigned int offset)
{
    point = buffer + offset;

    if (point > gapstart) {
        point += gapend - gapstart;
    }
}

int GapBuffer::SizeOfGap()
{
    return gapend - gapstart;
}

unsigned int GapBuffer::PointOffset()
{

    if (point > gapend)
        return ((point - buffer) - (gapend - gapstart));
    else
        return (point - buffer);
}

char GapBuffer::GetChar()
{
    if (point == gapstart)
        point = gapend;

    return *point;
}

char GapBuffer::PreviousChar()
{
    if (point == gapend)
        point = gapstart;

    return *(--point);
}

void GapBuffer::ReplaceChar(char ch)
{

    if (point == gapstart) {
        point = gapend;
    }

    if (point == bufend) {
        ExpandBuffer(1);
        bufend++;
    }

    *point = ch;
}

char GapBuffer::NextChar()
{   
    // point should not be in the gap.
    if (point == gapstart)
    {
        point = gapend;
        return *point;
    } 

    return *(++point);    

}

void GapBuffer::PutChar(char ch)
{
    InsertChar(ch);
    *point++;
}

void GapBuffer::InsertChar(char ch)
{
    if (point != gapstart)
        MoveGapToPoint();

    if (gapstart == gapend)
        ExpandGap(1);

    *(gapstart++) = ch;
}

void GapBuffer::DeleteChars(unsigned int size)
{

    if (point != gapstart)
        MoveGapToPoint();

    // We shifted the gap so that gapend points to the location
    // where we want to start deleting so extend it 
    // to cover all the characters.
    gapend += size;
}

void GapBuffer::InsertString(char *string, unsigned int length)
{
    MoveGapToPoint();

    if (length > SizeOfGap())
        ExpandGap(length);

    do
        PutChar(*(string++));
    while (length--);
}

int GapBuffer::InitBuffer(unsigned int size)
{

    if (buffer)
        free(buffer);

    buffer = (char *) malloc(size);

    if (!buffer)
        return 0;

    point = buffer;
    gapstart = buffer;
    
    gapend = buffer + size;     
    bufend = gapend;

    return 1;

}

int GapBuffer::BufferSize()
{
    return (bufend - buffer) - (gapend - gapstart);
}

void GapBuffer::PrintBuffer()
{
    /*
    char ch;

    cout << "Printing the buffer: " << endl;
    SetPoint(0);
    while (point < bufend) {
        cout << GetCharMovePoint();        
    }

    cout << "Printing the buffer in reverse: " << endl;

    while (point >= buffer) {
        cout << GetPrevCharMovePoint();
    }
    */

    char *temp = buffer;


    while (temp < bufend) {

        if ((temp >= gapstart) && (temp < gapend))
        {
            std::cout << "_";
            temp++;
        }
        else
        {
            std::cout << *(temp++);
        }

    }
    std::cout << std::endl;
}

int GapBuffer::SaveBufferToFile(FILE *file, unsigned int bytes)
{

    if (!bytes)
        return 1;

    if (point == gapstart)
        point = gapend;

    if ((gapstart > point) && (gapstart < (point + bytes)) && (gapstart != gapend))
    {
        if (gapstart - point != fwrite(point, 1, gapstart-point, file))
            return 0;

        if ((bytes - (gapstart - point)) != fwrite(gapend, 1, bytes-(gapstart - point), file))
            return 1;

        return 1;
    }
    else
    {
        return bytes == fwrite(point, 1, bytes, file);
    }


}
