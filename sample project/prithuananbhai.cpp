#include "iGraphics.h"
//#include "gl.h"
// These two header files are necessary for playing music
#include "windows.h"
#include "mmsystem.h"
#include <math.h>
#define scrnwidth 1080
#define scrnheight 1920

int sx = 520, sy = 100; // size of the spaceship 160 , 158
// player life has to be displayed on the screen
int playerlife = 100;

// asteroid algorithms

float asterx = 111;
float astery = 1500;
float asterx2 = asterx + 500;
float astery2 = 1700;
float asterx3 = asterx2 + 350;
float astery3 = 1900;

// explosion algorithm
int expomode = 0;
float expox, expoy;

// alien
int expomode2 = 0;
float expox2, expoy2;
float alx = sx;
float aly = 1500;
float alx2 = 450;
float aly2 = 1300;
float alx3 = sx + 150;
float aly3 = 1400;
int aldy3 = 5;
int aldx3 = 10;
float almode = 0;
int aldx = 5;
int aldy = 4;
int alienshot = 0;
int points = 0;
float alx4 = 450;
float aly4 = 1200;

float aldx4 = 7;
float aldy4 = 3;
int bullcount = 1;

// coordinate place korar age ettu image er height width gula check kore nish calculation er jonno

// bullet shooting

int released = 0;
struct bullet
{
    int bx;
    int by;
    int relay;
    int r;
    int g;
    int b;
};
// here number of bullets is 30 , just add a larger number if you want to increase. and inside every for loop that runs from 0 to 30 ,assign that new number instead of 30
struct bullet b[30];

char spaceship[50] = "bmp images\\spaceship.bmp";
char commet[50] = "bmp images\\commet.bmp";

char asteroid1[50] = "bmp images\\asteroid1.bmp";
char asteroid2[50] = "bmp images\\asteroid2.bmp";
char asteroid3[50] = "bmp images\\asteroid3.bmp";
char alien1[50] = "bmp images\\alien1.bmp";
char alien2[50] = "bmp images\\alien2.bmp";
char alien3[50] = "bmp images\\alien3.bmp";
char alien4[50] = "bmp images\\alien4.bmp";
char alien5[50] = "bmp images\\alien5.bmp";
char explosion[50] = "bmp images\\explosion.bmp";
char explosion2[50] = "bmp images\\explosion2.bmp";

// I can't see how I can crop these images even more.
char eight[50] = "eightyears.wav";
char intro[50] = "Menu Background\\Intro1.bmp";
char page[50] = "Menu Background\\Page 2-3.bmp";
char inspage[50] = "Menu Background\\Instruction Page2.bmp";
char highpage[50] = "Menu Background\\High score page2.bmp";
char credpage[50] = "Menu Background\\Credit Page2.bmp";
char newgame[50] = "Menu Icons\\New Game2.bmp";
char instructions[50] = "Menu Icons\\Instructions2.bmp";
char highscores[50] = "Menu Icons\\High Scores2.bmp";
char credits[50] = "Menu Icons\\Credits2.bmp";
char exits[50] = "Menu Icons\\Exit2.bmp";

int mode = 0;

