#include "fmpch.h"

#include "MemoryInputStream.h"

namespace fm {

	MemoryInputStream::MemoryInputStream()
		: m_Data(nullptr), m_Size(0), m_Offset(0)

	{}

	void MemoryInputStream::Open(const void* data, std::size_t size_in_bytes) {

		m_Data = static_cast<const char*>(data);
		m_Size = static_cast<Int64>(size_in_bytes);
		m_Offset = 0;
	}

	Int64 MemoryInputStream::Read(void* data, Int64 size) {

		if (!m_Data)
			return -1;

		Int64 endPosition = m_Offset + size;
		Int64 count = endPosition <= m_Size ? size : m_Size - m_Offset;

		if (count > 0) {

			std::memcpy(data, m_Data + m_Offset, static_cast<std::size_t>(count));
			m_Offset += count;
		}

		return count;
	}

	Int64 MemoryInputStream::Seek(Int64 position) {

		if (!m_Data)
			return -1;

		m_Offset = position < m_Size ? position : m_Size;

		return m_Offset;
	}

	Int64 MemoryInputStream::Tell() {

		if (!m_Data)
			return -1;

		return m_Offset;
	}

	Int64 MemoryInputStream::GetSize() {

		if (!m_Data)
			return -1;

		return m_Size;
	}
}
