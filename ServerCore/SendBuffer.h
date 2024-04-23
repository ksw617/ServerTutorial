#pragma once
class SendBufferChunk;

class SendBuffer  : public enable_shared_from_this<SendBuffer>
{
private:
	//주소 만 물고 있게
	BYTE* buffer;
	//여유 공간
	int freeSize = 0;
	int writeSize = 0;
	//내가 쓰고 있는 거대한 메모리의 주소 
	shared_ptr<SendBufferChunk> sendBufferChunk;

public:
	SendBuffer(shared_ptr<SendBufferChunk> chunk, BYTE* start, int size);
	~SendBuffer();
public:
	BYTE* GetBuffer() { return buffer; }
	int WriteSize() const { return writeSize; }
public:
	//내가 쓴만큼 넣어주기
	bool Close(int usedSize);
};

