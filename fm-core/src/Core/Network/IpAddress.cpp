#include "fmpch.h"

#include <enet.h>

#include "IpAddress.h"

namespace fm {

	const fm::IpAddress fm::IpAddress::Any(0);
	const fm::IpAddress fm::IpAddress::Invalid(-1);
	const fm::IpAddress fm::IpAddress::Localhost("127.0.0.1");

	IpAddress IpAddress::GetPublicAddress() {

		std::string ipAddress = "78.47.82.133";			// IP Address of the server
		int port = 80;									// Listening port # on the server

		// Initialize WinSock
		WSAData data;
		WORD ver = MAKEWORD(2, 2);
		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0) {

			return IpAddress::Invalid;
		}

		// Create socket
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) {

			WSACleanup();
			return IpAddress::Invalid;
		}

		// Fill in a hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		// Connect to server
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR) {

			closesocket(sock);
			WSACleanup();
			return IpAddress::Invalid;
		}

		std::string request =
			"GET /ip-provider.php HTTP/1.1\r\n"
			"Host: www.sfml-dev.org\r\n"
			"Connection: close\r\n\r\n";

		char buf[4096];
		int sendResult = send(sock, request.c_str(), (int)request.size() + 1, 0);
		if (sendResult != SOCKET_ERROR) {

			// Wait for response
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0) {

				// Echo response to console
				std::string response = std::string(buf, 0, bytesReceived);
				return IpAddress(response.substr(response.rfind("\r\n") + 2));
			}
		}

		return IpAddress::Invalid;
	}

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
