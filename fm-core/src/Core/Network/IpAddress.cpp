#include "fmpch.h"

#include <enet.h>

#include "IpAddress.h"

namespace fm {

	const fm::IpAddress fm::IpAddress::Any(0);
	const fm::IpAddress fm::IpAddress::Invalid(-1);
	const fm::IpAddress fm::IpAddress::Localhost("127.0.0.1");

	IpAddress::IpAddress(const std::string& address)
		: m_Address(0)

	{

		if (const std::uint32_t ip = inet_addr(address.data()); ip != INADDR_NONE)
			m_Address = ntohl(ip);

		addrinfo hints{};
		hints.ai_family = AF_INET;

		addrinfo* result = nullptr;
		if (getaddrinfo(address.data(), nullptr, &hints, &result) == 0 && result != nullptr) {

			sockaddr_in sin{};
			std::memcpy(&sin, result->ai_addr, sizeof(*result->ai_addr));

			const std::uint32_t ip = sin.sin_addr.s_addr;
			freeaddrinfo(result);

			m_Address = ntohl(ip);
		}
	}

	IpAddress::IpAddress(Uint32 address)
		: m_Address(address)

	{}

	IpAddress::IpAddress(const IpAddress& other)
		: m_Address(other.m_Address)

	{}

	IpAddress& IpAddress::operator=(const std::string& address) {

		if (const std::uint32_t ip = inet_addr(address.data()); ip != INADDR_NONE)
			m_Address = ntohl(ip);

		addrinfo hints{};
		hints.ai_family = AF_INET;

		addrinfo* result = nullptr;
		if (getaddrinfo(address.data(), nullptr, &hints, &result) == 0 && result != nullptr) {

			sockaddr_in sin{};
			std::memcpy(&sin, result->ai_addr, sizeof(*result->ai_addr));

			const std::uint32_t ip = sin.sin_addr.s_addr;
			freeaddrinfo(result);

			m_Address = ntohl(ip);
		}

		return *this;
	}

	IpAddress& IpAddress::operator=(Uint32 address) {

		m_Address = address;
		return *this;
	}

	IpAddress& IpAddress::operator=(const IpAddress& other) {

		m_Address = other.m_Address;
		return *this;
	}

	Uint32 IpAddress::ToInteger() const {

		return m_Address;
	}

	std::string IpAddress::ToString() const {

		in_addr address{};
		address.s_addr = htonl(m_Address);

		return inet_ntoa(address);
	}

	Packet& operator<<(Packet& packet, const IpAddress& address) {

		return packet << address.ToInteger();
	}

	Packet& operator>>(Packet& packet, IpAddress& address) {

		Uint32 ip;
		packet >> ip;
		address = ip;
		return packet;
	}
}
