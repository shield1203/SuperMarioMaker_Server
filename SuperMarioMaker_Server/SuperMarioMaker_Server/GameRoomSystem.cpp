#include "stdafx.h"
#include "SystemFrame.h"
#include "GameRoomSystem.h"

#include "GameUser.h"
#include "PacketManager.h"
#include "GameRoomManager.h"

GameRoomSystem::GameRoomSystem()
{
}

GameRoomSystem::~GameRoomSystem()
{
}

void GameRoomSystem::CheckPacket(void* packetManager)
{
	PacketManager* pPacketManager = (PacketManager*)packetManager;

	switch (pPacketManager->m_gameRoomData->userReq)
	{
	case USER_ROOM::ROOM_BACK_LOBBY:
		BackLobby(pPacketManager->m_userId);
		break;
	case USER_ROOM::ROOM_GAME_START:
		if (pPacketManager->m_userId == pPacketManager->m_ownerUserId)
		{
			GameStart(pPacketManager->m_userId);
		}
		break;
	}
}

void GameRoomSystem::BackLobby(int userId)
{
	GameRoomManager::getInstance()->ExitRoom(userId);
}

void GameRoomSystem::GameStart(int ownerUserId)
{
	GameRoomManager::getInstance()->GameStartRoom(ownerUserId);
}