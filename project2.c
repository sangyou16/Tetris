#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#pragma warning(disable: 4996)

#define LEFT 75		// Keyboard Input
#define RIGHT 77 
#define UP 72 
#define DOWN 80 
#define p 112
#define P 80 
#define W 87
#define w 119
#define A 65
#define a 97
#define S 83
#define s 115
#define D 68
#define d 100
#define F 70
#define f 102
#define M 77
#define m 109
#define ESC 27 
#define char '1';
#define char '2';
#define char '3';
#define char '4';
#define _CRT_SECURE_NO_WARNINGS
#define TURNCHECKP1 (turn %2 == 1)
#define TURNCHECKP2 (turn %2 == 0)




#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // Block State
#define CEILLING -1      
#define EMPTY 0         
#define WALL 1
#define INACTIVE_P1BLOCK 2
#define INACTIVE_P2BLOCK 3 

#define MAIN_X 11	// Game borad Size
#define MAIN_Y 23 
#define MAIN_X_ADJ 3 
#define MAIN_Y_ADJ 1  

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 // Status info position

int STATUS_Y_GOAL; // Status info position

int STATUS_Y_SCORE;
int STATUS_Y_SCORE2;
int STATUS_Y_ITEM;
int STATUS_Y_ITEM2;
int STATUS_Y_TURNNUMBER;

int blocks[7][4][4][4] = {
	{ { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 } },
	{ { 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 } },
	{ { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0 } },
	{ { 0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0 },{ 0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0 } },
	{ { 0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0 },{ 0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0 } }
}; // Shape of block

int b_type; // block type
int b_rotation; // block rotation
int b_type_next; //new block

int main_org[MAIN_Y][MAIN_X];
int main_cpy[MAIN_Y][MAIN_X];

int bx, by; // x,y position of moving block

int key; // keyboard input 

int speed; // Game Speed 

int p1score; // Game Score
int p2score;

int turn;
int p1item;
int p2item;
int totalp1item;
int totalp2item;
int u =1;

int new_block_on = 0;
int crush_on = 0;

int item_used;
int p1item_used;
int p2item_used;
time_t now;
struct tm *z;


void title(void);
void reset(void);
void reset_main(void); // 
void reset_main_cpy(void);
void draw_map(void);
void draw_main(void);
void new_block(void);
void p1check_key(void);
void p2check_key(void);




int check_crush(int bx, int by, int rotation);
void drop_block(void);
void move_block(int dir);
void check_line(void);
void p1item_press(void);
void p2item_press(void);

void check_game_over(void);
void pause(void);

void gotoxy(int x, int y) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; // hide cursor
void setcursortype(CURSOR_TYPE c) { // hide cursor
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

int main() {
	int i;

	now = time(NULL);
	z = localtime(&now);
	FILE *fp = NULL;

	srand((unsigned)time(NULL));
	setcursortype(NOCURSOR);
	title();
	reset();

	while (1) {
		for (i = 0; i<5; i++) {
		if(turn %2 ==0){ 
			p1check_key();
			} else{ 
			p2check_key();
			
			}
			draw_main();

			Sleep(speed);
			if (crush_on&&check_crush(bx, by + 1, b_rotation) == false) Sleep(100);

		}
		drop_block();

		check_game_over();
		if (new_block_on == 1) new_block();
	}
}



void title(void) {
	int x = 5; //
	int y = 4; //


	gotoxy(x, y + 0); printf("¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È                   Item Tutorial:");
	gotoxy(x, y + 1); printf("¢È                              ¢È	  1) Player must choose a number from 1 to 4");
	gotoxy(x, y + 2); printf("¢È                              ¢È      2) 1. 'I' Shaped   2. ¡á    3. 'T' Shaped   4. 'Z' Shaped");
	gotoxy(x, y + 3); printf("¢È                              ¢È");
	gotoxy(x, y + 4); printf("¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È");

	gotoxy(x + 5, y + 2); printf("T E T R I S");
	gotoxy(x, y + 5); printf("Please Enter Any Key to Start..");
	gotoxy(x, y + 7); printf("1P");
	gotoxy(x, y + 9); printf("  W   : Shift        Use Item: F"  );
	gotoxy(x, y + 10); printf("A  D : Left / Right     Pause : P");
	gotoxy(x, y + 11); printf("  S   : Drop            Quit: ESC");
	gotoxy(x, y + 12); printf("   P   : Pause");
	gotoxy(x, y + 13); printf("  ESC  : Quit");
	gotoxy(x, y + 17); printf("2P");
	gotoxy(x, y + 19); printf("  ¡è  : Shift        Use Item: M");
	gotoxy(x, y + 20); printf("¡ç  ¡æ  : Left / Right     Pause : P");
	gotoxy(x, y + 21); printf("  ¡é   : Drop            Quit: ESC");
	gotoxy(x, y + 22); printf("   P   : Pause");
	gotoxy(x, y + 23); printf("  ESC  : Quit");


	while (true) {
		if (kbhit()) break;
	}

	while (kbhit()) getch();

}

void reset(void) {

	
	now = time(NULL);
	z = localtime(&now);

	p1score = 0;
	turn = 1;
	p2score =0;
	key = 0;
	crush_on = 0;

	speed = 100;

	system("cls");
	reset_main();
	draw_map();
	draw_main();

	b_type_next = rand() % 7;
	new_block();

	GetAsyncKeyState('A');

}

void reset_main(void) {
	int i, j;

	for (i = 0; i<MAIN_Y; i++) { // Game board initialize
		for (j = 0; j<MAIN_X; j++) {
			main_org[i][j] = 0;
			main_cpy[i][j] = 100;
		}
	}
	for (j = 1; j<MAIN_X; j++) {
		main_org[3][j] = CEILLING;
	}
	for (i = 1; i<MAIN_Y - 1; i++) {
		main_org[i][0] = WALL;
		main_org[i][MAIN_X - 1] = WALL;
	}
	for (j = 0; j<MAIN_X; j++) {
		main_org[MAIN_Y - 1][j] = WALL;
	}
}

void reset_main_cpy(void) {
	int i, j;

	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) {
			main_cpy[i][j] = 100;
		}
	}
}

