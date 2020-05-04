#pragma once
class SystemFrame;
class PacketManager;

class LobbySystem : public SystemFrame
{
public:
	LobbySystem();
	~LobbySystem();

	virtual void CheckPacket(void*);

	void LobbyCreateRoom(PacketManager*);
	void LobbyEnterRoom(int, PacketManager*);
};