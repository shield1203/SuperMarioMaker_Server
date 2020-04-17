#pragma once
#define MAX_NICK_NAME 25
#define MAX_MAP_NAME 25

struct PacketData
{
	USER_STATE userState = USER_STATE::USER_LOGIN;
	unsigned short size = 0;
	char data[MAX_DATA_LEN] = "";
};

// Login
struct LoginData
{
	int userId = 0;
	char userNickName[MAX_NICK_NAME];
};

// Lobby
enum class USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_ROOM, LOBBY_ENTER_ROOM, LOBBY_BACK_TITLE };

struct LobbyData
{
	USER_LOBBY userReq = USER_LOBBY::LOBBY_IDLE;
	bool bEnterRoom = true;
	int ownerUserId = 0;
	char mapName[MAX_MAP_NAME];
};

struct LobbyData_GameRoom
{
	int ownerUserId = 0;
	char ownerUserNickName[MAX_NICK_NAME];
	char mapName[MAX_MAP_NAME];
	int userCount = 0;
};

// Room
enum class USER_ROOM : unsigned int { ROOM_IDLE, ROOM_BACK_LOBBY, ROOM_GAME_START };

struct GameRoomData
{
	USER_ROOM userReq = USER_ROOM::ROOM_IDLE;
	int ownerUserId = 0;
	bool bReady = false;
};

class PacketManager
{
public:
	int m_userId = 0;
	string m_userNickName = "";
	int m_ownerUserId = 0;

	PacketData* m_packetData = nullptr;
	LoginData* m_loginData = nullptr;
	LobbyData* m_lobbyData = nullptr;
	GameRoomData* m_gameRoomData = nullptr;
public:
	// Recv
	void CopyPacket(PacketData*);
	void GetData(USER_STATE);
	void GetLoginData();
	void GetLobbyData();
	void GetGameRoomData();

	// Send
	void SetPacket(USER_STATE);
	void SetLobbyData();
	void SetGameRoomData();

	PacketManager();
	~PacketManager();
};