void draw_map(void) { // Game Status 
	int y = 3;


	
	gotoxy(STATUS_X_ADJ, STATUS_Y_TURNNUMBER = y); printf("%s (Total Number of Turns : %d )", TURNCHECKP1? "Player 1's turn" : "Player 2's turn", turn);

	gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ ");
	gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
	gotoxy(STATUS_X_ADJ, y + 8); printf("P1 score  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 9); printf("        %6d", p1score);
	gotoxy(STATUS_X_ADJ, y+10); printf("P1 Item  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_ITEM = y + 11); printf("        %6d", p1item);

	gotoxy(STATUS_X_ADJ, y + 12); printf("P2 score  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE2 = y + 13); printf("        %6d", p2score);
	gotoxy(STATUS_X_ADJ, y + 14); printf("P2 Item  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_ITEM2 = y + 15); printf("        %6d", p2item);


	gotoxy(STATUS_X_ADJ, y + 16); printf("1P");
	gotoxy(STATUS_X_ADJ, y + 17); printf("  W   : Shift        Use Item: F");
	gotoxy(STATUS_X_ADJ, y + 18); printf("A  D : Left / Right     Pause : P");
	gotoxy(STATUS_X_ADJ, y + 19); printf("  S   : Drop            Quit: ESC");

	gotoxy(STATUS_X_ADJ, y + 21); printf("2P");
	gotoxy(STATUS_X_ADJ, y + 22); printf("  ¡è  : Shift        Use Item: M");
	gotoxy(STATUS_X_ADJ, y + 23); printf("¡ç  ¡æ  : Left / Right     Pause : P");
	gotoxy(STATUS_X_ADJ, y + 24); printf("  ¡é   : Drop            Quit: ESC");



}

void draw_main(void) {
	int i, j;

	for (j = 1; j<MAIN_X - 1; j++) {
		if (main_org[3][j] == EMPTY) main_org[3][j] = CEILLING;
	}
	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) {
			if (main_cpy[i][j] != main_org[i][j]) {

				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch (main_org[i][j]) {
				case EMPTY:
					printf("  ");
					break;
				case CEILLING:
					printf(". ");
					break;
				case WALL:
					printf("¢Ì");
					break;
				case INACTIVE_P1BLOCK:	
					printf("¡à");
					break;
				case INACTIVE_P2BLOCK:
					printf("¡á");
					break;
				case ACTIVE_BLOCK:
					if (turn % 2 == 0) {
						printf("¡á");
					}
					else {
						printf("¡à");
					}
					break;
				}
			}
		}
	}
	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) {
			main_cpy[i][j] = main_org[i][j];
		}
	}
}

