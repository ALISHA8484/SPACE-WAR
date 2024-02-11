#include<thread>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include <conio.h>
#include <Windows.h>
#include <locale.h>
#include <io.h>
#include <fcntl.h>
#include<mmsystem.h>

#ifndef ColorsDefine
#define Red "\x1b[38;2;255;0;0m"
#define Blue "\x1b[38;2;0;191;255m"
#define Green "\x1b[38;2;0;255;0m"
#define Yellow "\x1b[38;2;255;255;0m"
#define Purple "\x1b[38;2;128;0;128m"
#define Orange "\x1b[38;2;255;165;0m"
#define Pink "\x1b[38;2;248;160;184m"
#define Cyan "\x1b[38;2;160;184;248m"
#define BrightRed "\x1b[38;2;248;96;96m"
#define BrightBlue "\x1b[38;2;96;96;248m"
#define lavenderblush "\x1b[38;2;49;79;79m"
#define Brown "\x1b[38;2;139;69;19m" 
#define Reset "\x1b[0m"
#endif

#define Bulletspeed 5
#define Playertspeed 30

#ifdef _WIN32
#include <windows.h>
#define settxt _setmode(_fileno(stdout), _O_TEXT) 
#define setu16 _setmode(_fileno(stdout), _O_U16TEXT) 
#define SSleep(x) Sleep(1000 * (x))
void color(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}
#endif

#pragma warning (disable:4996)
#pragma comment(lib,"winmm.lib")

//map==0 ' '
//map==1 player1 location
//map==2 player2 location
//map==3 bullet location
//map==4 grenade
//map==5 heart 
//map==6 trap
//map==6 power boost
//map==8 mirror
//map==9 wall
//map==10 ghost
//map==11 teleport1
//map==22 teleport2 

using namespace std;

typedef struct player {
	char name[30] = { '\0' };
	int win = 0;

	int i;
	int j;
	COORD firstcoord;

	int health;
	int power;
	int ghostmode;
	char lastmovement;
	int grenade;
};      //players elements in game
typedef struct profile {
	int id;
	char username[30] = { '\0' };
	char email[30] = { '\0' };
	char password[30] = { '\0' };
	int point;
};       //username,password and email address of my players
typedef struct users {
	struct profile data;
	struct users* next;
};                        //my link list


typedef struct bullet {
	int i;
	int j;
	int damage;
	char side;
	int greanade;
	int ghost;
	bullet* next;
};


typedef struct history {

	int fplayerID;
	int splayerID;
	int firstwinnerID;
	int secondwinnerID;
	int thirdwinnerID;
	int totalwinnerID;
};