void iDraw()
{
    // place your drawing codes here

    iClear();
    // Worked with the menu
    // You'll need to adjust the coordinates again if you change the window size
    if (mode == 0)
    {
        iShowBMP(0, 0, intro);
    }
    if (mode == 1)
    {
        iShowBMP(0, 0, page);
        iShowBMP(340, 875, newgame);
        iShowBMP(340, 668, instructions);
        iShowBMP(340, 461, highscores);
        iShowBMP(340, 254, credits);
        iShowBMP(340, 47, exits);
    }
    if (mode == 2)
    {
        iShowBMP(0, 0, "bmp images\\background.bmp");
        iShowBMP2(sx, sy, spaceship, 0x00ffffff);

        // bullet drawing
        int i;

        for (i = 0; i < 30; i++)
        {

            iSetColor(b[i].r, b[i].g, b[i].b);
            iFilledCircle(b[i].bx, b[i].by, 10, 100);
        }
        // It works as an instruction. if you set alienmode 500 for the aliens to come , just put 500 instead of 100

        if (almode < 100)
        {
            iSetColor(rand() % 255, rand() % 255, rand() % 255);
            iText(450, 500, "AVOID COLLISION WITH THE METEORS", GLUT_BITMAP_HELVETICA_18);
        }
        if (almode > 500 && almode < 500 + 200)
        { // this rand function generates any random integer
            iSetColor(rand() % 255, rand() % 255, rand() % 255);
            iText(450, 500, "SHOOT THE ALIENS", GLUT_BITMAP_HELVETICA_18);
        }
        if (almode > 750 && almode < 850)
        { // this rand function generates any random integer
            iSetColor(rand() % 255, rand() % 255, rand() % 255);
            iText(450, 500, "Press x to go back", GLUT_BITMAP_HELVETICA_18);
        }

        // asteroid drawing
        iShowBMP2(asterx, astery, asteroid1, 0x00ffffff);
        iShowBMP2(asterx2, astery2, asteroid2, 0x00ffffff);
        iShowBMP2(asterx3, astery3, asteroid3, 0x00ffffff);

        if (expomode == 1)
        {
            iShowBMP2(expox, expoy, explosion, 0x00ffffff);
            expomode = 0;
        }

        // alien drawing
        iShowBMP2(alx, aly, alien1, 0x00ffffff);
        iShowBMP2(alx2, aly2, alien2, 0x00ffffff);
        iShowBMP2(alx3, aly3, alien3, 0x00ffffff);
        iShowBMP2(alx4, aly4, alien4, 0x00ffffff);

        if (expomode2 == 1)
        {
            iShowBMP2(expox2 + 8, expoy2 + 1, explosion2, 0x00ffffff);
            expomode2 = 0;
        }
    }
    if (mode == 3)
    {
        iShowBMP(275, 0, inspage);
        iSetColor(120, 200, 150);
        iFilledRectangle(465, 300, 347, 40);
        iSetColor(0, 0, 0);
        iText(474, 312, "Click here to return to the Main Section", GLUT_BITMAP_HELVETICA_18);
    }

    if (mode == 4)
    {
        iShowBMP(275, 0, highpage);
        iSetColor(120, 200, 150);
        iFilledRectangle(465, 300, 347, 40);
        iSetColor(0, 0, 0);
        iText(474, 312, "Click here to return to the Main Section", GLUT_BITMAP_HELVETICA_18);
    }
    if (mode == 5)
    {
        iShowBMP(275, 0, credpage);
        iSetColor(120, 200, 150);
        iFilledRectangle(465, 300, 347, 40);
        iSetColor(0, 0, 0);
        iText(474, 312, "Click here to return to the Main Section", GLUT_BITMAP_HELVETICA_18);
    }
}

/*
    function iMouseMove() is called when the user presses and drags the mouse.
    (mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
}

/*
    function iMouse() is called when the user presses/releases the mouse.
    (mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    /*if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //place your codes here

        x = mx - 50;
        y = my - 50;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        //place your codes here
        r=0; g=0; b=0 ;
    }   */

    // This is a blessing. Just click anywhere on the game and watch the corresponding coordinates get printed onto the terminal.
    // printf("%d %d\n",mx,my);

    if (mode == 0)
    {
        if (mx >= 405 && mx <= 673 && my >= 472 && my <= 569)
            mode = 1;
    }

    if (mode == 1)
    {
        if (mx >= 340 && mx <= 619 && my >= 875 && my <= 955)
            mode = 2;
        if (mx >= 340 && mx <= 619 && my >= 726 && my <= 806)
            mode = 3;
        if (mx >= 340 && mx <= 619 && my >= 557 && my <= 633)
            mode = 4;
        if (mx >= 340 && mx <= 619 && my >= 385 && my <= 466)
            mode = 5;
        if (mx >= 340 && mx <= 619 && my >= 216 && my <= 298)
            mode = 0;
    }
    if (mode == 3 || mode == 4 || mode == 5)
    {
        if (mx >= 463 && mx <= 812 && my >= 420 && my <= 456)
            mode = 1;
    }
}

/*
    function iKeyboard() is called whenever the user hits a key in keyboard.
    key- holds the ASCII value of the key pressed.
*/

void shootbullet()
{
    int i = released;
    b[i].relay = 1;
    released++;
}

