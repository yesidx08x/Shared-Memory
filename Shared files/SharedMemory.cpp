#include "SharedMemory.h"

SharedMemory::SharedMemory() {}

SharedMemory::~SharedMemory() {}

char* SharedMemory::OpenMemory(int size, bool sender)
{
	if (!_open)
	{
		_sender = sender;
		_readerCount = 0;
		//SE_CREATE_GLOBAL_NAME;
		size *= 1024 * 1024;
		_circInfo.freeMem = size;
		_circInfo.head = sizeof(CircInfo);
		_circInfo.tail = sizeof(CircInfo);
		_circInfo.totalMem = size;
		_circInfo.SentCount = 0;
		_circInfo.ReadCount = 0;

		// Main data
		if (_sender)
		{
			_fileMapping = CreateFileMappingW(
				INVALID_HANDLE_VALUE,
				NULL,
				PAGE_READWRITE,
				(DWORD)0,
				size,
				L"Global/MainData");
		}
		else
		{
			_fileMapping = OpenFileMappingW(FILE_MAP_ALL_ACCESS, false, L"Global/MainData");
		}
		if (GetLastError() == ERROR_ALREADY_EXISTS)
			return "Data allready exist\n";

		if (_fileMapping == nullptr)
			return "Could not open file mapping object!";

		_buffer = MapViewOfFile(_fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		if (_buffer == nullptr)
		{
			CloseHandle(_buffer);
			return "Could not map view of file!";
		}
		if (_sender)
		{
			memcpy((char*)_buffer, &_circInfo, sizeof(CircInfo));
		}
		if (!_sender)
		{
			memcpy(&_circInfo, (char*)_buffer, sizeof(CircInfo));
			_circInfo.ReaderCount++;
			memcpy((char*)_buffer + (sizeof(unsigned int) * 6), &_circInfo.ReaderCount, sizeof(unsigned int));
		}
		_open = true;
	}
	return "Shared memory open success!";
}

char* SharedMemory::CloseMemory()
{
	char* status = nullptr;

	if (UnmapViewOfFile(_buffer) == 0)
		status = "Failed UnmapViewOfFile";
	if (CloseHandle(_fileMapping) == 0)
		status = "Failed CloseHandle";
	if (status == nullptr)
	{
		status = "Closed all maps!";
		_open = false;
	}

	return status;
}

SharedMemory::CircInfo SharedMemory::GetInfo()
{
	if (_buffer != nullptr)
	{
		memcpy(&_circInfo, (char*)_buffer, sizeof(CircInfo));
	}
	return _circInfo;
}

void SharedMemory::SetInfo(CircInfo circInfo)
{
	_circInfo = circInfo;
	memcpy((char*)_buffer, &_circInfo, sizeof(CircInfo));
}

bool SharedMemory::AddedReader()
{
	memcpy(&_circInfo, (char*)_buffer, sizeof(CircInfo));
	if (_circInfo.ReaderCount > _readerCount)
	{
		_readerCount = _circInfo.ReaderCount;
		return true;
	}
	else
	{
		return false;
	}
}

unsigned int SharedMemory::GetCircSize(){return sizeof(CircInfo);}

bool SharedMemory::IsOpen()
{
	OpenMemory(_circInfo.totalMem, false);
	return _open;
}
