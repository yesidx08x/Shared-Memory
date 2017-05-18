#pragma once
#include <windows.h>

class SharedMemory
{
public:
	SharedMemory();
	~SharedMemory();

	struct CircInfo
	{
		unsigned int freeMem;
		unsigned int totalMem;
		unsigned int head;
		unsigned int tail;
		unsigned int SentCount;
		unsigned int ReadCount;
		unsigned int ReaderCount;
	};

	char* OpenMemory(int size, bool sender = false);
	char* CloseMemory();
	CircInfo GetInfo();
	void SetInfo(CircInfo circInfo);
	void* _buffer;
	bool AddedReader();
	unsigned int GetCircSize();
	bool IsOpen();

private:
	HANDLE _fileMapping;
	CircInfo _circInfo;
	bool _sender;
	unsigned int _readerCount;
	bool _open = false;
};