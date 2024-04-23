#include "pch.h"
#include "SendBufferChunk.h"
#include "SendBuffer.h"

void SendBufferChunk::Init()
{
    //���� ���丮�� �� 
    //�̱� ������ó�� ����ϸ� ��
    //���� �ʱ�ȭ
    open = false;
    usedSize = 0;
}

shared_ptr<SendBuffer> SendBufferChunk::Open(int size)
{
    //����ҷ��� �����Ͱ� BUFFER_SIZE���� ũ�� 
    if (size > BUFFER_SIZE)
        return nullptr;
    //����ϰ� �ִ� ���¶��
    if (open)
        return nullptr;
    //����ҷ��� �����Ͱ� ���� �������� ũ��
    if (size > FreeSize())
        return nullptr;

    //����ϰڴ� ǥ��
    open = true;
                             //[                      4096                    ]
    //[x][x][x][x][x][usedSize][][][][][][][][][][][][][][][][][][][][][][][][]
    return make_shared<SendBuffer>(shared_from_this(), &buffer[usedSize], size);
}

void SendBufferChunk::Close(int size)
{
    //����ϴ� ���°� �ƴ϶��
    if (!open)
        return;

    //��� ������ �ݾ��ְ�
    open = false;

                   //[ 21               ]
    //[x][x][x][x][x][usedSize][][][][][][][][][][][][][][][][][][][][][][][][]
                   //[ 21   ]
    //[x][x][x][x][x][x][x][x][x][x][x][x][usedSize][][][][][][][][][][][][][][]
    //�󸶳� ����ߴ��� �߰�.
    usedSize += size;
}
