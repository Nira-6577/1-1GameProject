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
#include <queue>
#include "iSound.h"

#define MAX_HIGHSCORES 4
#define stateMenu 0
#define statePlay 1
#define stateHelp 2
#define stateAbout 3
#define stateLevel 4

#define stateEasy 5
#define stateMedium 6
#define stateHard 7
#define stateGameOver 8
#define statePause 9
#define stateWin 10

#define g_row 15
#define g_col 14
#define BUBBLE_RADIUS 20
#define BUBBLE_DIAMETER (BUBBLE_RADIUS * 2)
#define SHOOTER_X (600 / 2)
#define SHOOTER_Y (BUBBLE_RADIUS+10)
#define BUBBLE_SPEED 15.0
#define SCREEN_HEIGHT 700
#define SCREEN_WIDTH 600
#define PI  3.1415926535897932384626


int highscores[MAX_HIGHSCORES] = {0};
int gamestate = stateMenu;
int sound=1;
int tid,bgid, limg=1 , fclickd=0;
int initialRow=5;
int score=0;
int highScore=0;
int Srow=(g_row-5);
int gameTime=0;
int gameTimerId=-1;
int scrollTimerId=-1; 
int gameLoopTimerId = -1;
int scrollInterval = 25000;
int difficulty=5;
bool scrollingStopped = false;
bool gameEnded = false;

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
    {3, 3, 4, 4, 5, 5, 5, 5, 4, 4, 3, 3,3,3},
    {3, 1, 2, 1, 2, 3, 3, 2, 1, 5, 5, 4, 5, 5},
    {2, 1, 0, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
     {2, 2, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1,4,1},
    {5, 1, 3, 3, 4, 4, 4, 4, 5, 2, 1, 1,4,4},
    {4, 1, 4, 4, 5, 5, 5, 5, 1, 4, 3, 3,3,3},
    {1, 0, 2, 1, 2, 3, 3, 2, 1, 3, 5, 5, 5, 5},
    {2, 1, 1, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
     {1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 3, 3,3,1},
    {5, 1, 2, 3, 4, 1, 4, 4, 3, 2, 1, 1,4,4},
    {0, 1, 4, 4, 5, 4, 5, 5, 4, 4, 3, 3,3,3},
    {1, 5, 2, 1, 2, 3, 3, 2, 1, 5, 5, 5, 5, 5},
    {2, 1, 1, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
};

const int level2_map[g_row][g_col] = {
    {1, 1, 1, 2, 3, 3, 3, 2, 2, 2, 1, 1,1,1},
    {5, 5, 2, 2, 4, 4, 4, 4, 3, 5, 5, 4,4,4},
    {1, 2, 4, 4, 5, 5, 5, 5, 4, 4, 5, 3,3,3},
    {1, 1, 2, 1, 2, 3, 3, 2, 1, 1, 5, 5, 1, 5},
    {2, 1, 0, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
       {2, 2, 2, 1, 1, 3, 3, 3, 2, 2, 4, 4,0,1},
    {5, 1, 2, 3, 4, 5, 4, 4, 3, 2, 1, 1,4,4},
    {4, 1, 4, 4, 5, 5, 4, 5, 4, 4, 3, 3,3,3},
    {1, 0, 2, 1, 2, 3, 3, 2, 1, 5, 5, 5, 5, 5},
    {2, 1, 1, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
       {1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 2,1,1},
    {5, 1, 2, 3, 4, 4, 4, 4, 3, 2, 1, 2,4,4},
    {0, 1, 4, 4, 5, 5, 5, 2, 4, 4, 3, 2,3,3},
    {1, 5, 2, 1, 2, 3, 3, 2, 1, 5, 5, 5, 5, 5},
    {2, 1, 1, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
};

const int level3_map[g_row][g_col] = {
    {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 1, 1,2,2},
    {5, 1, 2, 3, 4, 4, 1, 1, 3, 2, 1, 1,4,4},
    {1, 2, 1, 1, 2,2, 5, 5, 3, 2, 3, 3,3,3},
    {0, 4, 2, 1, 2, 3, 3, 2, 1, 5, 5, 5, 5, 5},
    {2, 1, 0, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
         {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 1, 1,2,2},
    {5, 1, 2, 3, 4, 4, 1, 1, 3, 2, 1, 1,4,4},
    {1, 2, 1, 1, 2,2, 5, 5, 3, 2, 3, 3,3,3},
    {0, 4, 2, 1, 2, 3, 3, 2, 1, 5, 5, 5, 5, 5},
    {2, 1, 0, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
         {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 1, 1,2,2},
    {5, 1, 2, 0, 4, 4, 1, 1, 3, 2, 1, 1,4,4},
    {1, 2, 1, 1, 3,3, 5, 5, 3, 2, 3, 3,3,3},
    {0, 4, 2, 1, 2, 2, 3, 2, 1, 5, 4, 5, 5, 5},
    {2, 1, 0, 2, 3, 4, 4, 3, 4, 2, 2, 1, 1, 3},
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
Bubble swapB;
double angle=90;

int load_tid;
//int gameTimerId;

Image load, bg, c, p2, L1, L2, L3,bgAbout,bgHelp,bgGame1;
Image bubbleDesign[design];


void getDesigns(int row, int col, double &x, double &y);
void Shooter();
void loadImage();
void initializeGrid(const int map[g_row][g_col]);
void initializeGame(int difficulty);
std::vector<Bubble*> getNeighbors(int r, int c);
void popMatches(int s_row, int s_col);
void floatingBubbles(); 
void idrawTrajectory() ;
void updateGame();
void iDraw();
void iMouseMove(int mx, int my);
void iMouse(int button, int state, int mx, int my);
void iKeyboard(unsigned char key);
void iMouseDrag(int mx, int my);
void iMouseWheel(int dir, int mx, int my);
void iSpecialKeyboard(unsigned char key);
void scroll();
BubbleDesign getRandomDesign();
bool areAllBubblesCleared();

void stopAllGameTimers() {
    if (scrollTimerId >= 0) {
        iPauseTimer(scrollTimerId);
        scrollTimerId = -1;
    }
    if (gameLoopTimerId >= 0) {
        iPauseTimer(gameLoopTimerId);
        gameLoopTimerId = -1;
    }
}

void loadHighScores() 
{
    FILE* file = fopen("highscores.txt", "r");
    if (file) 
    {
        for (int i = 0; i < MAX_HIGHSCORES; i++) 
        {
            if (fscanf(file, "%d", &highscores[i]) != 1) 
            {
                highscores[i] = 0;
            }
        }
        fclose(file);
    } 
    else 
    {
        for (int i = 0; i < MAX_HIGHSCORES; i++) highscores[i] = 0;
    }
}

void saveHighScores() 
{
    FILE* file = fopen("highscores.txt", "w");
    if (file) 
    {
        for (int i = 0; i < MAX_HIGHSCORES; i++)
            fprintf(file, "%d\n", highscores[i]);
        fclose(file);
    }
}

void tryUpdateHighScores(int newScore) 
{
    for (int i = 0; i < MAX_HIGHSCORES; i++) 
    {
        if (newScore > highscores[i]) 
        {
            for (int j = MAX_HIGHSCORES - 1; j > i; j--) 
            {
                highscores[j] = highscores[j - 1];
            }
            highscores[i] = newScore;
            break;
        }
    }
    saveHighScores();
}
/*
void scroll()
{
if(difficulty == 5)
{
    for (int r = g_row - 1; r >=2; r--) 
    {
        for (int c = 0; c < g_col; c++) 
        {
            grid[r][c] = grid[r-2][c];
            grid[r][c].row = r;
            grid[r][c].col = c;
            getDesigns(r, c, grid[r][c].x, grid[r][c].y);
        }
    }

    int nextMapRow = g_row - Srow;
    int nextMapRow2= g_row - Srow;
    if (Srow > 2 && nextMapRow < g_row && nextMapRow2 < g_row) 
    {
        for (int c = 0; c < g_col; c++) 
        {
            grid[0][c].bDes = static_cast<BubbleDesign>(level1_map[nextMapRow][c]);
            grid[0][c].row = 0;
            grid[0][c].col = c;
            grid[0][c].isMoving = false;
            getDesigns(0, c, grid[0][c].x, grid[0][c].y);
        }
        for (int c = 0; c < g_col; c++) 
        {
            grid[0][c].bDes = static_cast<BubbleDesign>(level1_map[nextMapRow2][c]);
            grid[0][c].row = 0;
            grid[0][c].col = c;
            grid[0][c].isMoving = false;
            getDesigns(0, c, grid[0][c].x, grid[0][c].y);
        }

        Srow-=2;
    } 
    else 
    {
        iPauseTimer(scrollTimerId);
        for (int r = 0; r <2; r++) 
        {
            for (int c = 0; c < g_col; c++) 
            {
                grid[0][c].bDes = Empty; 
                grid[0][c].row = 0;
                grid[0][c].col = c;
                grid[0][c].isMoving = false;
                getDesigns(0, c, grid[0][c].x, grid[0][c].y);
            }
        }
    }
    
    for (int c = 0; c < g_col; ++c) 
    {
        if (grid[g_row - 1][c].bDes != Empty || grid[g_row - 2][c].bDes != Empty) 
        {
            gamestate = stateGameOver;
            iPauseTimer(scrollTimerId);
            //iPauseTimer(gameTimerId);
            return;
        }
    }
}

else if(difficulty == 7)
{
    for (int r = g_row - 1; r >=2; r--) 
    {
        for (int c = 0; c < g_col; c++) 
        {
            grid[r][c] = grid[r-2][c];
            grid[r][c].row = r;
            grid[r][c].col = c;
            getDesigns(r, c, grid[r][c].x, grid[r][c].y);
        }
    }

    int nextMapRow = g_row - Srow;
    int nextMapRow2= g_row - Srow;
    if (Srow > 2 && nextMapRow < g_row && nextMapRow2 < g_row) 
    {
        for (int c = 0; c < g_col; c++) 
        {
            grid[0][c].bDes = static_cast<BubbleDesign>(level2_map[nextMapRow][c]);
            grid[0][c].row = 0;
            grid[0][c].col = c;
            grid[0][c].isMoving = false;
            getDesigns(0, c, grid[0][c].x, grid[0][c].y);
        }
        for (int c = 0; c < g_col; c++) 
        {
            grid[0][c].bDes = static_cast<BubbleDesign>(level2_map[nextMapRow2][c]);
            grid[0][c].row = 0;
            grid[0][c].col = c;
            grid[0][c].isMoving = false;
            getDesigns(0, c, grid[0][c].x, grid[0][c].y);
        }

        Srow-=2;
    } 
    else 
    {
        iPauseTimer(scrollTimerId);
        for (int r = 0; r <2; r++) 
        {
            for (int c = 0; c < g_col; c++) 
            {
                grid[0][c].bDes = Empty; 
                grid[0][c].row = 0;
                grid[0][c].col = c;
                grid[0][c].isMoving = false;
                getDesigns(0, c, grid[0][c].x, grid[0][c].y);
            }
        }
    }
    
    for (int c = 0; c < g_col; ++c) 
    {
        if (grid[g_row - 1][c].bDes != Empty || grid[g_row - 2][c].bDes != Empty) 
        {
            gamestate = stateGameOver;
            iPauseTimer(scrollTimerId);
            //iPauseTimer(gameTimerId);
            return;
        }
    }
}

else 
{
    for (int r = g_row - 1; r >=2; r--) 
    {
        for (int c = 0; c < g_col; c++) 
        {
            grid[r][c] = grid[r-2][c];
            grid[r][c].row = r;
            grid[r][c].col = c;
            getDesigns(r, c, grid[r][c].x, grid[r][c].y);
        }
    }

    int nextMapRow = g_row - Srow;
    int nextMapRow2= g_row - Srow;

    if (Srow > 2 && nextMapRow < g_row && nextMapRow2 < g_row) 
    {
        for (int c = 0; c < g_col; c++) 
        {
            grid[0][c].bDes = static_cast<BubbleDesign>(level3_map[nextMapRow][c]);
            grid[0][c].row = 0;
            grid[0][c].col = c;
            grid[0][c].isMoving = false;
            getDesigns(0, c, grid[0][c].x, grid[0][c].y);
        }
        for (int c = 0; c < g_col; c++) 
        {
            grid[0][c].bDes = static_cast<BubbleDesign>(level3_map[nextMapRow2][c]);
            grid[0][c].row = 0;
            grid[0][c].col = c;
            grid[0][c].isMoving = false;
            getDesigns(0, c, grid[0][c].x, grid[0][c].y);
        }

        Srow-=2;
    } 

    else 
    {
        iPauseTimer(scrollTimerId);
        for (int r = 0; r <2; r++) 
        {
            for (int c = 0; c < g_col; c++) 
            {
                grid[0][c].bDes = Empty; 
                grid[0][c].row = 0;
                grid[0][c].col = c;
                grid[0][c].isMoving = false;
                getDesigns(0, c, grid[0][c].x, grid[0][c].y);
            }
        }
    }
    
    for (int c = 0; c < g_col; ++c) 
    {
        if (grid[g_row - 1][c].bDes != Empty || grid[g_row - 2][c].bDes != Empty) 
        {
            gamestate = stateGameOver;
            iPauseTimer(scrollTimerId);
            //iPauseTimer(gameTimerId);
            return;
        }
    }
}
}
*/

void scroll()
{
    if(difficulty == 5) // Easy level
    {
        // Move all bubbles down by 2 rows
        for (int r = g_row - 1; r >= 2; r--) 
        {
            for (int c = 0; c < g_col; c++) 
            {
                grid[r][c] = grid[r-2][c];
                grid[r][c].row = r;
                grid[r][c].col = c;
                getDesigns(r, c, grid[r][c].x, grid[r][c].y);
            }
        }

        int nextMapRow = g_row - Srow;
        
        if (Srow > 2 && nextMapRow < g_row) 
        {
            // Add new rows from level1_map (only while map rows remain)
            for (int c = 0; c < g_col; c++) 
            {
                grid[0][c].bDes = static_cast<BubbleDesign>(level1_map[nextMapRow][c]);
                grid[0][c].row = 0;
                grid[0][c].col = c;
                grid[0][c].isMoving = false;
                getDesigns(0, c, grid[0][c].x, grid[0][c].y);
                
                if (nextMapRow > 0) {
                    grid[1][c].bDes = static_cast<BubbleDesign>(level1_map[nextMapRow-1][c]);
                    grid[1][c].row = 1;
                    grid[1][c].col = c;
                    grid[1][c].isMoving = false;
                    getDesigns(1, c, grid[1][c].x, grid[1][c].y);
                }
            }
            Srow -= 2;
            printf("Scrolling: Srow = %d, nextMapRow = %d\n", Srow, nextMapRow); // Debug
        } 
        else 
        {
            // Map exhausted - stop scrolling permanently
            if (!scrollingStopped) {
                printf("SCROLLING STOPPED! No more bubbles will be added.\n"); // Debug
                iPauseTimer(scrollTimerId);
                scrollingStopped = true;
            }
            
            // Clear the top rows (no new bubbles added)
            for (int r = 0; r < 2; r++) 
            {
                for (int c = 0; c < g_col; c++) 
                {
                    grid[r][c].bDes = Empty; 
                    grid[r][c].row = r;
                    grid[r][c].col = c;
                    grid[r][c].isMoving = false;
                    getDesigns(r, c, grid[r][c].x, grid[r][c].y);
                }
            }
        }
        
        // Check lose condition - bubbles reaching bottom
        for (int c = 0; c < g_col; ++c) 
        {
            if (grid[g_row - 1][c].bDes != Empty || grid[g_row - 2][c].bDes != Empty) 
            {
                printf("GAME OVER - Bubbles reached bottom!\n"); // Debug
                gamestate = stateGameOver;
                iPauseTimer(scrollTimerId);
                tryUpdateHighScores(score);
                return;
            }
        }
    }

    else if(difficulty == 7) // Medium level
    {
        // Move all bubbles down by 2 rows
        for (int r = g_row - 1; r >= 2; r--) 
        {
            for (int c = 0; c < g_col; c++) 
            {
                grid[r][c] = grid[r-2][c];
                grid[r][c].row = r;
                grid[r][c].col = c;
                getDesigns(r, c, grid[r][c].x, grid[r][c].y);
            }
        }

        int nextMapRow = g_row - Srow;
        
        if (Srow > 2 && nextMapRow < g_row) 
        {
            // Add new rows from level2_map (only while map rows remain)
            for (int c = 0; c < g_col; c++) 
            {
                grid[0][c].bDes = static_cast<BubbleDesign>(level2_map[nextMapRow][c]);
                grid[0][c].row = 0;
                grid[0][c].col = c;
                grid[0][c].isMoving = false;
                getDesigns(0, c, grid[0][c].x, grid[0][c].y);
                
                if (nextMapRow > 0) {
                    grid[1][c].bDes = static_cast<BubbleDesign>(level2_map[nextMapRow-1][c]);
                    grid[1][c].row = 1;
                    grid[1][c].col = c;
                    grid[1][c].isMoving = false;
                    getDesigns(1, c, grid[1][c].x, grid[1][c].y);
                }
            }
            Srow -= 2;
            printf("Scrolling: Srow = %d, nextMapRow = %d\n", Srow, nextMapRow); // Debug
        } 
        else 
        {
            // Map exhausted - stop scrolling permanently
            if (!scrollingStopped) {
                printf("SCROLLING STOPPED! No more bubbles will be added.\n"); // Debug
                iPauseTimer(scrollTimerId);
                scrollingStopped = true;
            }
            
            // Clear the top rows (no new bubbles added)
            for (int r = 0; r < 2; r++) 
            {
                for (int c = 0; c < g_col; c++) 
                {
                    grid[r][c].bDes = Empty; 
                    grid[r][c].row = r;
                    grid[r][c].col = c;
                    grid[r][c].isMoving = false;
                    getDesigns(r, c, grid[r][c].x, grid[r][c].y);
                }
            }
        }
        
        // Check lose condition - bubbles reaching bottom
        for (int c = 0; c < g_col; ++c) 
        {
            if (grid[g_row - 1][c].bDes != Empty || grid[g_row - 2][c].bDes != Empty) 
            {
                printf("GAME OVER - Bubbles reached bottom!\n"); // Debug
                gamestate = stateGameOver;
                iPauseTimer(scrollTimerId);
                tryUpdateHighScores(score);
                return;
            }
        }
    }

    else // Hard level (difficulty == 9)
    {
        // Move all bubbles down by 2 rows
        for (int r = g_row - 1; r >= 2; r--) 
        {
            for (int c = 0; c < g_col; c++) 
            {
                grid[r][c] = grid[r-2][c];
                grid[r][c].row = r;
                grid[r][c].col = c;
                getDesigns(r, c, grid[r][c].x, grid[r][c].y);
            }
        }

        int nextMapRow = g_row - Srow;
        
        if (Srow > 2 && nextMapRow < g_row) 
        {
            // Add new rows from level3_map (only while map rows remain)
            for (int c = 0; c < g_col; c++) 
            {
                grid[0][c].bDes = static_cast<BubbleDesign>(level3_map[nextMapRow][c]);
                grid[0][c].row = 0;
                grid[0][c].col = c;
                grid[0][c].isMoving = false;
                getDesigns(0, c, grid[0][c].x, grid[0][c].y);
                
                if (nextMapRow > 0) {
                    grid[1][c].bDes = static_cast<BubbleDesign>(level3_map[nextMapRow-1][c]);
                    grid[1][c].row = 1;
                    grid[1][c].col = c;
                    grid[1][c].isMoving = false;
                    getDesigns(1, c, grid[1][c].x, grid[1][c].y);
                }
            }
            Srow -= 2;
            printf("Scrolling: Srow = %d, nextMapRow = %d\n", Srow, nextMapRow); // Debug
        }
        else 
        {
            // Map exhausted - stop scrolling permanently
            if (!scrollingStopped) {
                printf("SCROLLING STOPPED! No more bubbles will be added.\n"); // Debug
                iPauseTimer(scrollTimerId);
                scrollingStopped = true;
            }
            
            // Clear the top rows (no new bubbles added)
            for (int r = 0; r < 2; r++) 
            {
                for (int c = 0; c < g_col; c++) 
                {
                    grid[r][c].bDes = Empty; 
                    grid[r][c].row = r;
                    grid[r][c].col = c;
                    grid[r][c].isMoving = false;
                    getDesigns(r, c, grid[r][c].x, grid[r][c].y);
                }
            }
        }
        
        // Check lose condition - bubbles reaching bottom
        for (int c = 0; c < g_col; ++c) 
    {
        if (grid[g_row - 1][c].bDes != Empty || grid[g_row - 2][c].bDes != Empty) 
        {
            printf("GAME OVER - Bubbles reached bottom!\n");
            gamestate = stateGameOver;
            iPauseTimer(scrollTimerId);
            tryUpdateHighScores(score);
            return;
        }
    }
    
    // Check win condition after scrolling
    if (areAllBubblesCleared()) 
    {
        printf("WIN! All bubbles cleared after scroll!\n");
        gamestate = stateWin;
        iPauseTimer(gameTimerId);
        if (scrollTimerId >= 0) 
            iPauseTimer(scrollTimerId);
        score += 1000; // Bonus for completing level
        tryUpdateHighScores(score);
    }
    }
}

bool areAllBubblesCleared()
{
    for (int r = 0; r < g_row; ++r)
    {
        for (int c = 0; c < g_col; ++c)
        {
            if (grid[r][c].bDes != Empty)
                return false;
        }
    }
    return true;
}

BubbleDesign getRandomDesign()
{
    return  (static_cast<BubbleDesign>((rand()%(design-1))+1));
}

void getDesigns(int row, int col, double &x, double &y)
{
    x = (col * (BUBBLE_DIAMETER - 2)) + BUBBLE_RADIUS + 30;
    if (row % 2 != 0) 
    x += BUBBLE_RADIUS;
    y = SCREEN_HEIGHT - (row * (BUBBLE_DIAMETER - 5)) - BUBBLE_RADIUS;

}

void Shooter()
{
    shooterB=nextB;
    shooterB.x=SHOOTER_X;
    shooterB.y=SHOOTER_Y+30;
    shooterB.isMoving=false;
    nextB.bDes=( static_cast<BubbleDesign>((rand()%(design-1))+1));
    nextB.x=SHOOTER_X-(2*BUBBLE_RADIUS);
    nextB.y=SHOOTER_Y;
}

void loadImage()
{
    iLoadImage(&load, "image/load.jpg");   
    iResizeImage(&load,SCREEN_WIDTH, SCREEN_HEIGHT);
    iLoadImage(&bg, "image/bg2.jpg");    
    iResizeImage(&bg, SCREEN_WIDTH, SCREEN_HEIGHT);
    iLoadImage(&c, "image/circle.png");   
    iResizeImage(&c, 2*BUBBLE_DIAMETER, 2*BUBBLE_DIAMETER);
    iLoadImage(&bgAbout, "image/bgAbout.jpg");  
    iResizeImage(&bgAbout, SCREEN_WIDTH, SCREEN_HEIGHT);
    iLoadImage(&bgGame1, "image/bgGame1.jpg");      
    iResizeImage(&bgGame1, SCREEN_WIDTH, SCREEN_HEIGHT);
    iLoadImage(&bgHelp, "image/bgHelp.jpg");   
    iResizeImage(&bgHelp, SCREEN_WIDTH, SCREEN_HEIGHT);

    // iLoadImage(&bubbleDesign[B1], "image/Expression1.png"); 
    // iResizeImage(&bubbleDesign[B1], BUBBLE_DIAMETER-4, BUBBLE_DIAMETER-4);
    // iLoadImage(&bubbleDesign[B2], "image/Expression2.png"); 
    // iResizeImage(&bubbleDesign[B2], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    // iLoadImage(&bubbleDesign[B3], "image/Expression3.png"); 
    // iResizeImage(&bubbleDesign[B3], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    // iLoadImage(&bubbleDesign[B4], "image/Expression4.png"); 
    // iResizeImage(&bubbleDesign[B4], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    // iLoadImage(&bubbleDesign[B5], "image/Expression5.png"); 
    // iResizeImage(&bubbleDesign[B5], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    
    iLoadImage(&bubbleDesign[B1], "assets/images/sprites/ball1.png"); 
    iResizeImage(&bubbleDesign[B1], BUBBLE_DIAMETER-4, BUBBLE_DIAMETER-4);
    iLoadImage(&bubbleDesign[B2], "assets/images/sprites/ball2.png"); 
    iResizeImage(&bubbleDesign[B2], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleDesign[B3], "assets/images/sprites/ball3.png"); 
    iResizeImage(&bubbleDesign[B3], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleDesign[B4], "assets/images/sprites/ball4.png"); 
    iResizeImage(&bubbleDesign[B4], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleDesign[B5], "assets/images/sprites/ball5.png"); 
    iResizeImage(&bubbleDesign[B5], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
  
}

void initializeGrid(const int map[g_row][g_col])
{
    for (int r = 0; r < (g_row-Srow); ++r)
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
    for (int i = 0; i < 6; ++i) 
    {
        int nr = r + directions[parity][i][0];
        int nc = c + directions[parity][i][1];
        if (nr >= 0 && nr < g_row && nc >= 0 && nc < g_col) 
        {
            if (grid[nr][nc].bDes != Empty) 
            {
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

    while (!to_visit.empty()) 
    {
        Bubble* current = to_visit.front();
        to_visit.pop();
        cluster.push_back(current);

        for (Bubble* neighbor : getNeighbors(current->row, current->col)) 
        {
            if (!visited[neighbor->row][neighbor->col] && neighbor->bDes == targetColor) 
            {
                visited[neighbor->row][neighbor->col] = true;
                to_visit.push(neighbor);
            }
        }
    }

    if (cluster.size() >= 3) 
    {
        for (Bubble* b : cluster) 
        {
            b->bDes = Empty;
            score += 10;
        }
         iPlaySound("sound/popsound.wav", false);
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

    while (!q.empty()) 
    {
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
                score += 15; 
            }
        }
    }

}
/*
void updateGame() 
{
    if (areAllBubblesCleared()) 
    {
        printf("WIN! All bubbles cleared!\n");
        gamestate = stateWin;
        iPauseTimer(gameTimerId);
        if (scrollTimerId >= 0) 
            iPauseTimer(scrollTimerId);
        score += 1000; // Bonus for completing level
        tryUpdateHighScores(score);
        return;
    }

    if (shooterB.isMoving) 
    {
        shooterB.x += shooterB.dx;
        shooterB.y += shooterB.dy;

        if (shooterB.x <= BUBBLE_RADIUS || shooterB.x >= SCREEN_WIDTH - BUBBLE_RADIUS)
            shooterB.dx *= -1;

        bool collision = false;
        if (shooterB.y >= SCREEN_HEIGHT - BUBBLE_RADIUS)
            collision = true;

        for (int r = 0; r < g_row && !collision; ++r) 
        {
            for (int c = 0; c < g_col && !collision; ++c) 
            {
                if (grid[r][c].bDes != Empty) 
                {
                    double dx = shooterB.x - grid[r][c].x;
                    double dy = shooterB.y - grid[r][c].y;
                    double dist = sqrt(dx * dx + dy * dy);
                    if (dist < BUBBLE_DIAMETER - 3) 
                        collision = true;
                }
            }
        }

        if (collision) 
        {
            shooterB.isMoving = false;
            double minDist = 1e9; 
            int snap_r = -1, snap_c = -1;

            for (int r = 0; r < g_row; r++) 
            {
                for (int c = 0; c < g_col; c++) 
                {
                    if (grid[r][c].bDes == Empty) 
                    {
                        double tx, ty;
                        getDesigns(r, c, tx, ty); 
                        double dx = shooterB.x - tx;
                        double dy = shooterB.y - ty;
                        double dist_sq = dx * dx + dy * dy; 
                        if (dist_sq < minDist) 
                        {
                            minDist = dist_sq;
                            snap_r = r;
                            snap_c = c;
                        }
                    }
                }
            }

            if (snap_r != -1 && snap_c != -1) 
            {
                grid[snap_r][snap_c].bDes = shooterB.bDes;
                grid[snap_r][snap_c].row = snap_r;
                grid[snap_r][snap_c].col = snap_c;
                grid[snap_r][snap_c].isMoving = false;
                grid[snap_r][snap_c].dx = 0;
                grid[snap_r][snap_c].dy = 0;
                getDesigns(snap_r, snap_c, grid[snap_r][snap_c].x, grid[snap_r][snap_c].y);
                popMatches(snap_r, snap_c);

            if (scrollingStopped) 
{
    if (areAllBubblesCleared()) 
    {
        printf("WIN! All bubbles cleared after scrolling stopped\n"); // Debug line
        gamestate = stateGameOver;
        iPauseTimer(gameTimerId);
        if (scrollTimerId >= 0) 
            iPauseTimer(scrollTimerId);
        tryUpdateHighScores(score);
        return ;
    }
}
        }
           Shooter(); 
    }
}
}
*/

void updateGame() 
{
    if (gameEnded && gamestate != statePlay) {
        return;
    }

    if (gameLoopTimerId < 0) {
        printf("Game loop timer invalid, stopping game\n");
        gamestate = stateMenu;
        return;
    }

     if (!gameEnded && areAllBubblesCleared()) 
    {
        printf("WIN! All bubbles cleared!\n");
        gamestate = stateWin;  // Changed from stateGameOver to stateWin
        gameEnded = true;      // Set flag to prevent further updates
        iPauseTimer(gameTimerId);
        if (scrollTimerId >= 0) 
            iPauseTimer(scrollTimerId);
        score += 1000; // Bonus for completing level
        tryUpdateHighScores(score);
        return;
    }

    if (gamestate != statePlay || gameEnded) 
    {
        return;
    }

    if (shooterB.isMoving) 
    {
        shooterB.x += shooterB.dx;
        shooterB.y += shooterB.dy;

        if (shooterB.x <= BUBBLE_RADIUS || shooterB.x >= SCREEN_WIDTH - BUBBLE_RADIUS)
            shooterB.dx *= -1;

        bool collision = false;
        if (shooterB.y >= SCREEN_HEIGHT - BUBBLE_RADIUS)
            collision = true;

        for (int r = 0; r < g_row && !collision; ++r) 
        {
            for (int c = 0; c < g_col && !collision; ++c) 
            {
                if (grid[r][c].bDes != Empty) 
                {
                    double dx = shooterB.x - grid[r][c].x;
                    double dy = shooterB.y - grid[r][c].y;
                    double dist = sqrt(dx * dx + dy * dy);
                    if (dist < BUBBLE_DIAMETER - 3) 
                        collision = true;
                }
            }
        }

        if (collision) 
        {
            shooterB.isMoving = false;
            double minDist = 1e9; 
            int snap_r = -1, snap_c = -1;

            for (int r = 0; r < g_row; r++) 
            {
                for (int c = 0; c < g_col; c++) 
                {
                    if (grid[r][c].bDes == Empty) 
                    {
                        double tx, ty;
                        getDesigns(r, c, tx, ty); 
                        double dx = shooterB.x - tx;
                        double dy = shooterB.y - ty;
                        double dist_sq = dx * dx + dy * dy; 
                        if (dist_sq < minDist) 
                        {
                            minDist = dist_sq;
                            snap_r = r;
                            snap_c = c;
                        }
                    }
                }
            }

            if (snap_r != -1 && snap_c != -1) 
            {
                grid[snap_r][snap_c].bDes = shooterB.bDes;
                grid[snap_r][snap_c].row = snap_r;
                grid[snap_r][snap_c].col = snap_c;
                grid[snap_r][snap_c].isMoving = false;
                grid[snap_r][snap_c].dx = 0;
                grid[snap_r][snap_c].dy = 0;
                getDesigns(snap_r, snap_c, grid[snap_r][snap_c].x, grid[snap_r][snap_c].y);
                popMatches(snap_r, snap_c);

                // Check win condition only after scrolling has stopped
                if (scrollingStopped && !gameEnded) 
                {
                    if (areAllBubblesCleared()) 
                    {
                        printf("WIN! All bubbles cleared after scrolling stopped\n");
                        gamestate = stateWin; 
                        gameEnded = true;
                        iPauseTimer(gameTimerId);
                        if (scrollTimerId >= 0) 
                            iPauseTimer(scrollTimerId);
                        score += 1000;
                        tryUpdateHighScores(score);
                        return;
                    }
                }
            }
           Shooter(); 
        }
    }
}


void idrawTrajectory() 
{
   // printf("Trajectory called before return\n");
    if (shooterB.isMoving) 
    {
        return;
    }
    //printf("Trajectory called\n");
    double temp_x = nextB.x + 2*BUBBLE_RADIUS;
    double temp_y = nextB.y + 2*BUBBLE_RADIUS;
    double aim_rad = angle * (PI / 180.0);
    double temp_dx = BUBBLE_SPEED * cos(aim_rad) * 0.5;
    double temp_dy = BUBBLE_SPEED * sin(aim_rad) * 0.5;

    double start_x = temp_x;
    double start_y = temp_y;

    for (int i = 0; i < 200; ++i) 
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
                    if (dist < BUBBLE_DIAMETER-2) 
                    {
                        iSetColor(255, 255, 255);
                        iSetLineWidth(2);
                        iLine(start_x, start_y, temp_x, temp_y- BUBBLE_RADIUS);
                        return; 
                    }
                }
            }
        }
        
        if (temp_x <= BUBBLE_RADIUS || temp_x >= SCREEN_WIDTH - BUBBLE_RADIUS)
        {
            iSetColor(255, 255, 255);
            iSetLineWidth(2);
            iLine(start_x, start_y, temp_x, temp_y- BUBBLE_RADIUS);
            temp_dx *= -1;
            start_x = temp_x;
            start_y = temp_y;
        }

        if (temp_y >= SCREEN_HEIGHT - BUBBLE_RADIUS) 
        {
            iSetColor(255, 255, 255);
            iSetLineWidth(2);
            iLine(start_x, start_y, temp_x, SCREEN_HEIGHT - BUBBLE_RADIUS);
            return;
        }
    }

    iSetColor(255, 255, 255);
    iSetLineWidth(2);
    iLine(start_x, start_y, temp_x, temp_y- BUBBLE_RADIUS);
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
        //iFilledRectangle(0, 620, 600, 80);
        //iSetColor(255,255,255);
        iSetColor(35,10,100);
        iText(150,630,"Bouncing Ball", GLUT_BITMAP_TIMES_ROMAN_24);

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
        iShowLoadedImage(0, 0, &bgHelp); 
        iSetColor(0,0,0);  
        iText(200, 580, "Help", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(100, 450, "# Use 'Mouse' or Use 'GLUT_KEY_RIGHT,GLUT_KEY_Left' ");
        iText(115,430,"to aim at the balls.");
        iText(100, 390, "# Use 'Mouse' or 'Use GLUT_KEY_UP' to shoot bubbles.");
        iText(100, 330, "# You have to match ball colors to destroy it");
        iText(150, 260, "Press 'b'  to go back");
    }

    if(gamestate==stateAbout)
    {
        iShowLoadedImage(0, 0, &bgHelp);
        iSetColor(255,255,255);
        iText(200, 650, "About", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(0,0,0);
        iText(160, 500, "Instructed by:");
        iText(160, 460, "Anwarul Bashir Shuaib");
        iText(160, 420, "Lecturer, Department of CSE,BUET");
        iText(120, 330, "Developed By:");
        iText(100, 270, "Nusrat Jahan Nira");
        iText(100, 235, "2405097");
        iText(370, 270, "Sheikh Sifat");
        iText(370, 235, "2405120");
        iText(150, 120, "Press 'b' arrow to go back"); 
    }

    if(gamestate==stateLevel)
    {
        iShowLoadedImage(0, 0, &bgGame1);
        iText(200, 650, "Levels", GLUT_BITMAP_TIMES_ROMAN_24);
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
        iFilledRectangle(150, 200, 250, 40);
        iSetColor(255,255,255);
        iText(160,210,"Press 'b' arrow to go back");
    }

    if(gamestate==stateEasy)
    {
        iShowLoadedImage(0, 0, &bgGame1);
        iSetColor(61,132,188);
        iFilledRectangle(200, 500, 150, 45);
        iSetColor(255,255,255);
        iText(230,520,"Level set 1");
        iSetColor(61,132,188);
        iFilledRectangle(200, 390, 150, 45);
        iSetColor(255,255,255);
        iText(230,410,"Play");
        iSetColor(61,132,188);
        iFilledRectangle(150, 200, 250, 40);
        iSetColor(255,255,255);
        iText(160,210,"Press 'b' arrow to go back");
    }

    if(gamestate==stateMedium)
    {
        iShowLoadedImage(0, 0, &bgGame1);
        iSetColor(61,132,188);
        iFilledRectangle(200, 500, 150, 45);
        iSetColor(255,255,255);
        iText(230,520,"Level set 2");
        iSetColor(61,132,188);
        iFilledRectangle(200, 390, 150, 45);
        iSetColor(255,255,255);
        iText(230,410,"Play");
        iSetColor(61,132,188);
        iFilledRectangle(150, 200, 250, 40);
        iSetColor(255,255,255);
        iText(160,210,"Press 'b' arrow to go back");
    }

    if(gamestate==stateHard)
    {
        iShowLoadedImage(0, 0, &bgGame1);
        iSetColor(61,132,188);
        iFilledRectangle(200, 500, 150, 45);
        iSetColor(255,255,255);
        iText(230,520,"Level set 3");
        iSetColor(61,132,188);
        iFilledRectangle(200, 390, 150, 45);
        iSetColor(255,255,255);
        iText(230,410,"Play");
        iSetColor(61,132,188);
        iFilledRectangle(150, 200, 250, 40);
        iSetColor(255,255,255);
        iText(160,210,"Press 'b' arrow to go back");
    }

    if(gamestate==statePlay)
    {
        iShowLoadedImage(0, 0, &bgGame1);


    char scoreStr[30];
    sprintf(scoreStr, "Score: %d", score);
    iSetColor(61,132,188);
    iFilledRectangle(0, 660, 120, 30);  
    iSetColor(255, 255, 255);
    iText(10, 670, scoreStr, GLUT_BITMAP_HELVETICA_18);



    for (int r = 0; r < g_row; r++) 
    {
        for (int c = 0; c < g_col; c++) 
        {
            if (grid[r][c].bDes != Empty) 
            {
                iShowLoadedImage(grid[r][c].x - BUBBLE_RADIUS, grid[r][c].y - BUBBLE_RADIUS - 35, &bubbleDesign[grid[r][c].bDes]);
            }
        }
    }
    
    iShowLoadedImage(SHOOTER_X-(3*BUBBLE_RADIUS),SHOOTER_Y-BUBBLE_RADIUS,&c);
   idrawTrajectory();

    if (shooterB.bDes != Empty)
    {
        iShowLoadedImage(shooterB.x - BUBBLE_RADIUS, shooterB.y - BUBBLE_RADIUS+10, &bubbleDesign[shooterB.bDes]);
        iShowLoadedImage(nextB.x - BUBBLE_RADIUS-5, nextB.y - BUBBLE_RADIUS, &bubbleDesign[nextB.bDes]);
    }
    }

    else if(gamestate == statePause)
    {
        iSetColor(0, 0 , 255);
        iFilledRectangle(250,530, 100, 30);
        iSetColor(255, 255 , 255);
        if(sound)
        iText(260, 540, "Sound: ON" );
        else
        iText(260, 540, "Sound: OFF" );
        iText(240, 400, "Press 'r' to resume", GLUT_BITMAP_TIMES_ROMAN_24);
    }

    if (gamestate == stateGameOver)
    {
    iClear();
    iSetColor(0, 0, 0);
    iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    iSetColor(255, 255, 255);
    iText(220, 400, " Game Over!", GLUT_BITMAP_HELVETICA_18);

    iSetColor(35, 10, 100);
iFilledRectangle(240, 200, 120, 40);
iSetColor(255, 255, 255);
iText(260, 215, "Play Again");

//         char highScoreStr[50];
// sprintf(highScoreStr, "High Score: %d", highScore);
// iText(240, 330, highScoreStr, GLUT_BITMAP_HELVETICA_18);

        iText(240, 310, "High Scores:", GLUT_BITMAP_HELVETICA_18);

        char scoreStr[50];
        sprintf(scoreStr, "Final Score: %d", score);
        iText(240, 360, scoreStr, GLUT_BITMAP_HELVETICA_18);

        char hscStr[100];
        for (int i = 0; i < MAX_HIGHSCORES; i++) 
        {
            sprintf(hscStr, "%d. %d", i + 1, highscores[i]);
            iText(240, 290 - i * 20, hscStr, GLUT_BITMAP_HELVETICA_18);
        }

        iText(200, 150, "Press 'b' to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
    }
    if (gamestate == stateWin)
{
    iClear();
    iSetColor(0, 100, 0); // Dark green background
    iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    iSetColor(255, 255, 0); // Yellow text
    iText(200, 450, "CONGRATULATIONS!", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(220, 400, "You Win!", GLUT_BITMAP_HELVETICA_18);

    char scoreStr[50];
    sprintf(scoreStr, "Final Score: %d", score);
    iSetColor(255, 255, 255);
    iText(240, 360, scoreStr, GLUT_BITMAP_HELVETICA_18);

    iText(240, 310, "High Scores:", GLUT_BITMAP_HELVETICA_18);
    char hscStr[100];
    for (int i = 0; i < MAX_HIGHSCORES; i++) 
    {
        sprintf(hscStr, "%d. %d", i + 1, highscores[i]);
        iText(240, 290 - i * 20, hscStr, GLUT_BITMAP_HELVETICA_18);
    }

    iSetColor(35, 10, 100);
    iFilledRectangle(200, 200, 200, 40);
    iSetColor(255, 255, 255);
    iText(240, 215, "Next Level / Play Again");

    iText(200, 150, "Press 'b' to return to Main Menu", GLUT_BITMAP_HELVETICA_18);
}

}

void initializeGame(int difficulty) 
{
    stopAllGameTimers();
    if (scrollTimerId >= 0) 
    {
        iPauseTimer(scrollTimerId);
        scrollTimerId = -1;
    }
    if (gameLoopTimerId >= 0) 
    {
        iPauseTimer(gameLoopTimerId);
        gameLoopTimerId = -1;
    }

    score = 0;
    scrollingStopped = false; 
    gameEnded = false;
    Srow = (g_row - 5); 
    gameTime = 0;
    angle = 90;  
    
    shooterB.isMoving = false;
    shooterB.dx = 0;
    shooterB.dy = 0;

    for (int r = 0; r < g_row; r++) 
    {
        for (int c = 0; c < g_col; c++) 
        {
            grid[r][c].bDes = Empty;
            grid[r][c].isMoving = false;
            grid[r][c].dx = 0;
            grid[r][c].dy = 0;
        }
    }

    if (difficulty == 5) 
    { 
        initializeGrid(level1_map); 
    } 
    else if (difficulty == 7) 
    {   
        initializeGrid(level2_map);
    } 
    else if (difficulty == 9) 
    {    
        initializeGrid(level3_map);
    }

    nextB.bDes = getRandomDesign(); 
    Shooter();
    //  if (scrollTimerId >= 0) 
    //     iPauseTimer(scrollTimerId);
    

    // //gameTime = 0;
    //  if (gameLoopTimerId >= 0)
    //     iPauseTimer(gameLoopTimerId);

       scrollTimerId = iSetTimer(scrollInterval, scroll);
    gameLoopTimerId = iSetTimer(20, updateGame);

    //gameTime = 0;
    gamestate = statePlay;
}

void iMouseMove(int mx, int my)
{
    if(gamestate!=statePlay)
    return ;
    double dx=mx-SHOOTER_X;
    double dy=my-SHOOTER_Y;
    angle=atan2(dy,dx)*(180.0/PI);
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
        if(gamestate==stateMenu)
       {
            if(mx>=250 && mx<=350 && my>=500 && my<=545 )
            {
                difficulty = 5;
                scrollInterval = 25000;
                initializeGame(5);
            }
            else if(mx>=250 && mx<=350 && my>=400 && my<=445 )
         gamestate=stateLevel ;
            else if(mx>=250 && mx<=350 && my>=300 && my<=345 )
         gamestate=stateHelp ;
            else if(mx>=250 && mx<=350 && my>=200 && my<=245 )
         gamestate=stateAbout ;
            else if(mx>=230 && mx<=370 && my>=100 && my<=145)
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
             stopAllGameTimers();    
                 difficulty=5;
                  scrollInterval = 25000;
                  initializeGame(5);
            }
        }

        if(gamestate==stateMedium)
        {
            if(mx>=200 && mx<=350 && my>=390 && my<=435 )
            {
                stopAllGameTimers();
                 difficulty=7;
                  scrollInterval = 20000;
                 initializeGame(7);
            }
        }

        if(gamestate==stateHard)
        {
            if(mx>=200 && mx<=350 && my>=390 && my<=435 )
            {
                stopAllGameTimers();
                 difficulty=9;
                  scrollInterval = 14000;
                 initializeGame(9);
            }
        }

        if(gamestate==statePlay)
        {
            if(mx>=(nextB.x - BUBBLE_RADIUS)&& my>=(nextB.y - BUBBLE_RADIUS) && mx<=(nextB.x + BUBBLE_RADIUS) && my<=(nextB.y + BUBBLE_RADIUS) )
            {
                swapB =shooterB;
                shooterB=nextB;
                nextB=swapB;
            }
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
            {
                if (!shooterB.isMoving) 
                {
                    shooterB.isMoving = true;
                    double angleRad = angle * PI / 180.0;
                    shooterB.dx = BUBBLE_SPEED * cos(angleRad);
                    shooterB.dy = BUBBLE_SPEED * sin(angleRad);
                }
            }
            if (mx >= 520 && mx <= 550 && my >= 670 && my <= 690)
            {
                gamestate = statePause;
            }
        }

        if (gamestate == statePause)
        {
            if(mx >= 250 && mx <= 350 && my >= 530 && my <= 560)
            {
               if(sound) 
               {
                sound = 0;
                iPauseSound(bgid);
               }
               else 
               {
               sound = 1;
               iResumeSound(bgid);
               } 
            }
        }

        if (gamestate == stateGameOver) 
        {
            if (mx >= 240 && mx <= 360 && my >= 200 && my <= 240) 
            {
                initializeGame(difficulty); 
            }
        }
        if (gamestate == stateWin) 
{
    if (mx >= 200 && mx <= 400 && my >= 200 && my <= 240) 
    {
        initializeGame(difficulty); 
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
    {
        if (gamestate == statePlay || gamestate == stateGameOver || gamestate == stateWin) {
            stopAllGameTimers();
            gameEnded = true; 
        }
        
        if (gamestate == stateEasy || gamestate == stateMedium || gamestate == stateHard)
            gamestate = stateLevel;  
        else if (gamestate == stateHelp || gamestate == stateAbout || gamestate == stateLevel)
            gamestate = stateMenu;  
        else if (gamestate == statePlay || gamestate == stateGameOver || gamestate == stateWin)
            gamestate = stateMenu;
        break;
    }

    case 'r':
    {
        if(gamestate == statePause)
        {
            gamestate = statePlay;
        }
        break;
    }

    default:
        break;
    }
}

void iSpecialKeyboard(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_RIGHT:
    {
        if(gamestate==statePlay)
        {
            angle-=2;
        }
        break;
    }

    case GLUT_KEY_LEFT:
    {
        if(gamestate==statePlay)
        {
            angle+=2;
        }
        break;
    }

    case GLUT_KEY_UP:
    {
    if(gamestate==statePlay)
       {
                
            {
                shooterB.isMoving = true;
                double angleRad = angle * PI / 180.0;
                shooterB.dx = BUBBLE_SPEED * cos(angleRad);
                shooterB.dy = BUBBLE_SPEED * sin(angleRad);
            }
        }
      
        break;
    }

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

    gamestate = stateMenu;
    gameEnded = false;

    loadImage();
    loadHighScores();
    //Shooter();
    // gameLoopTimerId = iSetTimer(20, updateGame);
    // if(Srow==0)
    // {
    //     iPauseTimer(scrollTimerId);
    // }

    tid = iSetTimer(2000, stopload);

    //loadHighScores();
     iInitializeSound();
    bgid = iPlaySound("sound/gamebg.wav", true, 80);
    if (!sound)
        iPauseSound(bgid);

    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Bouncing Ball");

    return 0;
}

