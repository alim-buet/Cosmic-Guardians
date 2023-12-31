/*
    Original Author: S. M. Shahriar Nirjon
    Last Modified by: Mohammad Saifur Rahman
    last modified: October 13, 2015
    Version: 2.0
*/





# include "iGraphics.h"
#include "story.h"
#include "shoot.h"
#include<iostream>
#include<string>
#include<sstream>
#include<stdlib.h>

//#include<iostream>
using namespace std;
int mposx, mposy;
int play = 0;
bool gameend = false;
bool file = false;
static int file1 = 0;

int totalEnemy = 5;
int filesave = 0;
//BossEnemy
//int bx = 500, by = 450;
int dx = 30;
//int boss = 0;
//collisinCheck
//bool colck = false;
//healthCheck
//bool hlck = false;
//SCORE&LIFE
int life = 5;
int score = -1;
char scoreArray[50];
char lifeArray[50] ;
char ch[10];
//bossLIFE
int bossLife = 5;
char bossLifeArray[50];
FILE *fp;
char highscoreStr[10];
//music
bool musicOn = true;
//enemy structure
typedef struct bossbullet bossbullet;


typedef struct enemy1 enemy1;
struct enemy1{
	int x, y;
	bool On= false;
};
enemy1 e1[5];
int enemyIndex1 = 0;
typedef struct enemy2 enemy2;
struct enemy2{
	int x = 0, y = 0;
	bool On = false;
};
enemy2 e2[5];
int enemyIndex2 = 0;
typedef struct enemy3 enemy3;
struct enemy3{
	int x = 0, y = 0;
	bool On = false;
};
enemy3 e3[5];
int enemyIndex3 = 0;
typedef struct enemy4 enemy4;
struct enemy4{
	int x = 0, y = 0;
	bool On = false;
};
enemy4 e4[5];
int enemyIndex4 = 0;
typedef struct enemy5 enemy5;
struct enemy5{
	int x = 0, y = 0;
	bool On = false;
};
enemy5 e5[5];
int enemyIndex5 = 0;

//selection of ship and change of bullet array
char ship[3][25] = { "Images//ship.bmp", "Images//ship2.bmp", "Images//ship3.bmp" };
int shipIndex = 0;
char bulletcolor[3][25] = { "Images//bullet.bmp", "Images//bullet2.bmp", "Images//bullet3.bmp" };
int bulletcolorIndex = 0;
int selection = 0;
char bg[6][25] = { "Images//bg1.bmp", "Images//bg2.bmp", "Images//bg3.bmp", "Images//bg4.bmp", "Images//bg5.bmp","Images//bg6.bmp" };
int bgIndex = 0;
int x = 650, y = 20;
int i, j;
int imagePosition[20];
//moving background image array
char imgName[20][25] = { "Images//bg01.bmp", "Images//bg02.bmp", "Images//bg03.bmp", "Images//bg04.bmp", "Images//bg05.bmp", "Images//bg06.bmp", "Images//bg07.bmp", "Images//bg08.bmp", "Images//bg09.bmp", "Images//bg10.bmp", "Images//bg11.bmp", "Images//bg12.bmp", "Images//bg13.bmp", "Images//bg14.bmp", "Images//bg15.bmp", "Images//bg16.bmp", "Images//bg17.bmp", "Images//bg18.bmp", "Images//bg19.bmp", "Images//bg20.bmp" };

//int play=0;



