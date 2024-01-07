// including necessary header files.
// to make the game challenging once in a while a clusture of asteroids will fall down with rather a speedy fashion we will
// need some special weapon to destroy it or a handy amount of regular bullet.
// i have to change the image ofship because i dont like 'em
#include "iGraphics.h"
#include <windows.h>
#include "mmsystem.h"
#include <math.h>
#include <string.h>
// defining constants
#define screenWidth 1080
#define screenHeight 608
#define buttonwidth 285
#define buttonheight 64
bool musicOn = true;
bool gameon = false;
bool gameover = false;
int t1;			   // timer 1 for changing color
int t2;			   // timer 2 is for animating the ship
int GameState = 0; // 0-main menu  1-game   2-story    3- high score  4-credit 5-how to play 6- playername  7- game over
char playername[100];
int PlayerScore = 0;
int PlayerHealth = 100;
int PlayerLife = 4;
int r = 0, g = 0, b = 0;
/*
A LOT OF THINGS TO BE INCLUDED HERE SUCH AS DEFINING SOME CONST. DEFINING STRUCTURES, DEFINING VARIABLES
*/
// including bmp images
char bg[10][40] = {
	"backgrounds\\menu.bmp",	   // 0
	"backgrounds\\play.bmp",	   // 1
	"backgrounds\\story.bmp",	   // 2
	"backgrounds\\highscores.bmp", // 3
	"backgrounds\\credit.bmp",	   // 4
	"backgrounds\\htp.bmp",		   // 5
	"backgrounds\\username.bmp",   // 6
	"backgrounds\\gameover.bmp"	   // 7

};
char music[6][40] = {"music\\menubgm.wav",	 // 0
					 "music\\gamebgm.wav",	 // 1
					 "music\\gunshot.wav",	 // 2
					 "music\\explosion.wav", // 3
					 "music\\gameover.wav"}; // 4
char ship[40][60] = {
	"rocket\\ships\\0.bmp",
	"rocket\\ships\\10.bmp",
	"rocket\\ships\\20.bmp",
	"rocket\\ships\\30.bmp",
	"rocket\\ships\\40.bmp",
	"rocket\\ships\\50.bmp",
	"rocket\\ships\\60.bmp",
	"rocket\\ships\\70.bmp",
	"rocket\\ships\\80.bmp",
	"rocket\\ships\\90.bmp",
	"rocket\\ships\\100.bmp",
	"rocket\\ships\\110.bmp",
	"rocket\\ships\\120.bmp",
	"rocket\\ships\\130.bmp",
	"rocket\\ships\\140.bmp",
	"rocket\\ships\\150.bmp",
	"rocket\\ships\\160.bmp",
	"rocket\\ships\\170.bmp",
	"rocket\\ships\\180.bmp",
	"rocket\\ships\\190.bmp",
	"rocket\\ships\\200.bmp",
	"rocket\\ships\\210.bmp",
	"rocket\\ships\\220.bmp",
	"rocket\\ships\\230.bmp",
	"rocket\\ships\\240.bmp",
	"rocket\\ships\\250.bmp",
	"rocket\\ships\\260.bmp",
	"rocket\\ships\\270.bmp",
	"rocket\\ships\\280.bmp",
	"rocket\\ships\\290.bmp",
	"rocket\\ships\\300.bmp",
	"rocket\\ships\\310.bmp",
	"rocket\\ships\\320.bmp",
	"rocket\\ships\\330.bmp",
	"rocket\\ships\\340.bmp",
	"rocket\\ships\\350.bmp"};
// variables related to ship
int ShipWidth;
int ShipHeight;
int ShipX = 500, ShipY = 100;
int ShipVelocity = 0;
int ShipMaxVelocity = 20;
int ShipCurrentVelocity = 0;
int ShipDeltaVelocity = 5;
int shipind = 0;

// including buttons
char buttons[5][60] = {"buttons\\sound off.bmp", "buttons\\sound on.bmp"};
char bullet[3][60] = {"rocket\\bullet.bmp"};
// prototype of functions
void soundbutton();
void soundcontrol();
void MenuSetup();
void menumousecontrol(int button, int state, int mx, int my);
void backbuttonfunction(int button, int state, int mx, int my);
void playernamemousecontrol(int button, int state, int mx, int my);
void showhighscore();
void modifyscoreboard();
void maingame();
void scorebar();
void healthbar();
void scoreupdate();
void healthupdate();
void gameoverscreen();
void animateship();
void iDraw()
{
	iClear();
	iShowBMP(0, 0, bg[GameState]); // draw the appropriate game bg
	if (GameState == 0)
	{
		soundbutton();
	}
	if (GameState == 6)
	{
		iSetColor(0, 0, 0);
		iText(300, 310, playername, GLUT_BITMAP_TIMES_ROMAN_24);
	}
	// highscore showing
	if (GameState == 3)
	{
		showhighscore();
	}
	// when we are in main game window
	if (GameState == 1)
	{
		iResumeTimer(t2);
		maingame();
	}
	if (GameState == 7)
	{
		gameoverscreen();
	}
}

