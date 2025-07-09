/*#include "iGraphics.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

#define g_row 15
#define g_col 12
#define BUBBLE_RADIUS 20
#define BUBBLE_DIAMETER (BUBBLE_RADIUS * 2)
#define SHOOTER_X (600 / 2)
#define SHOOTER_Y (BUBBLE_RADIUS + 10)
#define BUBBLE_SPEED 25.0
#define SCREEN_HEIGHT 700
#define SCREEN_WIDTH 600

int score = 0;
double angle = 90;

enum Bubblecolor { Empty, B1, B2, B3, B4, B5, colorCount };

struct Bubble {
    Bubblecolor color;
    double x, y;
    double dx, dy;
    bool isMoving;
    int row, col;
};

Bubble grid[g_row][g_col];
Bubble shooterB;
Bubble nextB;
Image bubbleImages[colorCount];
Image shooterImage;

Bubblecolor getRandomColor() {
    return static_cast<Bubblecolor>((rand() % (colorCount - 1)) + 1);
}

void getPixelCoords(int row, int col, double &x, double &y) {
    x = (col * (BUBBLE_DIAMETER - 2)) + BUBBLE_RADIUS + 30;
    if (row % 2 != 0) x += BUBBLE_RADIUS;
    y = SCREEN_HEIGHT - (row * (BUBBLE_DIAMETER - 5)) - BUBBLE_RADIUS;
}

void shooter() {
    shooterB = nextB;
    shooterB.x = SHOOTER_X;
    shooterB.y = SHOOTER_Y;
    shooterB.isMoving = false;

    nextB.color = getRandomColor();
    nextB.x = SHOOTER_X - BUBBLE_DIAMETER * 3;
    nextB.y = SHOOTER_Y;
}

void loadImage() {
    iLoadImage(&bubbleImages[B1], "image/e1.png"); iResizeImage(&bubbleImages[B1], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleImages[B2], "image/e2.png"); iResizeImage(&bubbleImages[B2], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleImages[B3], "image/e3.png"); iResizeImage(&bubbleImages[B3], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleImages[B4], "image/e4.png"); iResizeImage(&bubbleImages[B4], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleImages[B5], "image/e5.png"); iResizeImage(&bubbleImages[B5], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    //iLoadImage(&shooterImage, "image/"); iResizeImage(&shooterImage, 40, 80);
}

void initializeGrid(int initialRow) {
    for (int r = 0; r < g_row; ++r) {
        for (int c = 0; c < g_col; ++c) {
            if (r < initialRow) {
                grid[r][c].color = getRandomColor();
            } else {
                grid[r][c].color = Empty;
            }
            grid[r][c].row = r;
            grid[r][c].col = c;
            grid[r][c].isMoving = false;
            getPixelCoords(r, c, grid[r][c].x, grid[r][c].y);
        }
    }
    shooter();
}

std::vector<Bubble*> getNeighbors(int r, int c) {
    std::vector<Bubble*> neighbors;
    int parity = r % 2;
    int directions[2][6][2] = {
        {{0, -1}, {0, 1}, {-1, -1}, {-1, 0}, {1, -1}, {1, 0}},
        {{0, -1}, {0, 1}, {-1, 0}, {-1, 1}, {1, 0}, {1, 1}}
    };

    for (int i = 0; i < 6; ++i) {
        int nr = r + directions[parity][i][0];
        int nc = c + directions[parity][i][1];
        if (nr >= 0 && nr < g_row && nc >= 0 && nc < g_col) {
            if (grid[nr][nc].color != Empty) {
                neighbors.push_back(&grid[nr][nc]);
            }
        }
    }
    return neighbors;
}

 void findAndPopMatches(int start_r, int start_c) {
    if (start_r < 0 || start_r >= g_row || start_c < 0 || start_c >= g_col || grid[start_r][start_c].color == Empty) return;
    std::vector<Bubble*> to_visit;
    std::vector<Bubble*> cluster;
    bool visited[g_row][g_col] = {false};
    Bubblecolor targetColor = grid[start_r][start_c].color;

    to_visit.push_back(&grid[start_r][start_c]);
    visited[start_r][start_c] = true;

    size_t head = 0;
    while (head < to_visit.size()) {
        Bubble* current = to_visit[head++];
        cluster.push_back(current);
        for (Bubble* neighbor : getNeighbors(current->row, current->col)) {
            if (!visited[neighbor->row][neighbor->col] && neighbor->color == targetColor) {
                visited[neighbor->row][neighbor->col] = true;
                to_visit.push_back(neighbor);
            }
        }
    }

    if (cluster.size() >= 3) {
        for (Bubble* b : cluster) {
            b->color = Empty;
            score += 10;
        }
    }
}

void updateGame() {
    if (shooterB.isMoving) {
        shooterB.x += shooterB.dx;
        shooterB.y += shooterB.dy;

        if (shooterB.x <= BUBBLE_RADIUS || shooterB.x >= SCREEN_WIDTH - BUBBLE_RADIUS)
            shooterB.dx *= -1;

        bool collision = false;
        if (shooterB.y >= SCREEN_HEIGHT - BUBBLE_RADIUS)
            collision = true;

        for (int r = 0; r < g_row && !collision; ++r) {
            for (int c = 0; c < g_col && !collision; ++c) {
                if (grid[r][c].color != Empty) {
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
                    if (grid[r][c].color == Empty) {
                        double tx, ty;
                        getPixelCoords(r, c, tx, ty);
                        double dx = shooterB.x - tx;
                        double dy = shooterB.y - ty;
                        double dist = dx * dx + dy * dy;
                        if (dist < minDist) {
                            minDist = dist;
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
                getPixelCoords(snap_r, snap_c, grid[snap_r][snap_c].x, grid[snap_r][snap_c].y);
                findAndPopMatches(snap_r, snap_c);
            }
            shooter();
        }
    }
}

void iDraw() {
    iClear();
    updateGame();

    for (int r = 0; r < g_row; r++) {
        for (int c = 0; c < g_col; c++) {
            if (grid[r][c].color != Empty) {
                iShowLoadedImage(grid[r][c].x - BUBBLE_RADIUS, grid[r][c].y - BUBBLE_RADIUS, &bubbleImages[grid[r][c].color]);
            }
        }
    }

    iShowLoadedImage(SHOOTER_X - 20, SHOOTER_Y - 20, &shooterImage);
    if (shooterB.color != Empty) {
        iShowLoadedImage(shooterB.x - BUBBLE_RADIUS, shooterB.y - BUBBLE_RADIUS, &bubbleImages[shooterB.color]);
    }
}

void iMouseMove(int mx, int my) {
    double dx = mx - SHOOTER_X;
    double dy = my - SHOOTER_Y;
    angle = atan2(dy, dx) * (180.0 / M_PI);
    if (angle < 10) angle = 10;
    if (angle > 170) angle = 170;
}

void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!shooterB.isMoving) {
            shooterB.isMoving = true;
            double angleRad = angle * M_PI / 180.0;
            shooterB.dx = BUBBLE_SPEED * cos(angleRad);
            shooterB.dy = BUBBLE_SPEED * sin(angleRad);
        }
    }
}

void iKeyboard(unsigned char key) {}
void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}
void iSpecialKeyboard(unsigned char key) {}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);

    srand(static_cast<unsigned>(time(0)));

    loadImage();         
    initializeGrid(5);   
    shooter();           

    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Bubble Shooter - Game Only");

    return 0;

}*/








