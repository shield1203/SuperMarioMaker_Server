#include "stdafx.h"
#include "ServerSystem.h"

#include "GameUser.h"
#include "GameRoomManager.h"
#include "SystemFrame.h"

ServerSystem::ServerSystem()
{
}

ServerSystem::~ServerSystem()
{
	WSACleanup();

	for (auto i : m_userList)
	{
		SafeDelete(i);
	}
	m_userList.clear();

	GameRoomManager* m_roomManager = GameRoomManager::getInstance();
	SafeDelete(m_roomManager);
}

void ServerSystem::Process()
{
	WSADATA m_wsadata;
	WSAStartup(MAKEWORD(2, 2), &m_wsadata);//���� �ʱ�ȭ       

	m_acceptSocket = SetTCPServer();

	printf("[SuperMarioMaker2] ���� ����\n\n");

	unsigned int threadID = 0;
	HANDLE threadAccept = (HANDLE)_beginthreadex(NULL, 0, AcceptUser, (void*)this, 0, (unsigned*)&threadID);

	while (true)
	{
		for (auto i = m_userList.begin(); i != m_userList.end();)
		{
			if ((*i)->IsConnect())
			{
				i++;
			}
			else
			{
				SafeDelete(*i);
				i = m_userList.erase(i++);
			}
		}
	}
}

SOCKET ServerSystem::SetTCPServer()
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == -1)
	{
		printf("[Accept����] ���� ����");
		exit(1);
	}

	SOCKADDR_IN servaddr = { 0 };//���� �ּ�
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = GetDefaultMyIP(); //.s_addr = inet_addr(SERVER_IP);//GetDefaultMyIP();
	servaddr.sin_port = htons(PORT_NUM);

	int m_check = 0;

	m_check = bind(sock, (struct sockaddr*) & servaddr, sizeof(servaddr));//���� �ּҿ� ��Ʈ��ũ �������̽� ����

	if (m_check == -1)
	{
		printf("[Accept����] bind ����");
		exit(1);
	}

	m_check = listen(sock, BLOG_SIZE); //�� �α� ť ����

	if (m_check == -1)
	{
		printf("[Accept����] listen ����");
		exit(1);
	}

	printf("%s\n", inet_ntoa(servaddr.sin_addr));

	return sock;
}

unsigned int WINAPI ServerSystem::AcceptUser(void* param)
{
	ServerSystem* m_serverSystem = (ServerSystem*)param;

	while (true)
	{
		SOCKET userSoket;
		SOCKADDR_IN cliaddr;

		int len = sizeof(cliaddr);

		userSoket = accept((SOCKET)m_serverSystem->m_acceptSocket, (SOCKADDR*)&cliaddr, &len); // ���� ����
		if (userSoket == -1)
		{
			closesocket(userSoket);
			perror("Accept ����\n");
			break;
		}

		printf("%s:%d�� ���� ��û ����\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

		GameUser* pAddUser = new GameUser(userSoket, cliaddr);

		unsigned int threadID = 0;
		pAddUser->m_threadHandle = (HANDLE)_beginthreadex(NULL, 0, pAddUser->Communication, (void*)pAddUser, 0, (unsigned*)&threadID);

		m_serverSystem->m_userList.push_back(pAddUser);
	}

	closesocket(m_serverSystem->m_acceptSocket);//���� �ݱ�

	return 0;
}