void new_block(void) {
	 
	int i, j;
	int x = 3;
	int y =3;
	
	bx = (MAIN_X / 2) - 1;
	by = 0;
	b_type = b_type_next;
	if (item_used) {
		for (i =0; i< MAIN_X -2; i++){
		gotoxy(x, y + 5); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
		gotoxy(x, y + 6); printf("¢Ç                                        ¢Ç");
		gotoxy(x, y + 7); printf("¢Ç          Choose a next block           ¢Ç");
		gotoxy(x, y + 8); printf("¢Ç             for 5  seconds             ¢Ç");
		gotoxy(x, y + 9); printf("¢Ç     Press one key:  1  2  3  4         ¢Ç");
		gotoxy(x, y + 10); printf("¢Ç                                        ¢Ç");
		gotoxy(x, y + 11); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
	}
	Sleep(5000);
	key = getch();
		switch(key){
		case '1': 
			b_type_next =1; 
			b_type = b_type_next;
			break;
		case '2':
			b_type_next = 0;
			b_type = b_type_next;
			break;
		case '3':
			b_type_next = 6;
			b_type = b_type_next;
			break;
		case '4':
			b_type_next = 2;
			b_type = b_type_next;
			break;
}

item_used = 0;

system("cls");
reset_main_cpy();
draw_main();
draw_map();
	}
	
	b_type_next = rand() % 7;
	b_rotation = 0;


	new_block_on = 0;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
		}
	}
	for (i = 1; i<3; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("¡á");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}

void p1item_press(void) {

	if (!(item_used) && p1item) {
		if (turn % 2 == 0) {
			item_used = 1;
		}
		else {
			item_used = -1;
		}

		p1item_used++;

}
}


void p2item_press(void) {

	if (!(item_used) && p2item) {
		if (turn % 2 == 0) {
			item_used = -2;
		}
		else {
			item_used = 2;
		}

		p2item_used++;

	}
}


void p1check_key(void) {
	key = 0;

	if (kbhit()) {
		key = getch();
		if (key == 224) { // direction key
			do { key = getch(); } while (key == 224);
			switch (key) {
			case LEFT:
				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;
			case RIGHT:
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;
			case DOWN:
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;
			case UP:
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);

				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
			}
		}
		else { // Not direction eky
			switch (key) {
			case F:
			case f:
				p1item_press();

				p1item -= 1;
			
				break;

			case M:
			case m:
				p2item_press();
				p2item -= 1;
				
				break; 

			case P:
			case p:
				pause();
				break;
			case ESC:
				system("cls");
				exit(0);


			}
		}
	}
	while (kbhit()) getch();
}

void p2check_key(void){
key = 0;

if(kbhit()){
key = getch();
switch (key) {
case A:
case a:
	if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
	break;

case D:
case d:
	if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
	break;

case S:
case s:
	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
	break;

case W:
case w:
	if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);

	else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(101);
	break;

case F:
case f:
	p1item_press();

	p1item -= 1;
	break;

case M:
case m:
	p2item_press();

	p2item -= 1;
	break;

case P:
case p:
	pause();
	break;
case ESC:
	system("cls");
	exit(0);
}		
		}
		while (kbhit()) getch();
	}


int check_crush(int bx, int by, int b_rotation) {
	int i, j;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j]>0) return false;
		}
	}
	return true;
};

void drop_block(void) {
	int i, j;

	if (crush_on&&check_crush(bx, by + 1, b_rotation) == true) crush_on = 0;
	if (crush_on&&check_crush(bx, by + 1, b_rotation) == false) {
		for (i = 0; i<MAIN_Y; i++) {
			for (j = 0; j<MAIN_X; j++) {
				if (main_org[i][j] == ACTIVE_BLOCK) main_org[i][j] = TURNCHECKP1 ? INACTIVE_P1BLOCK: INACTIVE_P2BLOCK;
			}
		}
		crush_on = 0;
		check_line();
		new_block_on = 1;
		turn++;
		gotoxy(STATUS_X_ADJ, STATUS_Y_TURNNUMBER); printf("%s (Total Number of Turns : %d )", TURNCHECKP1? "Player 1's turn" : "Player 2's turn", turn);
		return;
	}
	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++;
}


void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT:
	case A:
	case a:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--;
		break;


	case RIGHT:
	case D:
	case d:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j + 1] = ACTIVE_BLOCK;
			}
		}
		bx++;
		break;


	case DOWN:
	case S:
	case s:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i + 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by++;
		break;

	case UP:
	case W:
	case w:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;

	case 101:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i - 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by--;
		break;


	}
}



