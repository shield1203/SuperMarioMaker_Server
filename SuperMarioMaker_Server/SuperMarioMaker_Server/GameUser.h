#pragma once
class SystemFrame;
class PacketManager;

class GameUser
{
private:
	SOCKET m_socket;
	SOCKADDR_IN m_cliaddr = { 0 };

	USER_STATE m_state;

	SystemFrame* m_systemFrame = nullptr;
public:
	HANDLE m_threadHandle = nullptr;
	PacketManager* m_packetManager = nullptr;
public:
	void Initialize();
	void Recv();
	void Send();
	static unsigned int WINAPI Communication(void*);

	bool IsConnect();

	GameUser(SOCKET, SOCKADDR_IN);
	~GameUser();
};