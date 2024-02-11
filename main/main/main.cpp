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

///////////////////////////////////////////////////////////////////////////////

void inputEROR() {
	system("cls||clear");
	printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\ \033[1;31m% WRONG INPUTE!!!!TRY AGAIN\033[0m");
	Sleep(1500);
	system("cls||clear");
}
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
void printwelcome() {

	printf(Blue);
	printf("                           oooo  oooo                                                  \n");
	printf("                           `888  `888                                                  \n");
	printf("oooo oooo    ooo  .ooooo.   888   888   .ooooo.    .ooooo.  ooo. .oo.  .oo.    .ooooo.  \n");
	printf(" `88. `88.  .8'  d88' `88b  888   888  d88' ` `Y  d88' `88b `888P`Y88bP`Y88b  d88' `88b \n");
	printf("  `88..]88..8'   888ooo888  888   888  888        888   888  888   888   888  888ooo888 \n");
	printf("   `888'`888'    888     .  888   888  888   .o8  888   888  888   888   888  888     . \n");
	printf("    `8'  `8'     `Y8bod8P' o888o o888o `Y8bod8P'  `Y8bod8P' o888o o888o o888o `Y8bod8P' \n");
	printf("                                                                                       \n");
	printf(Reset);
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
void savedata(users* head) {

	FILE* ppp;
	fopen_s(&ppp, "E:\\programing\\space war\\FILE\\users.bin", "wb");
	while (head != NULL)
	{
		fwrite(&head->data, sizeof(struct profile), 1, ppp);
		head = head->next;
	}
	fclose(ppp);
}


void signup(users** head) {
	system("cls||clear");
	printf(Green);
	printf("enter 1 to exit\n");
	printf("please enter your user name:");
	printf(Reset);
	/*int id = 0;*/
	char inputeusername[30] = { '\0' };
	char emailaddress[30] = { '\0' };
	char inputepassword[30] = { '\0' };
	char confirmpassword[30] = { '\0' };

	users* temp;
	temp = *head;
	scanf("%s", inputeusername);
	getchar();
	if (strcmp(inputeusername, "1") == 0) {
		system("cls||clear");
		return;
	}
	for (;;) {
		int t = strcmp(temp->data.username, inputeusername);
		if (t == 0) {
			system("cls||clear");
			printf(Red);
			printf("This user name is already exist");
			printf(Reset);
			Sleep(2000);
			signup(head);
			return;
		}
		if (temp->next == NULL) {
			break;
		}
		temp = temp->next;
	}
	system("cls||clear");
	printf(Yellow);
	printf("Hiiiiii %s", inputeusername);
	printf(Reset);
	Sleep(2000);
	for (;;) {
		system("cls||clear");
		printf(Green);
		printf("please enter your password:\n");
		printf(Reset);
		for (int i = 0; i < 30; ++i) {
			inputepassword[i] = '\0';
		}
		for (int i = 0; i < 30; ++i) {
			confirmpassword[i] = '\0';
		}
		for (int i = 0;;) {
			char t = _getch();
			if (t == '\r') {
				inputepassword[i] = '\0';
				break;
			}
			else if (t == '\b' && i > 0) {
				printf("\b \b");
				i--;
				continue;
			}
			if (t == '\b' && i == 0)continue;
			printf("*");
			inputepassword[i] = t;
			i++;
		}
		if (strlen(inputepassword) < 8) {
			system("cls||clear");
			printf(Red);
			printf("your password is too short!\nit must be more than 8 characters");
			printf(Reset);
			Sleep(2000);
			system("cls||clear");
			continue;
		}
		printf("\n");
		printf(Green);
		printf("please confirm your password\n");
		printf(Reset);
		for (int i = 0;; ) {
			char t = _getch();
			if (t == '\r') {
				confirmpassword[i] = '\0';
				break;
			}
			else if (t == '\b' && i > 0) {
				printf("\b \b");
				i--;
				continue;
			}
			if (t == '\b' && i == 0)continue;
			printf("*");
			confirmpassword[i] = t;
			i++;
		}

		if (strcmp(inputepassword, confirmpassword) != 0) {
			system("cls||clear");
			printf(Red);
			printf("password confirmation failed");
			printf(Reset);
			Sleep(2000);
			continue;
		}
		system("cls||clear");
		printf(Green);
		printf("please enter your email\n");
		printf(Reset);
		gets_s(emailaddress);
		system("cls||clear");
		printf(Pink);
		printf("your account successfully created");
		printf(Reset);
		Sleep(2000);
		system("cls||clear");
		break;
	}
	if (temp->data.id != 0) {
		users* newnode = (users*)malloc(sizeof(users));
		int maxID = 0;
		users* maxfinder = *head;
		for (; maxfinder != NULL;) {
			if (maxfinder->data.id > maxID)maxID = maxfinder->data.id;
			maxfinder = maxfinder->next;
		}
		strcpy_s(newnode->data.username, inputeusername);
		strcpy_s(newnode->data.password, inputepassword);
		strcpy_s(newnode->data.email, emailaddress);
		newnode->data.id = (maxID + 1);
		temp->next = newnode;
		newnode->next = NULL;
		newnode->data.point = 0;
		return;
	}
	/*id++;*/
	strcpy_s(temp->data.username, inputeusername);
	strcpy_s(temp->data.password, inputepassword);
	strcpy_s(temp->data.email, emailaddress);
	temp->data.id = 1;
	temp->data.point = 0;
	*head = temp;
}
profile signin(users** head) {
	for (;;) {
		system("cls||clear");
		printf(Blue);
		printf("please enter your user name: ");
		printf(Reset);
		char inputusername[20];
		gets_s(inputusername);
		users* temp;
		temp = *head;
		int userfound = 0;
		for (; temp != NULL;) {
			if (strcmp(temp->data.username, inputusername) != 0) {
				temp = temp->next;
				continue;
			}
			userfound = 1;
			break;
		}
		if (userfound == 0) {
			system("cls||clear");
			printf(Red);
			printf("user not found!");
			printf(Reset);
			Sleep(2000);
			continue;
		}
		system("cls||clear");
		printf(Yellow);
		printf("Hiiii %s", inputusername);
		printf(Reset);
		Sleep(2000);
		for (;;) {
			system("cls||clear");
			printf(Blue);
			printf("1 - Enter password\n\n2 - Forgot PASSWORD\n\n");
			printf(Reset);
			char choice[50];
			gets_s(choice);
			if (strlen(choice) > 1) {
				inputEROR();
				continue;
			}
			if (choice[0] == '1')
			{
				for (;;) {
					char inputepassword[30] = { '\0' };
					system("cls||clear");
					printf(Blue);
					printf("Please enter your password:\n ");
					printf(Reset);
					for (int i = 0;; ) {
						char t = _getch();

						if (t == '\r') {
							inputepassword[i] = '\0';
							break;
						}
						else if (t == '\b' && i > 0) {
							printf("\b \b");
							i--;
							continue;
						}
						if (t == '\b' && i == 0)continue;
						printf("*");
						inputepassword[i] = t;
						i++;
					}
					system("cls||clear");
					if (strcmp(inputepassword, temp->data.password) != 0) {
						printf(Red);
						printf("Password is WRONG!\ntry again\n");
						printf(Reset);
						Sleep(2000);
						continue;
					}
					printwelcome();
					Sleep(2000);
					system("cls||clear");
					return temp->data;
				}
			}
			if (choice[0] == '2')
			{
				for (;;) {
					system("cls||clear");
					printf(Blue);
					printf("please enter your email address: ");
					printf(Reset);
					char inputemail[30] = { '\0' };
					gets_s(inputemail);
					if (strcmp(inputemail, temp->data.email) != 0) {
						system("cls||clear");
						printf(Red);
						printf("email address is wrong!\ntry again\n");
						printf(Reset);
						Sleep(2000);
						continue;
					}
					break;
				}
				for (;;) {
					system("cls||clear");
					printf(Green);
					printf("Reset Password\n\n");
					printf(Blue);
					printf("please enter your new password:\n");
					printf(Reset);
					char inputepassword[30] = { '\0' };
					for (int i = 0;; i++) {
						char t = _getch();
						if (t == '\r') {
							inputepassword[i] = '\0';
							break;
						}
						else if (t == '\b' && i > 0) {
							printf("\b \b");
							i--;
							continue;
						}
						if (t == '\b' && i == 0)continue;
						printf("*");
						inputepassword[i] = t;
					}
					if (strlen(inputepassword) < 8) {
						system("cls||clear");
						printf(Red);
						printf("your password is too short!\nit must be more than 8 characters");
						printf(Reset);
						Sleep(2000);
						system("cls||clear");
						continue;
					}
					printf("\n");
					printf(Blue);
					printf("please confirm your password:\n");
					printf(Reset);
					char confirmpassword[30] = { '\0' };
					for (int i = 0;; i++) {
						char t = _getch();
						if (t == '\r') {
							confirmpassword[i] = '\0';
							break;
						}
						else if (t == '\b' && i > 0) {
							printf("\b \b");
							i--;
							continue;
						}
						if (t == '\b' && i == 0)continue;
						printf("*");
						confirmpassword[i] = t;
					}
					int t = strcmp(inputepassword, confirmpassword);
					if (t != 0) {
						system("cls||clear");
						printf(Red);
						printf("password confirmation failed");
						printf(Reset);
						Sleep(2000);
						system("cls||clear");
						continue;
					}
					system("cls||clear");
					printf(Green);
					printf("password changed");
					printf(Reset);
					Sleep(2000);
					system("cls||clear");
					strcpy_s(temp->data.password, inputepassword);
					return temp->data;
				}




			}
			else {
				inputEROR();
				continue;
			}



		}
	}
}
void Changeuserinformation(users** head, users* temp) {
	FILE* hhh;
	fopen_s(&hhh, "E:\\programing\\space war\\FILE\\history.bin", "r+b");
	for (;;) {

		system("cls||clear");
		printf("1-Change username\n\n2-Change password\n\n3-Change email address\n\n4-back\n");
		char choice[50];
		gets_s(choice);
		if (strlen(choice) > 1) {
			inputEROR();
			continue;
		}
		if (choice[0] == '1') {
			for (;;) {
				system("cls||clear");
				printf("please enter your new username:");
				char inputeusername[30] = { '\0' };
				gets_s(inputeusername);
				users* check = *head;
				int change = 0;
				for (;;) {
					if (strcmp(inputeusername, temp->data.username) == 0) {
						system("cls||clear");
						printf("This is your username!\nchange it :) ");
						Sleep(2000);
						break;
					}

					if (strcmp(check->data.username, inputeusername) == 0) {
						system("cls||clear");
						printf("This user name is already exist");
						Sleep(2000);
						break;
					}

					if (check->next == NULL) {
						change = 1;
						break;
					}
					check = check->next;
				}
				if (change == 0) {
					continue;
				}

				strcpy(temp->data.username, inputeusername);
				system("cls||clear");
				printf("User name changed");
				Sleep(2000);
				break;
			}

		}


		else if (choice[0] == '2') {

			for (;;) {
				system("cls||clear");
				printf("please enter your new password:\n");
				char inputepassword[30] = { '\0' };
				for (int i = 0;; i++) {
					char t = _getch();
					if (t == '\r') {
						break;
					}
					printf("*");
					inputepassword[i] = t;
				}
				if (strlen(inputepassword) < 8) {
					system("cls||clear");
					printf("your password is too short!\nit must be more than 8 characters");
					Sleep(2000);
					system("cls||clear");
					continue;
				}
				printf("\n");
				printf("please confirm your password:\n");
				char confirmpassword[30] = { '\0' };
				for (int i = 0;; i++) {
					char t = _getch();
					if (t == '\r') {
						break;
					}
					printf("*");
					confirmpassword[i] = t;
				}
				if (strcmp(inputepassword, confirmpassword) != 0) {
					system("cls||clear");
					printf("password confirmation failed");
					Sleep(2000);
					system("cls||clear");
					continue;
				}
				system("cls||clear");
				strcpy_s(temp->data.password, inputepassword);
				printf("password changed");

				Sleep(2000);
				system("cls||clear");
				break;
			}

		}



		else if (choice[0] == '3') {
			char newemail[30] = { '\0' };
			system("cls||clear");
			printf("please enter your new email address:");
			gets_s(newemail);
			strcpy(temp->data.email, newemail);
			system("cls||clear");
			printf("your email updated");
			Sleep(2000);
		}
		else if (choice[0] == '4') {
			return;
		}
		else {
			inputEROR();
			continue;
		}
	}








}
profile player2login(users** head, char username[30]) {
	for (;;) { 
		profile player2;
		system("cls||clear");
		printf(Pink);
		printf("Player2 login:\n\n");
		printf("1-Sign in\n\n2-Sign up\n\n");
		printf(Reset);
		char choice[50];
		scanf("%s", choice);
		getchar();
		if (strlen(choice) > 1) {
			inputEROR();
			continue;
		}
		if (choice[0] == '1') {
			player2 = signin(head);
			if (strcmp(player2.username, username) == 0) {
				system("cls||clear");
				printf(Red);
				printf("This user is player1!\n\ntry again");
				printf(Reset);
				Sleep(2000);
				continue;
			}
			return player2;
		}
		else if (choice[0] == '2') {
			system("cls||clear");
			signup(head);
			users* temp = *head;
			for (; temp->next != NULL;) {
				temp = temp->next;
			}
			strcpy(player2.username, temp->data.username);
			strcpy(player2.email, temp->data.email);
			strcpy(player2.password, temp->data.password);
			player2.id = temp->data.id;
			player2.point = temp->data.point;
			return player2;
		}
		else {
			inputEROR();
			continue;
		}

	}
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
int gamemenu(users** head, char username[30]) {
	users* temp;
	temp = *head;
	for (;;) {
		if (strcmp(temp->data.username, username) == 0) {
			break;
		}
		temp = temp->next;
	}
	for (;;) {
		system("cls||clear");
		printf(Purple);
		printf("GAME MENU\n\n1-start game\n\n2-Change user information\n\n3-history\n\n4-Return to login page\n");
		printf(Reset);
		char choice[50];
		gets_s(choice);
		if (strlen(choice) > 1) {
			inputEROR();
			continue;
		}
		else if (choice[0] == '1') {
			return 1;
		}
		else if (choice[0] == '2') {
			Changeuserinformation(head, temp);
			savedata(*head);
			continue;
		}
		else if (choice[0] == '3') {
			
		}
		else if (choice[0] == '4') {
			return 0;
		}
		else {
			inputEROR();
			continue;
		}
	}



}

///////////////////////////////////////////////////////////////////////////////

int  map1[25][100];


void converttochar1(int i, int j) {
	if (map1[i][j] == 9) {
		printf(lavenderblush);
		printf("%c", 219);
		printf(Reset);
	}
	if (map1[i][j] == 8) {
		printf(Yellow);
		printf("%c", 186);
		printf(Reset);
	}
	if (map1[i][j] == 0) {
		printf("%c", ' ');
	}
	if (map1[i][j] == 1) {
		printf(Blue);
		setu16;
		wprintf(L"ʘ");
		settxt;
		printf(Reset);
	}
	if (map1[i][j] == 2) {
		printf(Red);
		setu16;
		wprintf(L"ʘ");
		settxt;
		printf(Reset);
	}
	if (map1[i][j] == 3) {
		printf(Orange);
		setu16;
		wprintf(L"•");
		settxt;
		printf(Reset);
	}
	if (map1[i][j] == 4) {
		printf(Green);
		setu16;
		wprintf(L"Ơ");
		settxt;
		printf(Reset);
	}
	if (map1[i][j] == 5) {

		setu16;
		wprintf(L"❤");
		settxt;

	}
	if (map1[i][j] == 6) {

		printf("@");
	}
	if (map1[i][j] == 11) {
		printf(Yellow);
		setu16;
		wprintf(L"֍");
		settxt;
		printf(Reset);

	}
	if (map1[i][j] == 22) {

		printf(Pink);
		setu16;
		wprintf(L"֍");
		settxt;
		printf(Reset);

	}
	if (map1[i][j] == 7) {
		printf(Orange);
		setu16;
		wprintf(L"✶");
		settxt;
		printf(Reset);

	}
	if (map1[i][j] == 10) {

		setu16;
		wprintf(L"ᦪ");
		settxt;

	}
}

//////////////////////////////////////////////////////////////////////////////

void creatmap1() {

	for (int i = 0; i <= 24; i++) {
		for (int j = 0; j <= 99; j++) {
			if (i == 0 || i == 24) {
				map1[i][j] = 9;
			}
			if (j == 0 || j == 99) {
				map1[i][j] = 9;
			}
			if (i == 1) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 2) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
				if (j == 50) {
					map1[i][j] = 10;
				}
			}
			if (i == 3) {
				if (j == 20 || j == 79) {
					map1[i][j] = 9;
				}
				else if (j == 21) {
					map1[i][j] = 8;
				}
				else if (j == 90) {
					map1[i][j] = 11;
				}
				else if (j == 10) {
					map1[i][j] = 22;
				}
			}
			if (i == 4) {
				if (j == 20 || j == 79) {
					map1[i][j] = 9;
				}
				else if (j == 21) {
					map1[i][j] = 8;
				}
				else if (j == 50) {
					map1[i][j] = 5;
				}
				else if (j == 83) {
					map1[i][j] = 6;
				}
			}
			if (i == 5) {
				if (j == 20 || j == 79) {
					map1[i][j] = 9;
				}
				else if (j == 21) {
					map1[i][j] = 8;
				}
			}
			if (i == 6) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 7) {
				if (j <= 13 && j >= 8) {
					map1[i][j] = 9;
				}
				if (j <= 91 && j >= 86) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 8)
			{
				if (j == 13) {
					map1[i][j] = 9;
				}
				if (j == 14) {
					map1[i][j] = 8;
				}
				if (j == 50) {
					map1[i][j] = 9;
				}
				if (j == 85) {
					map1[i][j] = 8;
				}
				if (j == 86) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 9)
			{
				if (j == 13) {
					map1[i][j] = 9;
				}
				if (j == 14) {
					map1[i][j] = 8;
				}
				if (j == 50) {
					map1[i][j] = 9;
				}
				if (j == 85) {
					map1[i][j] = 8;
				}
				if (j == 86) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 10) {
				if (j == 13) {
					map1[i][j] = 9;
				}
				if (j == 29) {
					map1[i][j] = 7;
				}
				if (j == 50) {
					map1[i][j] = 9;
				}
				if (j >= 58 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j == 86) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 11) {
				if (j == 13) {
					map1[i][j] = 9;
				}
				if (j == 50) {
					map1[i][j] = 9;
				}
				if (j == 86) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 12) {
				if (j == 13) {
					map1[i][j] = 9;
				}
				if (j == 50) {
					map1[i][j] = 8;
				}
				if (j == 86) {
					map1[i][j] = 9;
				}
				if (j == 0) {
					map1[i][j] = 9;
				}
				if (j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 13) {
				if (j == 13) {
					map1[i][j] = 9;
				}
				if (j == 50) {
					map1[i][j] = 9;
				}
				if (j == 86) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 14) {
				if (j == 13) {
					map1[i][j] = 9;
				}
				if (j == 50) {
					map1[i][j] = 9;
				}
				if (j == 86) {
					map1[i][j] = 9;
				}
				if (j <= 42 && j >= 35) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 15) {
				if (j == 13) {
					map1[i][j] = 9;
				}
				if (j == 14) {
					map1[i][j] = 8;
				}
				if (j == 50) {
					map1[i][j] = 9;
				}
				if (j == 85) {
					map1[i][j] = 8;
				}
				if (j == 86) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 16) {
				if (j == 13) {
					map1[i][j] = 9;
				}
				if (j == 14) {
					map1[i][j] = 8;
				}
				if (j == 50) {
					map1[i][j] = 9;
				}
				if (j == 85) {
					map1[i][j] = 8;
				}
				if (j == 86) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 17) {
				if (j <= 13 && j >= 8) {
					map1[i][j] = 9;
				}
				if (j <= 91 && j >= 86) {
					map1[i][j] = 9;

				}
				if (j == 50) {
					map1[i][j] = 4;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
				else if (j == 17) {
					map1[i][j] = 6;
				}
			}
			if (i == 18) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 19) {
				if (j == 20 || j == 79) {
					map1[i][j] = 9;
				}

				else if (j == 78) {
					map1[i][j] = 8;
				}
			}
			if (i == 20) {
				if (j == 20 || j == 79) {
					map1[i][j] = 9;
				}
				else if (j == 78) {
					map1[i][j] = 8;
				}
				if (j == 56) {
					map1[i][j] = 7;
				}
			}
			if (i == 21) {
				if (j == 20 || j == 79) {
					map1[i][j] = 9;
				}
				else if (j == 10) {
					map1[i][j] = 11;
				}
				else if (j == 90) {
					map1[i][j] = 22;
				}
				else if (j == 78) {
					map1[i][j] = 8;
				}
			}
			if (i == 22) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 23) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}

		}
	}
}
int game1(users** head, player* player1, player* player2) {


	creatmap1(); 
	player1->health = 0;
	player2->health = 0;
	system("cls||clear");
	printf(Green);
	printf("Please enter players Hp\n");
	printf(Reset);
	scanf("%d", &player1->health);
	player2->health = player1->health;
	system("cls||clear");



	player1->i = 12;
	player1->j = 8;
	player1->power = 1;
	player1->ghostmode = 0;
	player1->lastmovement = 'd';
	player1->grenade = 0;
	player1->firstcoord.X = 12;
	player1->firstcoord.Y = 8;

	player2->i = 12;
	player2->j = 90;
	player2->power = 1;
	player2->ghostmode = 0;
	player2->lastmovement = 'j';
	player2->grenade = 0;
	player2->firstcoord.X = 12;
	player2->firstcoord.Y = 90;

	int END = 0;

	map1[player1->i][player1->j] = 1;
	map1[player2->i][player2->j] = 2;

	for (int i = 0; i <= 24; i++) {
		for (int j = 0; j <= 99; j++) {

			converttochar1(i, j);

		}
		printf("\n");
	}

}





int main() {

	printf("\033[48;2;128;0;128m");
	firstwellcome();

	struct users* head = readfile();
	player player1;
	player player2;
	for (;;) {
		int choice = firstmenu();
		profile user1;
		if (choice == 1) {
			user1 = signin(&head);
			savedata(head);
			strcpy(player1.name, user1.username);
			player1.win = 0;
		}
		if (choice == 2) {
			signup(&head);
			savedata(head);
			continue;
		}
		if (choice == 3) {
			savedata(head);
			system("cls||clear");
			return 0;
		}
		system("cls||clear");
		int y = gamemenu(&head, user1.username);
		if (y == 0) {
			system("cls||clear");
			continue;
		}
		if (y == 1) {

			for (;;) {
				profile user2;
				user2 = player2login(&head, user1.username);
				savedata(head);
				//
				strcpy(player2.name, user2.username);

				player1.win = 0;
				player2.win = 0;

				system("cls||clear");

				for (int i = 0; i < 25; i++) {
					for (int j = 0; j < 100; j++) {
						map1[i][j] = 0;
					}
				}
				int t = game1(&head, &player1, &player2);

			}
				

		}
	}
	exit(0);
}