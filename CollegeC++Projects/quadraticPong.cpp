#include <winbgim.h>
#include <iostream>
#include <math.h>

using namespace std;

void waitForLeftMouseClick();

int boundsX = 800;
int boundsY = 600;

int x1 = 1;
int x2 = 2;
int c = 50;

bool win = false;
bool lose = false;

int compWins = 0;
int playerWins = 0;

class ball
{
      private:
              //INSERT CODE HERE:
              // An object has an x position and a y position at all times
              int xpos;
              int ypos;
              int xdir;
              int ydir;
              int Accel;
      
      public:
             ball(int x, int y)
             {
                      xpos = x;
                      ypos = y;
                      ydir = 10;
                      xdir = 10;
                      Accel = 0;
             }
             int getXpos()
             {
                 return xpos;
             }
             int getYpos()
             {
                 return ypos;
             }
             void setXDirection(int a)
             {
                  if (Accel < 5)
                  Accel++;
                  
                  if (a > 0)
                  xdir = a + Accel;
                  else
                  xdir = a - Accel;
             }
             void setYDirection(int a)
             {
                  if (Accel < 5)
                  Accel++;
                  
                  if (a > 0)
                  ydir = a + Accel;
                  else
                  ydir = a - Accel;
             }
             void move()
             {
                  
                  xpos = xpos + xdir;
                  //INSERT CODE HERE
                  //Hint: since we have the x position what can we do to find the y position?
                  ypos = -x1 * pow((xpos - 400) / 20,2) + -x2 * ((xpos - 400) / 20) + c + 200;
             }
             void display()
             {
                  setcolor(BLUE);
                  setfillstyle(SOLID_FILL, BLUE);
                  circle(xpos,ypos, 30);
                  floodfill(xpos,ypos, BLUE);
             }
             void checkBounds()
             {
                  if (ypos > boundsY - 15)
                  {
                           setYDirection(-10);
                  }
                  else if (ypos < 15)
                  {
                       setYDirection(10);
                  }
             }
             void runTheBall()
             {
                  checkBounds();
                  move();
                  display();
             }
             void resetValues()
             {
                  Accel = 0;
                  xpos = 400;
                  ypos = 300;
                  xdir = 10;
                  ydir = 10;
             }
};

class paddle
{
      private:
              int xpos;
              int ypo;
              int xdir;
              int ydir;
      
      public:
             paddle(int x, int y)
             {
                      xpos = x;
                      ypo = y;
             }
             int getXpos()
             {
                 return xpos;
             }
             int getYpos()
             {
                 return ypo;
             }
             void setYDirection()
             {
                  if (ypo < mousey())
                  ydir = 10;
                  else
                  ydir = -10;
             }
             void moves()
             {
                  ypo = ypo + ydir;
             }
             void display()
             {
                  setcolor(RED);
                  setfillstyle(SOLID_FILL, RED);
                  rectangle(xpos - 10,ypo - 50, xpos + 10, ypo + 50);
                  floodfill(xpos,ypo, RED);
             }
             void runThePaddle()
             {
                  setYDirection();
                  moves();
                  display();
             }
};

class computerPaddle
{
      private:
              int xpos;
              int ypo;
              int xdir;
              int ydir;
      
      public:
             computerPaddle(int x, int y)
             {
                      xpos = x;
                      ypo = y;
             }
             int getXpos()
             {
                 return xpos;
             }
             int getYpos()
             {
                 return ypo;
             }
             void setYDirection(int a)
             {
                  ydir = a;
             }
             void moves()
             {
                  ypo = ypo + ydir;
             }
             void display()
             {
                  setcolor(RED);
                  setfillstyle(SOLID_FILL, RED);
                  rectangle(xpos - 10,ypo - 50, xpos + 10, ypo + 50);
                  floodfill(xpos,ypo, RED);
             }
             void runThePaddle()
             {
                  moves();
                  display();
             }
};

void clearScreen()
{     
     setcolor(WHITE);
     setfillstyle(SOLID_FILL, WHITE);
     floodfill(1,1, BROWN);     
}
void runThePaddles(computerPaddle& compPaddle, paddle playerPaddle, ball& Balls)
{
     int compY = compPaddle.getYpos();
     int ballY = Balls.getYpos();
     int playerY = playerPaddle.getYpos();
     
     int ballX = Balls.getXpos();
     

     if (compY < ballY)
        compPaddle.setYDirection(22);
     else
        compPaddle.setYDirection(-22);
        
     if (ballX < 70)
     {
        if (abs(playerY - ballY) < 50)
        {
                                     Balls.setXDirection(10);
        }
     }
     if (ballX > boundsX - 70)
     {
        if (abs(compY - ballY) < 50)
        {
                                   Balls.setXDirection(-10);
        } 
     }
}

