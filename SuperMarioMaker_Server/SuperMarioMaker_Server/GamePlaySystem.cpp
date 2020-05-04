#include "stdafx.h"
#include "SystemFrame.h"
#include "GamePlaySystem.h"

#include "GameUser.h"
#include "PacketManager.h"
#include "GameRoomManager.h"


GamePlaySystem::GamePlaySystem()
{
}

GamePlaySystem::~GamePlaySystem()
{
}

void GamePlaySystem::CheckPacket(void* packetManager)
{
	PacketManager* pPacketManager = (PacketManager*)packetManager;

	switch (pPacketManager->m_gamePlayData->userReq)
	{
	case USER_PLAY::PLAY_RESULT:
		GameClear(pPacketManager->m_ownerUserId);

		pPacketManager->m_gameRoomData->userReq = USER_ROOM::ROOM_IDLE;
		pPacketManager->m_gameRoomData->bReady = false;
		break;
	}
}

void GamePlaySystem::GameClear(int ownerUserId)
{
	GameRoomManager::getInstance()->GameClear(ownerUserId);
}