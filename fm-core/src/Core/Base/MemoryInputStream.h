#pragma once

#include "Core/Base/Base.h"
#include "Core/Base/InputStream.h"

namespace fm {

	class FM_API MemoryInputStream : public InputStream {

	public:

		MemoryInputStream();
		~MemoryInputStream() override = default;

		void Open(const void* data, std::size_t size_in_bytes);
		virtual Int64 Read(void* data, Int64 size) override;
		virtual Int64 Seek(Int64 position) override;
		virtual Int64 Tell() override;
		virtual Int64 GetSize() override;

	private:

		const char* m_Data;
		Int64 m_Size;
		Int64 m_Offset;
	};
}
