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

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = y;
	coord.Y = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

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

char* findusernamewithid(users* head, int x) {
	users* temp;
	temp = head;
	for (;;) {
		if (temp->data.id == x) {

			return temp->data.username;
		}
		temp = temp->next;

	}

}
void readhistory(users user, users* head) {
	system("cls||clear");
	FILE* hhh;
	fopen_s(&hhh, "E:\\programing\\space war\\FILE\\history.bin", "rb");
	if (hhh == NULL) {
		system("cls||clear");
		printf(Red);
		printf("History is empty!!!");
		printf(Reset);
		_getch();
		return;
	}
	history temp;
	int playedanygame = 0;
	printf("\tMatchs\t\t First winner\t   Second winner\t Third winner\t    Game winner\n");
	printf(Cyan);
	printf(" -------------------------------------------------------------------------------------------------\n");
	int linenum = 1;
	for (; fread(&temp, sizeof(history), 1, hhh) > 0;) {
		if (temp.fplayerID == user.data.id || temp.splayerID == user.data.id) {
			linenum++;
			char match[40];
			strcpy(match, findusernamewithid(head, temp.fplayerID));
			strcat(match, " vs ");
			strcat(match, findusernamewithid(head, temp.splayerID));
			playedanygame = 1;
			printf(Cyan);
			printf("| ");
			printf(Blue);
			printf("%s", match);
			printf(Cyan);
			gotoxy(linenum, 22);
			printf("|       ");
			if (strcmp(findusernamewithid(head, temp.firstwinnerID), user.data.username) == 0)printf(Green);
			else printf(Red);
			printf("%s", findusernamewithid(head, temp.firstwinnerID));
			printf(Cyan);
			gotoxy(linenum, 41);
			printf("|       ");
			if (strcmp(findusernamewithid(head, temp.secondwinnerID), user.data.username) == 0)printf(Green);
			else printf(Red);
			printf("%s", findusernamewithid(head, temp.secondwinnerID));
			printf(Cyan);
			gotoxy(linenum, 60);
			printf("|       ");
			if (strcmp(findusernamewithid(head, temp.thirdwinnerID), user.data.username) == 0)printf(Green);
			else printf(Red);
			printf("%s", findusernamewithid(head, temp.thirdwinnerID));
			printf(Cyan);
			gotoxy(linenum, 79);
			printf("|       ");
			if (strcmp(findusernamewithid(head, temp.totalwinnerID), user.data.username) == 0)printf(Yellow);
			else printf(Brown);
			printf("%s", findusernamewithid(head, temp.totalwinnerID), user.data.username);
			printf(Cyan);
			gotoxy(linenum, 98);
			printf("|");
			printf("\n");
			printf(" -------------------------------------------------------------------------------------------------\n");
			linenum++;
		}
	}
	fclose(hhh);
	if (playedanygame == 1) {
		printf(Purple);
		printf("\nYour score:   ");
		if (user.data.point >= 0)printf(Yellow);
		else printf(Red);
		printf("%d", user.data.point);
		printf(Reset);
	}
	if (playedanygame == 0) {
		system("cls||clear");
		printf(Red);
		printf("You have not played any games !!!");
		printf(Reset);
	}
	_getch();
	system("cls||clear");
}
void savehistory(history histo) {

	FILE* hhh;
	fopen_s(&hhh, "E:\\programing\\space war\\FILE\\history.bin", "ab");
	fwrite(&histo, sizeof(history), 1, hhh); 
	fclose(hhh); 
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
			readhistory(*temp, *head);
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
COORD changes;

int powercontrol1;
int powercontrol2;

int heartkiton;
int poweron;
int ghostexist;
int grenadeon;


int ghosthold;
int ghostmodeon;
int ghostmodecharge;

int timer1s = 0;
int timer2s = 0;

//////////////////////////////////////////////////////////////////////////////////

void winnername(player playerx) {
	system("cls||clear");
	printf(Yellow);
	printf("%s wons", playerx.name);
	printf(Reset);
	Sleep(2000);
	system("cls||clear");
}

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
void printhealthline(player* player1, player* player2) {
	gotoxy(26, 0);
	if (player1->health > 9) {
		printf("\tHp: % d ", player1->health);
		printf("\t\t\t\t\t\t\t        ");
		printf("Hp: % d ", player2->health);
	}
	else {
		printf("\tHp: % d ", player1->health);
		printf("\t\t\t\t\t\t\t\t        ");
		printf("Hp: % d ", player2->health);
	}
	gotoxy(0, 0);
}
void printdamageline(player* player1, player* player2) {
	gotoxy(27, 0);
	printf("\tpower: % dX ", player1->power);
	printf("\t\t\t\t\t\t\t        ");
	printf("power: % dX \n", player2->power);
	gotoxy(0, 0);
}

void printchanges() {

	int x = changes.X - 1;
	int y = changes.Y - 1;
	gotoxy((changes.X - 1), (changes.Y - 1));
	for (int t = 1; t <= 3; t++) {
		for (int w = 1; w <= 3; w++) {

			converttochar1(x, y);
			y++;
		}
		y -= 3;
		x++;
		gotoxy(x, y);
	}
	gotoxy(0, 0);


}
void whathappend(player* player, int i, int j, int whichplayer) {
	if (player->ghostmode == 0) {
		if (map1[i][j] == 9 || map1[i][j] == 8) {
			return;
		}
		else if (map1[i][j] == 6) {
			map1[player->i][player->j] = 0;
			printchanges();
			if (whichplayer == 1) {
				player->i = player->firstcoord.X;
				player->j = player->firstcoord.Y;
				changes.X = player->firstcoord.X;
				changes.Y = player->firstcoord.Y;
				player->health--;
				gotoxy(26, 13);
				printf("%d", player->health);
				gotoxy(0, 0);
			}
			if (whichplayer == 2) {
				player->i = player->firstcoord.X;
				player->j = player->firstcoord.Y;
				changes.X = player->firstcoord.X;
				changes.Y = player->firstcoord.Y;
				player->health--;
				gotoxy(26, 85);
				printf("%d", player->health);
				gotoxy(0, 0);
			}
		}
		else if (map1[i][j] == 11 || map1[i][j] == 22) {
			map1[player->i][player->j] = 0;
			printchanges();
			if (i == 3) {
				if (j == 10) {
					if (player->lastmovement == 'w' || player->lastmovement == 'W') {
						player->i = 20;
						player->j = 90;
					}
					if (player->lastmovement == 's' || player->lastmovement == 'S') {
						player->i = 22;
						player->j = 90;
					}
					if (player->lastmovement == 'a' || player->lastmovement == 'A') {
						player->i = 21;
						player->j = 89;
					}
					if (player->lastmovement == 'd' || player->lastmovement == 'D') {
						player->i = 21;
						player->j = 91;
					}
					if (player->lastmovement == 'i' || player->lastmovement == 'I') {
						player->i = 20;
						player->j = 90;
					}
					if (player->lastmovement == 'k' || player->lastmovement == 'K') {
						player->i = 22;
						player->j = 90;
					}
					if (player->lastmovement == 'j' || player->lastmovement == 'J') {
						player->i = 21;
						player->j = 89;
					}
					if (player->lastmovement == 'l' || player->lastmovement == 'L') {
						player->i = 21;
						player->j = 91;
					}

				}
				if (j == 90) {
					if (player->lastmovement == 'w' || player->lastmovement == 'W') {
						player->i = 20;
						player->j = 10;
					}
					if (player->lastmovement == 's' || player->lastmovement == 'S') {
						player->i = 22;
						player->j = 10;
					}
					if (player->lastmovement == 'a' || player->lastmovement == 'A') {
						player->i = 21;
						player->j = 9;
					}
					if (player->lastmovement == 'd' || player->lastmovement == 'D') {
						player->i = 21;
						player->j = 11;
					}
					if (player->lastmovement == 'i' || player->lastmovement == 'I') {
						player->i = 20;
						player->j = 10;
					}
					if (player->lastmovement == 'k' || player->lastmovement == 'K') {
						player->i = 22;
						player->j = 10;
					}
					if (player->lastmovement == 'j' || player->lastmovement == 'J') {
						player->i = 21;
						player->j = 9;
					}
					if (player->lastmovement == 'l' || player->lastmovement == 'L') {
						player->i = 21;
						player->j = 11;
					}
				}
			}
			if (i == 21) {
				if (j == 10) {
					if (player->lastmovement == 'w' || player->lastmovement == 'W') {
						player->i = 2;
						player->j = 90;
					}
					if (player->lastmovement == 's' || player->lastmovement == 'S') {
						player->i = 4;
						player->j = 90;
					}
					if (player->lastmovement == 'a' || player->lastmovement == 'A') {
						player->i = 3;
						player->j = 89;
					}
					if (player->lastmovement == 'd' || player->lastmovement == 'D') {
						player->i = 3;
						player->j = 91;
					}
					if (player->lastmovement == 'i' || player->lastmovement == 'I') {
						player->i = 2;
						player->j = 90;
					}
					if (player->lastmovement == 'k' || player->lastmovement == 'K') {
						player->i = 4;
						player->j = 90;
					}
					if (player->lastmovement == 'j' || player->lastmovement == 'J') {
						player->i = 3;
						player->j = 89;
					}
					if (player->lastmovement == 'l' || player->lastmovement == 'L') {
						player->i = 3;
						player->j = 91;
					}

				}
				if (j == 90) {
					if (player->lastmovement == 'w' || player->lastmovement == 'W') {
						player->i = 2;
						player->j = 10;
					}
					if (player->lastmovement == 's' || player->lastmovement == 'S') {
						player->i = 4;
						player->j = 10;
					}
					if (player->lastmovement == 'a' || player->lastmovement == 'A') {
						player->i = 3;
						player->j = 9;
					}
					if (player->lastmovement == 'd' || player->lastmovement == 'D') {
						player->i = 3;
						player->j = 11;
					}
					if (player->lastmovement == 'i' || player->lastmovement == 'I') {
						player->i = 2;
						player->j = 10;
					}
					if (player->lastmovement == 'k' || player->lastmovement == 'K') {
						player->i = 4;
						player->j = 10;
					}
					if (player->lastmovement == 'j' || player->lastmovement == 'J') {
						player->i = 3;
						player->j = 9;
					}
					if (player->lastmovement == 'l' || player->lastmovement == 'L') {
						player->i = 3;
						player->j = 11;
					}
				}
			}
			changes.X = player->i;
			changes.Y = player->j;
			printchanges();
		}

		else {
			if (map1[i][j] == 5) {
				player->health += 5;
				heartkiton = 0;
				if (whichplayer == 1) {
					gotoxy(26, 13);
				}
				if (whichplayer == 2) {
					gotoxy(26, 85);
				}
				printf("%d", player->health);
				gotoxy(0, 0);
			}
			if (map1[i][j] == 7) {
				player->power = 2;
				poweron = 0;
				if (whichplayer == 1) {
					powercontrol1 = 5;
					gotoxy(27, 16);
					printf("2");
					gotoxy(0, 0);
				}
				if (whichplayer == 2) {
					powercontrol2 = 5;
					gotoxy(27, 88);
					printf("2");
					gotoxy(0, 0);
				}
			}
			if (map1[i][j] == 4) {
				grenadeon = 0;
				player->grenade = 1;
			}
			if (map1[i][j] == 10) {
				player->ghostmode = 1;
				ghostexist = 0;
				ghostmodeon = 1;
				ghostmodecharge = 50;
			}
			map1[player->i][player->j] = 0;
			player->i = i;
			player->j = j;
			changes.X = player->i;
			changes.Y = player->j;
		}

		if (whichplayer == 1)map1[player->i][player->j] = 1;
		if (whichplayer == 2)map1[player->i][player->j] = 2;
		printchanges();
		if (player->ghostmode == 1)return;

	}
	if (player->ghostmode == 1) {
		if (j > 98 || j < 1 || i>23 || i < 1)return;
		map1[player->i][player->j] = ghosthold;
		player->i = i;
		player->j = j;
		ghosthold = map1[i][j];
		if (whichplayer == 1) {
			map1[player->i][player->j] = 1;
		}
		if (whichplayer == 2) {
			map1[player->i][player->j] = 2;
		}
		int x = player->i - 1;
		int y = player->j - 1;
		gotoxy((player->i - 1), (player->j - 1));
		for (int t = 1; t <= 3; t++) {
			for (int w = 1; w <= 3; w++) {
				if (x == player->i && y == player->j) {
					y++;
					continue;
				}
				converttochar1(x, y);
				y++;
			}
			y -= 3;
			x++;
			gotoxy(x, y);
		}
		gotoxy(0, 0);
	}

}

void controler1(player* player1, char input) {

	if (input == 'w' || input == 'W') {
		player1->lastmovement = input;
		changes.X = player1->i;
		changes.Y = player1->j;
		whathappend(player1, ((player1->i) - 1), (player1->j), 1);
	}
	else if (input == 's' || input == 'S') {
		player1->lastmovement = input;
		changes.X = player1->i;
		changes.Y = player1->j;
		whathappend(player1, ((player1->i) + 1), (player1->j), 1);

	}
	else if (input == 'a' || input == 'A') {
		player1->lastmovement = input;
		changes.X = player1->i;
		changes.Y = player1->j;
		whathappend(player1, ((player1->i)), ((player1->j) - 1), 1);
	}
	else if (input == 'd' || input == 'D') {
		player1->lastmovement = input;
		changes.X = player1->i;
		changes.Y = player1->j;
		whathappend(player1, ((player1->i)), ((player1->j) + 1), 1);
	}
	if (player1->ghostmode == 1) {
		ghostmodecharge--;
		if (ghostmodecharge == 0) {
			if (ghosthold != 0) {
				ghostmodecharge++;
				return;
			}
			player1->ghostmode = 0;
			ghostmodeon = 0;
		}
	}
}
void controler2(player* player2, char input) {
	if (input == 'i' || input == 'I') {
		player2->lastmovement = input;
		changes.X = player2->i;
		changes.Y = player2->j;
		whathappend(player2, ((player2->i) - 1), (player2->j), 2);
	}
	if (input == 'K' || input == 'k') {
		player2->lastmovement = input;
		changes.X = player2->i;
		changes.Y = player2->j;
		whathappend(player2, ((player2->i) + 1), (player2->j), 2);
	}
	if (input == 'j' || input == 'J') {
		player2->lastmovement = input;
		changes.X = player2->i;
		changes.Y = player2->j;
		whathappend(player2, ((player2->i)), ((player2->j) - 1), 2);
	}
	if (input == 'l' || input == 'L') {
		player2->lastmovement = input;
		changes.X = player2->i;
		changes.Y = player2->j;
		whathappend(player2, ((player2->i)), ((player2->j) + 1), 2);
	}
	if (player2->ghostmode == 1) {
		ghostmodecharge--;
		if (ghostmodecharge == 0) {
			if (ghosthold != 0) {
				ghostmodecharge++;
				return;
			}
			player2->ghostmode = 0;
			ghostmodeon = 0;
		}
	}

}

void grenadeexplosion(bullet* temp, player* player1, player* player2, int shot) {
	if (shot == 9 || shot == 1 || shot == 2) {
		int x = temp->i - 2;
		int y = temp->j - 2;
		gotoxy(x, y);
		printf(Red);
		for (int t = 1; t <= 5; t++) {
			if (x > 24 || x < 0) {

				x++;
				gotoxy(x, y);
				continue;
			}
			for (int w = 1; w <= 5; w++) {
				if (y > 99 || y < 0) {
					y++;
					gotoxy(x, y);
					continue;
				}
				printf("*");
				y++;
			}
			y -= 5;
			x++;
			gotoxy(x, y);
		}
		printf(Reset);
		Sleep(200);
		gotoxy(0, 0);
		x = temp->i - 2;
		y = temp->j - 2;
		gotoxy(x, y);
		for (int t = 1; t <= 5; t++) {
			if (x > 24 || x < 0) {

				x++;
				gotoxy(x, y);
				continue;
			}
			for (int w = 1; w <= 5; w++) {
				if (y > 99 || y < 0) {
					y++;
					gotoxy(x, y);

					continue;
				}
				if (map1[x][y] == 1) {
					player1->health -= temp->damage;
					printhealthline(player1, player2);
					gotoxy(x, y);
				}
				if (map1[x][y] == 2) {
					player2->health -= temp->damage;
					printhealthline(player1, player2);
					gotoxy(x, y);
				}
				converttochar1(x, y);
				y++;
			}
			y -= 5;
			x++;
			gotoxy(x, y);
		}
		gotoxy(0, 0);
	}
	if (shot == 8) {
		int x = temp->i - 1;
		int y = temp->j - 1;
		gotoxy(x, y);
		printf(Red);
		for (int t = 1; t <= 3; t++) {
			for (int w = 1; w <= 3; w++) {
				printf("*");
				y++;
			}
			y -= 3;
			x++;
			gotoxy(x, y);
		}
		printf(Reset);
		Sleep(200);
		x = temp->i - 1;
		y = temp->j - 1;
		if (temp->side == 'd' || temp->side == 'D' || temp->side == 'l' || temp->side == 'L') {
			map1[temp->i][temp->j + 1] = 0;
		}
		if (temp->side == 'w' || temp->side == 'W' || temp->side == 'i' || temp->side == 'I') {
			map1[temp->i - 1][temp->j] = 0;
		}
		if (temp->side == 's' || temp->side == 'S' || temp->side == 'K' || temp->side == 'k') {
			map1[temp->i + 1][temp->j] = 0;
		}
		if (temp->side == 'a' || temp->side == 'A' || temp->side == 'j' || temp->side == 'J') {
			map1[temp->i][temp->j - 1] = 0;
		}
		gotoxy(x, y);
		for (int t = 1; t <= 3; t++) {
			for (int w = 1; w <= 3; w++) {
				if (map1[x][y] == 1) {
					player1->health -= temp->damage;
					printhealthline(player1, player2);
					gotoxy(x, y);
				}
				if (map1[x][y] == 2) {
					player2->health -= temp->damage;
					printhealthline(player1, player2);
					gotoxy(x, y);
				}
				converttochar1(x, y);
				y++;
			}
			y -= 3;
			x++;
			gotoxy(x, y);
		}
		gotoxy(0, 0);
	}
}

bullet* deleteheadbullet(bullet** hbullet, bullet* temp) {
	if (temp == *hbullet) {
		bullet* db = (bullet*)malloc(sizeof(bullet));
		db = temp;
		*hbullet = ((*hbullet)->next);
		temp = temp->next;
		free(db);
	}
	return temp;
} 
void bulletmoving(bullet** hbullet, player* player1, player* player2) {
	bullet* temp = *hbullet;
	bullet* prev = (bullet*)malloc(sizeof(bullet));
	if (*hbullet == NULL) {
		return;
	}
	else {
		for (; temp != NULL;) {

			if (temp->ghost == 0) {
				if (temp->side == 'd' || temp->side == 'D') {

					if (map1[temp->i][temp->j + 1] == 9) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i][temp->j + 1] == 8) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 8);

							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						temp->side = 'a';
						continue;
					}
					else if (map1[temp->i][temp->j + 1] == 1) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 1);

							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player1->health -= temp->damage;
						printhealthline(player1, player2);
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i][temp->j + 1] == 2) {
						if (temp->greanade == 1) {
							grenadeexplosion(temp, player1, player2, 2);
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							changes.X = temp->i;
							changes.Y = temp->j;
							printchanges();
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						player2->health -= temp->damage;
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}

					if (map1[temp->i][temp->j + 1] != 0) {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j += 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j++;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'a' || temp->side == 'A') {
					if (map1[temp->i][temp->j - 1] == 9) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i][temp->j - 1] == 8) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 8);

							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						temp->side = 'd';
						continue;
					}
					else if (map1[temp->i][temp->j - 1] == 1) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 1);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player1->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						gotoxy(0, 0);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i][temp->j - 1] == 2) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 2);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player2->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i][temp->j - 1] != 0) {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j -= 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j--;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'w' || temp->side == 'W') {

					if (map1[temp->i - 1][temp->j] == 9) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);

							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i - 1][temp->j] == 8) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 8);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						temp->side = 's';
						continue;
					}
					else if (map1[temp->i - 1][temp->j] == 1) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 1);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player1->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i - 1][temp->j] == 2) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 2);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player2->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i - 1][temp->j] != 0) {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i -= 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i--;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 's' || temp->side == 'S') {
					if (map1[temp->i + 1][temp->j] == 9) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i + 1][temp->j] == 8) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 8);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						temp->side = 'w';
						continue;
					}
					else if (map1[temp->i + 1][temp->j] == 1) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 1);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player1->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i + 1][temp->j] == 2) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 2);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player2->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i + 1][temp->j] != 0) {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i += 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i++;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}


				if (temp->side == 'l' || temp->side == 'L') {
					if (map1[temp->i][temp->j + 1] == 9) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i][temp->j + 1] == 8) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 8);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						temp->side = 'j';
						continue;
					}
					else if (map1[temp->i][temp->j + 1] == 1) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 1);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player1->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i][temp->j + 1] == 2) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 2);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player2->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i][temp->j + 1] != 0) {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j += 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j++;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'j' || temp->side == 'J') {
					if (map1[temp->i][temp->j - 1] == 9) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i][temp->j - 1] == 8) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 8);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						temp->side = 'l';
						continue;
					}
					else if (map1[temp->i][temp->j - 1] == 1) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 1);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player1->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i][temp->j - 1] == 2) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 2);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player2->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i][temp->j - 1] != 0) {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j -= 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j--;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'i' || temp->side == 'I') {
					if (temp->greanade == 1) {
						player2->grenade = 0;
						map1[temp->i][temp->j] = 0;
						grenadeexplosion(temp, player1, player2, 9);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i - 1][temp->j] == 9) {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();

						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i - 1][temp->j] == 8) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 8);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						temp->side = 'k';
						continue;
					}
					else if (map1[temp->i - 1][temp->j] == 1) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 1);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player1->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i - 1][temp->j] == 2) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 2);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player2->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i - 1][temp->j] != 0) {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i -= 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i--;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'k' || temp->side == 'K') {
					if (map1[temp->i + 1][temp->j] == 9) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i + 1][temp->j] == 8) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 8);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						temp->side = 'i';
						continue;
					}
					else if (map1[temp->i + 1][temp->j] == 1) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 1);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player1->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					else if (map1[temp->i + 1][temp->j] == 2) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 2);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						map1[temp->i][temp->j] = 0;
						player2->health -= temp->damage;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						printhealthline(player1, player2);
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						temp = temp->next;
						prev->next = bdelete->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i + 1][temp->j] != 0) {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i += 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i++;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}


			}

			else {
				if (temp->side == 'd' || temp->side == 'D') {

					if (temp->j == 98 || map1[temp->i][temp->j + 1] == 1 || map1[temp->i][temp->j + 1] == 2) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						if (map1[temp->i][temp->j + 1] == 1) {
							player1->health -= temp->damage;
							printhealthline(player1, player2);
						}
						if (map1[temp->i][temp->j + 1] == 2) {
							player2->health -= temp->damage;
							printhealthline(player1, player2);
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}

					else if (map1[temp->i][temp->j + 1] != 0) {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j += 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }

						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j++;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'a' || temp->side == 'A') {
					if (temp->j == 1 || map1[temp->i][temp->j - 1] == 1 || map1[temp->i][temp->j - 1] == 2) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						if (map1[temp->i][temp->j - 1] == 1) {
							player1->health -= temp->damage;
							printhealthline(player1, player2);
						}
						if (map1[temp->i][temp->j - 1] == 2) {
							player2->health -= temp->damage;
							printhealthline(player1, player2);
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}

					else if (map1[temp->i][temp->j - 1] != 0) {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j -= 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }

						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j--;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'w' || temp->side == 'W') {

					if (temp->i == 1 || map1[temp->i - 1][temp->j] == 1 || map1[temp->i - 1][temp->j] == 2) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						if (map1[temp->i - 1][temp->j] == 1) {
							player1->health -= temp->damage;
							printhealthline(player1, player2);
						}
						if (map1[temp->i - 1][temp->j] == 2) {
							player2->health -= temp->damage;
							printhealthline(player1, player2);
						}

						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i - 1][temp->j] != 0) {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i -= 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }

						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i--;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 's' || temp->side == 'S') {
					if (temp->i == 23 || map1[temp->i + 1][temp->j] == 1 || map1[temp->i + 1][temp->j] == 2) {
						if (temp->greanade == 1) {
							player1->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						if (map1[temp->i + 1][temp->j] == 1) {
							player1->health -= temp->damage;
							printhealthline(player1, player2);
						}
						if (map1[temp->i + 1][temp->j] == 2) {
							player2->health -= temp->damage;
							printhealthline(player1, player2);
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i + 1][temp->j] != 0) {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i += 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }

						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i++;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}


				if (temp->side == 'l' || temp->side == 'L') {
					if (temp->j == 98 || map1[temp->i][temp->j + 1] == 1 || map1[temp->i][temp->j + 1] == 2) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						if (map1[temp->i][temp->j + 1] == 1) {
							player1->health -= temp->damage;
							printhealthline(player1, player2);
						}
						if (map1[temp->i][temp->j + 1] == 2) {
							player2->health -= temp->damage;
							printhealthline(player1, player2);
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}

					else if (map1[temp->i][temp->j + 1] != 0) {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j += 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }

						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j++;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'j' || temp->side == 'J') {
					if (temp->j == 1 || map1[temp->i][temp->j - 1] == 1 || map1[temp->i][temp->j - 1] == 2) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						if (map1[temp->i][temp->j - 1] == 1) {
							player1->health -= temp->damage;
							printhealthline(player1, player2);
						}
						if (map1[temp->i][temp->j - 1] == 2) {
							player2->health -= temp->damage;
							printhealthline(player1, player2);
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}

					else if (map1[temp->i][temp->j - 1] != 0) {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j -= 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }

						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->j--;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'i' || temp->side == 'I') {
					if (temp->i == 1 || map1[temp->i - 1][temp->j] == 1 || map1[temp->i - 1][temp->j] == 2) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						if (map1[temp->i - 1][temp->j] == 1) {
							player1->health -= temp->damage;
							printhealthline(player1, player2);
						}
						if (map1[temp->i - 1][temp->j] == 2) {
							player2->health -= temp->damage;
							printhealthline(player1, player2);
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i - 1][temp->j] != 0) {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i -= 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }

						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i--;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				if (temp->side == 'k' || temp->side == 'K') {
					if (temp->i == 23 || map1[temp->i + 1][temp->j] == 1 || map1[temp->i + 1][temp->j] == 2) {
						if (temp->greanade == 1) {
							player2->grenade = 0;
							map1[temp->i][temp->j] = 0;
							grenadeexplosion(temp, player1, player2, 9);
							if (*hbullet == temp) {
								temp = deleteheadbullet(hbullet, temp);
								continue;
							}
							bullet* bdelete = (bullet*)malloc(sizeof(bullet));
							bdelete = temp;
							prev->next = temp->next;
							temp = temp->next;
							free(bdelete);
							continue;
						}
						if (map1[temp->i + 1][temp->j] == 1) {
							player1->health -= temp->damage;
							printhealthline(player1, player2);
						}
						if (map1[temp->i + 1][temp->j] == 2) {
							player2->health -= temp->damage;
							printhealthline(player1, player2);
						}
						map1[temp->i][temp->j] = 0;
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						if (*hbullet == temp) {
							temp = deleteheadbullet(hbullet, temp);
							continue;
						}
						bullet* bdelete = (bullet*)malloc(sizeof(bullet));
						bdelete = temp;
						prev->next = temp->next;
						temp = temp->next;
						free(bdelete);
						continue;
					}
					if (map1[temp->i + 1][temp->j] != 0) {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }
						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i += 2;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
					else {
						if (map1[temp->i][temp->j] != 9) { map1[temp->i][temp->j] = 0; }

						changes.X = temp->i;
						changes.Y = temp->j;
						printchanges();
						temp->i++;
						if (map1[temp->i][temp->j] == 0) {
							map1[temp->i][temp->j] = 3;
						}
					}
				}
				}

			changes.X = temp->i;
			changes.Y = temp->j;
			printchanges();
			prev = temp;
			temp = temp->next;
		}
		if (prev->i < 0) {
			*hbullet = NULL;
		}
	}
}
bullet* creatbullet(bullet** hbullet, player* playerx) {
	bullet* newb = (bullet*)malloc(sizeof(bullet));
	newb->side = playerx->lastmovement;
	if (newb->side == 'w' || newb->side == 'W') {
		newb->i = (playerx->i - 1);
		newb->j = playerx->j;
	}
	if (newb->side == 's' || newb->side == 'S') {
		newb->i = (playerx->i + 1);
		newb->j = playerx->j;
	}
	if (newb->side == 'a' || newb->side == 'A') {
		newb->i = (playerx->i);
		newb->j = (playerx->j - 1);
	}
	if (newb->side == 'd' || newb->side == 'D') {
		newb->i = playerx->i;
		newb->j = (playerx->j + 1);
	}
	if (newb->side == 'i' || newb->side == 'I') {
		newb->i = (playerx->i - 1);
		newb->j = playerx->j;
	}
	if (newb->side == 'k' || newb->side == 'K') {
		newb->i = (playerx->i + 1);
		newb->j = playerx->j;
	}
	if (newb->side == 'j' || newb->side == 'J') {
		newb->i = (playerx->i);
		newb->j = (playerx->j - 1);
	}
	if (newb->side == 'l' || newb->side == 'L') {
		newb->i = playerx->i;
		newb->j = (playerx->j + 1);
	}
	newb->damage = playerx->power;
	newb->greanade = playerx->grenade;
	newb->ghost = playerx->ghostmode;
	playerx->grenade = 0;


	newb->next = *hbullet;
	*hbullet = newb;
	return *hbullet;
}

