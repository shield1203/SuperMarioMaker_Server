#include "stdafx.h"
#include "SystemFrame.h"
#include "LoginSystem.h"

#include "GameUser.h"
#include "PacketManager.h"

LoginSystem::LoginSystem()
{
}

LoginSystem::~LoginSystem()
{
}

void LoginSystem::CheckPacket(void* packetManager)
{
	PacketManager* pPacketManager = (PacketManager*)packetManager;

	SetUserId(pPacketManager);
}

void LoginSystem::SetUserId(PacketManager* packet)
{
	packet->m_userId = packet->m_loginData->userId;
	packet->m_userNickName = packet->m_loginData->userNickName;
}