void iKeyboard(unsigned char key)
{
    // place your codes for other keys here */
    if (mode == 2)
    {
        if ((key == 'w' || key == 'W') && sy < 700)
        {
            sy += 16;
        }

        // spaceship er movement ami set kore disi. tor ar change korar dorkar nai

        else if ((key == 's' || key == 'S') && sy > 45)
        {
            sy -= 16;
        }

        else if ((key == 'a' || key == 'A') && sx > 10)
        {
            sx -= 16;
        }

        else if ((key == 'd' || key == 'D') && sx < 900)
        {
            sx += 16;
        }

        // bullet movement

        if (key == 'm')
        {
            shootbullet();
        }

        // Kindly draw an iText in the bottom to let the users know how they can return to the main menu
        // sorry , I forgot :')
        if (key == 'p')
        {
            iPauseTimer(0);
        }
        if (key == 'r')
        {
            iResumeTimer(0);
        }

        if ((key == 'x') || (key == 'X'))
            mode = 1;
    }

    if (mode > 2)
    {
        if ((key == 'x') || (key == 'X'))
            mode = 1;
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

    // place your codes for other keys here

    // There's no ENTER key available, but the instructions imply that the user must press ENTER to shoot.
}

void animation()
{
    if (mode == 2)
    {

        almode++;

        // alien movement

        // almode works as a time count for the aliens to come
        if (almode > 500)
        {
            // movement of the first alien
            alx += aldx;
            aly -= aldy;

            if (alx > 900 || alx < 1)
            {
                aldx = -aldx;
            }
            if (aly == 0)
            {
                aly = 1200;
            }

            for (int i = 0; i < 30; i++)
            {
                if (alx <= b[i].bx && b[i].bx <= alx + 100 && b[i].by > aly && b[i].by < aly + 40)
                {
                    alienshot = 1;
                    // points is the variable you need to show on the screen
                    points += 10;
                }
            }
            if (alienshot == 1)
            {

                alienshot = 0;
                aly = 1200;
            }

            // movement of second alien
            if (alx2 < 1030)
                aly2 -= 10;

            if (aly2 == 0)
            {
                alx2 = sx - 200;
                aly2 = 1400;

                if (alx2 == 800)
                {
                    alx2 = 111;
                }
            }

            for (int i = 0; i < 30; i++)
            {
                if (alx2 <= b[i].bx && b[i].bx <= alx2 + 100 && b[i].by > aly2 && b[i].by < aly2 + 40)
                {
                    alienshot = 1;
                    // points is the variable you need to show on the screen
                    points += 10;
                }
            }

            if (alienshot == 1)
            {

                alienshot = 0;
                aly2 = 1500;
            }

            // movement of the third alien

            alx3 += aldx3;
            aly3 -= aldy3;

            if (alx3 > 900 || alx3 < 1)
            {
                aldx3 = -aldx3;
            }
            if (aly3 == 0)
            {
                aly3 = 1200;
            }

            for (int i = 0; i < 30; i++)
            {
                if (alx3 <= b[i].bx && b[i].bx <= alx3 + 100 && b[i].by > aly3 && b[i].by < aly3 + 40)
                {
                    alienshot = 1;
                    // points is the variable you need to show on the screen
                    points += 10;
                }
            }
            if (alienshot == 1)
            {

                alienshot = 0;
                aly3 = 1200;
            }

            // movement of fourth alien
            if (almode > 900)
            {

                // if(almode>2000)
                //{
                // aly4=1200;

                //}

                aly4 -= aldy4;

                if (aly4 < 500)
                {
                    aldy4 = 0;
                    alx4 -= aldx4;
                    if (alx4 < 0 || alx4 > 900)
                    {
                        aldx4 = -aldx4;
                    }
                }

                for (int i = 0; i < 30; i++)
                {
                    if (alx4 <= b[i].bx && b[i].bx <= alx4 + 150 && b[i].by > aly4 && b[i].by < aly4 + 60)
                    {
                        alienshot = 1;
                        // bullcount++;
                        //  points is the variable you need to show on the screen
                        points += 10;
                    }
                }
                if (alienshot == 1)
                {

                    alienshot = 0;
                    aly4 -= 50;
                }
                if (aly4 < 200)
                {
                    aly4 = 1200;
                }
            }
        }

        // alien explosion. there are some slightest error in explosion. work on it a bit.
        if ((alx >= sx && aly < sy + 158 && alx < sx + 150) || (alx + 100 > sx && alx < sx + 160 && aly < sy + 158))
        {
            expox2 = alx;
            expoy2 = aly;
            expomode2 = 1;
            playerlife--;
        }
        if ((alx2 >= sx && aly2 < sy + 158 && alx2 < sx + 160) || (alx2 + 100 > sx && alx2 < sx + 160 && aly2 < sy + 158))
        {
            expox2 = alx2;
            expoy2 = aly2;
            expomode2 = 1;
            playerlife--;
        }
        if ((alx3 >= sx && aly3 < sy + 158 && alx3 < sx + 160) || (alx3 + 100 > sx && alx3 < sx + 160 && aly3 < sy + 158))
        {
            expox2 = alx3;
            expoy2 = aly3;
            expomode2 = 1;
            playerlife--;
        }
        if ((alx4 >= sx && aly4 < sy + 200 && alx4 < sx + 200) || (alx4 + 100 > sx && alx4 < sx + 120 && aly4 < sy + 200))
        {
            expox2 = alx4;
            expoy2 = aly4;
            expomode2 = 1;
            playerlife--;
        }

        // commet that targets the spaceship

        // asteroid movements
        if (almode < 471)
        {
            if (almode == 470)
            {
                astery = 2500;
                astery2 = 2500;
                astery3 = 2500;
            }

            if (asterx < 1030)
            {
                astery -= 10;
            }
            if (asterx2 < 1000)
                astery2 -= 10;
            if (asterx3 < 1000)
                astery3 -= 10;

            if (astery == 0)
            {
                asterx = sx + 150;
                astery = 1400;

                if (asterx == 800)
                {
                    asterx = 111;
                }
            }
            if (astery2 == 0)
            {
                asterx2 = sx;
                astery2 = 1500;

                if (asterx2 == 950)
                {
                    asterx2 = 261;
                }
            }

            if (astery3 == 0)
            {
                asterx3 = asterx - 400;
                astery3 = 1600;

                if (asterx3 == 950)
                {
                    asterx3 = 411;
                }
            }

            // explosion
            if ((asterx >= sx && astery < sy + 158 && asterx < sx + 150) || (asterx + 100 > sx && asterx < sx + 160 && astery < sy + 158))
            {
                expox = asterx;
                expoy = astery;
                expomode = 1;
                playerlife--;
            }
            if ((asterx2 >= sx && astery2 < sy + 158 && asterx2 < sx + 160) || (asterx2 + 100 > sx && asterx2 < sx + 160 && astery2 < sy + 158))
            {
                expox = asterx2;
                expoy = astery2;
                expomode = 1;
                playerlife--;
            }
            if ((asterx3 >= sx && astery3 < sy + 158 && asterx3 < sx + 160) || (asterx3 + 100 > sx && asterx3 < sx + 160 && astery3 < sy + 158))
            {
                expox = asterx3;
                expoy = astery3;
                expomode = 1;
                playerlife--;
            }
        }

        // bullet issue resolved
        int i;
        for (i = 0; i < 30; i++)
        {
            if (b[i].relay == 1)
            {
                b[i].by += 20;
            }

            else
            {
                b[i].bx = sx + 80;
                b[i].by = sy + 170;
            }
        }
    }
}

int main()
{
    //    PlaySound((LPCSTR) "C:\\Users\\Public\\Music\\Sample Music\\Sleep Away.mp3", NULL, SND_FILENAME | SND_ASYNC);
    //    DWORD dwError = GetLastError();

    // place your own initialization codes here.
    iSetTimer(10, animation);
    srand(time(NULL)); // This line is necessary to play the music and run the game simultaneously
    PlaySound("spacesound.wav", NULL, SND_LOOP | SND_ASYNC);
    // If you get an error while using the PlaySound function, go to Project -> Build Options -> Linker Settings -> Type winmm -> Add
    for (int i = 0; i < 30; i++)
    {
        b[i].by = sy + 170;
        b[i].r = rand() % 255;
        b[i].g = rand() % 255;
        b[i].b = rand() % 255;
    }

    iInitialize(1200, 1000, "Spacehship Demo");

    return 0;
}
