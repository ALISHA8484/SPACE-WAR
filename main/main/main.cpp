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


void firstwellcome() {
	system("cls||clear");
	printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t");
	char wellcome[30] = " WELLCOME TO  SPACE WAR!!";
	printf(Green);
	for (int i = 0; i < 25; i++) {
		printf("%c", wellcome[i]);
		_sleep(85);
	}
	printf(Reset);
	printf("\n\n\t\t\t\t\t\t\t\t");
	char creator[35] = "DEVELOPED BY ALISHA";
	for (int i = 0; i < 19; i++) {
		printf("\033[1;34m%c\033[0m", creator[i]);
		_sleep(200);
	}
	_sleep(4000);
	system("cls||clear");
}

users* readfile() {
	FILE* ppp;
	fopen_s(&ppp, "E:\\programing\\space war\\FILE\\users.bin", "r+b");
	if (ppp == NULL) {
		fopen_s(&ppp, "E:\\programing\\space war\\FILE\\users.bin", "w+b");
	}
	if (ppp == NULL) {
		printf("opening file failed");
		return 0;
	}
	users temp;
	users* head = NULL;
	for (; fread(&temp.data, sizeof(profile), 1, ppp) > 0;) {
		users* newnode = (users*)malloc(sizeof(users));
		newnode->data = temp.data;
		newnode->next = NULL;
		newnode->data.point = temp.data.point;

		if (head == NULL)
		{
			head = newnode;
		}
		else {
			newnode->next = head;
			head = newnode;
		}
	}
	fclose(ppp);
	if (head == NULL)
	{
		head = (struct users*)malloc(sizeof(struct users));
		head->next = NULL;
		head->data.id = 0;
		head->data.point = 0;
	}
	return head;
}

int firstmenu() {
	for (;;) {
		system("cls||clear");
		printf(Orange);
		printf("MENU\n\n1-Sign in\n\n2-Sign up\n\n3-EXIT:(\n");
		printf(Reset);
		char choice[50];
		gets_s(choice);
		if (strlen(choice) > 1) {
			inputEROR();
			continue;
		}
		switch (choice[0])
		{
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		default:
			inputEROR();
			break;
		}
	}

}

int main() {
	printf("\033[48;2;128;0;128m");
	firstwellcome();
	player player1;
	player player2;
	for (;;) {
		int choice = firstmenu();
		profile user1;
		if (choice == 1) {
			
		}
		if (choice == 2) {
			
		}
		if (choice == 3) {
			system("cls||clear");
			return 0;
		}
		

	}
	exit(0);
}