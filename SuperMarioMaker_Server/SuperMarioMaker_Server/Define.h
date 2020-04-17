#pragma once
#pragma once

#define SERVER_IP "203.232.193.170"
#define PORT_NUM 3000
#define BLOG_SIZE 10
#define MAX_MSG_LEN 2937
#define MAX_DATA_LEN 2937 

enum class USER_STATE : unsigned int { USER_LOGIN, USER_LOBBY, USER_ROOM, USER_PLAY_GAME, CLOSE_CONNECT };

/////////////
// LINKING //
/////////////
#pragma comment(lib,"ws2_32")

//////////////
// INCLUDES //
//////////////
#include <WinSock2.h>
#include <process.h>
#include <mutex>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>

using namespace std;