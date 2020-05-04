#include "stdafx.h"
#include "SystemFrame.h"
#include "GameMapSystem.h"

#include "GameUser.h"
#include "PacketManager.h"
#include "GameRoomManager.h"

GameMapSystem::GameMapSystem()
{
}

GameMapSystem::~GameMapSystem()
{
}

void GameMapSystem::CheckPacket(void* packetManager)
{
	PacketManager* pPacketManager = (PacketManager*)packetManager;

	switch (pPacketManager->m_packetData->userState)
	{
	case USER_STATE::USER_MAP_UPLOAD:
		UploadRoomMapData(pPacketManager);
		break;
	case USER_STATE::USER_MAP_DOWNLOAD:
		break;
	}
}

void GameMapSystem::UploadRoomMapData(PacketManager* pPacketManager)
{
	GameRoomManager::getInstance()->UploadMapData(pPacketManager);
}