#pragma once
class SystemFrame;
class PacketManager;

class GameRoomSystem : public SystemFrame
{
public:
	GameRoomSystem();
	~GameRoomSystem();

	virtual void CheckPacket(void*);

	void BackLobby(int);
	void GameStart(int);
};