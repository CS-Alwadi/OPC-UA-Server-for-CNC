#ifndef H_SHARED_MEMORY_H
#define H_SHARED_MEMORY_H

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include "opcua_log.h"


#define SH_MEM_BUF_SIZE 256
/*----------------- Shared Memory Class ------------------- 
One of mechanisms for communication between process within the system is Shared Memory mechanism
*/
// Shared Memory type ( Creating mapping object , Opening mapping object )
enum ShMemType { Server , Client }; 

class SharedMemory
{
	HANDLE    m_hFile;            // the shared memory it self
	HANDLE    m_hMapObject;       // Mapping Object, takes m_hFile as a parameter 
	HANDLE    m_hMutexObject;     // Mutex object 

	DWORD     m_dwWaitResult;     // indicator for the waiting mutex to release
	PVOID	  m_buffer;           // temperory buffer 
	ShMemType m_SharedMemoryType; // Shared memory type ( Server - Client )
	
public:
	SharedMemory() = delete;
	SharedMemory(ShMemType);
	~SharedMemory();

	void SetMode(bool Type)
	{
		if (Type == ShMemType::Server)
			m_SharedMemoryType = ShMemType::Server;
		else if (Type == ShMemType::Client)
			m_SharedMemoryType = ShMemType::Client;
	}
	inline unsigned int GetSizeOfView()
	{
		return SH_MEM_BUF_SIZE;
	}
	void ReadMemory(uint8_t * destenationBuffer , unsigned int destinationBufferSize);
	void WriteMemory(const uint8_t * destenationBuffer, unsigned int destinationBufferSize);

	/* Private functions */
private:
	bool Init();
};

#endif // H_SHARED_MEMORY_H