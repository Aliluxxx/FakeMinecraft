#pragma once

#include "Core/Base/Base.h"

namespace fm {

	class FM_API InputStream {

	public:

		virtual ~InputStream() {}

		virtual Int64 Read(void* data, Int64 size) = 0;
		virtual Int64 Seek(Int64 position) = 0;
		virtual Int64 Tell() = 0;
		virtual Int64 GetSize() = 0;
	};
}
