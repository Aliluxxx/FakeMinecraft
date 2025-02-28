#include "fmpch.h"

#define ENET_IMPLEMENTATION
#include <enet.h>

#include "Packet.h"

namespace fm {

	Packet::Packet(const Packet& other)
		: m_Data(nullptr), m_Size(other.m_Size), m_ReadPos(other.m_ReadPos), m_IsValid(other.m_IsValid)

	{

		Append(other.m_Data, other.m_Size);
	}

	Packet::Packet(Packet&& other) noexcept
		: m_Data(other.m_Data), m_Size(other.m_Size), m_ReadPos(other.m_ReadPos), m_IsValid(other.m_IsValid)

	{

		other.m_Data = nullptr;
		other.m_Size = 0;
		other.m_ReadPos = 0;
		other.m_IsValid = true;
	}

	Packet::Packet()
		: m_Data(nullptr), m_Size(0), m_ReadPos(0), m_IsValid(true)

	{}

	Packet::~Packet() {

		Clear();
	}

	void Packet::Append(const void* data, std::size_t size_in_bytes) {

		if (data && (size_in_bytes > 0)) {

			std::size_t start = m_Size;
			Resize(start + size_in_bytes);
			std::memcpy(&m_Data[start], data, size_in_bytes);
		}
	}

	void Packet::Clear() {

		if (m_Data != nullptr)
			delete[] m_Data;
		m_Data = nullptr;
		m_Size = 0;
		m_ReadPos = 0;
		m_IsValid = true;
	}

	const void* Packet::GetData() const {

		return m_Data;
	}

	std::size_t Packet::GetDataSize() const {

		return m_Size;
	}

	bool Packet::EndOfPacket() const {

		return m_ReadPos >= m_Size;
	}

	std::size_t Packet::GetReadPosition() const {

		return m_ReadPos;
	}

	Packet& Packet::operator=(const Packet& other) {

		m_Data = other.m_Data;
		m_Size = other.m_Size;
		m_ReadPos = other.m_ReadPos;
		m_IsValid = other.m_IsValid;

		return *this;
	}

	Packet& Packet::operator=(Packet&& other) noexcept {

		if (this != &other) {

			if (m_Data != nullptr)
				delete[] m_Data;

			m_Data = other.m_Data;
			m_Size = other.m_Size;
			m_ReadPos = other.m_ReadPos;
			m_IsValid = other.m_IsValid;

			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_ReadPos = 0;
			other.m_IsValid = true;
		}

		return *this;
	}

	Packet::operator bool() const {

		return m_IsValid;
	}

	Packet& Packet::operator>>(bool& data) {

		Uint8 value;
		if (*this >> value)
			data = (value != 0);

		return *this;
	}

