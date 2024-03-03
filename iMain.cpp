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
int t3;			   // timer 3 is for animating the bullets
int t4;			   // timer 4 is for animating the asteroids
int texp;		   // this timer will control the animations of explosions
int twave;         // this timer will animate and control the functionality of annihilation wave
int GameState = 0; // 0-main menu  1-game   2-story    3- high score  4-credit 5-how to play 6- playername  7- game over
char playername[100];
int PlayerScore = 0;
int PlayerHealth = 100;
int PlayerLife = 4;
int r = 0, g = 0, b = 0;
// including bmp images
char bg[15][40] = {
	"backgrounds\\menu.bmp",   // 0
	"backgrounds\\play.bmp",   // 1
	"backgrounds\\story1.bmp", // 2

	"backgrounds\\highscores.bmp", // 3
	"backgrounds\\credit.bmp",	   // 4
	"backgrounds\\htp.bmp",		   // 5
	"backgrounds\\username.bmp",   // 6
	"backgrounds\\gameover.bmp",   // 7
	"backgrounds\\story2.bmp",	   // 8
	"backgrounds\\story3.bmp",	   // 9

};
char music[8][40] = {"music\\menubgm.wav",	 // 0
					 "music\\gamebgm.wav",	 // 1
					 "music\\gunshot.wav",	 // 2
					 "music\\explosion.wav", // 3
					 "music\\gameover.wav",  //4
					 "music\\crash.wav",     //5
					 }; 				 
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
char explosionimg[10][40] = {"explosion\\1.bmp",
							 "explosion\\2.bmp",
							 "explosion\\3.bmp",
							 "explosion\\4.bmp",
							 "explosion\\5.bmp",
							 "explosion\\6.bmp",
							 "explosion\\7.bmp"};
int explosionimgind = 0;
int impactX, impactY;
bool isExploding = false;
// variables related to ship
int ShipWidth = 125;
int ShipHeight = 123;
double ShipX = 500, ShipY = 260;
int ShipVelocity = 0;
int ShipMaxVelocity = 20;
int ShipCurrentVelocity = 0;
int ShipDeltaVelocity = 5;
int shipind = 0;
// bullets
typedef struct bullet
{
	double X, Y;

	int velocity = 30;
	double angle;
	bool isActive = false;
};
// asteroids and their characteristics
typedef struct asteroid
{
	double X, Y;
	double velocity; // ekhane velocity mane asteroid er ship e pouchate koyta step lagbe setar measurement.. so er man komale ashol velocity barbe
	double direction;
	bool isAlive = false;
	int asteroidframe;
	double slope; // slope toward the ship
	double targetX;
	double targetY;
};
// at a time we will deal with max 20 bullet 
int maxbullet = 60;
bullet bullets[60];
int bulletind = 0;
asteroid asteroids[40];
int asteroidind = 0;
int maxasteroid = 5;			// to store how many maximum asteroid to be in screen at a time
int asteroidsbasevelocity = 70; // we will change it to increase the diffiulity.. less value means higher velocity and vice versa
// including buttons
char buttons[5][60] = {"buttons\\sound off.bmp", "buttons\\sound on.bmp"};
char bulletimg[3][60] = {"rocket\\bullet.bmp"};
char asteroidimg[50][60] = {
	"asteroids\\tile001.bmp",
	"asteroids\\tile002.bmp",
	"asteroids\\tile003.bmp",
	"asteroids\\tile004.bmp",
	"asteroids\\tile005.bmp",
	"asteroids\\tile006.bmp",
	"asteroids\\tile007.bmp",
	"asteroids\\tile008.bmp",
	"asteroids\\tile009.bmp",
	"asteroids\\tile010.bmp",
	"asteroids\\tile011.bmp",
	"asteroids\\tile012.bmp",
	"asteroids\\tile013.bmp",
	"asteroids\\tile014.bmp",
	"asteroids\\tile015.bmp",
	"asteroids\\tile016.bmp",
	"asteroids\\tile017.bmp",
	"asteroids\\tile018.bmp",
	"asteroids\\tile019.bmp",
	"asteroids\\tile020.bmp",
	"asteroids\\tile021.bmp",
	"asteroids\\tile022.bmp",
	"asteroids\\tile023.bmp",
	"asteroids\\tile024.bmp",
	"asteroids\\tile025.bmp",
	"asteroids\\tile026.bmp",
	"asteroids\\tile027.bmp",
	"asteroids\\tile028.bmp",
	"asteroids\\tile029.bmp",
	"asteroids\\tile030.bmp",
	"asteroids\\tile031.bmp"};
