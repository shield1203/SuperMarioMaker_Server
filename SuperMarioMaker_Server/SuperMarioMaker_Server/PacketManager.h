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
	char userNickName[MAX_NICK_NAME] = "";
};

// Lobby
enum class USER_LOBBY : unsigned int { LOBBY_IDLE, LOBBY_CREATE_ROOM, LOBBY_ENTER_ROOM, LOBBY_BACK_TITLE };

struct LobbyData
{
	USER_LOBBY userReq = USER_LOBBY::LOBBY_IDLE;
	bool bEnterRoom = true;
	int ownerUserId = 0;
	char mapName[MAX_MAP_NAME] = "";
};

struct LobbyData_GameRoom
{
	int ownerUserId = 0;
	char ownerUserNickName[MAX_NICK_NAME] = "";
	char mapName[MAX_MAP_NAME] = "";
	int userCount = 0;
};

// Up / Download
enum class USER_MAP : unsigned short { MAP_OBJECT, MAP_TILE, MAP_ITEM, MAP_ENEMY };

struct GameMapData
{
	USER_MAP map = USER_MAP::MAP_OBJECT;
	unsigned short col = 0;
	unsigned short row = 0;
	unsigned short kind = 0;
	unsigned short width = 0;
	unsigned short height = 0;
};

// Room
enum class USER_ROOM : unsigned int { ROOM_IDLE, ROOM_BACK_LOBBY, ROOM_GAME_START, ROOM_BACK_TITLE };

// Player
enum PLAYER_TYPE { MARIO, LUIGI, TOAD, TOADETTE };

enum PLAYER_STATE { STATE_SMALL, STATE_BIG, STATE_FIRE, STATE_DIE };

enum DIRECTION { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3 };

enum PLAYER_ACT { ACT_SIT = 0, ACT_IDLE = 2, ACT_RUN = 4, ACT_STOP = 10, ACT_JUMP = 12, ACT_ATTACT = 14 };

struct GameRoomData
{
	USER_ROOM userReq = USER_ROOM::ROOM_IDLE;
	int ownerUserId = 0;
	PLAYER_TYPE type = PLAYER_TYPE::MARIO;
	bool bReady = false;
};

// Play
enum class USER_PLAY : unsigned int { PLAY_GAME, PLAY_RESULT, PLAY_BACK_TITLE };

struct GamePlayData
{
	USER_PLAY userReq = USER_PLAY::PLAY_GAME;

	PLAYER_TYPE type;
	PLAYER_STATE state;
	DIRECTION direction;
	PLAYER_ACT act;

	int xPos;
	int yPos;

	bool jump = false;
	float speed = 0;
	float jumpForce = 0;

	unsigned int imageCount = 0;
	unsigned int time = 0;

	bool change = false;
	unsigned int changeCount = 0;
	PLAYER_STATE prevState;

	bool respawn = false;
	unsigned int respawnCount = 0;
	bool gameOver = false;

	bool goal = false;
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
	GamePlayData* m_gamePlayData = nullptr;
public:
	// Recv
	void CopyPacket(PacketData*);
	void GetData(USER_STATE);

	void GetLoginData();
	void GetLobbyData();
	void GetGameRoomData();
	void GetGamePlayData();

	// Send
	void SetPacket(USER_STATE);

	void SetLobbyData();
	void SetMapData();
	void SetGameRoomData();
	void SetGamePlayData();

	PacketManager();
	~PacketManager();
};

