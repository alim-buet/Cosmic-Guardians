// including necessary header files.

// kalke last bhibhinno state er jonno mouse click dara bhinno jinish control er code korchilam

#include "iGraphics.h"
#include <windows.h>
#include "mmsystem.h"
#include <math.h>

// defining constants
#define screenWidth 1080
#define screenHeight 608
#define buttonwidth 285
#define buttonheight 64
bool musicOn = true;
bool playactive = false;
bool storyactive = false;
bool highscoreactive = false;
bool gameon = false;
int GameState = 0; // 0-main menu  1-game   2-story    3- high score  4-credit 5-how to play 6- name input
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
};

// including music and sound fx files
char music[5][40] = {"music\\menubgm.wav",	  // 0
					 "music\\gamebgm.wav",	  // 1
					 "music\\gunshot.wav",	  // 2
					 "music\\explosion.wav"}; // 3

// including buttons
char buttons[12][60] = {"buttons\\sound off.bmp", "buttons\\sound on.bmp"};

// prototype of functions
void soundbutton();
void soundcontrol();
void MenuSetup();
void menumousecontrol(int button, int state, int mx, int my);
void backbuttonfunction(int button, int state, int mx, int my);

void iDraw()
{
	iClear();
	iShowBMP(0, 0, bg[GameState]);
	if (GameState == 0)
	{
		soundbutton();
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

		default:
			backbuttonfunction(button, state, mx, my);
			break;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
	}
}

void iKeyboard(unsigned char key)
{
	if (key == 'q')
	{
		exit(0);
	}
	// place your codes for other keys here
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

	if (key == GLUT_KEY_END)
	{
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
		GameState = 1;
		gameon = true;
		soundcontrol();
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
	if (GameState > 1)
	{
		// back button clicked
		if (mx >= 35 && mx <= 160 && my >= 508 && my <= 564)
		{
			GameState = 0; // getting back to main menu
		}
	}
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
	PlaySound(music[0], NULL, SND_LOOP | SND_ASYNC);
	iInitialize(screenWidth, screenHeight, "Cosmic Guardians");
	return 0;
}
