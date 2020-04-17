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
	printf("���� [%d]�� ���ӷ� ����\n", packetManager->m_userId);
}

void LobbySystem::LobbyEnterRoom(int ownerUserId, PacketManager* packetManager)
{
	bool bEnter = GameRoomManager::getInstance()->EnterRoom(ownerUserId, packetManager);

	if (bEnter)
	{
		printf("[%d]������ [%d]�� ���ӷ� ���强��\n", packetManager->m_userId, ownerUserId);
	}
	else
	{
		printf("[%d]������ [%d]�� ���ӷ� �������\n", packetManager->m_userId, ownerUserId);
	}
	packetManager->m_lobbyData->bEnterRoom = bEnter;
}