#include "iGraphics.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue> 

#define g_row 15
#define g_col 12
#define BUBBLE_RADIUS 20
#define BUBBLE_DIAMETER (BUBBLE_RADIUS * 2)
#define SHOOTER_X (600 / 2)
#define SHOOTER_Y (BUBBLE_RADIUS + 10)
#define BUBBLE_SPEED 25.0
#define SCREEN_HEIGHT 700
#define SCREEN_WIDTH 600

int score = 0;
double angle = 90;

enum Bubblecolor { Empty, B1, B2, B3, B4, B5, colorCount };

struct Bubble {
    Bubblecolor color;
    double x, y;
    double dx, dy;
    bool isMoving;
    int row, col;
};

Bubble grid[g_row][g_col];
Bubble shooterB;
Bubble nextB;
Image bubbleImages[colorCount];
Image shooterImage;

Bubblecolor getRandomColor();
void getPixelCoords(int row, int col, double &x, double &y);
void shooter();
void loadImage();
void initializeGrid(int initialRow);
std::vector<Bubble*> getNeighbors(int r, int c);
void findAndPopMatches(int start_r, int start_c);
void removeFloatingBubbles(); // New function for gravity effect
void updateGame();
void iDraw();
void iMouseMove(int mx, int my);
void iMouse(int button, int state, int mx, int my);
void iKeyboard(unsigned char key);
void iMouseDrag(int mx, int my);
void iMouseWheel(int dir, int mx, int my);
void iSpecialKeyboard(unsigned char key);

