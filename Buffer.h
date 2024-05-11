#ifndef _BUFFER_H
#define _BUFFER_H
#include <stdlib.h>
#include <algorithm>
#include <stdint.h>
#include <assert.h>
class Buffer
{
public:
	static const int initialSize;

	explicit Buffer();
	~Buffer();

	int readableBytes() const
	{
		return mWriteIndex - mReadIndex;
	}

	int writableBytes() const
	{
		return mBufferSize - mWriteIndex;
	}

	int prependableBytes() const
	{
		return mReadIndex;
	}

	char* peek()
	{
		return begin() + mReadIndex;
	}

	const char* peek() const
	{
		return begin() + mReadIndex;
	}

	const char* findCRLF() const
	{
		const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
		return crlf == beginWrite() ? NULL : crlf;
	}

	const char* findCRLF(const char* start) const
	{
		assert(peek() <= start);
		assert(start <= beginWrite());
		const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
		return crlf == beginWrite() ? NULL : crlf;
	}

	const char* findLastCrlf() const
	{
		const char* crlf = std::find_end(peek(), beginWrite(), kCRLF, kCRLF + 2);
		return crlf == beginWrite() ? NULL : crlf;
	}
	void retrieveReadZero() {
		mReadIndex = 0;
	}

	void retrieve(int len)
	{
		assert(len <= readableBytes());
		if (len < readableBytes())
		{
			mReadIndex += len;
		}
		else
		{
			retrieveAll();
		}
	}

	void retrieveUntil(const char* end)
	{
		assert(peek() <= end);
		assert(end <= beginWrite());
		retrieve(end - peek());
	}

	void retrieveAll()//�ָ�����
	{
		mReadIndex = 0;
		mWriteIndex = 0;
	}

	char* beginWrite()
	{
		return begin() + mWriteIndex;
	}

	const char* beginWrite() const
	{
		return begin() + mWriteIndex;
	}

	void unwrite(int len)
	{
		assert(len <= readableBytes());
		mWriteIndex -= len;
	}

	/* ȷ�����㹻�Ŀռ� */
	void ensureWritableBytes(int len)
	{
		if (writableBytes() < len)
		{
			makeSpace(len);
		}
		assert(writableBytes() >= len);
	}

	void makeSpace(int len)
	{
		if (writableBytes() + prependableBytes() < len) //���ʣ��ռ䲻��
		{
			/* ����ռ� */
			mBufferSize = mWriteIndex + len;
			mBuffer = (char*)realloc(mBuffer, mBufferSize);
		}
		else //ʣ��ռ��㹻
		{
			/* �ƶ����� */
			int readable = readableBytes();
			std::copy(begin() + mReadIndex,
				begin() + mWriteIndex,
				begin());
			mReadIndex = 0;
			mWriteIndex = mReadIndex + readable;
			assert(readable == readableBytes());
		}
	}

	void append(const char* data, int len)
	{
		ensureWritableBytes(len); //��������Ŀռ�
		std::copy(data, data + len, beginWrite()); //��������

		assert(len <= writableBytes());
		mWriteIndex += len;//���µ���дλ��
	}

	void append(const void* data, int len)
	{
		append((const char*)(data), len);
	}

	int read(int fd);
	int write(int fd);

private:
	char* begin()
	{
		return mBuffer;
	}

	const char* begin() const
	{
		return mBuffer;
	}

private:
	char* mBuffer;
	int mBufferSize;
	int mReadIndex; // ��ǰ
	int mWriteIndex;// ��ǰ��socketʵ�ʶ�ȡ�����ֽڳ���

	static const char* kCRLF;
};

#endif