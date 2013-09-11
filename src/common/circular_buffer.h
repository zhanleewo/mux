#ifndef COMMON_CIRCULAR_BUFFER 
#define COMMON_CIRCULAR_BUFFER
#include <public.h>

namespace common {
	class CircularBuffer
	{
	public:
		CircularBuffer(size_t size);
		~CircularBuffer();

		/** append l bytes from p to buffer */
		bool Write(const char *p,size_t l);
		/** copy l bytes from buffer to dest */
		bool Read(char *dest,size_t l);
		/** skip l bytes from buffer */
		bool Remove(size_t l);
		/** read l bytes from buffer, returns as string. */
		std::string ReadString(size_t l);

		/** total buffer length */
		size_t GetLength();
		/** pointer to circular buffer beginning */
		const char *GetStart();
		/** return number of bytes from circular buffer beginning to buffer physical end */
		size_t GetL();
		/** return free space in buffer, number of bytes until buffer overrun */
		size_t Space();

		/** return total number of bytes written to this buffer, ever */
		unsigned long ByteCounter(bool clear = false);

	private:
		CircularBuffer(const CircularBuffer& ) {}
		CircularBuffer& operator=(const CircularBuffer& ) { return *this; }
		char *buf;
		size_t m_max;
		size_t m_q;
		size_t m_b;
		size_t m_t;
		unsigned long m_count;
	};
}
#endif