Bubblecolor getRandomColor() {
    return static_cast<Bubblecolor>((rand() % (colorCount - 1)) + 1);
}

void getPixelCoords(int row, int col, double &x, double &y) {
    x = (col * (BUBBLE_DIAMETER - 2)) + BUBBLE_RADIUS + 30;
    if (row % 2 != 0) x += BUBBLE_RADIUS;
    y = SCREEN_HEIGHT - (row * (BUBBLE_DIAMETER - 5)) - BUBBLE_RADIUS;
}

void shooter() {
    shooterB = nextB;
    shooterB.x = SHOOTER_X;
    shooterB.y = SHOOTER_Y;
    shooterB.isMoving = false;

    nextB.color = getRandomColor();
    nextB.x = SHOOTER_X - BUBBLE_DIAMETER * 3;
    nextB.y = SHOOTER_Y;
}

void loadImage() {
    iLoadImage(&bubbleImages[B1], "image/e1.png"); iResizeImage(&bubbleImages[B1], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleImages[B2], "image/e2.png"); iResizeImage(&bubbleImages[B2], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleImages[B3], "image/e3.png"); iResizeImage(&bubbleImages[B3], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleImages[B4], "image/e4.png"); iResizeImage(&bubbleImages[B4], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&bubbleImages[B5], "image/e5.png"); iResizeImage(&bubbleImages[B5], BUBBLE_DIAMETER, BUBBLE_DIAMETER);
    iLoadImage(&shooterImage, "image/T.jpeg"); iResizeImage(&shooterImage, 40, 80);
}

void initializeGrid(int initialRow) {
    for (int r = 0; r < g_row; ++r) {
        for (int c = 0; c < g_col; ++c) {
            if (r < initialRow) {
                grid[r][c].color = getRandomColor();
            } else {
                grid[r][c].color = Empty;
            }
            grid[r][c].row = r;
            grid[r][c].col = c;
            grid[r][c].isMoving = false;
            getPixelCoords(r, c, grid[r][c].x, grid[r][c].y);
        }
    }
    shooter();
}