	Packet& Packet::operator>>(Int8& data) {

		if (CheckSize(sizeof(data))) {

			std::memcpy(&data, &m_Data[m_ReadPos], sizeof(data));
			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(Uint8& data) {

		if (CheckSize(sizeof(data))) {

			std::memcpy(&data, &m_Data[m_ReadPos], sizeof(data));
			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(Int16& data) {

		if (CheckSize(sizeof(data))) {

			std::memcpy(&data, &m_Data[m_ReadPos], sizeof(data));
			data = static_cast<Int16>(ntohs(static_cast<Uint16>(data)));
			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(Uint16& data) {

		if (CheckSize(sizeof(data))) {

			std::memcpy(&data, &m_Data[m_ReadPos], sizeof(data));
			data = ntohs(data);
			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(Int32& data) {

		if (CheckSize(sizeof(data))) {

			std::memcpy(&data, &m_Data[m_ReadPos], sizeof(data));
			data = static_cast<Int32>(ntohl(static_cast<Uint32>(data)));
			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(Uint32& data) {

		if (CheckSize(sizeof(data))) {

			std::memcpy(&data, &m_Data[m_ReadPos], sizeof(data));
			data = ntohl(data);
			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(Int64& data) {

		if (CheckSize(sizeof(data))) {

			Uint8 bytes[sizeof(data)]{};
			std::memcpy(bytes, &m_Data[m_ReadPos], sizeof(data));

			data = (static_cast<Int64>(bytes[0]) << 56) | (static_cast<Int64>(bytes[1]) << 48) |
				   (static_cast<Int64>(bytes[2]) << 40) | (static_cast<Int64>(bytes[3]) << 32) |
				   (static_cast<Int64>(bytes[4]) << 24) | (static_cast<Int64>(bytes[5]) << 16) |
				   (static_cast<Int64>(bytes[6]) << 8) | (static_cast<Int64>(bytes[7]));

			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(Uint64& data) {
		if (CheckSize(sizeof(data))) {

			Uint8 bytes[sizeof(data)]{};
			std::memcpy(bytes, &m_Data[m_ReadPos], sizeof(data));

			data = (static_cast<Int64>(bytes[0]) << 56) | (static_cast<Int64>(bytes[1]) << 48) |
				   (static_cast<Int64>(bytes[2]) << 40) | (static_cast<Int64>(bytes[3]) << 32) |
				   (static_cast<Int64>(bytes[4]) << 24) | (static_cast<Int64>(bytes[5]) << 16) |
				   (static_cast<Int64>(bytes[6]) << 8) | (static_cast<Int64>(bytes[7]));

			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(float& data) {

		if (CheckSize(sizeof(data))) {

			std::memcpy(&data, &m_Data[m_ReadPos], sizeof(data));
			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(double& data) {

		if (CheckSize(sizeof(data))) {

			std::memcpy(&data, &m_Data[m_ReadPos], sizeof(data));
			m_ReadPos += sizeof(data);
		}

		return *this;
	}

	Packet& Packet::operator>>(char* data) {

		Uint32 length;
		*this >> length;

		if ((length > 0) && CheckSize(length)) {

			std::memcpy(data, &m_Data[m_ReadPos], length);

			m_ReadPos += length;
		}

		return *this;
	}

	Packet& Packet::operator>>(std::string& data) {

		Uint32 length;
		*this >> length;

		data.clear();
		data.resize(length);
		if ((length > 0) && CheckSize(length)) {

			for (Uint32 i = 0; i < length; i++)
				data[i] = static_cast<char>(m_Data[m_ReadPos + i]);

			m_ReadPos += length;
		}

		return *this;
	}

	Packet& Packet::operator<<(bool data) {

		*this << static_cast<Uint8>(data);

		return *this;
	}

	Packet& Packet::operator<<(Int8 data) {

		Append(&data, sizeof(data));

		return *this;
	}

	Packet& Packet::operator<<(Uint8 data) {

		Append(&data, sizeof(data));

		return *this;
	}

	Packet& Packet::operator<<(Int16 data) {

		auto toWrite = static_cast<Int16>(htons(static_cast<Uint16>(data)));
		Append(&toWrite, sizeof(toWrite));

		return *this;
	}

	Packet& Packet::operator<<(Uint16 data) {

		Uint16 toWrite = htons(data);
		Append(&toWrite, sizeof(toWrite));

		return *this;
	}

	Packet& Packet::operator<<(Int32 data) {

		Int32 toWrite = static_cast<Int32>(htonl(static_cast<Uint32>(data)));
		Append(&toWrite, sizeof(toWrite));

		return *this;
	}

	Packet& Packet::operator<<(Uint32 data) {

		Uint32 toWrite = htonl(static_cast<Uint32>(data));
		Append(&toWrite, sizeof(toWrite));

		return *this;
	}

	Packet& Packet::operator<<(Int64 data) {

		Uint8 toWrite[] = {

			static_cast<Uint8>((data >> 56) & 0xFF),
			static_cast<Uint8>((data >> 48) & 0xFF),
			static_cast<Uint8>((data >> 40) & 0xFF),
			static_cast<Uint8>((data >> 32) & 0xFF),
			static_cast<Uint8>((data >> 24) & 0xFF),
			static_cast<Uint8>((data >> 16) & 0xFF),
			static_cast<Uint8>((data >> 8) & 0xFF),
			static_cast<Uint8>((data) & 0xFF)
		};

		Append(&toWrite, sizeof(toWrite));

		return *this;
	}

	Packet& Packet::operator<<(Uint64 data) {

		Uint8 toWrite[] = {

			static_cast<Uint8>((data >> 56) & 0xFF),
			static_cast<Uint8>((data >> 48) & 0xFF),
			static_cast<Uint8>((data >> 40) & 0xFF),
			static_cast<Uint8>((data >> 32) & 0xFF),
			static_cast<Uint8>((data >> 24) & 0xFF),
			static_cast<Uint8>((data >> 16) & 0xFF),
			static_cast<Uint8>((data >> 8) & 0xFF),
			static_cast<Uint8>((data) & 0xFF)
		};

		Append(&toWrite, sizeof(toWrite));

		return *this;
	}

	Packet& Packet::operator<<(float data) {

		Append(&data, sizeof(data));
		return *this;
	}

	Packet& Packet::operator<<(double data) {

		Append(&data, sizeof(data));
		return *this;
	}

	Packet& Packet::operator<<(const char* data) {

		auto length = static_cast<Uint32>(std::strlen(data) + 1);
		*this << length;

		Append(data, length * sizeof(char));

		return *this;
	}

	Packet& Packet::operator<<(const std::string& data) {

		auto length = static_cast<Uint32>(data.size() + 1);
		*this << length;

		if (length > 0)
			Append(data.c_str(), length * sizeof(std::string::value_type));

		return *this;
	}

	bool Packet::CheckSize(std::size_t size) {

		m_IsValid = m_IsValid && (m_ReadPos + size <= m_Size);

		return m_IsValid;
	}

	void Packet::Resize(std::size_t size) {

		Uint8* tmpData = m_Data;
		m_Data = new Uint8[size];
		memcpy_s(m_Data, size, tmpData, m_Size);
		m_Size = size;
	}
}