void startScreen()
{
     settextstyle(0,0,2);
     outtextxy(175,100, "Welcome to quadratic pong!");
     outtextxy(175,150, "Click anywhere to continue");
     waitForLeftMouseClick();
}

void instructions()
{
     clearScreen();
     settextstyle(0,0,1);
     setcolor(BLACK);
     setfillstyle(SOLID_FILL, BLACK);
     floodfill(0,0,BLACK);
     setcolor(WHITE);
     outtextxy(175,100, "Welcome to Quadratic Pong, in this game you will");
     outtextxy(175,150, "learn how use quadratic equations to manipulate");
     outtextxy(175,200, "the movement of a ball. You will be controlling the");
     outtextxy(175,250, "left paddle with your mouse, the paddle will move");
     outtextxy(175,300, "to the y co-ordinate of your mouse. (Hence if your");
     outtextxy(175,350, "mouse is above the paddle the paddle will go up)");
     outtextxy(175,400, "Your objective is to beat the computer five times!");
     outtextxy(175,500, "After this, go the the screen behind this to enter");
     outtextxy(175,550, "your values for the quadratic");
     waitForLeftMouseClick();
}

void checkWin(ball x)
{
     int xpos = x.getXpos();
     
     settextstyle(0,0,3);
     if (xpos < 0)
     {
          lose = true;
          compWins++;
          outtextxy(300,100, "YOU LOSE!");
          outtextxy(100,150, "Click anywhere to continue!");
          waitForLeftMouseClick();
     }
     else if (xpos > boundsX)
     {
          win = true;
          playerWins++;
          outtextxy(300,100, "YOU WIN!");
          outtextxy(100,150, "Click anywhere to continue!");
          waitForLeftMouseClick();
     }
}

void getValues()
{
     cout << "Enter the quadratic in the form of ax^2 + bx + c" << endl;
     cout << "Please note: if you enter values too large the ball WILL NOT go offscreen," << endl;
     cout << "doing so will cause the game to be much less fun and eventually cause you to lose" << endl;
     
     bool tryAgain = true;
     
     while (tryAgain == true)
     {
           cout << endl;
           tryAgain = false;
           cout << "Enter x1 (Less than 3)" << endl;
           cin >> x1;
           if (abs(x1) > 3)
           tryAgain = true;
     }
     
     tryAgain = true;
     
     while (tryAgain == true)
     {
           cout << endl;
           tryAgain = false;
           cout << "Enter x2 (Less than 10)" << endl;
           cin >> x2;
           if (abs(x2) > 10)
           tryAgain = true;
     }
     
     tryAgain = true;
     
     while (tryAgain == true)
     {
           cout << endl;
           tryAgain = false;
           cout << "Enter c (Greater than 100, Less than 400)" << endl;
           cin >> c;
           if (abs(c) < 100 && abs(c) > 400)
           tryAgain = true;
     }
     cout << "Thank you, go back to the previous screen now to play the game!" << endl;
     
     waitForLeftMouseClick();
}
int main()
{
    initwindow(boundsX,boundsY); 
    
    ball BallOne(300,300);
    paddle PaddleOne(20,300);
    computerPaddle compPaddles(780,300);
    BallOne.display();
    PaddleOne.display();
    compPaddles.display();
    
    //INSERT CODE HERE:
    //What to we need to do to show the user how to use the program?
    
    startScreen();
    instructions();
    clearScreen();
    
    while (compWins < 5 && playerWins < 5)
    {
        BallOne.resetValues();
        win = false;
        lose = false;
        getValues();
        
        while (win == false && lose == false)
        {
              //INSERT CODE HERE:
              // C++ refreshes the screen every time an object moves, what line of code
              // can we use to clear the screen?
              
              clearScreen();
              runThePaddles(compPaddles, PaddleOne, BallOne);
              BallOne.runTheBall();
              PaddleOne.runThePaddle();
              compPaddles.runThePaddle();
              checkWin(BallOne);
              
              //INSERT CODE HERE:
              // To keep the objects on the screen what can we do to keep the screen from
              // clearing itself for a short period of time?
              
              delay(200);
        }
    }
    
    if (playerWins == 5)
    outtextxy(100,100,"YOU BEAT THE COMPUTER!");
    else 
    outtextxy(100,100,"YOU LOST TO THE COMPUTER");
    
    waitForLeftMouseClick();
    closegraph();
	return 0;
}

void waitForLeftMouseClick()
{
    clearmouseclick(WM_LBUTTONDOWN);
    const int DELAY = 50; // Milliseconds of delay between checks
    int x, y;
    while (!ismouseclick(WM_LBUTTONDOWN))
        delay(DELAY);
    getmouseclick(WM_LBUTTONDOWN, x, y);
}
