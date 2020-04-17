#include"stdafx.h"
#include"ServerSystem.h"

int main()
{
	system("title KJH's MarioMaker_Server");

	ServerSystem* pServerSystem = new ServerSystem();

	pServerSystem->Process();

	SafeDelete(pServerSystem);
}