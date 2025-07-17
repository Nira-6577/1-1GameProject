#include "iGraphics.h"
#include "iSound.h"

/*
function iDraw() is called again and again by the system.
*/
const int row = 10, col = 16, ballRadius = 15;
double vSpacing = ballRadius * sqrt(3.0);

#define stateMenu 0
#define statePlay 1
#define stateHelp 2
#define stateAbout 3
#define stateLevel 4
#define SHOOTER_Y (ballRadius + 10)
int gamestate = stateMenu;
int sound = 0;
int bgid, tid, limg = 1, fclickd = 0;

typedef struct
{
    int x, y, r, color; // 0=red, 1=green, 2=blue
    bool active;
} Bubble;
Bubble grid[row][col];

struct Ball
{
    double x, y;
    int color;
    double dx, dy;
    int moving;
} shooter, nextBall;

void initGrid()
{
    srand(time(NULL));
    for (int r = 0; r < row; r++)
    {
        for (int c = 0; c < col; c++)
        {
            grid[r][c].color = rand() % 3; // 0=red,1=green,2=blue
            int x = c * 2 * ballRadius + ballRadius + 50;
            if (r % 2 == 1)
                x += ballRadius; // shift odd rows right :contentReference[oaicite:1]{index=1}
            int y = 620 - (r * vSpacing + 50);
            grid[r][c].x = x;
            grid[r][c].y = y;
            grid[r][c].r = ballRadius;
            grid[r][c].active = true;
        }
    }
    shooter.x = 600 / 2;
    shooter.y = SHOOTER_Y;
    shooter.moving = 0;
    shooter.color = rand() % 3;
    nextBall.color = rand() % 3;
    nextBall.x = shooter.x + 2 * 2 * ballRadius;
    nextBall.y = shooter.y;
}
void iDraw()
{
    // place your drawing codes here
    Image load, bg, level, help, about;
    iLoadImage(&load, "assets/images/load.jpg");
    iResizeImage(&load, 600, 620);
    iLoadImage(&bg, "assets/images/bg2.jpg");
    iResizeImage(&bg, 600, 620);
    iLoadImage(&level, "assets/images/1.jpeg");
    iResizeImage(&level, 600, 620);
    iLoadImage(&help, "assets/images/2.jpeg");
    iResizeImage(&help, 600, 620);
    iLoadImage(&about, "assets/images/5.jpeg");
    iResizeImage(&about, 600, 620);
    iClear();
    if (limg == 1)
    {
        iShowLoadedImage(0, 0, &load);
    }
    else if (gamestate == stateMenu)
    {
        iShowLoadedImage(0, 0, &bg);
        iSetColor(0, 0, 0);
        iFilledRectangle(250, 500, 100, 45);
        iSetColor(255, 255, 255);
        iText(280, 520, "Play");
        iSetColor(0, 0, 0);
        iFilledRectangle(250, 400, 100, 45);
        iSetColor(255, 255, 255);
        iText(280, 420, "Levels");
        iSetColor(0, 0, 0);
        iFilledRectangle(250, 300, 100, 45);
        iSetColor(255, 255, 255);
        iText(280, 320, "Help");
        iSetColor(0, 0, 0);
        iFilledRectangle(250, 200, 100, 45);
        iSetColor(255, 255, 255);
        iText(280, 220, "About");
        iSetColor(0, 0, 0);
        iFilledRectangle(230, 100, 140, 45);
        iSetColor(255, 255, 255);
        if (sound)
            iText(260, 120, "Sound: ON");
        else
            iText(260, 120, "Sound: OFF");
    }
    else if (gamestate == stateHelp)
    {
        iShowLoadedImage(0, 0, &help);
        iText(200, 580, "Help");
        iText(150, 450, "Use 'Mouse' to aim at the balls.");
        iText(150, 390, "Use 'Mouse' to shoot bubbles.");
        iText(150, 330, "You have to match ball colors to destroy it");
        iText(150, 260, "Press '<-' arrow to go back");
    }
    else if (gamestate == stateAbout)
    {
        iShowLoadedImage(0, 0, &about);
        iText(200, 650, "About");
        iText(150, 450, "");
        iText(150, 390, "");
        iText(150, 330, "");
        iText(150, 260, "Press '<-' arrow to go back");
    }
    else if (gamestate == stateLevel)
    {
        iShowLoadedImage(0, 0, &level);
        iText(200, 650, "Levels");
        iSetColor(16, 73, 183);
        iFilledRectangle(200, 500, 100, 45);
        iSetColor(255, 255, 255);
        iText(220, 520, "Easy");

        iSetColor(16, 73, 183);
        iFilledRectangle(200, 400, 100, 45);
        iSetColor(255, 255, 255);
        iText(220, 420, "Medium");

        iSetColor(16, 73, 183);
        iFilledRectangle(200, 300, 100, 45);
        iSetColor(255, 255, 255);
        iText(220, 320, "Hard");

        iSetColor(255, 0, 255);
        iText(200, 160, "Press '<-' arrow to go back");
    }
    else if (gamestate == statePlay)
    {
        iClear();
        for (int r = 0; r < row; r++)
        {
            for (int c = 0; c < col; c++)
            {
                if (!grid[r][c].active)
                    continue;
                switch (grid[r][c].color)
                {
                case 0:
                    iSetColor(255, 0, 0);
                    break;
                case 1:
                    iSetColor(0, 255, 0);
                    break;
                case 2:
                    iSetColor(0, 0, 255);
                    break;
                }
                iFilledCircle(grid[r][c].x, grid[r][c].y, ballRadius);
            }
        }
        switch (shooter.color)
        {
        case 0:
            iSetColor(255, 0, 0);
            break;
        case 1:
            iSetColor(0, 255, 0);
            break;
        case 2:
            iSetColor(0, 0, 255);
            break;
        }
        iFilledCircle(shooter.x, shooter.y, ballRadius);
        switch (nextBall.color)
        {
        case 0:
            iSetColor(255, 0, 0);
            break;
        case 1:
            iSetColor(0, 255, 0);
            break;
        case 2:
            iSetColor(0, 0, 255);
            break;
        }
        iFilledCircle(nextBall.x, nextBall.y, ballRadius);
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
void iTick()
{
    if (shooter.moving)
    {
        shooter.x += shooter.dx;
        shooter.y += shooter.dy;
        // wall bounce
        if (shooter.x <= ballRadius || shooter.x >= 600 - ballRadius)
            shooter.dx = -shooter.dx;
        // collision
        for (int r = 0; r < row; r++)
        {
            for (int c = 0; c < col; c++)
            {
                if (grid[r][c].active)
                {
                    double dx = shooter.x - grid[r][c].x;
                    double dy = shooter.y - grid[r][c].y;
                    if (dx * dx + dy * dy <= (2 * ballRadius) * (2 * ballRadius))
                    {
                        shooter.moving = 0;
                        // snap
                        int rr = (620 - shooter.y - 50 + vSpacing / 2) / vSpacing;
                        rr = fmax(0, fmin(row - 1, rr));
                        int off = (rr & 1) ? ballRadius : 0;
                        int cc = (shooter.x - off) / (2 * ballRadius);
                        cc = fmax(0, fmin(col - 1, cc));
                        if (!grid[rr][cc].active)
                        {
                            grid[rr][cc].active = 1;
                            grid[rr][cc].color = shooter.color;
                        }
                        // advance queue
                        shooter.color = nextBall.color;
                        shooter.x = 600 / 2;
                        shooter.y = SHOOTER_Y;
                        nextBall.color = rand() % 3;
                    }
                }
            }
        }
    }
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
        if (gamestate == stateMenu)
        {
            if (mx >= 250 && mx <= 350 && my >= 500 && my <= 545)
                gamestate = statePlay;
            else if (mx >= 250 && mx <= 350 && my >= 400 && my <= 445)
                gamestate = stateLevel;
            else if (mx >= 250 && mx <= 350 && my >= 300 && my <= 345)
                gamestate = stateHelp;
            else if (mx >= 250 && mx <= 350 && my >= 200 && my <= 245)
                gamestate = stateAbout;
            else if (mx >= 230 && mx <= 370 && my >= 100 && my <= 145)
            {
                if (sound)
                {
                    iPauseSound(bgid);
                    sound = 0;
                }
                else
                {
                    iResumeSound(bgid);
                    sound = 1;
                }
            }
        }
        if (gamestate == statePlay && !shooter.moving)
        {
            if (fclickd)
            {
                // Calculate direction vector from ball to mouse click
                double dirX = mx - shooter.x;
                double dirY = my - shooter.y;

                // Calculate distance
                double distance = sqrt(dirX * dirX + dirY * dirY);

                // Avoid division by zero
                if (distance > 0)
                {
                    // Normalize the direction vector and set speed
                    double speed = 30; // Adjust this value to change ball speed
                    shooter.dx = (dirX / distance) * speed;
                    shooter.dy = (dirY / distance) * speed;
                    shooter.moving = 1;
                }
            }
            else
                fclickd = 1;
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
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
    case GLUT_KEY_LEFT:
    {
        if (gamestate == stateAbout || gamestate == stateLevel || gamestate == stateHelp)
        {
            gamestate = stateMenu;
            break;
        }
    }
    case GLUT_KEY_DOWN:
        // do smthn
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
    tid = iSetTimer(1000, stopload);
    // place your own initialization codes here.
    iInitializeSound();
    bgid = iPlaySound("assets/sounds/gamebg.wav", true, 80);
    if (!sound)
        iPauseSound(bgid);
    initGrid();
    iSetTimer(20, iTick);
    iInitialize(600, 620, "Bouncing Ball");
    return 0;
}