void check_line(void) {
	int i, j, k, l;

	int block_amount; // block num of one line
	int p1combo =0;
	int p2combo =0;

	for (i = MAIN_Y - 2; i>3;) {
		block_amount = 0;
		for (j = 1; j<MAIN_X - 1; j++) {
			if (main_org[i][j]>0) block_amount++;

		}

		if (block_amount == MAIN_X - 2) {
		if (turn % 2 != 0){
			p1score += 1;
			p1combo++;
		} else
			p2score += 1;
			p2combo++;


			for (k = i; k>1; k--) {  // Block Down
				for (l = 1; l<MAIN_X - 1; l++) {
					if (main_org[k - 1][l] != CEILLING) main_org[k][l] = main_org[k - 1][l];
					if (main_org[k - 1][l] == CEILLING) main_org[k][l] = EMPTY;

				}
			}
		}
		else i--;
	}

	if (p1combo || p2combo) {
		if(p1combo > 1){
		p1item += 1;
		totalp1item +=1;
		} else if(p2combo > 1) {
		totalp2item += 1;
		p2item += 1;
		}
		
	
	}
	gotoxy(STATUS_X_ADJ, STATUS_Y_ITEM); printf("        %6d", p1item);
	gotoxy(STATUS_X_ADJ, STATUS_Y_ITEM2); printf("        %6d", p2item);
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", p1score);
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE2); printf("        %6d", p2score);
}



void check_game_over(void) {
	int i;




	int x = 5;
	int y = 5;

	for (i = 1; i<MAIN_X - 2; i++) {
		
		if (turn > 60 || main_org[3][i]>0) {  // end condition 
			
			FILE *fp =NULL;
			struct tm *z = localtime(&now);

			fp = fopen("save.txt", "wt");
			fprintf(fp, "Starting time when the game started: %02d/%02d/%d %02d:%02d:%02d\n"
				, z->tm_mday, z->tm_mon + 1, z -> tm_year + 1900, z->tm_hour, z->tm_min, z->tm_sec);
			fprintf(fp, " Player 1: ");
			fprintf(fp, "\n Score: %d", p1score);
			fprintf(fp, "\n Obtained Items: %d", totalp1item);
			fprintf(fp, " \n Used Items: %d" , (p1item_used));

			fprintf(fp, " \n\n Player 2: ");
			fprintf(fp, "\n Score: %d", p2score);
			fprintf(fp, "\n Obtained Items: %d", totalp2item);
			fprintf(fp, " \n Used Items: %d", (p2item_used));
			fclose(fp);


			gotoxy(x, y + 0); printf("¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È");
			gotoxy(x, y + 1); printf("¢È                              ¢È");
			gotoxy(x, y + 2); printf("¢È  +-----------------------+   ¢È");
			gotoxy(x, y + 3); printf("¢È  |  G A M E  O V E R..   |   ¢È");
			gotoxy(x, y + 4); printf("¢È  +-----------------------+   ¢È");
			gotoxy(x, y + 5); printf("¢È   PLAYER 1 SCORE: %6d     ¢È", p1score);
			gotoxy(x, y + 6); printf("¢È   PLAYER 2 SCORE: %6d     ¢È", p2score);
			gotoxy(x, y + 7); printf("¢È                              ¢È");

			if (p1score > p2score) {
				gotoxy(x, y + 8); printf("¢È         PLAYER 1 WINS!       ¢È");
			}
			else if (p1score < p2score) {
				gotoxy(x, y + 8); printf("¢È         PLAYER 2 WINS!       ¢È");
			}
			else{
				gotoxy(x, y + 8); printf("¢È             DRAW             ¢È");
			}

			gotoxy(x, y + 9); printf("¢È  Press any key to restart..  ¢È");
			gotoxy(x, y + 10); printf("¢È                              ¢È");
			gotoxy(x, y + 11); printf("¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È");





			Sleep(1000);
			while (kbhit()) getch();
			key = getch();
			reset();
		}
	}
}

void pause(void) {
	int i, j;

	int x = 5;
	int y = 5;

	for (i = 1; i<MAIN_X - 2; i++) {
		gotoxy(x, y + 0); printf("¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È");
		gotoxy(x, y + 1); printf("¢È                                    ¢È");
		gotoxy(x, y + 2); printf("¢È  +-----------------------+         ¢È");
		gotoxy(x, y + 3); printf("¢È  |       P A U S E       |         ¢È");
		gotoxy(x, y + 4); printf("¢È  +-----------------------+         ¢È");
		gotoxy(x, y + 5); printf("¢È  Press any key to resume..         ¢È");
		gotoxy(x, y + 6); printf("¢È                                    ¢È");
		gotoxy(x, y + 7); printf("¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È¢È");
	}
	getch();
	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();

	for (i = 1; i<3; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("¡á");
			}
			else {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("  ");
			}
		}
	}
}