void iMouseMove(int mx, int my)
{
}

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("curren pos x: %d     y: %d\n", mx, my);
		switch (GameState)
		{
			// when we are in main menu
		case 0:
			menumousecontrol(button, state, mx, my);
			break;
		case 6:
			playernamemousecontrol(button, state, mx, my);
			break;
		// this is temporary
		case 1:
			PlayerScore += 50;
			break;
		case 7:
			// when we are in game over screen press any key to continue
			GameState = 0; // going back to menu
			soundcontrol();
			iPauseTimer(t1); // pausing the text blinker timer
			break;
		default:
			backbuttonfunction(button, state, mx, my);
			break;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (GameState == 1)
		{
			// just a temporary system for testing health update
			if (PlayerHealth != 0)
			{
				PlayerHealth -= 25;
			}
		}
	}
}
// taking playername
int ind = -1;

void iKeyboard(unsigned char key)
{
	// taking name from user and storing it in playername array
	if (GameState == 6)
	{
		if (ind != -1 && key == '\b')
		{
			ind--;
			playername[ind + 1] = '\0';
		}
		else if (ind != -1 && key == '\r')
		{
			GameState = 1;
			gameon = true;
			soundcontrol();
			maingame();
			printf("Player name is %s\n", playername);
		}
		else if (key != '\r')
		{
			playername[ind + 1] = key;
			playername[ind + 2] = '\0';
			ind++;
		}
	}
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
	// when we are in main game
	if (GameState == 1)
	{
		if (key == GLUT_KEY_RIGHT)
		{
			if (shipind == 35)
			{
				shipind = 0;

			} // changing the direction of the ship
			else
			{
				shipind += 1;
			}
		}
		else if (key == GLUT_KEY_LEFT)
		{
			if (shipind == 0)
			{
				shipind = 35;
			}
			else
			{
				shipind -= 1;
			}
		}
		else if (key == GLUT_KEY_UP)
		{
			if (ShipCurrentVelocity < ShipMaxVelocity)
			{
				ShipCurrentVelocity += ShipDeltaVelocity;
			}
		}
		else if (key == GLUT_KEY_DOWN)
		{
			if (ShipCurrentVelocity > 0)
			{
				ShipCurrentVelocity -= ShipDeltaVelocity;
			}
		}
	}
}

void MenuSetup()
{
	iShowBMP(0, 0, bg[0]);
	// showmenubuttons();
	soundbutton();
}
void menumousecontrol(int button, int state, int mx, int my)
{
	// 0-main menu  1-game   2-story    3- high score  4-credit 5-how to play 6- name input
	// clicked play button
	if (mx >= 195 && mx <= 195 + buttonwidth && my >= 403 && my <= 403 + buttonheight)
	{
		// initializing the values for playing again
		PlayerHealth = 100;
		PlayerScore = 0;
		memset(playername, 0, sizeof(playername));
		ind = -1;

		GameState = 6;
	}
	// clicked story button
	else if (mx >= 195 && mx <= 195 + buttonwidth && my >= 325 && my <= 325 + buttonheight)
	{
		GameState = 2;
	}
	// clicked highscore button
	else if (mx >= 195 && mx <= 195 + buttonwidth && my >= 243 && my <= 243 + buttonheight)
	{
		GameState = 3;
		showhighscore();
		// highscores er window dekhabe
	}

	// clicked credit button
	else if (mx >= 195 && mx <= 195 + buttonwidth && my >= 160 && my <= 160 + buttonheight)
	{
		GameState = 4;
	}
	// clicked how to play button
	else if (mx >= 195 && mx <= 195 + buttonwidth && my >= 78 && my <= 78 + buttonheight)
	{
		GameState = 5;
	}
	// changing the music button when clicked
	if (mx >= 590 && mx <= 675 && my >= 5 && my <= 90)
	{
		musicOn = !(musicOn);
		soundcontrol();
	}
}
void backbuttonfunction(int button, int state, int mx, int my)
{
	if (GameState > 1 && GameState != 7)
	{
		// back button clicked
		if (mx >= 35 && mx <= 160 && my >= 508 && my <= 564)
		{
			GameState = 0; // getting back to main menu
		}
	}
}
void modifyscoreboard()
{
	FILE *fptr;
	char names[5][50]; // storing the name of the top scorers
	int HighScores[5]; // storing their score in respective index
	fptr = fopen("highscores.txt", "r");
	int i = 0;
	while (fscanf(fptr, "%s %d", names[i], &HighScores[i]) != EOF)
	{
		i++;
	} // now we have extracted score data from highscore file
	fclose(fptr);
	// now we will modify the leaderboard
	int isdone = 0;
	for (int i = 0; i < 5; i++)
	{
		if (isdone)
			break;
		if (PlayerScore >= HighScores[i])
		{
			isdone = 1;
			for (int j = 4; j > i; j--)
			{
				strcpy(names[j], names[j - 1]);
				HighScores[j] = HighScores[j - 1];
			}
			HighScores[i] = PlayerScore;
			strcpy(names[i], playername);
		}
	}
	// now we have our sorted arrays.. we will write the modified text in the highscore file
	FILE *fptr2;
	fptr2 = fopen("highscores.txt", "w");
	for (int i = 0; i < 5; ++i)
	{
		fprintf(fptr2, "%s %d\n", names[i], HighScores[i]);
	}
	fclose(fptr2);
}

