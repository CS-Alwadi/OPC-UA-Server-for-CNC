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
	uint8_t *     m_pshared_file;
	ShMemType m_shared_memory_Type; // Shared memory type ( Server - Client )
	uint16_t n_writtin = 0;
	bool m_is_init = false;
	bool m_is_opcua_running = true;
public:
	shared_memory() = delete;
	shared_memory(ShMemType);
	~shared_memory();

	bool ReadMemory(uint8_t * destenationBuffer , unsigned int destinationBufferSize);
	bool WriteMemory(const uint8_t * destenationBuffer, unsigned int destinationBufferSize);

	inline unsigned int GetSizeOfView()
	{
		#ifdef SH_MEM_BUF_SIZE
			return SH_MEM_BUF_SIZE;
		#else 
			return 0;
		#endif
	}

	inline PVOID get_shmem_pointer()
	{
		return m_buffer;
	}

	bool is_init()
	{
		return this->m_is_init;
	}
	
	inline void rewind()
	{
		n_writtin = 0;
		m_pshared_file = (uint8_t*)m_buffer;
	}

	inline uint32_t get_fseek()
	{
		return n_writtin; 
	}
	inline uint8_t * get_pshared_memory()
	{	
		if(n_writtin == 0)
			return &m_pshared_file[0];
		
		return  &m_pshared_file[get_fseek() - 1];
	}
	/* Private functions */
private:
	bool Init();
};

#endif // H_SHARED_MEMORY_H