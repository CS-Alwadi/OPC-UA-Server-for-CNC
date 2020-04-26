#include "shared_memory.h"
#include "libs_config.h"
shared_memory::shared_memory(ShMemType type) : m_shared_memory_Type (type)
{
	if (m_shared_memory_Type == ShMemType::Server)
        UA_LogI("Shared memory mode : Server!");
	
	else if (m_shared_memory_Type == ShMemType::Client)
		UA_LogI("shared_memory mode : Client!");
	
	// Init shared_memory
	this->Init();
}

shared_memory::~shared_memory()
{
	CloseHandle(m_hFile);        // Close handle of the file ( Shared Memory )
	UnmapViewOfFile(m_buffer);   // Un map the view from the mapping object
	CloseHandle(m_hMapObject);   // Close handle to the mapping object
	CloseHandle(m_hMutexObject); // Close handle to the mutex
}

// Init shared_memory
bool shared_memory::Init()
{
	// Shared Memory mode is  ( Server )

	if (m_shared_memory_Type == ShMemType::Server)
	{
		// Creating File 
		m_hFile = CreateFile("shared_memory.bin", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ
			, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			UA_LogE("Error with creating the file shared_memory.bin VAR{0}", GetLastError());
			return false;
		}
		else
			UA_LogI("File shared_memory.bin created!");

		// Creating file mapping object
		m_hMapObject = CreateFileMappingA(m_hFile, NULL, PAGE_READWRITE, 0,
			SH_MEM_BUF_SIZE, (LPCSTR)"FMshared_memory");
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
		m_hMutexObject = CreateMutex(NULL, false, "shared_memoryMutex");
		if (m_hMutexObject == INVALID_HANDLE_VALUE)
		{
			UA_LogE("Error: Could't create mutex object.");
			return false;
		}
		m_pshared_file = (uint8_t*)m_buffer;
		this->m_is_init = true;
		return true;

	}

	// Shared Memory mode is  ( Client )
	else if (m_shared_memory_Type == ShMemType::Client)
	{
		// Openning file mapping object
		m_hMapObject = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, "FMshared_memory");

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
			this->m_is_init = false;
			CloseHandle(m_hMapObject);
			return false;
		}
		else
			UA_LogI("Map view created");

		//Open the mutex object
		m_hMutexObject = OpenMutex(MUTEX_ALL_ACCESS, false, "shared_memoryMutex");
		if (m_hMutexObject == INVALID_HANDLE_VALUE)
		{
			UA_LogE("Error: Could't Open mutex object.");
			this->m_is_init = false;
			return false;
		}
		m_pshared_file = (uint8_t*)m_buffer;
		this->m_is_init = true;
		return true;
	}

	// TODO
	UA_LogW("Unkown type of shared_memory!");
	return false;
}

bool shared_memory::ReadMemory(uint8_t * destenationBuffer, unsigned int destinationBufferSize)
{
	if((uint8_t * )m_pshared_file + destinationBufferSize > (uint8_t * )m_buffer + SH_MEM_BUF_SIZE)
		return false;
	m_dwWaitResult = WaitForSingleObject(m_hMutexObject, INFINITE);
	switch (m_dwWaitResult)
	{
	// TODO
	case WAIT_OBJECT_0:
		memcpy(destenationBuffer, m_buffer, destinationBufferSize);
		m_pshared_file = (uint8_t*)m_pshared_file + destinationBufferSize;
		n_writtin += destinationBufferSize;
		ReleaseMutex(m_hMutexObject);
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
	return true;
}

bool shared_memory::WriteMemory(const uint8_t* destenationBuffer, unsigned int destinationBufferSize)
{
	if((uint8_t * )m_pshared_file + destinationBufferSize > (uint8_t * )m_buffer + SH_MEM_BUF_SIZE)
		return false;
	
	m_dwWaitResult = WaitForSingleObject(m_hMutexObject, INFINITE);
	switch (m_dwWaitResult)
	{
		// TODO
	case WAIT_OBJECT_0:
		memcpy(m_pshared_file, destenationBuffer, destinationBufferSize);
		m_pshared_file = (uint8_t*)m_pshared_file + destinationBufferSize;
		n_writtin += destinationBufferSize;
		ReleaseMutex(m_hMutexObject);
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
	
	return true;
}