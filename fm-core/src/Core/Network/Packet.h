#pragma once

#include <vector>

#include "Core/Base/Base.h"

namespace fm {

	class Socket;

	typedef Uint32 PacketFlags_;
	static const Uint32 MAX_PACKET_SIZE = 33554432;

	enum PacketFlags {

		Reliable = BIT(0),
		Unsequenced = BIT(1),
		NoAllocate = BIT(2),
		Unreliable = BIT(3),
		Sent = BIT(8)
	};

	class FM_API Packet {

	public:

		Packet(const Packet& other);
		Packet(Packet&& other) noexcept;
		Packet();
		virtual ~Packet();

		void Append(const void* data, std::size_t size_in_bytes);
		void Clear();
		const void* GetData() const;
		std::size_t GetDataSize() const;
		bool EndOfPacket() const;
		std::size_t GetReadPosition() const;

		Packet& operator=(const Packet& other);
		Packet& operator=(Packet&& other) noexcept;
		explicit operator bool() const;
		Packet& operator>>(bool& data);
		Packet& operator>>(Int8& data);
		Packet& operator>>(Uint8& data);
		Packet& operator>>(Int16& data);
		Packet& operator>>(Uint16& data);
		Packet& operator>>(Int32& data);
		Packet& operator>>(Uint32& data);
		Packet& operator>>(Int64& data);
		Packet& operator>>(Uint64& data);
		Packet& operator>>(float& data);
		Packet& operator>>(double& data);
		Packet& operator>>(char* data);
		Packet& operator>>(std::string& data);
		Packet& operator<<(bool data);
		Packet& operator<<(Int8 data);
		Packet& operator<<(Uint8 data);
		Packet& operator<<(Int16 data);
		Packet& operator<<(Uint16 data);
		Packet& operator<<(Int32 data);
		Packet& operator<<(Uint32 data);
		Packet& operator<<(Int64 data);
		Packet& operator<<(Uint64 data);
		Packet& operator<<(float data);
		Packet& operator<<(double data);
		Packet& operator<<(const char* data);
		Packet& operator<<(const std::string& data);

	private:

		bool CheckSize(std::size_t size);
		void Resize(std::size_t size);

		Uint8* m_Data;
		std::size_t m_Size;
		std::size_t m_ReadPos;
		bool m_IsValid;

		friend Socket;
	};
}