void scorebar()
{
	iSetColor(255, 255, 255);
	iText(950, 580, "Score: ", GLUT_BITMAP_HELVETICA_18);
	char score[10];
	sprintf(score, "%d", PlayerScore);
	iText(1015, 580, score, GLUT_BITMAP_HELVETICA_18);
}
void healthbar()
{
	iSetColor(255, 255, 255);
	iText(950, 550, "Health: ", GLUT_BITMAP_HELVETICA_18);
	iSetColor(255, 0, 0);
	iRectangle(950, 530, 100, 10);
	iSetColor(0, 250, 0);
	iFilledRectangle(950, 530, PlayerHealth, 10);
}
void animateship()
{
	double theta = ((90 - shipind * 10) * 3.1416) / 180.0;

	ShipX += ((double)(ShipCurrentVelocity)*cos(theta));
	ShipY += ((double)(ShipCurrentVelocity)*sin(theta));
}
void maingame()
{
	// called by idraw
	//  in the main game section we will have score and life option in the corner;
	scorebar();
	healthbar();
	// load ship
	iShowBMP2(ShipX, ShipY, ship[shipind], 0);

	// scoreupdate();
	// healthupdate();  //will update them accordingly when needed.. currently wrote a demo update code in imouse founction

	if (PlayerHealth == 0)
	{
		GameState = 7;
		soundcontrol();
		gameover = true;
		gameon = false;

		modifyscoreboard();
	}
}
void showhighscore()
{
	FILE *fptr;
	char names[5][50]; // storing the name of the top scorers
	int HighScores[5]; // storing their score in respective index
	fptr = fopen("highscores.txt", "r");
	int i = 0;
	while (fscanf(fptr, "%s %d", names[i], &HighScores[i]) != EOF)
	{
		i++;
	} // now we have extracted score data from highscore file
	fclose(fptr);
	// showing the names
	for (int i = 0; i < 5; i++)
	{
		iText(350, 175 + i * 55, names[4 - i], GLUT_BITMAP_TIMES_ROMAN_24);
	}
	// showing the scores
	iSetColor(255, 255, 255);
	for (int i = 0; i < 5; i++)
	{
		char StrScore[10];
		sprintf(StrScore, "%d", HighScores[4 - i]);
		iText(650, 175 + i * 55, StrScore, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

void gameoverscreen()
{

	iResumeTimer(t1);
	char score[10];
	sprintf(score, "%d", PlayerScore);
	iSetColor(255, 255, 255);
	iText(618, 250, score, GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(r, g, b);
	iText(400, 200, "Click anywhere to continue", GLUT_BITMAP_TIMES_ROMAN_24);
}
void playernamemousecontrol(int button, int state, int mx, int my)
{
	if (mx >= 428 && mx <= 658 && my >= 185 && my <= 245)
	{
		if (ind != -1)
		{
			GameState = 1;
			gameon = true;
			soundcontrol();

			maingame();
			printf("Player name is %s\n", playername);
		}
	}
}
void colorchanger()
{
	r = rand() % 255;
	g = rand() % 255;
	b = rand() % 255;
}

void soundbutton()
{
	if (musicOn)
	{
		iShowBMP2(590, 5, buttons[1], 0);
	}

	else
		iShowBMP2(590, 5, buttons[0], 0);
}
void soundcontrol()
{
	if (musicOn)
	{
		if (GameState == 1)
		{
			PlaySound(music[1], NULL, SND_LOOP | SND_ASYNC);
		}
		else if (GameState == 7)
		{
			PlaySound(music[4], NULL, SND_ASYNC);
		}
		else
		{
			PlaySound(music[0], NULL, SND_LOOP | SND_ASYNC);
		}
	}
	else
	{
		PlaySound(0, 0, 0);
	}
}
int main()
{
	// place your own initialization codes here.
	t1 = iSetTimer(200, colorchanger);
	iPauseTimer(t1);
	t2 = iSetTimer(10, animateship);
	iPauseTimer(t2);
	PlaySound(music[0], NULL, SND_LOOP | SND_ASYNC);
	iInitialize(screenWidth, screenHeight, "Cosmic Guardians");
	return 0;
}
