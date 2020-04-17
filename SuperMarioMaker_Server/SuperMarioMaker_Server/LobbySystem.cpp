#include "stdafx.h"
#include "SystemFrame.h"
#include "LobbySystem.h"

#include "GameUser.h"
#include "PacketManager.h"
#include "GameRoomManager.h"

LobbySystem::LobbySystem()
{
}

LobbySystem::~LobbySystem()
{
}

void LobbySystem::CheckPacket(void* packetManager)
{
	PacketManager* pPacketManager = (PacketManager*)packetManager;

	switch (pPacketManager->m_lobbyData->userReq)
	{
	case USER_LOBBY::LOBBY_CREATE_ROOM:
		LobbyCreateRoom(pPacketManager);
		break;
	case USER_LOBBY::LOBBY_ENTER_ROOM:
		LobbyEnterRoom(pPacketManager->m_lobbyData->ownerUserId, pPacketManager);
		break;
	}
}

void LobbySystem::LobbyCreateRoom(PacketManager* packetManager)
{
	GameRoomManager::getInstance()->CreateGameRoom(packetManager);
	printf("유저 [%d]가 게임룸 생성\n", packetManager->m_userId);
}

void LobbySystem::LobbyEnterRoom(int ownerUserId, PacketManager* packetManager)
{
	bool bEnter = GameRoomManager::getInstance()->EnterRoom(ownerUserId, packetManager);

	if (bEnter)
	{
		printf("[%d]유저가 [%d]의 게임룸 입장성공\n", packetManager->m_userId, ownerUserId);
	}
	else
	{
		printf("[%d]유저가 [%d]의 게임룸 입장실패\n", packetManager->m_userId, ownerUserId);
	}
	packetManager->m_lobbyData->bEnterRoom = bEnter;
}