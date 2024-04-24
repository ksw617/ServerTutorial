#include "pch.h"
#include "PacketSession.h"

//[0]..........................[25][26]..........................[50]....
//[packet                         ][packet                          ]
//[size(2byte)][id(2byte)][data...][size(2byte)][id(2bytes)][data...]...
int PacketSession::OnRecv(BYTE* buffer, int len)
{
    //ó���� ����
    int processLen = 0;
    while (true)
    {
        //50         =  50 - 0
        int dataSize = len - processLen;

        //50         < 4 == sizeof(PacketHeader)
        if (dataSize < sizeof(PacketHeader))
            break;

        //[size(2)][id(2)]  = *(4byte  point )(buffer[0]]��°�� �ּ�)
        PacketHeader header = *(PacketHeader*)(&buffer[processLen]);

        //OnRecvPacket���ٰ�(��Ŷ�� ���� �ּ�, ũ��)
        //ū�����͸� �ɰ��� �ѱ�
        //�����ּ� ->[size(2byte)][id(2byte)][data...] <- ��ü ũ�⸸
        OnRecvPacket(&buffer[processLen], header.size);

        //����� ���̴� header�� size�� ����ִ� �ش� ��Ŷ�� ũ�⸸ŭ ����
        processLen += header.size;
    }

    return processLen;
}