void userpointsave(users** head, profile user) {
	users* temp;
	temp = *head;
	for (;;) {
		if (strcmp(temp->data.username, user.username) == 0) {
			temp->data = user;
			savedata(*head);
			return;
		}
		temp = temp->next;

	}

}


///////////////////////////////////////////////////////////////////////////////

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
	srand(time(NULL));
	creatmap1();

	bullet* hbullet = NULL;
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

	ghostmodeon = 0;
	heartkiton = 1;
	poweron = 1;
	grenadeon = 1;
	ghostexist = 1;
	powercontrol1 = 0;
	powercontrol2 = 0;
	ghosthold = 0;

	int END = 0;

	map1[player1->i][player1->j] = 1;
	map1[player2->i][player2->j] = 2;

	for (int i = 0; i <= 24; i++) {
		for (int j = 0; j <= 99; j++) {

			converttochar1(i, j);

		}
		printf("\n");
	}

	printf("\tplayer1:%s \t\t\t\t\t\t\t        player2:%s\n", player1->name, player2->name);
	printhealthline(player1, player2);
	printdamageline(player1, player2);
	gotoxy(28, 0);
	printf("\twins: % d \t\t\t\t\t\t\t\twins: % d \n", player1->win, player2->win);
	gotoxy(0, 0);
	clock_t starttime1, currenttime1, starttime2, currenttime2;
	starttime1 = clock();;
	starttime2 = clock();
	for (;;) {
		currenttime1 = clock();;
		currenttime2 = clock();
		if ((double)(currenttime1 - starttime1) / CLOCKS_PER_SEC >= 0.15) {
			timer1s = 1;
		}
		if ((double)(currenttime2 - starttime2) / CLOCKS_PER_SEC >= 0.15) {
			timer2s = 1;
		}
		_sleep(Bulletspeed);
		bulletmoving(&hbullet, player1, player2);
		if (_kbhit()) {
			//
			map1[player1->i][player1->j] = 1;
			map1[player2->i][player2->j] = 2;
			char input = _getch();
			//
			controler1(player1, input);
			controler2(player2, input);
			//
			if (input == 'c' || input == 'C') {
				if (timer1s == 0)continue;
				if (powercontrol1 > 0)powercontrol1--;
				if (powercontrol1 == 0) {
					player1->power = 1;
					printdamageline(player1, player2);
				}
				hbullet = creatbullet(&hbullet, player1);
				starttime1 = clock();
				timer1s = 0;
			}
			if (input == 'N' || input == 'n') {
				if (timer2s == 0)continue;
				if (powercontrol2 > 0)powercontrol2--;
				if (powercontrol2 == 0) {
					player2->power = 1;
					printdamageline(player1, player2);
				}
				hbullet = creatbullet(&hbullet, player2);
				starttime2 = clock();
				timer2s = 0;
			}
			printchanges();
		}
		//
		if (player1->health <= 0) {
			END = 1;
			winnername(*player2);
			player2->win++;
			return 2;
		}
		if (player2->health <= 0) {
			END = 2;
			winnername(*player1);
			player1->win++;
			return 1;
		}
		//
		//
		int h = (rand() % (500 - 0 + 1)) + 0;
		if (h == 5) {
			if (heartkiton == 1)continue;
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 5;
				heartkiton = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {
				converttochar1(numi, j);
			}
		}
		if (h == 0) {
			if (poweron == 1)continue;
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 7;
				poweron = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {

				converttochar1(numi, j);
			}

		}
		if (h == 4) {
			if (grenadeon == 1)continue;
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 4;
				grenadeon = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {
				converttochar1(numi, j);
			}
		}
		if (h == 100) {
			if (ghostexist == 1) {
				continue;
			}
			if (ghostmodeon == 1) {
				continue;
			}
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 10;
				ghostexist = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {
				converttochar1(numi, j);
			}
		}
		//
	}


}