//we will have another special but limited weapon-> annihilation wave. which will destroy 5 closest asteroids at once
int annihilationWaveCount = 1;
bool isWaveActive = false;
double WaveRadius;
double WaveX,WaveY;
int wavehits = 0; //the number of asteroids annihilation wave has killed
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
void shootbullet();
void resetgamedata();
void InitializeAsteroids();
void LoadAsteroids();
void AnimateAsteroids();
void collision();
void ShipCollidedAsteroids();
void ShowShipExplosion();
void BulletHitCheck();
void BulletHittedAsteroids();
void AnimateExplosion();
void AnimateAnnihilationWave();
void WaveImpact();

void iDraw()
{
	iClear();
	iShowBMP(0, 0, bg[GameState]); // draw the appropriate game bg

	if (GameState == 0)
	{
		soundbutton(); // main menu sound button
	}
	if (GameState == 6) // showing the player name
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

		maingame(); // main game function will be called over and over again
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
		// printf("curren pos x: %d     y: %d\n", mx, my);
		switch (GameState)
		{
			// when we are in main menu
		case 0:
			menumousecontrol(button, state, mx, my); // this function will handle the mouse clicks for the menu
			break;
		case 6:
			playernamemousecontrol(button, state, mx, my);
			break;
		case 1:
			// main games mouse events to be handled here

			break;
		case 2:

			// story r bhinno page navigation
			if (mx > 460 && mx < 520 && my > 28 && my < 88)
				GameState = 2;
			else if (mx > 522 && mx < 582 && my > 28 && my < 88)
				GameState = 8;
			else if (mx > 584 && mx < 644 && my > 28 && my < 88)
				GameState = 9;
			else if (mx >= 35 && mx <= 160 && my >= 508 && my <= 564)
			{
				GameState = 0; // getting back to main menu
			}
			break;
		case 7:
			// when we are in game over screen press any key to continue
			// intialize the ship and rockets
			resetgamedata();
			GameState = 0; // going back to menu
			soundcontrol();
			iPauseTimer(t1); // pausing the text blinker timer
			break;
		case 8:
			// story r bhinno page navigation
			if (mx > 460 && mx < 520 && my > 28 && my < 88)
				GameState = 2;
			else if (mx > 522 && mx < 582 && my > 28 && my < 88)
				GameState = 8;
			else if (mx > 584 && mx < 644 && my > 28 && my < 88)
				GameState = 9;
			else if (mx >= 35 && mx <= 160 && my >= 508 && my <= 564)
			{
				GameState = 0; // getting back to main menu
			}
			break;
		case 9:
			// story r bhinno page navigation
			if (mx > 460 && mx < 520 && my > 28 && my < 88)
				GameState = 2;
			else if (mx > 522 && mx < 582 && my > 28 && my < 88)
				GameState = 8;
			else if (mx > 584 && mx < 644 && my > 28 && my < 88)
				GameState = 9;
			else if (mx >= 35 && mx <= 160 && my >= 508 && my <= 564)
			{
				GameState = 0; // getting back to main menu
			}
			break;
		default:
			backbuttonfunction(button, state, mx, my); // for any other regular window, we just need to handle the back buttons functionality
			break;
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
	}
}
// taking playername
int ind = -1; //index of the playername string
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
		else if (ind != -1 && key == '\r') // enter pressed
		{
			GameState = 1;
			gameon = true;
			soundcontrol();
			iResumeTimer(t2); // starting animating the ship
			iResumeTimer(t4); // starting animating the asteroids
		}
		else if (key != '\r')
		{
			playername[ind + 1] = key;
			playername[ind + 2] = '\0';
			ind++;
		}
	}
	else if (GameState == 1)
	{
		if (key == ' ')
		{
			shootbullet();
			iResumeTimer(t3); // starting the bullet animation timer;
		}
		if(key=='\r'){ //annihilation wave will be generated if available
			if(!isWaveActive && annihilationWaveCount!=0){
				WaveRadius = 1;
				WaveX = ShipX+62;
				WaveY = ShipY+61;
				isWaveActive = true;
				annihilationWaveCount--;
				iResumeTimer(twave);
                //wave is initialized
			}

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
	{ // changing the direction
		if (key == GLUT_KEY_RIGHT)
		{
			if (shipind == 34)
			{
				shipind = 0;

			} // changing the direction of the ship
			else
			{
				shipind += 2;
			}
		}
		else if (key == GLUT_KEY_LEFT)
		{
			if (shipind == 0)
			{
				shipind = 34;
			}
			else
			{
				shipind -= 2;
			}
		}
		// giving thurst and drag
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
void animatebullet()
{
	for (int i = 0; i < maxbullet; i++)
	{
		if (bullets[i].isActive)
		{
			if (bullets[i].X > screenWidth || bullets[i].Y > screenHeight)
			{ // the bullet has traveled out of the screen
				bullets[i].isActive = false;
				// bullets[i].velocity = 30;
			}
			else
			{
				bullets[i].X += cos(bullets[i].angle) * bullets[i].velocity;
				bullets[i].Y += sin(bullets[i].angle) * bullets[i].velocity;
			}
		}
	}
}
void shootbullet()
{
	// printf("Bullet ind %d\n", bulletind);
	if (!isExploding)
	{
		PlaySound(music[2], NULL, SND_FILENAME | SND_ASYNC);
	}

	if (bulletind == maxbullet - 1)
	{
		bulletind = 0;
	}
	bullets[bulletind].isActive = true;
	bullets[bulletind].angle = ((90 - shipind * 10) * 3.1416) / 180.0;
	bullets[bulletind].X = ShipX + 59 + cos(bullets[bulletind].angle) * 36.0;
	bullets[bulletind].Y = ShipY + 60 + sin(bullets[bulletind].angle) * 36.0;

	bulletind++;
	// the ship should feel the reaction force
	double theta = ((90 - shipind * 10) * 3.1416) / 180.0;
	ShipX -= ((double)(1.5) * cos(theta));
	ShipY -= ((double)(1.5) * sin(theta));
}
void MenuSetup()
{
	iShowBMP(0, 0, bg[0]);
	// showmenubuttons();
	soundbutton();
}
void menumousecontrol(int button, int state, int mx, int my)
{
	// 0-main menu  1-game   2-story1    3- high score  4-credit 5-how to play 6- name input 8-story2  9-story3
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
    char names[100][50]; 
    int HighScores[100]; 
    fptr = fopen("highscores.txt", "r");
    int i = 0;
    while (fscanf(fptr, "%s %d", names[i], &HighScores[i]) != EOF)
    {
        i++;
        if (i >= 100)
            break;
    } // now we have extracted score data from highscore file
    fclose(fptr);

    // new playername ar score ke add korbo
    int j;
    for (j = i - 1; j >= 0; j--)
    {
        if (PlayerScore > HighScores[j])
        {
            strcpy(names[j + 1], names[j]);
            HighScores[j + 1] = HighScores[j];
        }
        else
        {
            break;
        }
    }
    strcpy(names[j + 1], playername);
    HighScores[j + 1] = PlayerScore;

    // now we have our sorted arrays with the new score added
    // modified scoreboard ekhon txt file e push korbo
    fptr = fopen("highscores.txt", "w");
    for (int k = 0; k < i + 1 && k < 100; ++k)
    {
        fprintf(fptr, "%s %d\n", names[k], HighScores[k]);
    }
    fclose(fptr);
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
	if(annihilationWaveCount>0) iSetColor(0,255,0); //if there is a wave the text will be green else white
	else iSetColor(255,255,255);
	if(annihilationWaveCount == 0) iText(910, 500, "Annihilation Wave: 0", GLUT_BITMAP_HELVETICA_18);
	if(annihilationWaveCount == 1) iText(910, 500, "Annihilation Wave: 1", GLUT_BITMAP_HELVETICA_18);
	if(annihilationWaveCount == 2) iText(910, 500, "Annihilation Wave: 2", GLUT_BITMAP_HELVETICA_18); //annihilation wave count
	iSetColor(255, 0, 0);
	iRectangle(950, 530, 100, 10);
	if (PlayerHealth > 25)
	{
		iSetColor(0, 250, 0);
	}
	else
		iSetColor(255, 0, 0);

	iFilledRectangle(950, 530, PlayerHealth, 10);

}
void animateship()
{
	if ((int)ShipX < 0)
	{
		ShipX = screenWidth;
	}
	else if ((int)ShipX > screenWidth)
	{
		ShipX = 0;
	}
	else if ((int)ShipY > screenHeight)
	{
		ShipY = 0;
	}
	else if ((int)ShipY < 0)
	{
		ShipY = screenHeight;
	}
	double theta = ((90 - shipind * 10) * 3.1416) / 180.0;

	ShipX += ((double)(ShipCurrentVelocity)*cos(theta));
	ShipY += ((double)(ShipCurrentVelocity)*sin(theta));
}
void ShipCollidedAsteroids()
{
	PlayerHealth -= 25;
	// explosion
	//
	// ShowShipExplosion();
	PlaySound(music[5], NULL, SND_FILENAME | SND_ASYNC);
	ShipX = 500, ShipY = 260;
	

}
void collision()
{
	// finding out weather there is a collision between asteroids and ship or not
	for (int as = 0; as < maxasteroid; as++)
	{
		// center of asteroid is asteroidX+40,asteroidY+40    redius of asteroid is 26
		// center of ship is shipX63,shipY+61         radius is 38

		double centertocenter = ((asteroids[as].X + 40 - ShipX - 63) * (asteroids[as].X + 40 - ShipX - 63) + (asteroids[as].Y + 40 - ShipY - 61) * (asteroids[as].Y + 40 - ShipY - 61));

		if (centertocenter < ((26 + 40) * (26 + 30)))
		{
			// there is indeed a collision
			ShipCollidedAsteroids(); // decisions to make after a collision
			asteroids[as].isAlive = false;
		}
	}
}
void BulletHitCheck()
{
	for (int as = 0; as < maxasteroid; as++)
	{
		for (int bul = 0; bul < maxbullet; bul++)
		{
			double centertocenter = ((asteroids[as].X + 40 - bullets[bul].X - 6) * (asteroids[as].X + 40 - bullets[bul].X - 6) + (asteroids[as].Y + 40 - bullets[bul].Y - 6) * (asteroids[as].Y + 40 - bullets[bul].Y - 6));
			if (centertocenter < 32 * 32) // the bullet is close enough for this perticular asteroids
			{
				if (bullets[bul].isActive && asteroids[as].isAlive)
				{
					bullets[bul].isActive = false;
					asteroids[as].isAlive = false;
					explosionimgind = 0;
					isExploding = true;
					// decisions to make if a bullet hit the asteroid
					impactX = asteroids[as].X;
					impactY = asteroids[as].Y;
					BulletHittedAsteroids();
				}
			}
		}
	}
}
void WaveImpact(){
	
	for(int as= 0; as<maxasteroid;as++){
		double centertocenter = ((asteroids[as].X + 40 - ShipX-62) * (asteroids[as].X + 40 - ShipX-62) + (asteroids[as].Y + 40 - ShipY-61) * (asteroids[as].Y + 40 - ShipY-61));
		if(centertocenter<WaveRadius*WaveRadius && isWaveActive && asteroids[as].isAlive && asteroids[as].X+40 >= 5 &&  asteroids[as].X+40 <= 1075 && asteroids[as].Y+40 >= 5 && asteroids[as].Y+40 <= 600 ){
			//wave has crossed that particular asteroid and we've ensured that the asteroid is inside the screem
			asteroids[as].isAlive = false;
			explosionimgind = 0;
			isExploding = true;
			impactX = asteroids[as].X;
			impactY = asteroids[as].Y;
			BulletHittedAsteroids(); //bullet hitting an asteroid and the wave crossing the asteroid will result in similar scenario
			wavehits++;

		}
	}
	if(wavehits>=5) {
		isWaveActive =false;
		wavehits = 0;
	}
}
void AnimateExplosion()
{
	// printf("Explosiond is going on explosion img ind %d",explosionimgind);
	if (explosionimgind == 7)
	{
		iPauseTimer(texp);
	}
	else
	{

		explosionimgind++;
	}
}
void BulletHittedAsteroids()
{
	PlayerScore += 10;
	maxasteroid = 5 + (PlayerScore) / 200; // the number of asteroids on the screen will increase by one for every 200 points scored
	if (asteroidsbasevelocity >= 30)
	{
		asteroidsbasevelocity = 70 - ((PlayerScore) / 200) * 5; // velocity of the asteroids will increase to.. may need some tweaking here
	}
	// explosion will be shown at that time
	iResumeTimer(texp);
	// explosion sound will be played
	PlaySound(music[3], NULL, SND_ASYNC);
	//bonus annihilation wave will be added for every 30 hits 
	if(PlayerScore%300 == 0){ //pore 300 dibo
		if(annihilationWaveCount==0) annihilationWaveCount++;
		else if(annihilationWaveCount==1) annihilationWaveCount++;
	}
}
void AnimateAnnihilationWave(){

	WaveRadius+=20;
	int d1,d2,d3,d4; // ship theke screen er 4 corner er distance er square
	int shipcx = ShipX + 62;
	int shipcy = ShipY + 61;
	d1 = (shipcx)*(shipcx) + (shipcy)*(shipcy); //(0,0) er distance er square
	d2 = (shipcx-1080)*(shipcx-1080)  + (shipcy)*(shipcy); // bottom right corner (1080,0) er distance er square
	d3 = (shipcx-1080)*(shipcx-1080)  + (shipcy-608)*(shipcy-608); //top right corner (1080,608) er distance er  square
	d4 = (shipcx)*(shipcx) + (shipcy-608)*(shipcy-608);            //top left corner er distance er square
	if((WaveRadius* WaveRadius)> d1 && (WaveRadius* WaveRadius)> d2 && (WaveRadius* WaveRadius)> d3 && (WaveRadius* WaveRadius)> d4 ){
		//the wave is out of the scene
		printf("Wave readius is %lf\n",WaveRadius);
		isWaveActive = false;
		WaveRadius = 1;
		iPauseTimer(twave);
	}


}
void maingame()
{
	// called by idraw
	//  in the main game section we will have score and life option in the corner;
	scorebar();
	healthbar();
	// // load ship
	// iShowBMP2(ShipX, ShipY, ship[shipind], 0);
	// check wether there is any ship-asteroid collision
	collision(); // collision checker between ship and asteroids
	// loading the asteroids
	LoadAsteroids();
	BulletHitCheck();
	// loading the bullets
	for (int i = 0; i < maxbullet; i++)
	{
		if (bullets[i].isActive)
		{
			iShowBMP2(bullets[i].X, bullets[i].Y, bulletimg[0], 0);
		}
	}
	// showing explosion with appropriate conditions
	if (explosionimgind == 7)
		isExploding = false;
	if (isExploding) // meaning there is an explosion going on
		iShowBMP2(impactX, impactY, explosionimg[explosionimgind], 0);
	if (PlayerHealth == 0)
	{
		GameState = 7;
		soundcontrol();
		gameover = true;
		gameon = false;
		iPauseTimer(t2); // animation for ship is paused
		iPauseTimer(t4); // animation for asteroid is paused
		modifyscoreboard();
	}
	// load ship
	iShowBMP2(ShipX, ShipY, ship[shipind], 0);
	//if a wave is active then generate it
	if(isWaveActive) {

		iSetColor(0,255,0);
		iCircle(WaveX, WaveY, WaveRadius,10000);//generating a new frame of the wave
		WaveImpact(); // check if wave crossed an asteroid
	}

}
void AnimateAsteroids()
{
	for (int i = 0; i < maxasteroid; i++)
	{
		if (asteroids[i].isAlive)
		{
			asteroids[i].asteroidframe = (++asteroids[i].asteroidframe) % 31;
			asteroids[i].X += asteroids[i].targetX / asteroids[i].velocity;
			asteroids[i].Y += asteroids[i].targetY / asteroids[i].velocity;
			if (asteroids[i].X > screenWidth || asteroids[i].X < 0 || asteroids[i].Y > screenHeight || asteroids[i].Y < 0) // if asteroid go beyond the screen
				asteroids[i].isAlive = false;
		}
	}
}
void LoadAsteroids()
{
	// printf("asteroid load function is being called\n");
	for (int i = 0; i < maxasteroid; i++)
	{
		if (!asteroids[i].isAlive)
		{
			int x = rand() % 4;
			if (x == 0)
			{
				asteroids[i].X = 0; // ******************
				asteroids[i].Y = rand() % screenHeight;
			}
			else if (x == 1)
			{
				asteroids[i].X = rand() % screenWidth;
				asteroids[i].Y = screenHeight;
			}
			else if (x == 2)
			{
				asteroids[i].X = screenWidth;
				asteroids[i].Y = rand() % screenHeight;
			}
			else
			{
				asteroids[i].X = rand() % screenWidth;
				asteroids[i].Y = 0; //**************
			}
			asteroids[i].isAlive = true;
			asteroids[i].asteroidframe = rand() % 30;
			asteroids[i].targetX = (ShipX - 120) + rand() % 240 - asteroids[i].X;
			asteroids[i].targetY = (ShipY - 120) + rand() % 240 - asteroids[i].Y;
			asteroids[i].velocity = asteroidsbasevelocity - rand() % 10;
		}
		// asteroids[i].slope = ((double)(targetY - asteroids[i].Y) / (targetX - asteroids[i].X));
		iShowBMP2(asteroids[i].X, asteroids[i].Y, asteroidimg[asteroids[i].asteroidframe], 0);
	}
}
void resetgamedata()
{
	ShipX = 500, ShipY = 260, ShipVelocity = 0;
	ShipCurrentVelocity = 0;
	iPauseTimer(t2);
	iPauseTimer(t4);
	shipind = 0;
	bulletind = 0;
	ind = -1; // the player name index.
	for (int i = 0; i < 25; i++)
	{
		bullets[i].isActive = false;
		bullets[i].velocity = 25;
	}
	for (int i = 0; i < 30; i++)
	{
		asteroids[i].isAlive = false;
	}
	annihilationWaveCount = 1;
}
void showhighscore()
{
    FILE *fptr;
    char names[100][50]; 
    int HighScores[100]; 

    fptr = fopen("highscores.txt", "r");
    int i = 0;
    while (fscanf(fptr, "%s %d", names[i], &HighScores[i]) != EOF)
    {
        i++;
        if (i >= 100)
            break;
    } 
    fclose(fptr);

    // bubble sort diye data sorting
    for (int m = 0; m < i - 1; m++)
    {
        for (int n = 0; n < i - m - 1; n++)
        {
            if (HighScores[n] < HighScores[n + 1])
            {
                int tempScore = HighScores[n];
                HighScores[n] = HighScores[n + 1];
                HighScores[n + 1] = tempScore;
                char tempName[50];
                strcpy(tempName, names[n]);
                strcpy(names[n], names[n + 1]);
                strcpy(names[n + 1], tempName);
            }
        }
    }
    iSetColor(255,255,255);
    for (int j = 0; j < i && j < 5; j++)
    {
        iText(350, 395 - j * 55, names[j], GLUT_BITMAP_TIMES_ROMAN_24);
    }
    for (int j = 0; j < i && j < 5; j++)
	{
	
		char StrScore[10];
		sprintf(StrScore, "%d", HighScores[j]);
		iText(650, 395 - j * 55, StrScore, GLUT_BITMAP_TIMES_ROMAN_24);
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
			iResumeTimer(t2);
			iResumeTimer(t4);
			printf("Player name is %s\n", playername);
		}
	}
	else if (mx >= 16 && mx <= 134 && my >= 545 && my <= 592)
	{ // back button clicked
		GameState = 0;
		ind = -1; // the player name index.
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
			PlaySound(0, 0, 0);
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
	t3 = iSetTimer(15, animatebullet);
	iPauseTimer(t3);
	t4 = iSetTimer(100, AnimateAsteroids);
	iPauseTimer(t4);
	texp = iSetTimer(1, AnimateExplosion);
	iPauseTimer(texp);
	twave = iSetTimer(0.01,AnimateAnnihilationWave);
	iPauseTimer(twave);
	PlaySound(music[0], NULL, SND_LOOP | SND_ASYNC);
	iInitialize(screenWidth, screenHeight, "Cosmic Guardians");
	return 0;
}
