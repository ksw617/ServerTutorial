#include "pch.h"
#include "SendBuffer.h"

SendBuffer::SendBuffer(int size)
{
    //���� ũ�� �Ҵ�
    buffer.resize(size);
}

SendBuffer::~SendBuffer()
{
}

bool SendBuffer::CopyData(void* data, int len)
{
    //������ �������� ���̰� �뷮���� ũ��
	if (Capacity() < len)
        return false;

    //������ ����
    memcpy(buffer.data(), data, len);
    writeSize = len;
    return true;
}