void creatmap2() {
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
				if (j == 30 || j == 70) {
					map1[i][j] = 9;
				}
				if (j == 31) {
					map1[i][j] = 8;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
				else if (j == 90) {
					map1[i][j] = 11;
				}
				else if (j == 10) {
					map1[i][j] = 22;
				}
			}
			if (i == 4) {
				if (j == 30 || j == 70) {
					map1[i][j] = 9;
				}
				if (j == 69) {
					map1[i][j] = 8;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 5) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 6) {
				if (j >= 48 && j <= 52) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 7) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 8) {
				if (j == 86 || j == 13) {
					map1[i][j] = 8;
				}
				if (j == 50 || j == 38 || j == 62) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}

			}
			if (i == 9)
			{
				if (j == 86 || j == 13) {
					map1[i][j] = 8;
				}
				if ((j <= 50 && j >= 46) || j == 37 || j == 63) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 10)
			{
				if (j == 86 || j == 13) {
					map1[i][j] = 8;
				}
				if (j == 55) {
					map1[i][j] = 6;
				}
				if (j == 29) {
					map1[i][j] = 7;
				}
				if (j == 46 || j == 36 || j == 64) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 11) {
				if (j == 86 || j == 13) {
					map1[i][j] = 8;
				}
				if ((j >= 44 && j <= 46) || (j >= 54 && j <= 56) || j == 35 || j == 65) {
					map1[i][j] = 9;
				}
				if (j == 50) {
					map1[i][j] = 5;
				}
				if (j == 47) {
					map1[i][j] = 8;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 12) {
				if (j == 86 || j == 13) {
					map1[i][j] = 8;
				}
				if (j == 45) {
					map1[i][j] = 6;
				}
				if (j == 54 || j == 36 || j == 64) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 13) {
				if (j == 86 || j == 13) {
					map1[i][j] = 8;
				}
				if ((j >= 50 && j <= 54) || j == 37 || j == 63) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 14) {
				if (j == 86 || j == 13) {
					map1[i][j] = 8;
				}
				if (j == 50 || j == 38 || j == 62) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 15) {
				if (j == 86 || j == 13) {
					map1[i][j] = 8;
				}
				if (j == 50) {
					map1[i][j] = 6;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 16) {
				if (j == 86 || j == 13) {
					map1[i][j] = 8;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 17) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 18) {
				if (j >= 48 && j <= 52) {
					map1[i][j] = 9;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 19) {
				if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
				if (j == 67) {
					map1[i][j] = 4;
				}
			}
			if (i == 20) {
				if (j == 30 || j == 70) {
					map1[i][j] = 9;
				}
				if (j == 56) {
					map1[i][j] = 7;
				}
				if (j == 31) {
					map1[i][j] = 8;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
			}
			if (i == 21) {
				if (j == 30 || j == 70) {
					map1[i][j] = 9;
				}
				if (j == 69) {
					map1[i][j] = 8;
				}
				else if (j == 0 || j == 99) {
					map1[i][j] = 9;
				}
				else if (j == 10) {
					map1[i][j] = 11;
				}
				else if (j == 90) {
					map1[i][j] = 22;
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
int game2(users** head, player* player1, player* player2) {
	creatmap2();
	srand(time(NULL));

	bullet* hbullet = NULL;
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
	player1->j = 9;
	player1->power = 1;
	player1->ghostmode = 0;
	player1->grenade = 0;
	player1->lastmovement = 'd';
	player1->firstcoord.X = 12;
	player1->firstcoord.Y = 9;


	player2->i = 12;
	player2->j = 90;
	player2->power = 1;
	player2->ghostmode = 0;
	player2->grenade = 0;
	player2->lastmovement = 'j';
	player2->firstcoord.X = 12;
	player2->firstcoord.Y = 90;

	ghostmodeon = 0;
	heartkiton = 1;
	poweron = 1;
	grenadeon = 1;
	ghostexist = 1;
	powercontrol1 = 0;
	powercontrol2 = 0;
	ghosthold = 0;

	int END = 0;

	map1[player1->i][player1->j] = 1;
	map1[player2->i][player2->j] = 2;

	for (int i = 0; i <= 24; i++) {
		for (int j = 0; j <= 99; j++) {
			converttochar1(i, j);

		}
		printf("\n");
	}
	printf("\tplayer1:%s \t\t\t\t\t\t\t        player2:%s\n", player1->name, player2->name);
	printhealthline(player1, player2);
	printdamageline(player1, player2);
	gotoxy(28, 0);
	printf("\twins: % d \t\t\t\t\t\t\t\twins: % d \n", player1->win, player2->win);
	gotoxy(0, 0);

	clock_t starttime1, currenttime1, starttime2, currenttime2;
	starttime1 = clock();
	starttime2 = clock();

	for (;;) {
		currenttime1 = clock();
		currenttime2 = clock();
		if ((double)(currenttime1 - starttime1) / CLOCKS_PER_SEC >= 0.15) {
			timer1s = 1;
		}
		if ((double)(currenttime2 - starttime2) / CLOCKS_PER_SEC >= 0.15) {
			timer2s = 1;
		}
		_sleep(Bulletspeed);
		bulletmoving(&hbullet, player1, player2);
		if (_kbhit()) {
			map1[player1->i][player1->j] = 1;
			map1[player2->i][player2->j] = 2;
			char input = _getch();

			controler1(player1, input);
			controler2(player2, input);
			if (input == 'c' || input == 'C') {
				if (timer1s == 0)continue;
				if (powercontrol1 > 0)powercontrol1--;
				if (powercontrol1 == 0) {

					player1->power = 1;
					printdamageline(player1, player2);
				}

				hbullet = creatbullet(&hbullet, player1);
				starttime1 = clock();
				timer1s = 0;
			}
			if (input == 'N' || input == 'n') {
				if (timer2s == 0)continue;
				if (powercontrol2 > 0)powercontrol2--;
				if (powercontrol2 == 0) {

					player2->power = 1;
					printdamageline(player1, player2);
				}
				hbullet = creatbullet(&hbullet, player2);
				starttime2 = clock();
				timer2s = 0;
			}
			printchanges();

		}
		if (player1->health <= 0) {
			END = 1;
			winnername(*player2);
			player2->win++;
			return 2;
		}
		if (player2->health <= 0) {
			END = 2;
			winnername(*player1);
			player1->win++;
			return 1;
		}
		int h = (rand() % (500 - 0 + 1)) + 0;
		if (h == 5) {
			if (heartkiton == 1)continue;
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 5;
				heartkiton = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {
				converttochar1(numi, j);
			}
		}
		if (h == 0) {
			if (poweron == 1)continue;
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 7;
				poweron = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {

				converttochar1(numi, j);
			}

		}
		if (h == 4) {
			if (grenadeon == 1)continue;
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 4;
				grenadeon = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {
				converttochar1(numi, j);
			}
		}
		if (h == 100) {
			if (ghostexist == 1) {
				continue;
			}
			if (ghostmodeon == 1) {
				continue;
			}
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 10;
				ghostexist = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {
				converttochar1(numi, j);
			}
		}
	}


}

void creatmap3() {

	for (int i = 0; i <= 24; i++) {
		for (int j = 0; j <= 99; j++) {
			if (i == 0 || i == 24) {
				map1[i][j] = 9;
			}
			if (j == 0 || j == 99) {
				map1[i][j] = 9;
			}
			if (i == 1) {

			}
			if (i == 2) {
				if (j == 50) {
					map1[i][j] = 10;
				}
			}
			if (i == 3) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 75 && j <= 88) {
					map1[i][j] = 9;
				}
				if (j == 90) {
					map1[i][j] = 11;
				}
				if (j == 10) {
					map1[i][j] = 22;
				}
				if (j == 15 || j == 34 || j == 59 || j == 74)map1[i][j] = 8;
			}
			if (i == 4) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 75 && j <= 88) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 59 || j == 74)map1[i][j] = 8;
			}
			if (i == 5) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 75) {
					map1[i][j] = 7;
				}
				if (j == 15 || j == 34 || j == 59 || j == 79)map1[i][j] = 8;
			}
			if (i == 6) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 59 || j == 79)map1[i][j] = 8;
			}
			if (i == 7) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 59 || j == 79)map1[i][j] = 8;
			}
			if (i == 8)
			{
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 59 || j == 79)map1[i][j] = 8;
			}
			if (i == 9) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 59 || j == 79)map1[i][j] = 8;

			}
			if (i == 10) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 59 || j == 79)map1[i][j] = 8;
			}
			if (i == 11) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 59 || j == 79)map1[i][j] = 8;
			}
			if (i == 12) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 59 || j == 79)map1[i][j] = 8;
			}
			if (i == 13) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 40) {
					map1[i][j] = 9;
				}
				if (j == 50)map1[i][j] = 5;
				if (j >= 60 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 59 || j == 79)map1[i][j] = 8;
			}
			if (i == 14) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 65) {
					map1[i][j] = 9;
				}

				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 79)map1[i][j] = 8;
			}
			if (i == 15) {
				if (j >= 16 && j <= 20) {
					map1[i][j] = 9;
				}
				if (j >= 35 && j <= 65) {
					map1[i][j] = 9;
				}
				if (j == 75 || j == 29) {
					map1[i][j] = 6;
				}
				if (j >= 80 && j <= 83) {
					map1[i][j] = 9;
				}
				if (j == 15 || j == 34 || j == 79)map1[i][j] = 8;
			}

			if (i == 16) {

			}
			if (i == 17) {

			}

			if (i == 18) {
				if (j == 16 || j == 27 || (j >= 39 && j <= 41) || (j >= 47 && j <= 54) || j == 64 || j == 65 || j == 71 || j == 72 || j == 85) {
					map1[i][j] = 9;
				}

				if (j == 15 || j == 26 || j == 38 || j == 46 || j == 84 || j == 70 || j == 63)map1[i][j] = 8;
			}
			if (i == 19) {
				if (j == 15 || j == 17 || j == 27 || (j >= 39 && j <= 41) || j == 47 || j == 64 || j == 65 || j == 71 || j == 72 || j == 84 || j == 86) {
					map1[i][j] = 9;
				}
				if (j == 14 || j == 26 || j == 38 || j == 46 || j == 83 || j == 70 || j == 63)map1[i][j] = 8;
				if (j == 67)map1[i][j] = 4;
			}
			if (i == 20) {
				if (j == 14 || j == 18 || j == 27 || (j >= 39 && j <= 41) || (j >= 47 && j <= 54) || j == 64 || j == 65 || (j >= 65 && j <= 70) || j == 71 || j == 72 || j == 83 || j == 87) {
					map1[i][j] = 9;
				}
				if (j == 13 || j == 26 || j == 38 || j == 46 || j == 82 || j == 63)map1[i][j] = 8;
			}
			if (i == 21) {
				if (j >= 13 && j <= 19)map1[i][j] = 9;
				if (j == 27 || (j >= 39 && j <= 41) || j == 54 || j == 64 || j == 65 || j == 71 || j == 72 || (j >= 82 && j <= 88))map1[i][j] = 9;
				if (j == 12 || j == 26 || j == 38 || j == 53 || j == 63 || j == 81 || j == 70)map1[i][j] = 8;
				if (j == 10) {
					map1[i][j] = 11;
				}
				if (j == 90) {
					map1[i][j] = 22;
				}
				if (j == 49) {
					map1[i][j] = 7;
				}
			}
			if (i == 22) {
				if (j == 12 || j == 20 || (j >= 39 && j <= 41) || (j >= 47 && j <= 54) || j == 64 || j == 65 || j == 63 || j == 71 || j == 72 || j == 81 || j == 89) {
					map1[i][j] = 9;
				}
				if (j >= 27 && j <= 33) {
					map1[i][j] = 9;
				}
				if (j == 11 || j == 26 || j == 38 || j == 46 || j == 80 || j == 70 || j == 63)map1[i][j] = 8;
			}
			if (i == 23) {

			}

		}
	}
}
int game3(users** head, player* player1, player* player2) {
	creatmap3();
	srand(time(NULL));
	bullet* hbullet = NULL;
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
	player1->firstcoord.X = 12;
	player1->firstcoord.Y = 8;

	player2->i = 12;
	player2->j = 90;
	player2->power = 1;
	player2->ghostmode = 0;
	player2->lastmovement = 'j';
	player2->firstcoord.X = 12;
	player2->firstcoord.Y = 90;

	ghostmodeon = 0;
	heartkiton = 1;
	poweron = 1;
	grenadeon = 1;
	ghostexist = 1;
	powercontrol1 = 0;
	powercontrol2 = 0;
	ghosthold = 0;


	int END = 0;

	map1[player1->i][player1->j] = 1;
	map1[player2->i][player2->j] = 2;

	for (int i = 0; i <= 24; i++) {
		for (int j = 0; j <= 99; j++) {
			converttochar1(i, j);

		}
		printf("\n");
	}
	printf("\tplayer1:%s \t\t\t\t\t\t\t        player2:%s\n", player1->name, player2->name);
	printhealthline(player1, player2);
	printdamageline(player1, player2);
	gotoxy(28, 0);
	printf("\twins: % d \t\t\t\t\t\t\t\twins: % d \n", player1->win, player2->win);
	gotoxy(0, 0);
	clock_t starttime1, currenttime1, starttime2, currenttime2;
	starttime1 = clock();
	starttime2 = clock();
	for (;;) {
		currenttime1 = clock();;
		currenttime2 = clock();
		if ((double)(currenttime1 - starttime1) / CLOCKS_PER_SEC >= 0.15) {
			timer1s = 1;
		}
		if ((double)(currenttime2 - starttime2) / CLOCKS_PER_SEC >= 0.15) {
			timer2s = 1;
		}
		_sleep(Bulletspeed);
		bulletmoving(&hbullet, player1, player2);
		if (_kbhit()) {
			map1[player1->i][player1->j] = 1;
			map1[player2->i][player2->j] = 2;
			char input = _getch();

			controler1(player1, input);
			controler2(player2, input);
			if (input == 'c' || input == 'C') {
				if (timer1s == 0)continue;
				if (powercontrol1 > 0)powercontrol1--;
				if (powercontrol1 == 0) {
					player1->power = 1;
					printdamageline(player1, player2);
				}
				hbullet = creatbullet(&hbullet, player1);
				starttime1 = clock();
				timer1s = 0;
			}
			if (input == 'N' || input == 'n') {
				if (timer2s == 0)continue;
				if (powercontrol2 > 0)powercontrol2--;
				if (powercontrol2 == 0) {
					player2->power = 1;
					printdamageline(player1, player2);
				}
				hbullet = creatbullet(&hbullet, player2);
				starttime2 = clock();
				timer2s = 0;
			}
			printchanges();


		}
		if (player1->health <= 0) {
			END = 1;
			winnername(*player2);
			player2->win++;
			return 2;
		}
		if (player2->health <= 0) {
			END = 2;
			winnername(*player1);
			player1->win++;
			return 1;
		}
		int h = (rand() % (500 - 0 + 1)) + 0;
		if (h == 5) {
			if (heartkiton == 1)continue;
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 5;
				heartkiton = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {
				converttochar1(numi, j);
			}
		}
		if (h == 0) {
			if (poweron == 1)continue;
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 7;
				poweron = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {

				converttochar1(numi, j);
			}

		}
		if (h == 4) {
			if (grenadeon == 1)continue;
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 4;
				grenadeon = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {
				converttochar1(numi, j);
			}
		}
		if (h == 100) {
			if (ghostexist == 1) {
				continue;
			}
			if (ghostmodeon == 1) {
				continue;
			}
			int numi = (rand() % (23 - 1 + 1)) + 0;     // i
			int numj = (rand() % (98 - 1 + 1)) + 0;     // j
			if (map1[numi][numj] == 0) {
				map1[numi][numj] = 10;
				ghostexist = 1;
			}
			gotoxy(numi, 0);
			for (int j = 0; j <= 99; j++) {
				converttochar1(numi, j);
			}
		}

	}


}

