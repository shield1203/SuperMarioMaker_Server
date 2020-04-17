#pragma once
#define MAX_ROOM_USER_COUNT 4

class PacketManager;

struct GameRoom
{
	int ownerUserId = 0;
	string ownerUserNickName = "";
	string mapName = "";
	bool bGameStart = false;
	list<PacketManager*>gameUserList;
};

class GameRoomManager
{
private:
	static GameRoomManager* Inst;
	GameRoomManager();
public:
	mutex m_mutex;
	list<GameRoom*>m_roomList;
public:
	static GameRoomManager* getInstance();

	void CreateGameRoom(PacketManager*);
	bool EnterRoom(int, PacketManager*);
	void ExitRoom(int);
	void GameStartRoom(int);

	~GameRoomManager();
};