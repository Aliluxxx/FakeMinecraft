#pragma once

#include "Core/Base/Base.h"
#include "Core/Network/Packet.h"

namespace fm {

	static const Uint16 AnyPort = 0;

	class FM_API IpAddress {

	public:

		static const IpAddress Any;
		static const IpAddress Invalid;
		static const IpAddress Localhost;

		IpAddress(const std::string& address);
		IpAddress(Uint32 address);
		IpAddress(const IpAddress& other);

		IpAddress& operator=(const std::string& address);
		IpAddress& operator=(Uint32 address);
		IpAddress& operator=(const IpAddress& other);

		Uint32 ToInteger() const;
		std::string ToString() const;

	private:

		Uint32 m_Address;
	};

	FM_API Packet& operator<<(Packet& packet, const IpAddress& address);
	FM_API Packet& operator>>(Packet& packet, IpAddress& address);
}