void iDraw()
{

	iClear();
	//story flow
	if (storyIndex == 0){
		iShowBMP(0, 0, "Images//storybg1.bmp");
	}
	if (storyIndex == 1){
		iShowBMP(0, 0, "Images//storybg2.bmp");
	}
	if (storyIndex == 2){
		iShowBMP(0, 0, "Images//storybg3.bmp");
	}
	if (storyIndex == 3){
		iShowBMP(0, 0, "Images//storybg4.bmp");
	}
	if (storyIndex > 3){
		iShowBMP(0, 0, bg[bgIndex]);
		if (bgIndex == 1)
			//buttons of the menu screen
		{

			iShowBMP2(970, 440, "Images//button1.bmp", 255);
			iShowBMP2(970, 350, "Images//button2.bmp", 0);
			iShowBMP2(970, 260, "Images//button3.bmp", 0);
			iShowBMP2(970, 170, "Images//button4.bmp", 0);
			iShowBMP2(970, 80, "Images//button5.bmp", 0);
		}
		//ship selection
		if (bgIndex == 4){
			iShowBMP2(200, 100, "Images//ship.bmp", 0);
			iShowBMP2(200, 300, "Images//ship2.bmp", 0);
			iShowBMP2(200, 500, "Images//ship3.bmp", 0);


		}
		//background rendering and gameplay stuffs
		if (play == 1){
			for (i = 0; i < 20; i++){
				iShowBMP(0, imagePosition[i], imgName[i]);
			}
			for (i = 0; i < 20; i++){
				imagePosition[i] -= 100;
			}
			for (i = 0; i < 20; i++){
				if (imagePosition[i] < 0){
					imagePosition[i] = 800;
				}
			}
		}
		//gameplay ui
		if (play == 1)
		{
			iShowBMP2(x, y, ship[shipIndex], 0);
			iSetColor(13, 15, 255);
			_itoa(score, scoreArray, 10);
			iText(7, 710, "SC0RE:", GLUT_BITMAP_TIMES_ROMAN_24);
			iText(100, 710, scoreArray, GLUT_BITMAP_TIMES_ROMAN_24);

			iSetColor(21, 222, 210);
			if (boss == 1){
				_itoa(bossLife, bossLifeArray, 10);
				iText(980, 680, bossLifeArray, GLUT_BITMAP_TIMES_ROMAN_24);
				iShowBMP2(1000, 680, "Images//dead.bmp", 0);
				iShowBMP2(1030, 680, "Images//dead.bmp", 0);
				iShowBMP2(1060, 680, "Images//dead.bmp", 0);
				iShowBMP2(1090, 680, "Images//dead.bmp", 0);
				iShowBMP2(1120, 680, "Images//dead.bmp", 0);
				if (bossLife <= 4){
					iShowBMP2(1120, 680, "Images//shiplife.bmp", 0);
				}
				if (bossLife <= 3){
					iShowBMP2(1090, 680, "Images//shiplife.bmp", 0);
				}
				if (bossLife <= 2){
					iShowBMP2(1060, 680, "Images//shiplife.bmp", 0);
				}
				if (bossLife <= 1){
					iShowBMP2(1030, 680, "Images//shiplife.bmp", 0);
				}

			}
			_itoa(life, lifeArray, 10);
			iText(980, 710, lifeArray, GLUT_BITMAP_TIMES_ROMAN_24);
			iShowBMP2(1000, 705, "Images//shiplife.bmp", 0);
			iShowBMP2(1030, 705, "Images//shiplife.bmp", 0);
			iShowBMP2(1060, 705, "Images//shiplife.bmp", 0);
			iShowBMP2(1090, 705, "Images//shiplife.bmp", 0);
			iShowBMP2(1120, 705, "Images//shiplife.bmp", 0);
			if ( life<=4){
				iShowBMP2(1120, 705, "Images//dead.bmp", 0);
			}
			if (life<=3){
				iShowBMP2(1090, 705, "Images//dead.bmp", 0);
			}
			if ( life<=2){
				iShowBMP2(1060, 705, "Images//dead.bmp", 0);
			}
			if ( life<=1){
				iShowBMP2(1030, 705, "Images//dead.bmp", 0);
			}
			//strcpy(scoreArray, "SCORE:  ");
			//sprintf(ch, "%d", (score));
			//zprintf("%d\n", score);

			for (int i = 0; i < 5; i++){
				if (e1[i].On){
					iShowBMP2(e1[i].x, e1[i].y, "Images//enemy01.bmp", 0);
				}
				if (e2[i].On){
					iShowBMP2(e2[i].x, e2[i].y, "Images//enemy02.bmp", 0);
				}
				if (e3[i].On){
					iShowBMP2(e3[i].x, e3[i].y, "Images//enemy03.bmp", 0);
				}
				if (e4[i].On){
					iShowBMP2(e4[i].x, e4[i].y, "Images//enemy04.bmp", 0);
				}
				if (e5[i].On){
					iShowBMP2(e5[i].x, e5[i].y, "Images//enemy05.bmp", 0);
				}

			}


			strcat(scoreArray, ch);
			if (filesave == 1){
				FILE *fp = fopen("saveload.txt", "w");


				fprintf(fp, "%d\n", x);
				fprintf(fp, "%d\n", y);
				fprintf(fp, "%d\n", score);
				fprintf(fp, "%d\n", life);
				fprintf(fp, "%d\n", bulletcolorIndex);
				fprintf(fp, "%d\n", shipIndex);
				fprintf(fp, "%d\n", boss);
				fprintf(fp, "%d\n", bossLife);

				for (int j = 0; j <= totalEnemy; j++){
					fprintf(fp, "%d\n", e1[j].x);
					fprintf(fp, "%d\n", e2[j].x);
					fprintf(fp, "%d\n", e3[j].x);
					fprintf(fp, "%d\n", e4[j].x);
					fprintf(fp, "%d\n", e5[j].x);
					fprintf(fp, "%d\n", e1[j].y);
					fprintf(fp, "%d\n", e2[j].y);
					fprintf(fp, "%d\n", e3[j].y);
					fprintf(fp, "%d\n", e4[j].y);
					fprintf(fp, "%d\n", e5[j].y);


				}
				for (int i = 0; i <= totalBullet; i++){
					fprintf(fp, "%d\n", s[i].x1);
					fprintf(fp, "%d\n", s[i].y1);

				}

				fclose(fp);
			}


		}

		if (bgIndex == 1 && filesave == 2){
			play = 1;
			FILE *fp = fopen("saveload.txt", "r");

			fscanf(fp, "%d", &x);
			fscanf(fp, "%d", &y);
			fscanf(fp, "%d", &score);
			fscanf(fp, "%d", &life);
			fscanf(fp, "%d", &bulletcolorIndex);
			fscanf(fp, "%d", &shipIndex);
			fscanf(fp, "%d\n", &boss);
			fscanf(fp, "%d\n", &bossLife);

			for (int j = 0; j <= totalEnemy; j++){
				fscanf(fp, "%d", &e1[j].x);
				fscanf(fp, "%d", &e2[j].x);
				fscanf(fp, "%d", &e3[j].x);
				fscanf(fp, "%d", &e4[j].x);
				fscanf(fp, "%d", &e5[j].x);
				fscanf(fp, "%d", &e1[j].y);
				fscanf(fp, "%d", &e2[j].y);
				fscanf(fp, "%d", &e3[j].y);
				fscanf(fp, "%d", &e4[j].y);
				fscanf(fp, "%d", &e5[j].y);

			}
			for (int i = 0; i <= totalBullet; i++){
				fscanf(fp, "%d", &s[i].x1);
				fscanf(fp, "%d", &s[i].y1);

			}

			fclose(fp);
			filesave = 3;
		}
	}
	if (score%15  == 0 && score>0 && play == 1 && bgIndex != 5){
		boss = 1;
		iShowBMP2(bx, by, "Images//BossEnemy.bmp", 0);
		for (i = 0; i <= totalEnemy; i++){
			e1[i].On = false;
			e2[i].On = false;
			e3[i].On = false;
			e4[i].On = false;
			e5[i].On = false;

		}
		/*for (j = 0; j <= totalBullet; j++){
		s[j].gulliOn = false;
		}*/
	}


	//bullet from the ship
	for (int i = 0; i < totalBullet; i++){
		if (s[i].gulliOn){
			iShowBMP2(s[i].x1, s[i].y1, bulletcolor[bulletcolorIndex], 0);
		}
	}
	if (boss == 1){
	for (int k = 0; k < bosstotal; k++){
		if (bb[k].setPosition && bb[k].bossshoot)
		{
			iShowBMP2(bb[k].xb, bb[k].yb, "Images//BossBullet.bmp", 0);
		}
	}
}
	if (play == 1 && life == 0){

		bgIndex = 5;
		boss = 0;
		play = 2;
		life = 5;
		for (int i = 0; i <= totalEnemy; i++){
			e1[i].On = false;
			e2[i].On = false;
			e3[i].On = false;
			e4[i].On = false;
			e5[i].On = false;

		}
		for (int j = 0; j <= totalBullet; j++){
			s[j].gulliOn = false;
		}

		gameend = true;
		if (file1 == 0){
			file = true;
			file1++;
		}

		//bossLife = 5;

		for (int k = 0; k <= bosstotal; k++){
			bb[k].bossshoot = false;
			bb[k].setPosition = false;
		}
	}
	if (bgIndex == 5 && gameend){
		_itoa(score, scoreArray, 10);
		if (file){
			fp = fopen("highscore.txt", "a");
			fprintf(fp, "%d \n", score);
			fclose(fp);
			file = false;
		}
		file1 = 0;
	}
	if (bgIndex == 3 && (play == 0 || play == 2)){
		fp = fopen("highscore.txt", "r");
		int highscoreArray[1000];
		for (int i = 0; i < 1000; i++){
			highscoreArray[i] = -1;
		}

		for (int i = 0; fscanf(fp, "%d", &highscoreArray[i]) != EOF; i++){

		}
		int temp = 0;
		for (int i = 0; i < 1000; i++){
			for (int j = i; j < 1000; j++){
				if (highscoreArray[i] < highscoreArray[j]){
					temp = highscoreArray[i];
					highscoreArray[i] = highscoreArray[j];
					highscoreArray[j] = temp;
				}
			}
		}
		fclose(fp);
		fp = fopen("highscore.txt", "w");
		for (int i = 0; i < 10; i++){
			fprintf(fp, "%d\n", highscoreArray[i]);
		}
		fclose(fp);
		fp = fopen("highscore.txt", "r");
		int highscore[5];
		fscanf(fp, "%d%d%d%d%d", &highscore[0], &highscore[1], &highscore[2], &highscore[3], &highscore[4]);
		_itoa(highscore[0], highscoreStr, 10);
		iSetColor(255, 255, 255);
		_itoa(highscore[0], highscoreStr, 10);
		iText(980, 550, highscoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(650, 550, "1st:           -----", GLUT_BITMAP_TIMES_ROMAN_24);

		_itoa(highscore[1], highscoreStr, 10);
		iText(980, 450, highscoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(650, 450, "2nd:           -----", GLUT_BITMAP_TIMES_ROMAN_24);

		_itoa(highscore[2], highscoreStr, 10);
		iText(980, 350, highscoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(650, 350, "3rd:           -----", GLUT_BITMAP_TIMES_ROMAN_24);

		_itoa(highscore[3], highscoreStr, 10);
		iText(980, 250, highscoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(650, 250, "4th:           -----", GLUT_BITMAP_TIMES_ROMAN_24);

		_itoa(highscore[4], highscoreStr, 10);
		iText(980, 150, highscoreStr, GLUT_BITMAP_TIMES_ROMAN_24);
		iText(650, 150, "5th:           -----", GLUT_BITMAP_TIMES_ROMAN_24);


		fclose(fp);
	}
}




/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (storyIndex <= 3){
			storyIndex =4;
		}
		//menu selectioning script
			if (bgIndex == 0&&storyIndex>4)
			{

				bgIndex = 1;
			}

			if (bgIndex == 1 && mx >= 970 && mx <= 1170 && my >= 440 && my <= 510&&play==0)
			{
				bgIndex=4;
            }
			if (bgIndex == 1 && mx >= 970 && mx <= 1170 && my >= 350 && my <= 420)
			{
                bgIndex = 2;
			}
			if (bgIndex == 1 && mx >= 970 && mx <= 1170 && my >= 260 && my <= 320)
			{
				bgIndex = 3;
			}
			if (bgIndex == 1 && mx >= 970 && mx <= 1170 && my >= 170 && my <= 240)
			{
				exit(0);
			}
			if (bgIndex == 1 && mx >= 970 && mx <= 1170 && my >= 80 && my <= 150){
				filesave = 2;
			}
			if (play == 2 && bgIndex == 5){

			}
			//spacecraft selection
			if (bgIndex == 4 && mx >= 200 && mx <= 280 && my >= 100 && my <= 180 && play == 0){
				play = 1;
				if (musicOn){
					musicOn = false;
					//PlaySound(0, 0, 0);
				}
				shipIndex = 0;
				bulletcolorIndex = 0;
				life = 5;
			}
			if (bgIndex == 4 && mx >= 200 && mx <= 280 && my >= 300 && my <= 380 && play == 0){
				play = 1;
				if (musicOn){
					musicOn = false;
					//PlaySound(0, 0, 0);
				}
				shipIndex = 1;
				bulletcolorIndex = 1;
				life = 5;
			}
			if (bgIndex == 4 && mx >= 200 && mx <= 280 && my >= 500 && my <= 580 && play == 0){
				play = 1;
				if (musicOn){
					musicOn = false;
					//PlaySound(0, 0, 0);
				}
				shipIndex = 2;
				bulletcolorIndex = 2;
				life = 5;
			}
	}
	//the starting position of the ship bullet
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (play == 1){
			//PlaySound("Music\\shootMusic.wav", NULL, SND_ASYNC);
			s[gulliIndex].x1 = x + 23;
			s[gulliIndex].y1 = y + 40;

			s[gulliIndex].gulliOn = true;
			gulliIndex++;
			if (gulliIndex > totalBullet)
			{
				gulliIndex = 0;
			}
		}
	}
	}


/*iPassiveMouseMove is called to detect and use
the mouse point without pressing any button */

void iPassiveMouseMove(int mx,int my)
{
	//place your code here

 mposx = mx;
 mposy = my;
 if(mx== 2){}        /*Something to do with mx*/
 else if(my== 2){}   /*Something to do with my*/

}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
	if (key == 's')
	{
		filesave = 1;
	}
	if (key == 'l'){

	}
	//place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/

void iSpecialKeyboard(unsigned char key)
{
	//movement of the ship in a particular area
	if(key == GLUT_KEY_LEFT)
	{
		if (x >= 20 && x <= 1200)
		{
			x = x - 20;


		}
		else if (x<10)
		{
			x = 20;
		}

	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (x >= 0 && x < 1110)
		{
			x = x + 20;


		}
		else if (x > 1110)
		{
			x = 1110;
		}

	}
	if (key == GLUT_KEY_UP)
	{
		if (y > 10 && y <= 700)
			y = y + 20;


	}
	if (key == GLUT_KEY_DOWN)
	{
		if (y > 20 && x <= 1170)
		{
			y = y - 20;


		}
		else if (y < 20)
		{
			y = 20;
		}

	}
	if (key == GLUT_KEY_HOME){
		if (bgIndex == 2 || bgIndex == 3 || bgIndex == 4){
			bgIndex = 1;
			//bgIndex = 4;
			score = 0;
			life = 5;
			x = 650;
			y = 20;
			boss = 0;
			bossLife = 5;
			play = 0;
		}
		if (play == 1){
			bgIndex = 4;
			score = 0;
			life = 5;
			x = 650;
				y = 20;
				boss = 0;
				bossLife = 5;
				play = 0;
		}
	}
	if (key == GLUT_KEY_HOME&&selection == 0 && bgIndex == 4){
		bgIndex = 1;
		bgIndex = 4;
		score = 0;
		life = 5;
		x = 650;
		y = 20;
		boss = 0;
		bossLife = 5;
		play = 0;
	}
	if (key == GLUT_KEY_HOME&&play == 1){
		play = 0;
		selection = 0;
		bgIndex = 4;
		life = 5;
		for (int i = 0; i <= totalEnemy; i++){
			e1[i].On = false;
			e2[i].On = false;
			e3[i].On = false;
			e4[i].On = false;
			e5[i].On = false;

		}
		x = 650;
		y = 20;
		for (int j = 0; j <= totalBullet; j++){
			s[j].gulliOn = false;
		}
		for (int k = 0; k <= bosstotal; k++){
			bb[k].bossshoot = false;
			bb[k].setPosition = false;
		}
	}
	if (key == GLUT_KEY_HOME&&play == 2 && bgIndex == 5){
		bgIndex = 1;
		life = 5;
		score = 0;
		boss = 0;
		play = 0;
		file1 = 0;
		for (int i = 0; i <= totalEnemy; i++){
			e1[i].On = false;
			e2[i].On = false;
			e3[i].On = false;
			e4[i].On = false;
			e5[i].On = false;

		}
		x = 650;
		y = 20;
		for (int j = 0; j <= totalBullet; j++){
			s[j].gulliOn = false;
		}
		for (int k = 0; k <= bosstotal; k++){
			bb[k].bossshoot = false;
			bb[k].setPosition = false;
		}
	}
}
	//place your codes for other keys here

//background rendering
void imagePositionf()
{
	int i, j;
	for (i = 0, j = 0; i<20; i++){
		imagePosition[i] = j;
		j += 100;
	}
}
//BossEnemyFunctions
/*void bossMove(){
	bx = bx + dx;
	if (x > 530){
		dx = dx*(-1);
	}
	if (x < 20){
		dx = dx*(-1);
	}
}*/




//enemy fuctions
void callEnemy1(){
	e1[enemyIndex1].On = true;
	e1[enemyIndex1].x = rand() % 1100;
	e1[enemyIndex1].y = 730;
	enemyIndex1++;
	if (enemyIndex1 >totalEnemy){
		enemyIndex1 = 0;
	}
}
void callEnemy2(){
	e2[enemyIndex2].On = true;
	e2[enemyIndex2].x = rand() % 1100;
	e2[enemyIndex2].y = 730;
	enemyIndex2++;
	if (enemyIndex2 > totalEnemy){
		enemyIndex2 = 0;
	}
}
void callEnemy3(){
	e3[enemyIndex3].On = true;
	e3[enemyIndex3].x = rand() % 1100;
	e3[enemyIndex3].y = 730;
	enemyIndex3++;
	if (enemyIndex3 >totalEnemy){
		enemyIndex3 = 0;
	}
}
void callEnemy4(){
	e4[enemyIndex4].On = true;
	e4[enemyIndex4].x = rand() % 1100;
	e4[enemyIndex4].y = 730;
	enemyIndex4++;
	if (enemyIndex4 >totalEnemy){
		enemyIndex4 = 0;
	}
}
void callEnemy5(){
	e5[enemyIndex5].On = true;
	e5[enemyIndex5].x = rand() % 1100;
	e5[enemyIndex5].y = 730;
	enemyIndex5++;
	if (enemyIndex5 >totalEnemy){
		enemyIndex5 = 0;
	}
}
void enemyMove(){
	if (boss == 0){
		for (int i = 0; i < totalEnemy; i++){
			if (e1[i].On == true){
				if (e1[i].y >= 550)e1[i].y -= 20;
				if (e1[i].y <= 550){
					e1[i].x -= 20;
					e1[i].y -= 10;
				}
				if (e1[i].x <= 0){
					e1[i].y = 730;
					e1[i].On = false;
				}
			}
			if (e2[i].On == true){
				e2[i].y -= 20;
				if (e2[i].y <= 0){
					e2[i].y = 730;
					e2[i].On = false;
				}
			}
			if (e3[i].On == true){
				if (e3[i].y >= 450)e3[i].y -= 20;
				if (e3[i].y <= 450){
					e3[i].x += 20;
					e3[i].y -= 10;
				}
				if (e4[i].x <= 0){
					e4[i].y = 730;
					e4[i].On = false;
				}
			}
			if (e4[i].On == true){
				if (e4[i].y >= 450)e4[i].y -= 20;
				if (e4[i].y <= 450){
					e4[i].x += 20;
					e4[i].y -= 10;
				}
				if (e4[i].x <= 0){
					e4[i].y = 730;
					e4[i].On = false;
				}
			}
			if (e5[i].On == true){
				e5[i].y -= 20;
				if (e5[i].y <= 0){
					e5[i].y = 730;
					e5[i].On = false;
				}
			}
		}
	}
	else if (boss == 1){
		bx = bx + dx;
		if (bx > 1000){
			dx = dx*(-1);
		}
		if (bx < 20){
			dx = dx*(-1);
		}
	}
}
void collitionCheck(){
	if (play == 1){
		for (int i = 0; i <= totalBullet; i++){
			for (int j = 0; j <= totalEnemy; j++){





				if ((s[i].x1 + 31 >= e1[j].x &&s[i].x1 <= e1[j].x + 77) && (s[i].y1 + 51 >= e1[j].y&&s[i].y1 <= e1[j].y + 87)){
					if (e1[j].On == true && s[i].gulliOn == true)
					{

						e1[j].On = false;
						//colck = true;
						//printf("dead");
						score = score++;
						s[i].gulliOn = false;
						/*strcpy(scoreArray, "SCORE:  ");
						sprintf(ch, "%d", (score));
						printf("%d", score);
						strcat(scoreArray, ch);*/
					}


				}
				if ((s[i].x1 + 31 >= e2[j].x &&s[i].x1 <= e2[j].x + 77) && (s[i].y1 + 51 >= e2[j].y&&s[i].y1 <= e2[j].y + 87)){

					if (e2[j].On == true && s[i].gulliOn == true)
					{
						e2[j].On = false;
						//colck = true;
						//printf("dead");
						score = score++;
						s[i].gulliOn = false;

					}

				}

				if ((s[i].x1 + 31 >= e3[j].x &&s[i].x1 <= e3[j].x + 77) && (s[i].y1 + 51 >= e3[j].y&&s[i].y1 <= e3[j].y + 87)){

					if (e3[j].On == true && s[i].gulliOn == true)
					{
						e3[j].On = false;
						//colck = true;
						//printf("dead");
						score = score++;
						s[i].gulliOn = false;

						/*strcpy(scoreArray, "SCORE:  ");
						sprintf(ch, "%d", (score));
						printf("%d", score);

						strcat(scoreArray, ch);*/
					}


				}

				if ((s[i].x1 + 31 >= e4[j].x &&s[i].x1 <= e4[j].x + 54) && (s[i].y1 + 51 >= e4[j].y&&s[i].y1 <= e4[j].y + 107)){

					if (e4[j].On == true && s[i].gulliOn == true)
					{
						e4[j].On = false;
						//colck = true;
						//printf("dead");
						score = score++;
						s[i].gulliOn = false;

						/*strcpy(scoreArray, "SCORE:  ");
						sprintf(ch, "%d", (score));
						printf("%d", score);

						strcat(scoreArray, ch);*/

					}
				}

				if ((s[i].x1 + 31 >= e5[j].x &&s[i].x1 <= e5[j].x + 54) && (s[i].y1 + 51 >= e5[j].y&&s[i].y1 <= e5[j].y + 107)){

					if (e5[j].On == true && s[i].gulliOn == true)
					{
						e5[j].On = false;
						//colck = true;
						//printf("dead");
						score = score++;
						s[i].gulliOn = false;

						/*strcpy(scoreArray, "SCORE:  ");
						sprintf(ch, "%d", (score));
						printf("%d", score);*/
					}


					strcat(scoreArray, ch);

				}
			}
		}
			for (int i = 0; i <= totalBullet; i++){

				if (boss == 1){
					if ((s[i].x1+31>=bx&&s[i].x1<=bx+250)&&(s[i].y1+51>=by&&s[i].y1<=by+250)&& s[i].gulliOn == true&&bossLife>0 ){
						s[i].gulliOn = false;
						bossLife = bossLife--;
						if (bossLife == 0){
							score = score + 5;
							boss = 0;
							bossLife = 5;
						}
					}
					strcat(scoreArray, ch);
					strcat(bossLifeArray, ch);
				}
			}
			if (boss == 1){
				for (int i = 0; i <= bosstotal; i++){

						if ((bb[i].xb+42>x&&bb[i].xb<x+80)&&(bb[i].yb+66>y&&bb[i].yb<y+80)&&bossLife>0&&life>0&&bb[i].bossshoot==true&&bb[i].setPosition==true){
							bb[i].bossshoot = false;
							life = life--;
							//printf("collisionwithboss");
						}
						strcat(lifeArray, ch);
					}

			}
		}

	for (int j = 0; j <= totalEnemy; j++){
		//if (s[j].y1 >= 1170)s[j].gulliOn = false;
		if ((x+80>e1[j].x&&x<e1[j].x+77)&&(y+80>e1[j].y&&y<e1[j].y+87)&&life>0){

			if (e1[j].On == true){
				e1[j].On = false;
				life = life--;
			}
		}
		else if ((x + 80>e2[j].x&&x<e2[j].x + 77) && (y + 80>e2[j].y&&y<e2[j].y + 87) && life>0){

			if (e2[j].On == true){
				e2[j].On = false;
				life = life--;
			}

		}
		else if ((x + 80>e3[j].x&&x<e3[j].x + 77) && (y + 80>e3[j].y&&y<e3[j].y + 87) && life>0){
			if (e3[j].On == true){
				e3[j].On = false;
				life = life--;
			}

		}
		else if ((x + 80>e4[j].x&&x<e4[j].x + 54) && (y + 80>e4[j].y&&y<e4[j].y + 107) && life>0){

			if (e4[j].On == true){
				e4[j].On = false;
				life = life--;
			}
		}
		else if ((x + 80>e5[j].x&&x<e5[j].x + 54) && (y + 80>e5[j].y&&y<e5[j].y + 107) && life>0){

			if (e5[j].On == true){
				e5[j].On = false;
				life = life--;
			}

		}
		strcat(lifeArray, ch);
	}
	}





void bossshoot(){
	if (boss == 1){
		if (!bb[bbindex].setPosition){
			bb[bbindex].xb = bx + 120;
			bb[bbindex].yb = by;
			bb[bbindex].bossshoot = true;
			bb[bbindex].setPosition = true;

		}
		bbindex++;
		if (bbindex >= bosstotal){
			bbindex = 0;
		}

	}
}
/*void  scoreCount(){
	if (colck == true){
		score = score++;
		strcpy(scoreArray, "SCORE:  ");
		sprintf(ch, "%d", (score));
		strcat(scoreArray, ch);
	}
}*/
/*void healthcheck(){
	if (hlck == true){
		life = life--;
		strcpy(lifeArray, "LIFE:  ");
		sprintf(ch, "%d", (life));
		strcat(lifeArray, ch);
	}
}*/
int main()
{
	if (musicOn){
		//PlaySound("Music\\backgroundMusic.wav", NULL, SND_ASYNC);
	}
	imagePositionf();
	iSetTimer(1, collitionCheck);
	iSetTimer(ti, storyshow);
	iSetTimer(5, shoot);
	iSetTimer(11000 + rand() % 4000, callEnemy1);
	iSetTimer(11500 + rand() % 1500, callEnemy2);
	iSetTimer(14000 + rand() % 1000, callEnemy3);
	iSetTimer(15000 + rand() % 1500, callEnemy4);
	iSetTimer(16000 + rand() % 1000, callEnemy5);
	iSetTimer(80, enemyMove);
	iSetTimer(800, bossshoot);
	//iSetTimer(50, healthCount);
	//iSetTimer(70, scoreCount);
	//iSetTimer(40, healthcheck);
	//place your own initialization codes here.
	iInitialize(1200, 750, "THE RETRO SPACE SHOOTER");
	iStart();
	return 0;
}