///////////////////////////////////////////////////////////////////////////////
int main() {

	PlaySound(TEXT("E:\\programing\\space war\\MUSIC\\PS2.wav"), NULL, SND_FILENAME | SND_ASYNC);
	printf("\033[48;2;128;0;128m");
	firstwellcome();
	PlaySound(TEXT("E:\\programing\\space war\\MUSIC\\GTA.wav"), NULL, SND_FILENAME | SND_ASYNC);

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
			printf("\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\033[1;32m%HOPE YOU ENJOYED:)\033[0m");
			printf("\n\n\t\t\t\t\t\t\t\t \033[1;34m%[GOODBYE]\033[0m");
			_sleep(4000);
			exit(0);
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
				strcpy(player2.name, user2.username);
				savedata(head);
				player1.win = 0;
				player2.win = 0;
				//
				history histo;
				histo.fplayerID = user1.id;
				histo.splayerID = user2.id;
				//
				PlaySound(TEXT("E:\\programing\\space war\\MUSIC\\epic2.wav"), NULL, SND_FILENAME | SND_ASYNC);
				system("cls||clear");
				//
				for (int i = 0; i < 25; i++) {
					for (int j = 0; j < 100; j++) {
						map1[i][j] = 0;
					}
				}
				int t = game1(&head, &player1, &player2);
				if (t == 1) {
					histo.firstwinnerID = user1.id;
				}
				if (t == 2) {
					histo.firstwinnerID = user2.id;
				}
				//
				for (int i = 0; i < 25; i++) {
					for (int j = 0; j < 100; j++) {
						map1[i][j] = 0;
					}
				}
				t = game2(&head, &player1, &player2);
				if (t == 1) {
					histo.secondwinnerID = user1.id;
				}
				if (t == 2) {
					histo.secondwinnerID = user2.id;
				}
				//
				for (int i = 0; i < 25; i++) {
					for (int j = 0; j < 100; j++) {
						map1[i][j] = 0;
					}
				}
				t = game3(&head, &player1, &player2);
				if (t == 1) {
					histo.thirdwinnerID = user1.id;
				}
				if (t == 2) {
					histo.thirdwinnerID = user2.id;
				}

				///
				if (player1.win > player2.win) {
					user1.point += 20;
					user2.point -= 10;
					histo.totalwinnerID = user1.id;
				}
				else {

					user2.point += 20;
					user1.point -= 10;
					histo.totalwinnerID = user2.id;
				}

				//

				savehistory(histo);
				userpointsave(&head, user1);
				userpointsave(&head, user2); 

				system("cls || clear");
				printf("do you wanna play again?(y/n)");
				char input[20];
				scanf("%s", input);
				getchar();
				if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0) {
					continue;
				}
				else {
					break;
				}

			}
				

		}
	}
	
}