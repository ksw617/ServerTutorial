#include "pch.h"
#include "RecvBuffer.h"

RecvBuffer::RecvBuffer(int size) : bufferSize(size)
{
    //엄청나게 큰 공간 할당
    capacity = bufferSize * BUFFER_COUNT;
    //capacity만큼 늘려주기
    buffer.resize(capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Clear()
{                      
    //만날 확률을 높이는 방식
    int dataSize = DataSize(); 
    if (dataSize == 0)         
    {
        readPos = 0;           
        writePos = 0;           
    }
    else
    {
        //[64kb][64kb][64kb][64kb][64kb][64kb][64kb][64kb][64kb][ r,1,2,3,w ]
        //[r,1,2,3,w][64kb][64kb][64kb][64kb][64kb][64kb][64kb][64kb][64kb]
        if (FreeSize() < bufferSize)
        {
            //복사진행
            memcpy(&buffer[0], &buffer[readPos], dataSize);
            readPos = 0;
            writePos = dataSize;

        }
      
    }
}

bool RecvBuffer::OnRead(int numOfBytes)
{
    if (numOfBytes > DataSize())
        return false;

    readPos += numOfBytes;

    return true;
}

bool RecvBuffer::OnWrite(int numOfBytes)
{
    if (numOfBytes > FreeSize())
        return false;

    writePos += numOfBytes;

    return true;
}
