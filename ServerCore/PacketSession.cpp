#include "pch.h"
#include "PacketSession.h"

//[0]..........................[25][26]..........................[50]....
//[packet                         ][packet                          ]
//[size(2byte)][id(2byte)][data...][size(2byte)][id(2bytes)][data...]...
int PacketSession::OnRecv(BYTE* buffer, int len)
{
    //처리한 길이
    int processLen = 0;
    while (true)
    {
        //50         =  50 - 0
        int dataSize = len - processLen;

        //50         < 4 == sizeof(PacketHeader)
        if (dataSize < sizeof(PacketHeader))
            break;

        //[size(2)][id(2)]  = *(4byte  point )(buffer[0]]번째의 주소)
        PacketHeader header = *(PacketHeader*)(&buffer[processLen]);

        //OnRecvPacket에다가(패킷의 시작 주소, 크기)
        //큰데이터를 쪼개서 넘김
        //시작주소 ->[size(2byte)][id(2byte)][data...] <- 전체 크기만
        OnRecvPacket(&buffer[processLen], header.size);

        //진행된 길이는 header의 size에 담겨있는 해당 패킷의 크기만큼 증가
        processLen += header.size;
    }

    return processLen;
}
