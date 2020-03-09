#ifndef H_SHARED_MEMORY_H
#define H_SHARED_MEMORY_H

#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "opcua_log.h"
#include "libs_config.h"



/*----------------- Shared Memory Class ------------------- 
One of mechanisms for communication between process within the system is Shared Memory mechanism
*/
// Shared Memory type ( Creating mapping object , Opening mapping object )
enum ShMemType { Server , Client }; 

/**
 * @brief 
 * 
 */
class shared_memory
{
	HANDLE    m_hFile;            // the shared memory it self
	HANDLE    m_hMapObject;       // Mapping Object, takes m_hFile as a parameter 
	HANDLE    m_hMutexObject;     // Mutex object 

	DWORD     m_dwWaitResult;     // indicator for the waiting mutex to release
	PVOID	  m_buffer;           // temperory buffer 
	ShMemType m_shared_memory_Type; // Shared memory type ( Server - Client )
	bool m_is_init = false;
public:
	shared_memory() = delete;
	shared_memory(ShMemType);
	~shared_memory();

	inline unsigned int GetSizeOfView()
	{
		return SH_MEM_BUF_SIZE;
	}
	inline PVOID get_shmem_pointer()
	{
		if (m_is_init)
			return m_buffer;
		return NULL;
	}
	void ReadMemory(uint8_t * destenationBuffer , unsigned int destinationBufferSize);
	void WriteMemory(const uint8_t * destenationBuffer, unsigned int destinationBufferSize);
	bool is_init()
	{
		return this->m_is_init;
	}
	/* Private functions */
private:
	bool Init();
};

#endif // H_SHARED_MEMORY_H