void drawTrajectory() {
    if (shooterB.isMoving) return;

    // Simulate a temporary bubble to calculate the path
    double temp_x = SHOOTER_X;
    double temp_y = SHOOTER_Y;
    double aim_rad = angle * (M_PI / 180.0);
    double temp_dx = BUBBLE_SPEED * cos(aim_rad);
    double temp_dy = BUBBLE_SPEED * sin(aim_rad);

    double start_x = temp_x;
    double start_y = temp_y;

    // Loop to trace the path
    for (int i = 0; i < 100; ++i) { // Limit iterations to prevent infinite loops
        temp_x += temp_dx;
        temp_y += temp_dy;

        // Check for collision with existing bubbles
        for (int r = 0; r < g_row; ++r) {
            for (int c = 0; c < g_col; ++c) {
                if (grid[r][c].color != Empty) {
                    double dist = sqrt(pow(temp_x - grid[r][c].x, 2) + pow(temp_y - grid[r][c].y, 2));
                    if (dist < BUBBLE_DIAMETER) {
                        iSetColor(255, 255, 255);
                        iSetLineWidth(2);
                        iLine(start_x, start_y, temp_x, temp_y);
                        return; // Stop drawing after first predicted collision
                    }
                }
            }
        }
        
        // Check for wall bounce
        if (temp_x <= BUBBLE_RADIUS || temp_x >= SCREEN_WIDTH - BUBBLE_RADIUS) {
            // Draw line up to the wall
            iSetColor(255, 255, 255);
            iSetLineWidth(2);
            iLine(start_x, start_y, temp_x, temp_y);

            // Bounce and continue from the wall
            temp_dx *= -1;
            start_x = temp_x;
            start_y = temp_y;
        }

        // Check for top wall
        if (temp_y >= SCREEN_HEIGHT - BUBBLE_RADIUS) {
            iSetColor(255, 255, 255);
            iSetLineWidth(2);
            iLine(start_x, start_y, temp_x, temp_y);
            return;
        }
    }

    // Draw the final segment if no collision was found
    iSetColor(255, 255, 255);
    iSetLineWidth(2);
    iLine(start_x, start_y, temp_x, temp_y);
}



std::vector<Bubble*> getNeighbors(int r, int c) {
    std::vector<Bubble*> neighbors;
    int parity = r % 2;
    // Directions for even rows (parity 0) and odd rows (parity 1)
     //Even rows: (0, -1), (0, 1), (-1, -1), (-1, 0), (1, -1), (1, 0)
     //Odd rows: (0, -1), (0, 1), (-1, 0), (-1, 1), (1, 0), (1, 1)
    int directions[2][6][2] = {
        {{0, -1}, {0, 1}, {-1, -1}, {-1, 0}, {1, -1}, {1, 0}},
        {{0, -1}, {0, 1}, {-1, 0}, {-1, 1}, {1, 0}, {1, 1}}
    };

    for (int i = 0; i < 6; ++i) {
        int nr = r + directions[parity][i][0];
        int nc = c + directions[parity][i][1];
        if (nr >= 0 && nr < g_row && nc >= 0 && nc < g_col) {
                if (grid[nr][nc].color != Empty) {
                neighbors.push_back(&grid[nr][nc]);
            }
        }
    }
    return neighbors;
}

void findAndPopMatches(int start_r, int start_c) {
    if (start_r < 0 || start_r >= g_row || start_c < 0 || start_c >= g_col || grid[start_r][start_c].color == Empty) return;

    std::queue<Bubble*> to_visit; 
    std::vector<Bubble*> cluster;
    bool visited[g_row][g_col]; 
    for(int r = 0; r < g_row; ++r) {
        for(int c = 0; c < g_col; ++c) {
            visited[r][c] = false; 
        }
    }
    
    Bubblecolor targetColor = grid[start_r][start_c].color;

    to_visit.push(&grid[start_r][start_c]);
    visited[start_r][start_c] = true;

    while (!to_visit.empty()) {
        Bubble* current = to_visit.front();
        to_visit.pop();
        cluster.push_back(current);

        for (Bubble* neighbor : getNeighbors(current->row, current->col)) {
            if (!visited[neighbor->row][neighbor->col] && neighbor->color == targetColor) {
                visited[neighbor->row][neighbor->col] = true;
                to_visit.push(neighbor);
            }
        }
    }

    if (cluster.size() >= 3) {
        for (Bubble* b : cluster) {
            b->color = Empty;
            score += 10;
        }
        removeFloatingBubbles();
    }
}

