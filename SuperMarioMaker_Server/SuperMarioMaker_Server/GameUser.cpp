#include "stdafx.h"
#include "GameUser.h"

#include "PacketManager.h"
#include "GameRoomManager.h"

#include "SystemFrame.h"
#include "LoginSystem.h"
#include "LobbySystem.h"
#include "GameMapSystem.h"
#include "GameRoomSystem.h"
#include "GamePlaySystem.h"

GameUser::GameUser(SOCKET socket, SOCKADDR_IN cliaddr)
{
	this->m_socket = socket;
	this->m_cliaddr = cliaddr;

	m_packetManager = new PacketManager;

	m_state = USER_STATE::USER_LOGIN;
	m_systemFrame = new LoginSystem();
}

GameUser::~GameUser()
{
	closesocket(m_socket);

	SafeDelete(m_systemFrame);

	CloseHandle(m_threadHandle);

	SafeDelete(m_packetManager);
}

void GameUser::Initialize()
{
	if (m_packetManager->m_packetData->userState != m_state)
	{
		m_state = m_packetManager->m_packetData->userState;

		SafeDelete(m_systemFrame);

		switch (m_state)
		{
		case USER_STATE::USER_LOGIN:
			m_systemFrame = new LoginSystem();
			break;
		case USER_STATE::USER_LOBBY:
			m_systemFrame = new LobbySystem();
			break;
		case USER_STATE::USER_MAP_UPLOAD:
		case USER_STATE::USER_MAP_DOWNLOAD:
			m_systemFrame = new GameMapSystem();
			break;
		case USER_STATE::USER_ROOM:
			m_systemFrame = new GameRoomSystem();
			break;
		case USER_STATE::USER_PLAY_GAME:
			m_systemFrame = new GamePlaySystem();
			break;
		}
	}
}

void GameUser::Recv()
{
	char msg[MAX_MSG_LEN] = "";
	int connectCheck = recv(m_socket, msg, sizeof(msg), 0);

	if (connectCheck > 0)
	{
		msg[sizeof(PacketData)] = '\0';
		PacketData* packetData = (PacketData*)msg;

		m_packetManager->CopyPacket(packetData);

		GameRoomManager::getInstance()->m_mutex.lock();
		m_packetManager->GetData(m_packetManager->m_packetData->userState);
		GameRoomManager::getInstance()->m_mutex.unlock();
	}
	else
	{
		m_packetManager->m_packetData->userState = USER_STATE::CLOSE_CONNECT;
	}
}

void GameUser::Send()
{
	m_packetManager->SetPacket(m_state);
	send(m_socket, (char*)m_packetManager->m_packetData, sizeof(PacketData), 0);
}

bool GameUser::IsConnect()
{
	bool bConnect = (m_state != USER_STATE::CLOSE_CONNECT);

	return bConnect;
}

unsigned int WINAPI GameUser::Communication(void* gameUser)
{
	GameUser* pGameUser = (GameUser*)gameUser;

	while (pGameUser->m_packetManager->m_packetData->userState != USER_STATE::CLOSE_CONNECT)
	{
		pGameUser->Recv();

		if (pGameUser->m_packetManager->m_packetData->userState != USER_STATE::CLOSE_CONNECT)
		{
			pGameUser->Initialize();

			pGameUser->m_systemFrame->CheckPacket(pGameUser->m_packetManager);

			pGameUser->Send();
		}
	}

	GameRoomManager::getInstance()->m_mutex.lock();
	switch (pGameUser->m_state)
	{
	case USER_STATE::USER_ROOM:
	case USER_STATE::USER_PLAY_GAME:
		GameRoomManager::getInstance()->ExitRoom(pGameUser->m_packetManager->m_userId);
		break;
	}

	printf("%s:[%d]와 통신 종료\n", inet_ntoa(pGameUser->m_cliaddr.sin_addr), ntohs(pGameUser->m_cliaddr.sin_port));
	pGameUser->m_state = USER_STATE::CLOSE_CONNECT;
	GameRoomManager::getInstance()->m_mutex.unlock();

	return 0;
}