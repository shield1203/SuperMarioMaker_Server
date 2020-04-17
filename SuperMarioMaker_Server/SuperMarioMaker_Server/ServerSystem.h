#pragma once
class GameUser;

class ServerSystem
{
private:
	SOCKET m_acceptSocket;

	list<GameUser*> m_userList;
public:
	void Process();

	SOCKET SetTCPServer();
	static unsigned int WINAPI AcceptUser(void*);

	ServerSystem();
	~ServerSystem();
};