void removeFloatingBubbles() {
    bool connected[g_row][g_col];
    for(int r = 0; r < g_row; ++r) {
        for(int c = 0; c < g_col; ++c) {
            connected[r][c] = false; 
        }
    }

    std::queue<Bubble*> q;

    for (int c = 0; c < g_col; ++c) {
        if (grid[0][c].color != Empty) {
            q.push(&grid[0][c]);
            connected[0][c] = true;
        }
    }

    while (!q.empty()) {
        Bubble* current = q.front();
        q.pop();

        for (Bubble* neighbor : getNeighbors(current->row, current->col)) {
             if (neighbor->color != Empty && !connected[neighbor->row][neighbor->col]) {
                connected[neighbor->row][neighbor->col] = true;
                q.push(neighbor);
            }
        }
    }

     for (int r = 0; r < g_row; ++r) {
        for (int c = 0; c < g_col; ++c) {
            if (grid[r][c].color != Empty && !connected[r][c]) {
                grid[r][c].color = Empty;
                score += 5; 
            }
        }
    }
}


void updateGame() {
    if (shooterB.isMoving) {
        shooterB.x += shooterB.dx;
        shooterB.y += shooterB.dy;

        if (shooterB.x <= BUBBLE_RADIUS || shooterB.x >= SCREEN_WIDTH - BUBBLE_RADIUS)
            shooterB.dx *= -1;

        bool collision = false;
        if (shooterB.y >= SCREEN_HEIGHT - BUBBLE_RADIUS)
            collision = true;

        for (int r = 0; r < g_row && !collision; ++r) {
            for (int c = 0; c < g_col && !collision; ++c) {
                if (grid[r][c].color != Empty) {
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
                    if (grid[r][c].color == Empty) {
                        double tx, ty;
                        getPixelCoords(r, c, tx, ty); 
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
                getPixelCoords(snap_r, snap_c, grid[snap_r][snap_c].x, grid[snap_r][snap_c].y); 
                findAndPopMatches(snap_r, snap_c);
            }
            shooter(); 
        }
    }
}

void iDraw() {
    iClear();
    updateGame(); 

    for (int r = 0; r < g_row; r++) {
        for (int c = 0; c < g_col; c++) {
            if (grid[r][c].color != Empty) {
                iShowLoadedImage(grid[r][c].x - BUBBLE_RADIUS, grid[r][c].y - BUBBLE_RADIUS, &bubbleImages[grid[r][c].color]);
            }
        }
    }



    drawTrajectory();

    iShowLoadedImage(SHOOTER_X - 20, SHOOTER_Y - 20, &shooterImage);
    
    if (shooterB.color != Empty) {
        iShowLoadedImage(shooterB.x - BUBBLE_RADIUS, shooterB.y - BUBBLE_RADIUS, &bubbleImages[shooterB.color]);
    }
    
    }

void iMouseMove(int mx, int my) {
    double dx = mx - SHOOTER_X;
    double dy = my - SHOOTER_Y;
    angle = atan2(dy, dx) * (180.0 / M_PI);
    if (angle < 10) angle = 10;
    if (angle > 170) angle = 170;
}

void iMouse(int button, int state, int mx, int my) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!shooterB.isMoving) {
            shooterB.isMoving = true;
            double angleRad = angle * M_PI / 180.0;
            shooterB.dx = BUBBLE_SPEED * cos(angleRad);
            shooterB.dy = BUBBLE_SPEED * sin(angleRad);
        }
    }
}

void iKeyboard(unsigned char key) {}
void iMouseDrag(int mx, int my) {}
void iMouseWheel(int dir, int mx, int my) {}
void iSpecialKeyboard(unsigned char key) {}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);

    srand(static_cast<unsigned>(time(0)));

    loadImage();
    initializeGrid(5);
    shooter();

    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Bubble Shooter - Game Only");

    return 0;
}