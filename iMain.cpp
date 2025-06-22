#include "iGraphics.h"
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

const int row=12, col=8, ballRadius=15;
double vSpacing=ballRadius*sqrt(3.0);
int grid[row][col] ;

#define stateMenu 0
#define statePlay 1
#define stateHelp 2
#define stateAbout 3
#define stateLevel 4

#define stateEasy 5
#define stateMedium 6
#define stateHard 7
#define statePlay1 8
#define statePlay2 9
#define statePlay3 10


int gamestate=stateMenu;
int sound=1;
int tid, limg=1;

Image load, bg, p, p2, L1, L2, L3;
void loadImage()
{
    iLoadImage(&load, "image/load.jpg");        iResizeImage(&load, 600, 620);
    iLoadImage(&bg, "image/bg2.jpg");           iResizeImage(&bg, 600, 620);
    iLoadImage(&p, "image/Rabbit.jpg");         iResizeImage(&p, 600, 620);
    iLoadImage(&p2, "image/2.jpeg");            iResizeImage(&p2, 600, 620);
    iLoadImage(&L1, "image/5.jpeg");            iResizeImage(&L1, 600, 620);
    iLoadImage(&L2, "image/3.jpeg");            iResizeImage(&L2, 600, 620);
    iLoadImage(&L3, "image/4.jpeg");            iResizeImage(&L3, 600, 620);
}

/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    // place your drawing codes here
    /*Image load, bg, p, p2,L1,L2,L3;
    iLoadImage(&load, "image/load.jpg");
    iResizeImage(&load, 600, 620);
    iLoadImage(&bg, "image/bg2.jpg");
    iResizeImage(&bg, 600, 620);
    iLoadImage(&p, "image/Rabbit.jpg");
    iResizeImage(&p, 600, 620);
    iLoadImage(&p2, "image/2.jpeg");
    iResizeImage(&p2, 600, 620);
    iLoadImage(&L1, "image/5.jpeg");
    iResizeImage(&L1, 600, 620);
    iLoadImage(&L2, "image/3.jpeg");
    iResizeImage(&L2, 600, 620);
    iLoadImage(&L3, "image/4.jpeg");
    iResizeImage(&L3, 600, 620);*/
    iClear();
    //iText(140, 180, "Hello World");
     if (limg == 1)
    {
        iShowLoadedImage(0, 0, &load);
    }
    else if(gamestate==stateMenu)
    {
         iShowLoadedImage(0, 0, &bg);
        iSetColor(61,132,188);
        iFilledRectangle(0, 620, 600, 80);
        iSetColor(255,255,255);
        iText(150,630,"Bouncing Ball");

        iSetColor(35,10,100);
        iFilledRectangle(250, 500, 100, 45);
        iSetColor(255,255,255);
        iText(280, 520, "Play");

        iSetColor(35,10,100);
        iFilledRectangle(250, 400, 100, 45);
        iSetColor(255,255,255);
        iText(280, 420, "Levels");

        iSetColor(35,10,100);
        iFilledRectangle(250, 300, 100, 45);
        iSetColor(255,255,255);
        iText(280, 320, "Help");

        iSetColor(35,10,100);
        iFilledRectangle(250, 200, 100, 45);
        iSetColor(255,255,255);
        iText(280, 220, "About");

        iSetColor(35,10,100);
        iFilledRectangle(230, 100, 140, 45);
        iSetColor(255,255,255);
        if (sound)
            iText(260, 120, "Sound: ON");
        else
            iText(260, 120, "Sound: OFF");
    }
    if(gamestate==stateHelp)
    {
        iText(200, 580, "Help");
        iText(150, 450, "# Use 'Mouse' to aim at the balls.");
        iText(150, 390, "# Use 'Mouse' to shoot bubbles.");
        iText(150, 330, "# You have to match ball colors to destroy it");
        iText(150, 260, "Press '<-' arrow to go back");
    }
    if(gamestate==stateAbout)
    {
       // iShowLoadedImage(0, 0, &p);
        iSetColor(255,255,255);
        iText(200, 650, "About");
        iSetColor(0,0,0);
        iText(120, 450, "Developed By:");
        iText(100, 370, "Nusrat Jahan Nira");
        iText(100, 335, "2405097");
        iText(370, 370, "Sheikh Sifat");
        iText(370, 335, "2405120");
        iText(150, 260, "Press '<-' arrow to go back"); 
    }
    if(gamestate==stateLevel)
    {
        iShowLoadedImage(0, 0, &p2);
        iText(200, 650, "Levels");
        iSetColor(16,73,183);
        iFilledRectangle(200, 500, 100, 45);
        iSetColor(255,255,255);
        iText(220, 520, "Easy");

        iSetColor(16,73,183);
        iFilledRectangle(200, 400, 100, 45);
        iSetColor(255,255,255);
        iText(220, 420, "Medium");

        iSetColor(16,73,183);
        iFilledRectangle(200, 300, 100, 45);
        iSetColor(255,255,255);
        iText(220, 320, "Hard");

        iSetColor(16,73,183);
        iFilledRectangle(200, 150, 100, 40);
        iSetColor(255,255,255);
        iText(220, 160, "Press '<-' arrow to go back");
    }
    if(gamestate==statePlay)
    {
        iText(200,650,"Play");
    }
    if(gamestate==stateEasy)
    {
        //iShowLoadedImage(0, 0, &p2);
        iSetColor(61,132,188);
        iFilledRectangle(200, 500, 150, 45);
        iSetColor(255,255,255);
        iText(230,520,"Level set 1");
        iSetColor(61,132,188);
        iFilledRectangle(200, 390, 150, 45);
        iSetColor(255,255,255);
        iText(230,410,"Play");
        iSetColor(61,132,188);
        iFilledRectangle(150, 200, 200, 40);
        iSetColor(255,255,255);
        iText(180,210,"Press '<-' arrow to go back");
    }
    if(gamestate==stateMedium)
    {
       // iShowLoadedImage(0, 0, &p2);
        iSetColor(61,132,188);
        iFilledRectangle(200, 500, 150, 45);
        iSetColor(255,255,255);
        iText(230,520,"Level set 2");
        iSetColor(61,132,188);
        iFilledRectangle(200, 390, 150, 45);
        iSetColor(255,255,255);
        iText(230,410,"Play");
        iSetColor(61,132,188);
        iFilledRectangle(150, 200, 200, 40);
        iSetColor(255,255,255);
        iText(180,210,"Press '<-' arrow to go back");
    }
    if(gamestate==stateHard)
    {
       // iShowLoadedImage(0, 0, &p2);
        iSetColor(61,132,188);
        iFilledRectangle(200, 500, 150, 45);
        iSetColor(255,255,255);
        iText(230,520,"Level set 3");
        iSetColor(61,132,188);
        iFilledRectangle(200, 390, 150, 45);
        iSetColor(255,255,255);
        iText(230,410,"Play");
        iSetColor(61,132,188);
        iFilledRectangle(150, 200, 200, 40);
        iSetColor(255,255,255);
        iText(180,210,"Press '<-' arrow to go back");
    }
    if(gamestate==statePlay1)
    {
        iShowLoadedImage(0, 0, &L1);
    }
    if(gamestate==statePlay2)
    {
        iShowLoadedImage(0, 0, &L2);
    }
    if(gamestate==statePlay3)
    {
        iShowLoadedImage(0, 0, &L3);
    }
}

