#include "stdafx.h"
#include "GameRoomManager.h"
#include "PacketManager.h"

GameRoomManager* GameRoomManager::Inst = nullptr;

GameRoomManager* GameRoomManager::getInstance()
{
	if (Inst == nullptr) {
		Inst = new GameRoomManager();
	}

	return Inst;
}

GameRoomManager::GameRoomManager()
{
}

GameRoomManager::~GameRoomManager()
{
	for (auto room : m_roomList)
	{
		room->gameUserList.clear();

		SafeDelete(room);
	}
	m_roomList.clear();

	Inst = nullptr;
}

void GameRoomManager::CreateGameRoom(PacketManager* ownerUser)
{
	m_mutex.lock();

	GameRoom* newGameRoom = new GameRoom;
	ownerUser->m_ownerUserId = ownerUser->m_userId;
	newGameRoom->gameUserList.push_back(ownerUser);

	// Room Info
	newGameRoom->ownerUserId = ownerUser->m_userId;
	newGameRoom->ownerUserNickName = ownerUser->m_userNickName;
	newGameRoom->mapName = ownerUser->m_lobbyData->mapName;

	ownerUser->m_lobbyData->ownerUserId = ownerUser->m_userId;

	m_roomList.push_back(newGameRoom);

	m_mutex.unlock();
}

bool GameRoomManager::EnterRoom(int ownerUserId, PacketManager* gameUser)
{
	m_mutex.lock();

	for (auto i : m_roomList)
	{
		if (i->ownerUserId == ownerUserId)
		{
			if (i->gameUserList.size() < MAX_ROOM_USER_COUNT)
			{
				gameUser->m_ownerUserId = ownerUserId;
				gameUser->m_lobbyData->bEnterRoom = true;
				i->gameUserList.push_back(gameUser);
				return true;
			}
			else
			{
				gameUser->m_lobbyData->bEnterRoom = false;
				return false;
			}
		}
	}

	gameUser->m_lobbyData->bEnterRoom = false;

	m_mutex.unlock();

	return false;
}

void GameRoomManager::ExitRoom(int userId)
{
	m_mutex.lock();

	for (auto room = m_roomList.begin(); room != m_roomList.end();)
	{
		if ((*room)->ownerUserId == userId)
		{
			for (auto user = (*room)->gameUserList.begin(); user != (*room)->gameUserList.end();)
			{
				printf("유저 [%d]가 [%d]의 게임룸에서 퇴장\n", (*user)->m_userId, (*room)->ownerUserId);
				(*user)->m_ownerUserId = 0;
				user = (*room)->gameUserList.erase(user++);
			}
		}
		else
		{
			for (auto user = (*room)->gameUserList.begin(); user != (*room)->gameUserList.end();)
			{
				if ((*user)->m_userId == userId)
				{
					printf("유저 [%d]가 [%d]의 게임룸에서 퇴장\n", userId, (*room)->ownerUserId);
					(*user)->m_ownerUserId = 0;
					user = (*room)->gameUserList.erase(user++);
					break;
				}
				else
				{
					user++;
				}
			}
		}

		if ((*room)->gameUserList.empty())
		{
			printf("[%d]의 게임룸 제거\n", (*room)->ownerUserId);
			room = m_roomList.erase(room++);
			break;
		}

		room++;
	}

	m_mutex.unlock();
}

void GameRoomManager::GameStartRoom(int ownerUserId)
{
	m_mutex.lock();

	printf("[%d]방 게임시작\n", ownerUserId);

	for (auto room : m_roomList)
	{
		if (room->ownerUserId == ownerUserId)
		{
			room->bGameStart = true;
			break;
		}
	}

	m_mutex.unlock();
}