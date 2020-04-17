#pragma once

// SafeDelete
template<typename T>
static void SafeDelete(T*& p)
{
	if (p == nullptr) return;

	delete p;
	p = nullptr;
}

template<typename T>
static void SageDelete_Array(T*& p)
{
	if (p == nullptr) return;

	delete[] p;
	p = nullptr;
}

// Get_My_IP
static IN_ADDR GetDefaultMyIP()
{
	char m_localhostname[MAX_PATH];
	IN_ADDR addr = { 0, };

	if (gethostname(m_localhostname, MAX_PATH) == SOCKET_ERROR) { return addr; }

	HOSTENT* ptr = gethostbyname(m_localhostname);

	while (ptr && ptr->h_name)
	{
		if (ptr->h_addrtype == PF_INET)
		{
			memcpy(&addr, ptr->h_addr_list[0], ptr->h_length);
			break;
		}
		ptr++;
	}

	return addr;
}