/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    // place your codes here
}

/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}
/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
        if(gamestate==stateMenu)
       {
            if(mx>=250 && mx<=350 && my>=500 && my<=545 )
         gamestate=statePlay ;
            else if(mx>=250 && mx<=350 && my>=400 && my<=445 )
         gamestate=stateLevel ;
            else if(mx>=250 && mx<=350 && my>=300 && my<=345 )
         gamestate=stateHelp ;
            else if(mx>=250 && mx<=350 && my>=200 && my<=245 )
         gamestate=stateAbout ;
            else if(mx>=230 && mx<=370 && my>=100 && my<=145)
         sound = 1 - sound;
       }
        
        if(gamestate==stateLevel)
       {
            if(mx >= 200 && mx <= 300 && my >= 500 && my <= 545)
         gamestate=stateEasy;
            if(mx >= 200 && mx <= 300 && my >= 400 && my <= 445)
         gamestate=stateMedium;
            if(mx >= 200 && mx <= 300 && my >= 300 && my <= 345)
         gamestate=stateHard;
       
     }

        if(gamestate==stateEasy)
        {
            if(mx>=200 && mx<=350 && my>=390 && my<=435 )
         gamestate=statePlay1;
        }

        if(gamestate==stateMedium)
        {
            if(mx>=200 && mx<=350 && my>=390 && my<=435 )
         gamestate=statePlay2;
        }

        if(gamestate==stateHard)
        {
            if(mx>=200 && mx<=350 && my>=390 && my<=435 )
         gamestate=statePlay3;
        }
    }
    /*if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }*/
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    switch (key)
    {
    case 'q':
        // do something with 'q'
        break;
    // place your codes for other keys here
    default:
        break;
    }
}

/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_END:
        // do something
        break;
    case GLUT_KEY_LEFT:
            if (gamestate == stateEasy || gamestate == stateMedium || gamestate == stateHard)
        gamestate = stateLevel;  
    else if (gamestate == stateHelp || gamestate == stateAbout || gamestate == stateLevel)
        gamestate = stateMenu;  
    else if (gamestate == statePlay1 || gamestate == statePlay2 || gamestate == statePlay3)
        gamestate = stateLevel; 
        break;
    // place your codes for other keys here
    default:
        break;
    }
}
void stopload()
{
    limg = 0;
    iPauseTimer(tid);
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    tid = iSetTimer(3000, stopload);
    // place your own initialization codes here.
    loadImage();
    iInitialize(600, 700 , "Bouncing Ball");
    return 0;
}






