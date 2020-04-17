#pragma once
class SystemFrame;
class PacketManager;

class LoginSystem : public SystemFrame
{
public:
	LoginSystem();
	~LoginSystem();

	virtual void CheckPacket(void*);

	void SetUserId(PacketManager*);
};