#pragma once
class SystemFrame;
class PacketManager;

class GamePlaySystem : public SystemFrame
{
public:
	GamePlaySystem();
	~GamePlaySystem();

	virtual void CheckPacket(void*);

	void GameClear(int);
};