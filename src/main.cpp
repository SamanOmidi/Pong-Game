#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

// define window size
int WIDTH = 800;
int HEIGHT = 600;

// update rate(60 fps)
int interval = 1000 / 60;

// init parameters
int score_left = 0;
int score_right = 0;

// rackets
int racket_width = 50;
int racket_height = 80;
int racket_speed = 15;

// position of left racket
float racket_left_x = 10.0f;
float racket_left_y = HEIGHT / 2;

// position of right racket
float racket_right_x = WIDTH - racket_width - 10;
float racket_right_y = HEIGHT / 2;

// ball
float ball_pos_x = WIDTH / 2;
float ball_pos_y = HEIGHT / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 10;
int ball_speed = 5;

float total_time = 0.0;

// functions
void my_display();
void update(int value);
void enable2D();
void init();
void drawScore(float x, float y, std::string text);
void drawRect(float x, float y, float width, float height);
void keyboard(unsigned char key, int x, int y);
void updateBall();

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Pong");
    init();
    enable2D();
    glutDisplayFunc(my_display);
    glutTimerFunc(interval, update, 0);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}

// do basic configuration
void init()
{
    // here we set the color of drawing to white
    glColor3f(1.0f, 1.0f, 1.0f);
}

// keyboard handler
void keyboard(unsigned char key, int x, int y)
{
    if (key == 'w') racket_left_y += racket_speed;
    if (key == 's') racket_left_y -= racket_speed;
    if (key == 'o') racket_right_y += racket_speed;
    if (key == 'l') racket_right_y -= racket_speed;
    if (key == 'f')
    {  
        float t = 0;
        t = ball_dir_x * ball_speed;
        float t2 = 0;
        t2 = ball_dir_y * ball_speed;
        if (abs(t) < racket_width || abs(t2)<racket_width) ball_speed += 1;

    }
    if (key == 'g')
    {
        if (ball_speed != 0) ball_speed -= 1;
    }
}

// draw score on the window
void drawScore(float x, float y, std::string text)
{
    glRasterPos2d(x, y);
    glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

// draw rackets
void drawRect(float x, float y, float width, float height)
{
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

// tell opengl that all our configuration is in 2D
void enable2D()
{
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WIDTH, 0.0f, HEIGHT, 0.0f, 1.0f);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

void my_display()
{
    // clear in the beginning
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // draw rackets
    drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
    drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

    // draw ball
    drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);

    // draw score on the window
    drawScore((WIDTH / 2) - 10, HEIGHT - 15, std::to_string(score_left) + ":" + std::to_string(score_right));
    drawScore((WIDTH / 2) - 20, HEIGHT - 30, std::to_string(total_time));
    // swap our double buffers
    glutSwapBuffers();
}

void updateBall()
{
    // move the ball
    ball_pos_x += ball_dir_x * ball_speed;
    ball_pos_y += ball_dir_y * ball_speed;

    // hit left wall
    if (ball_pos_x < 0)
    {
        score_right ++;
        ball_pos_x = WIDTH / 2;
        ball_pos_y = HEIGHT / 2;
        ball_dir_x = abs(ball_dir_x);
        ball_dir_y = 0;
    }

    // hit right wall
    if (ball_pos_x > WIDTH)
    {
        score_left ++;
        ball_pos_x = WIDTH / 2;
        ball_pos_y = HEIGHT / 2;
        ball_dir_x = -abs(ball_dir_x);
        ball_dir_y = 0;
    }

    // hit top
    if (ball_pos_y > HEIGHT) ball_dir_y = -abs(ball_dir_y);
    
    // hit bottom
    if (ball_pos_y < 0) ball_dir_y = abs(ball_dir_y);

    // hit by left racket?
    if (ball_pos_x < racket_left_x + racket_width &&
        ball_pos_x > racket_left_x &&
        ball_pos_y < racket_left_y + racket_height &&
        ball_pos_y > racket_left_y) 
    {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
        ball_dir_x = fabs(ball_dir_x);
        ball_dir_y = t;
    }

    // hit by right racket?
    if (ball_pos_x > racket_right_x &&
        ball_pos_x < racket_right_x + racket_width &&
        ball_pos_y < racket_right_y + racket_height &&
        ball_pos_y > racket_right_y)
    {
        // set fly direction depending on where it hit the racket
        // (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
        float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
        ball_dir_x = -fabs(ball_dir_x);
        ball_dir_y = t;
    }

    // make sure that length of dir stays at 1
    //std::vec2_norm(ball_dir_x, ball_dir_y);
}

void update(int value)
{
    total_time += interval;
    drawScore((WIDTH / 2) - 10, HEIGHT - 15, std::to_string(total_time));
    if (total_time >= 15000)
    {
        ball_speed = 0;
        racket_speed = 0;
    }
    
    // move the ball
    updateBall();
    
    glutTimerFunc(interval, update, 0);
    // redisplay frame
    glutPostRedisplay();
}
