#include "stdafx.h"
#include "PacketManager.h"
#include "GameRoomManager.h"

PacketManager::PacketManager()
{
	m_packetData = new PacketData;
	m_loginData = new LoginData;
	m_lobbyData = new LobbyData;
	m_gameRoomData = new GameRoomData;
}

PacketManager::~PacketManager()
{
	SafeDelete(m_packetData);
	SafeDelete(m_lobbyData);
}

void PacketManager::CopyPacket(PacketData* packetData)
{
	memcpy(m_packetData, packetData, sizeof(PacketData));
}

void PacketManager::GetData(USER_STATE userState)
{
	switch (userState)
	{
	case USER_STATE::USER_LOGIN:
		GetLoginData();
		break;
	case USER_STATE::USER_LOBBY:
		GetLobbyData();
		break;
	case USER_STATE::USER_MAP_UPLOAD:
		// 게임룸에 저장함
		break;
	case USER_STATE::USER_ROOM:
		GetGameRoomData();
		break;
	case USER_STATE::USER_PLAY_GAME:
		GetGamePlayData();
		break;
	}
}

void PacketManager::GetLoginData()
{
	ZeroMemory(m_loginData, sizeof(LoginData));
	memcpy(m_loginData, m_packetData->data, sizeof(LoginData));
}

void PacketManager::GetLobbyData()
{
	ZeroMemory(m_lobbyData, sizeof(LobbyData));
	memcpy(m_lobbyData, m_packetData->data, sizeof(LobbyData));
}

void PacketManager::GetGameRoomData()
{
	ZeroMemory(m_gameRoomData, sizeof(GameRoomData));
	memcpy(m_gameRoomData, m_packetData->data, sizeof(GameRoomData));
}

void PacketManager::GetGamePlayData()
{
	ZeroMemory(m_gamePlayData, sizeof(GamePlayData));
	memcpy(m_gamePlayData, m_packetData->data, sizeof(GamePlayData));
}

void PacketManager::SetPacket(USER_STATE userState)
{
	switch (userState)
	{
	case USER_STATE::USER_LOGIN:
		// 유저 아이디를 저장하는 스텝이라 보낼 데이터가 없음
		break;
	case USER_STATE::USER_LOBBY:
		SetLobbyData();
		break;
	case USER_STATE::USER_MAP_UPLOAD:
		// 맵 올리는 거라 보낼 데이터 없음
		break;
	case USER_STATE::USER_MAP_DOWNLOAD:
		SetMapData();
		break;
	case USER_STATE::USER_ROOM:
		SetGameRoomData();
		break;
	case USER_STATE::USER_PLAY_GAME:
		SetGamePlayData();
		break;
	}
}

void PacketManager::SetLobbyData()
{
	m_packetData->size = 0;

	memcpy(m_packetData->data, m_lobbyData, sizeof(LobbyData));
	m_packetData->size += static_cast<unsigned short>(sizeof(LobbyData));

	LobbyData_GameRoom* room = new LobbyData_GameRoom;
	for (auto roomList : GameRoomManager::getInstance()->m_roomList)
	{
		room->ownerUserId = roomList->ownerUserId;
		strcpy(room->ownerUserNickName, roomList->ownerUserNickName.c_str());
		strcpy(room->mapName, roomList->mapName.c_str());
		room->userCount = roomList->gameUserList.size();
		memcpy(m_packetData->data + m_packetData->size, room, sizeof(LobbyData_GameRoom));
		m_packetData->size += static_cast <unsigned short>(sizeof(LobbyData_GameRoom));
	}
	SafeDelete(room);
}

void PacketManager::SetMapData()
{
	m_packetData->size = 0;

	for (auto room : GameRoomManager::getInstance()->m_roomList)
	{
		if (room->ownerUserId == m_ownerUserId)
		{
			for (auto mapData : room->gameMapData)
			{
				memcpy(m_packetData->data + m_packetData->size, mapData, sizeof(GameMapData));
				m_packetData->size += static_cast<unsigned short>(sizeof(GameMapData));
			}
			break;
		}
	}
}

void PacketManager::SetGameRoomData()
{
	m_packetData->size = static_cast<unsigned short>(sizeof(GameRoomData));

	bool bRoomExist = true;

	for (auto room : GameRoomManager::getInstance()->m_roomList)
	{
		if (room->ownerUserId == m_ownerUserId)
		{
			if (room->gameUserList.empty())
			{
				bRoomExist = false;
			}

			if (room->bGameStart)
			{
				m_gameRoomData->userReq = USER_ROOM::ROOM_GAME_START;
			}

			int type = 0;
			for (auto user : room->gameUserList)
			{
				user->m_gameRoomData->type = static_cast<PLAYER_TYPE>(type);

				if (user->m_userId == m_userId)
				{
					memcpy(m_packetData->data, m_gameRoomData, sizeof(GameRoomData));
				}
				else
				{
					memcpy(m_packetData->data + m_packetData->size, user->m_gameRoomData, sizeof(GameRoomData));
					m_packetData->size += static_cast<unsigned short>(sizeof(GameRoomData));
				}
				type++;
			}
			break;
		}
	}

	// 방 폭파
	if (!bRoomExist)
	{
		m_ownerUserId = 0;
		m_gameRoomData->userReq = USER_ROOM::ROOM_BACK_LOBBY;

		memcpy(m_packetData->data, m_gameRoomData, sizeof(GameRoomData));
	}
}

void PacketManager::SetGamePlayData()
{
	m_packetData->size = static_cast<unsigned short>(sizeof(GamePlayData));
	
	for (auto room : GameRoomManager::getInstance()->m_roomList)
	{
		if (!room->bGameStart)
		{
			m_gamePlayData->userReq = USER_PLAY::PLAY_RESULT;
		}

		if (room->ownerUserId == m_ownerUserId)
		{
			for (auto user : room->gameUserList)
			{
				if (user->m_userId == m_userId)
				{
					memcpy(m_packetData->data, m_gameRoomData, sizeof(GameRoomData));
				}
				else
				{
					memcpy(m_packetData->data + m_packetData->size, user->m_gameRoomData, sizeof(GameRoomData));
					m_packetData->size += static_cast<unsigned short>(sizeof(GameRoomData));
				}
			}
			break;
		}
	}
}