#include "shared_memory.h"

SharedMemory::SharedMemory(ShMemType type) : m_SharedMemoryType (type)
{
	if (m_SharedMemoryType == ShMemType::Server)
        UA_LogI("Shared memory mode : Server!");
	
	else if (m_SharedMemoryType == ShMemType::Client)
		UA_LogI("SharedMemory mode : Client!");
	
	// Init SharedMemory
	this->Init();
}

SharedMemory::~SharedMemory()
{
	CloseHandle(m_hFile);        // Close handle of the file ( Shared Memory )
	UnmapViewOfFile(m_buffer);   // Un map the view from the mapping object
	CloseHandle(m_hMapObject);   // Close handle to the mapping object
	CloseHandle(m_hMutexObject); // Close handle to the mutex
}

// Init SharedMemory
bool SharedMemory::Init()
{
	// Shared Memory mode is  ( Server )

	if (m_SharedMemoryType == ShMemType::Server)
	{
		// Creating File 
		m_hFile = CreateFile("SharedMemory.bin", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ
			, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			UA_LogE("Error with creating the file SharedMemory.bin VAR{0}", GetLastError());
			return false;
		}
		else
			UA_LogI("File SharedMemory.bin created!");

		// Creating file mapping object
		m_hMapObject = CreateFileMappingA(m_hFile, NULL, PAGE_READWRITE, 0,
			SH_MEM_BUF_SIZE, (LPCSTR)"FMSharedMemory");
		if (m_hMapObject == INVALID_HANDLE_VALUE)
		{
			UA_LogE("Error with Creating Mapping Object!");
			return false;
		}
		else
			UA_LogI("Mapping Object Created!");


		// View map object to access the memory.
		m_buffer = (PVOID)MapViewOfFile(m_hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, SH_MEM_BUF_SIZE);
		if (m_buffer == NULL)
		{
			UA_LogE("Error MapView is NOT created!");
			CloseHandle(m_hMapObject);
			return false;
		}
		else
			UA_LogI("Map view created");

		// Creating the mutex object
		m_hMutexObject = CreateMutex(NULL, false, "SharedMemoryMutex");
		if (m_hMutexObject == INVALID_HANDLE_VALUE)
		{
			UA_LogE("Error: Could't create mutex object.");
			return false;
		}

		return true;

	}

	// Shared Memory mode is  ( Client )
	else if (m_SharedMemoryType == ShMemType::Client)
	{
		// Openning file mapping object
		m_hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, "FMSharedMemory");

		if (m_hMapObject == INVALID_HANDLE_VALUE)
		{
			UA_LogE("Error could't open File Mapping Object!");
			return false;
		}
		else
			UA_LogI("Mapping Object Opened!");

		// Creating a view of file mapping object
		m_buffer = (PVOID)MapViewOfFile(m_hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, SH_MEM_BUF_SIZE);
		if (m_buffer == NULL)
		{
			UA_LogE("Error MapView is NOT created!");
			CloseHandle(m_hMapObject);
			return false;
		}
		else
			UA_LogI("Map view created");

		//Open the mutex object
		m_hMutexObject = OpenMutex(MUTEX_ALL_ACCESS, false, "SharedMemoryMutex");
		if (m_hMutexObject == INVALID_HANDLE_VALUE)
		{
			UA_LogE("Error: Could't Open mutex object.");
			return false;
		}

		return true;
	}

	// TODO
	UA_LogW("Unkown type of SharedMemory!");
	return false;
}

void SharedMemory::ReadMemory(uint8_t * destenationBuffer, unsigned int destinationBufferSize)
{
	
	m_dwWaitResult = WaitForSingleObject(m_hMutexObject, 10);
	switch (m_dwWaitResult)
	{
	// TODO
	case WAIT_OBJECT_0:
		try
		{
			if (destinationBufferSize > SH_MEM_BUF_SIZE)
				throw('E');
			memcpy(destenationBuffer, m_buffer, destinationBufferSize);
		}
		catch (...)
		{
			perror("ReadMemory Error!");
		}
		if (!ReleaseMutex(m_hMutexObject))
		{
			UA_LogE("Release Mutex ERORR: VAR{0}", GetLastError());
		}
		break;

	case WAIT_TIMEOUT:
		UA_LogW("Thread VAR{0}: wait timed out", GetCurrentThreadId());
		break;

	case WAIT_FAILED:
		UA_LogE("ERROR with getting waiting object");
		break;

	case WAIT_ABANDONED:
		UA_LogW("WAIT_ABANDONED\n");
		break;

	default:
		UA_LogE("ERORR Unknown\n");
		break;
	}
	
}

void SharedMemory::WriteMemory(const uint8_t* destenationBuffer, unsigned int destinationBufferSize)
{
	m_dwWaitResult = WaitForSingleObject(m_hMutexObject, 10);
	switch (m_dwWaitResult)
	{
		// TODO
	case WAIT_OBJECT_0:
		try
		{
			if (destinationBufferSize > SH_MEM_BUF_SIZE)
				throw('E');
			memcpy(m_buffer, destenationBuffer, destinationBufferSize);
		}
		catch (...)
		{
			perror("WriteMemory Error!");
		}
		if (!ReleaseMutex(m_hMutexObject))
		{
			printf("Release Mutex ERORR: %d\n", GetLastError());
		}
		break;

	case WAIT_TIMEOUT:
		printf("Thread %d: wait timed out\n", GetCurrentThreadId());
		break;

	case WAIT_FAILED:
		printf("ERROR with getting waiting object\n");
		break;

	case WAIT_ABANDONED:
		printf("WAIT_ABANDONED\n");
		break;

	default:
		printf("ERORR Unknown\n");
		break;
	}
	
	
}