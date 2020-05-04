#pragma once
class SystemFrame;
class PacketManager;

class GameMapSystem : public SystemFrame
{
public:
	GameMapSystem();
	~GameMapSystem();

	virtual void CheckPacket(void*);

	void UploadRoomMapData(PacketManager*);
};