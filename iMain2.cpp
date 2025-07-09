#include "iGraphics.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include<queue>


#define stateMenu 0
#define statePlay 1
#define stateHelp 2
#define stateAbout 3
#define stateLevel 4

#define stateEasy 5
#define stateMedium 6
#define stateHard 7
#define stateGameOver 8

#define g_row 30
#define g_col 14
#define BUBBLE_RADIUS 20
#define BUBBLE_DIAMETER (BUBBLE_RADIUS * 2)
#define SHOOTER_X (600 / 2)
#define SHOOTER_Y (BUBBLE_RADIUS+10)
#define BUBBLE_SPEED 25.0
#define SCREEN_HEIGHT 700
#define SCREEN_WIDTH 600


int gamestate=stateMenu;
int sound=1;
int tid, limg=1;
int initialRow=5, score=0;


enum BubbleDesign
{
    Empty,
    B1,
    B2,
    B3,
    B4,
    B5,
    design
};


const int level1_map[g_row][g_col] = {
    {1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1,1,1},
    {5, 1, 2, 3, 4, 4, 4, 4, 3, 2, 1, 1,4,4},
    {1, 1, 4, 4, 5, 5, 5, 5, 4, 4, 3, 3,3,3},
    {1, 1, 2, 1, 2, 3, 3, 2, 1, 5, 5, 5, 5, 5},
    {2, 1, 1, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
     {1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1,1,1},
    {5, 1, 2, 3, 4, 4, 4, 4, 3, 2, 1, 1,4,4},
    {1, 1, 4, 4, 5, 5, 5, 5, 4, 4, 3, 3,3,3},
    {1, 1, 2, 1, 2, 3, 3, 2, 1, 5, 5, 5, 5, 5},
    {2, 1, 1, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
     {1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1,1,1},
    {5, 1, 2, 3, 4, 4, 4, 4, 3, 2, 1, 1,4,4},
    {1, 1, 4, 4, 5, 5, 5, 5, 4, 4, 3, 3,3,3},
    {1, 1, 2, 1, 2, 3, 3, 2, 1, 5, 5, 5, 5, 5},
    {2, 1, 1, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
};

struct Bubble
{
    BubbleDesign bDes;
    double x,y;
    double dx,dy;
    bool isMoving;
    int row,col;
};

Bubble grid[g_row][g_col];
Bubble shooterB;
Bubble nextB;
double angle=90;

int load_tid;
int gameTimerId;

Image load, bg, p, p2, L1, L2, L3;
Image bubbleDesign[design];


void getDesigns(int row, int col, double &x, double &y);
void Shooter();
void loadImage();
void initializeGrid(int initialRow);
std::vector<Bubble*> getNeighbors(int r, int c);
void popMatches(int s_row, int s_col);
void floatingBubbles(); 
void drawTrajectory() ;
void updateGame();
void iDraw();
void iMouseMove(int mx, int my);
void iMouse(int button, int state, int mx, int my);
void iKeyboard(unsigned char key);
void iMouseDrag(int mx, int my);
void iMouseWheel(int dir, int mx, int my);
void iSpecialKeyboard(unsigned char key);



void getDesigns(int row, int col, double &x, double &y)
{
    x = (col * (BUBBLE_DIAMETER - 2)) + BUBBLE_RADIUS + 30;
    if (row % 2 != 0) x += BUBBLE_RADIUS;
    y = SCREEN_HEIGHT - (row * (BUBBLE_DIAMETER - 5)) - BUBBLE_RADIUS;

}

void Shooter()
{
    shooterB=nextB;
    shooterB.x=SHOOTER_X;
    shooterB.y=SHOOTER_Y;
    shooterB.isMoving=false;
    nextB.bDes=( static_cast<BubbleDesign>((rand()%(design-1))+1));
    nextB.x=SHOOTER_X-(10*BUBBLE_RADIUS);
    nextB.y=SHOOTER_Y;
}

void loadImage()
{
     iLoadImage(&load, "image/load.jpg");        iResizeImage(&load, 600, 620);
    iLoadImage(&bg, "image/bg2.jpg");           iResizeImage(&bg, 600, 620);
    iLoadImage(&p, "image/T.jpg");              iResizeImage(&p, 600, 620);
   // iLoadImage(&p2, "image/2.jpeg");            iResizeImage(&p2, 600, 620);
    //iLoadImage(&L1, "image/5.jpeg");            iResizeImage(&L1, 600, 620);
    //iLoadImage(&L2, "image/3.jpeg");            iResizeImage(&L2, 600, 620);
    //iLoadImage(&L3, "image/4.jpeg");            iResizeImage(&L3, 600, 620);

    iLoadImage(&bubbleDesign[B1], "image/e1.png"); iResizeImage(&bubbleDesign[B1], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleDesign[B2], "image/e2.png"); iResizeImage(&bubbleDesign[B2], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleDesign[B3], "image/e3.png"); iResizeImage(&bubbleDesign[B3], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleDesign[B4], "image/e4.png"); iResizeImage(&bubbleDesign[B4], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleDesign[B5], "image/e5.png"); iResizeImage(&bubbleDesign[B5], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
  
}

void initializeGrid(const int map[g_row][g_col])
{
    for (int r = 0; r < g_row; ++r)
    {
        for (int c = 0; c < g_col; ++c)
        {
            grid[r][c].bDes = static_cast<BubbleDesign>(map[r][c]);
            grid[r][c].row = r;
            grid[r][c].col = c;
            grid[r][c].isMoving = false;
            getDesigns(r, c, grid[r][c].x, grid[r][c].y);
        }
    }
    Shooter();
}

std::vector<Bubble*> getNeighbors(int r, int c)
{
    std::vector<Bubble*> neighbors;
    int parity=r%2;
    int directions[2][6][2]=
    {
        {{0, -1}, {0, 1}, {-1, -1}, {-1, 0}, {1, -1}, {1, 0}},
        {{0, -1}, {0, 1}, {-1, 0}, {-1, 1}, {1, 0}, {1, 1}}
    };
    for (int i = 0; i < 6; ++i) {
        int nr = r + directions[parity][i][0];
        int nc = c + directions[parity][i][1];
        if (nr >= 0 && nr < g_row && nc >= 0 && nc < g_col) {
                if (grid[nr][nc].bDes != Empty) {
                neighbors.push_back(&grid[nr][nc]);
            }
        }
    }
    return neighbors;
}

void popMatches(int s_row, int s_col)
{
    if(s_row<0 ||s_col<0 || s_row>g_row || s_col>g_col || grid[s_row][s_col].bDes==Empty)
    {
        return;
    }

    std::queue<Bubble*> to_visit; 
    std::vector<Bubble*> cluster;
    bool visited[g_row][g_col]; 
    for(int r = 0; r < g_row; ++r)
    {
        for(int c = 0; c < g_col; ++c) 
        {
            visited[r][c] = false; 
        }
    }
 
    BubbleDesign targetColor = grid[s_row][s_col].bDes;

    to_visit.push(&grid[s_row][s_col]);
    visited[s_row][s_col] = true;

    while (!to_visit.empty()) {
        Bubble* current = to_visit.front();
        to_visit.pop();
        cluster.push_back(current);

        for (Bubble* neighbor : getNeighbors(current->row, current->col)) {
            if (!visited[neighbor->row][neighbor->col] && neighbor->bDes == targetColor) {
                visited[neighbor->row][neighbor->col] = true;
                to_visit.push(neighbor);
            }
        }
    }

    if (cluster.size() >= 3) {
        for (Bubble* b : cluster) {
            b->bDes = Empty;
            score += 10;
        }
        floatingBubbles();
    }
}

void floatingBubbles ()
{
    bool connected[g_row][g_col];
    for(int r = 0; r < g_row; ++r) 
    {
        for(int c = 0; c < g_col; ++c) 
        {
            connected[r][c] = false; 
        }
    }

    std::queue<Bubble*> q;

    for (int c = 0; c < g_col; ++c) 
    {
        if (grid[0][c].bDes != Empty) 
        {
            q.push(&grid[0][c]);
            connected[0][c] = true;
        }
    }

    while (!q.empty()) {
        Bubble* current = q.front();
        q.pop();

        for (Bubble* neighbor : getNeighbors(current->row, current->col)) 
        {
            if (neighbor->bDes != Empty && !connected[neighbor->row][neighbor->col]) 
            {
                connected[neighbor->row][neighbor->col] = true;
                q.push(neighbor);
            }
        }
    }

     for (int r = 0; r < g_row; ++r) 
     {
        for (int c = 0; c < g_col; ++c) 
        {
            if (grid[r][c].bDes != Empty && !connected[r][c]) 
            {
                grid[r][c].bDes = Empty;
                score += 5; 
            }
        }
    }

}

void updateGame() 
{
    if (shooterB.isMoving) 
    {
        shooterB.x += shooterB.dx;
        shooterB.y += shooterB.dy;

        if (shooterB.x <= BUBBLE_RADIUS || shooterB.x >= SCREEN_WIDTH - BUBBLE_RADIUS)
            shooterB.dx *= -1;

        bool collision = false;
        if (shooterB.y >= SCREEN_HEIGHT - BUBBLE_RADIUS)
            collision = true;

        for (int r = 0; r < g_row && !collision; ++r) {
            for (int c = 0; c < g_col && !collision; ++c) {
                if (grid[r][c].bDes != Empty) {
                    double dx = shooterB.x - grid[r][c].x;
                    double dy = shooterB.y - grid[r][c].y;
                    double dist = sqrt(dx * dx + dy * dy);
                    if (dist < BUBBLE_DIAMETER - 2) 
                        collision = true;
                }
            }
        }

        if (collision) {
            shooterB.isMoving = false;
            double minDist = 1e9; 
            int snap_r = -1, snap_c = -1;

            for (int r = 0; r < g_row; r++) {
                for (int c = 0; c < g_col; c++) {
                    if (grid[r][c].bDes == Empty) {
                        double tx, ty;
                        getDesigns(r, c, tx, ty); 
                        double dx = shooterB.x - tx;
                        double dy = shooterB.y - ty;
                        double dist_sq = dx * dx + dy * dy; 
                        if (dist_sq < minDist) {
                            minDist = dist_sq;
                            snap_r = r;
                            snap_c = c;
                        }
                    }
                }
            }

            if (snap_r != -1 && snap_c != -1) {
                grid[snap_r][snap_c] = shooterB; 
                grid[snap_r][snap_c].row = snap_r;
                grid[snap_r][snap_c].col = snap_c;
                getDesigns(snap_r, snap_c, grid[snap_r][snap_c].x, grid[snap_r][snap_c].y); 
                popMatches(snap_r, snap_c);
            }
           Shooter(); 
        }
    }
}

void drawTrajectory() 
{
   // printf("Trajectory called before return\n");
    // if (shooterB.isMoving) return;
    //printf("Trajectory called\n");

    double temp_x = SHOOTER_X;
    double temp_y = SHOOTER_Y;
    double aim_rad = angle * (M_PI / 180.0);
    double temp_dx = BUBBLE_SPEED * cos(aim_rad);
    double temp_dy = BUBBLE_SPEED * sin(aim_rad);

    double start_x = temp_x;
    double start_y = temp_y;

    for (int i = 0; i < 100; ++i) 
    { 
        temp_x += temp_dx;
        temp_y += temp_dy;

        for (int r = 0; r < g_row; ++r) 
        {
            for (int c = 0; c < g_col; ++c) 
            {
                if (grid[r][c].bDes != Empty) 
                {
                    double dist = sqrt(pow(temp_x - grid[r][c].x, 2) + pow(temp_y - grid[r][c].y, 2));
                    if (dist < BUBBLE_DIAMETER) 
                    {
                        iSetColor(255, 255, 255);
                        iSetLineWidth(2);
                        iLine(start_x, start_y, temp_x, temp_y);
                        return; 
                    }
                }
            }
        }
        
        if (temp_x <= BUBBLE_RADIUS || temp_x >= SCREEN_WIDTH - BUBBLE_RADIUS)
        {
            iSetColor(255, 255, 255);
            iSetLineWidth(2);
            iLine(start_x, start_y, temp_x, temp_y);
            temp_dx *= -1;
            start_x = temp_x;
            start_y = temp_y;
        }

        if (temp_y >= SCREEN_HEIGHT - BUBBLE_RADIUS) 
        {
            iSetColor(255, 255, 255);
            iSetLineWidth(2);
            iLine(start_x, start_y, temp_x, temp_y);
            return;
        }
    }

    iSetColor(255, 255, 255);
    iSetLineWidth(2);
    iLine(start_x, start_y, temp_x, temp_y);
}

void iDraw()
{
    iClear();
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
        iShowLoadedImage(0, 0, &p);
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
    if(gamestate==stateEasy)
    {
        iShowLoadedImage(0, 0, &p2);
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
        iShowLoadedImage(0, 0, &p2);
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
        iShowLoadedImage(0, 0, &p2);
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
    if(gamestate==statePlay)
    {
        //iShowLoadImage(0,0,& );
    for (int r = 0; r < g_row; r++) 
    {
        for (int c = 0; c < g_col; c++) 
        {
            if (grid[r][c].bDes != Empty) 
            {
                iShowLoadedImage(grid[r][c].x - BUBBLE_RADIUS, grid[r][c].y - BUBBLE_RADIUS, &bubbleDesign[grid[r][c].bDes]);
            }
        }
    }

   drawTrajectory();

    //iShowLoadedImage(SHOOTER_X - 20, SHOOTER_Y - 20, &shooterImage);
    if (shooterB.bDes != Empty)
    {
        iShowLoadedImage(shooterB.x - BUBBLE_RADIUS, shooterB.y - BUBBLE_RADIUS, &bubbleDesign[shooterB.bDes]);
    }
    }
}


void initializeGame(int difficulty) 
{
    score = 0;
    
    if (difficulty == 5) 
    { 
        initializeGrid(level1_map); 
    } 
    else if (difficulty == 7) 
    {   
        initializeGrid(level1_map);
    } 
    else if (difficulty == 9) 
    {    
        initializeGrid(level1_map);
    }

    nextB.bDes = B1; 
    Shooter();
    gamestate = statePlay;

    if (gameTimerId) iPauseTimer(gameTimerId);
    gameTimerId = iSetTimer(16, updateGame);
}

void iMouseMove(int mx, int my)
{
    // place your codes here
    if(gamestate!=statePlay)
    return ;
    double dx=mx-SHOOTER_X;
    double dy=my-SHOOTER_Y;
    angle=atan2(dy,dx)*(180.0/M_PI);
    if(angle<10)
    angle=10;
    if(angle>170)
    angle=170;

}

void iMouseDrag(int mx, int my)
{
    // place your codes here
}

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
            {
                 //iSetTimer
                 initializeGame(5);
            }
        }

        if(gamestate==stateMedium)
        {
            if(mx>=200 && mx<=350 && my>=390 && my<=435 )
            {
                 //iSetTimer
                 initializeGame(7);
            }
        }

        if(gamestate==stateHard)
        {
            if(mx>=200 && mx<=350 && my>=390 && my<=435 )
            {
                 //iSetTimer
                 initializeGame(9);
            }
         }
        if(gamestate==statePlay)
        {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
            {
                if (!shooterB.isMoving) 
                {
                    shooterB.isMoving = true;
                    double angleRad = angle * M_PI / 180.0;
                    shooterB.dx = BUBBLE_SPEED * cos(angleRad);
                    shooterB.dy = BUBBLE_SPEED * sin(angleRad);
                }
            }
        }
    }
    /*if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }*/
}

void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

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
    else if (gamestate == statePlay)
        gamestate = stateMenu; 
        //break;
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    srand(42);

    loadImage();
    initializeGrid(level1_map); 
    Shooter();
    initializeGame(5);
    tid = iSetTimer(2000, stopload);
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Bouncing Ball");

    return 0;
}





