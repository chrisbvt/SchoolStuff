#include <winbgim.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <dos.h>
#include <math.h>
#include <conio.h>

using namespace std;

//Immune System Defense
//
// By:
// Chris Balles
// Devin Hurley
// Michael Schram
// Steven Arnold


//Steps:
//                                                              Status:
// 1. Create Cell class that properly displays cells            Done
// 2. Create Virus class that properly attacks cells            Done
// 3. Create Macrophages class which detects viruses            Done
// 4. Rest of immune system (Killer T's and Helper B's)         Done
// 5. Make into a defense type game                             Done
// 6. Thread music into game (Maybe)                            TBC

void waitForLeftMouseClick();
void runTheGame();
void doNothing();
bool checkTarget(int);
void checkWin();
void mainMenu();
void selectDifficulty();
void instructions();
void setGame();

void question(int);
void question1();
void question2();
void question3();
void question4();
void question5();

void displayCells();
void displayViruses();
void displayMacophage();
void displayTCells();
void displayAntiBodies();
void displayBCells();
void displayHTCells();

void spawnVirus(int);
void spawnTCell();
void spawnAntiBody(int, int);
bool checkAntiBodyTargets(int);

int correct = 0;
int wave = 1;
int numAntiBody = 0;
int numVirus = 0;
int numTCells = 0;
int numHTCells = 0;

//Mechanics
int initialHealth = 500;
int virusSpeed = 5;
int macroSpeed = 5;




// Cell Class
class Cell
{
      
      public:
      int xpos;
      int ypos;
      bool alive;
      bool infected;
      int health; 
      
      
             
             void display()
             {
                  if (alive == true)
                  {
                  setcolor(BLUE);
                  circle(xpos,ypos,10);
                  /*setcolor(RED);
                  circle(xpos+5,ypos+2,2);
                  setfillstyle(SOLID_FILL,RED);
                  floodfill(xpos+5,ypos+2,RED);
                  setcolor(CYAN);
                  circle(xpos-2,ypos-2,2);
                  setfillstyle(SOLID_FILL,CYAN);
                  floodfill(xpos-2,ypos-2,CYAN);
                  */
                     if (infected == true)
                     {
                           setfillstyle(SOLID_FILL, YELLOW);
                           floodfill(xpos,ypos,BLUE);
                     }
                  }
                  
             }
             void getInfected()
             {
                  if (health < initialHealth / 4)
                  infected = true;
             }
             void death()
             {
                  alive = false;
             }
             void printValues()
             {
                  cout << xpos << endl;
                  cout << ypos << endl;
                  cout << alive << endl;
                  cout << infected << endl;
             }
             void setValues(int x,int y, bool a ,bool i, int h)
             {
                  xpos = x;
                  ypos = y;
                  alive = a;
                  infected = i;
                  health = h;
             }
}cells[500];  

class Virus
{
      public:
             int xpos; //current position
             int ypos;
             bool alive;
             int xdir; //direction
             int ydir;
             int xdest; //destination
             int ydest;
             int tarCell;
             bool caught;
             
             void displayVirus()
             {
                  setcolor(RED);
                  if (alive == true)
                  {
                            if (caught == true)
                            {
                            setcolor(YELLOW);
                            }
                            
                            line(xpos - 5, ypos - 5, xpos + 5, xpos - 5);
                  }
             }
             
             void move()
             {
                  xpos += xdir;
                  ypos += ydir;
             }
             void getDirection()
             {
             
                  if(xpos < xdest)
                  xdir = virusSpeed;
                  
                  if(xpos > xdest)
                  xdir = -virusSpeed;
                  
                  if (ypos < ydest)
                  ydir = virusSpeed;
                  
                  if (ypos > ydest)
                  ydir = -virusSpeed;
               
             }
             void infect()
             {
                  if(abs(xdest - xpos) < 20 && abs(ydest - ypos) < 20 && cells[tarCell].alive == true)
                  {
                          cells[tarCell].health -= 10;
                          
                          if(cells[tarCell].health < 0)
                          {
                                                  cells[tarCell].death();
                                                  spawnVirus(tarCell);
                                                  //spawnVirus(tarCell);
                                                  
                          }
                  }     
             }
             
             void getClosestCell()
             {
                  double z = 0;
                  double closest = 10000;
                  for (int i = 0; i < 500; i++)
                  {
                    if(cells[i].alive == true)
                    {  
                       z = sqrt(pow(abs(xpos - cells[i].xpos),2) + pow(abs(ypos - cells[i].ypos),2));
                       if(z < closest)
                            {
                            xdest = cells[i].xpos;
                            ydest = cells[i].ypos;
                            tarCell = i;
                            closest = z;
                            }
                    }
                  }
             }
             void death()
             {
                  alive = false;
             }             
             void setValues(int x, int y, bool a)
             {
                           xpos = x;
                           ypos = y;
                           alive = a;
             }
             void display()
             {
                  setcolor(BLUE);
                  line(xpos - 10, ypos, xpos + 10, ypos);
             }
}virus[2000];
    
class Macrophage
{
      public:
      int xpos;
      int ypos;
      int xdir;
      int ydir;
      bool alive;
      bool alert;
      bool killer;
      
      void display()
      {
           setcolor(YELLOW);
           circle(xpos, ypos, 30);
           setfillstyle(SOLID_FILL,YELLOW);
           floodfill(xpos,ypos,YELLOW);
    
           setcolor(WHITE);
           circle(xpos,ypos,30);
           line(xpos,ypos,xpos+22,ypos+22);
           line(xpos,ypos,xpos+22,ypos-22);
           setfillstyle(SOLID_FILL,WHITE);
           floodfill(xpos+1,ypos,WHITE);
           
           if (killer == true)
           {
                      setcolor(BLUE);
                      line(xpos - 3, ypos + 10, xpos + 5, ypos + 10);
           }
      }
      void detect()
      {
           
           double distance = 0;
           for(int i = 0; i < numVirus && alert == false; i++)
           {
                   distance = sqrt(pow(abs(xpos - virus[i].xpos),2) + pow(abs(ypos - virus[i].ypos),2)); // Distance formula
                   if(distance < 500)
                   {
                               alert = true;
                   }
           }
      }
      void move()
             {
                  xpos += xdir;
                  ypos += ydir;
             }
             void getDirection()
             {
             
                  if(xpos < mousex() && abs(mousex() - xpos) > 3)
                  xdir = macroSpeed;
                  
                  if(xpos > mousex() && abs(mousex() - xpos) > 3)
                  xdir = -macroSpeed;
                  
                  if (ypos < mousey() && abs(mousey() - ypos) > 3)
                  ydir = macroSpeed;
                  
                  if (ypos > mousey() && abs(mousey() - ypos) > 3)
                  ydir = -macroSpeed;
               
             }
             void kill()
             {
                  double distance;
                  int a = 0;
                  for (int i = 0; i <= 2000; i++)
                  {
                      if (virus[i].alive == true)
                      {
                          distance = sqrt(pow(abs(xpos - virus[i].xpos),2) + pow(abs(ypos - virus[i].ypos),2));
                          if (distance < 30 && a < 15)
                          {
                          virus[i].death();
                          killer = true;
                          a++;
                          }
                      }
                  }
             }
             void setValues(int x, int y, bool a)
             {
                  xpos = x;
                  ypos = y;
                  alive = a;
                  killer = false;
             }
}macro[4];

class Killer_T
{
      public:
             int xpos,ypos;
             int xdir,ydir;
             int xdest,ydest;
             int tarCell;
             bool target;
             bool alive;
             
             void killCell()
             {
                  double distance, vDistance;
                  distance = sqrt(pow(abs(xpos - cells[tarCell].xpos),2) + pow(abs(ypos - cells[tarCell].ypos),2));
                  
                  if (distance < 5)
                  {
                               cells[tarCell].alive = false;
                               target = false;
                               for (int i = 0; i <= numVirus; i++)
                               {
                                   vDistance = sqrt(pow(abs(virus[i].xpos - cells[tarCell].xpos),2) + pow(abs(virus[i].ypos - cells[tarCell].ypos),2));
                                   if(virus[i].alive == true && virus[i].tarCell == tarCell && vDistance < 20)
                                   {
                                                     virus[i].death();
                                   }
                               }
                  }
             }
             /*void checkAlive()
             {
                  if (cells[tarCell].alive == true)
                  {
                  target = true;
                  }
                  else
                  {
                  target = false;
                  }
             }
             */
             void move()
             {
                  if (cells[tarCell].alive == true)
                  {
                  xpos += xdir;
                  ypos += ydir;
                  }
                  else
                  {
                  target = false;
                  }
             }
             void getDirection()
             {
                  if(xpos < xdest)
                  xdir = 3;
                  
                  if(xpos > xdest)
                  xdir = -3;
                  
                  if (ypos < ydest)
                  ydir = 3;
                  
                  if (ypos > ydest)
                  ydir = -3;
             }
             void display()
             {
                  line(xpos - 5, ypos - 5, xpos + 5, ypos - 5);
                  line(xpos, ypos - 5, xpos, ypos + 5);                  
             }
             
             void getCell()
             {
                  int x;
                  bool targets;
                  int y;
                  double distance;
                  getmouseclick(WM_LBUTTONDOWN,x,y);
                  
                  if(x != -1 && y != -1)
                  {    
                       for (int i = 0; i < 500; i++)
                       {
                               //targets = checkTarget(i);
                               /*if (targets == true)
                               {
                                                  doNothing();
                               }*/
                               
                               
                                                  if (cells[i].alive == true)
                                                  {        
                                                  distance = sqrt(pow(abs(x - cells[i].xpos),2) + pow(abs(y - cells[i].ypos),2));
                                                  if(distance < 5)
                                                  {                                              
                                                  tarCell = i;
                                                  xdest = cells[i].xpos;
                                                  ydest = cells[i].ypos;
                                                  target = true;
                                                  }                                            
                                                  }
                               
                           
                       }
                  }
             }
             void setValues(int x, int y, bool a)
             {
                  xpos = x;
                  ypos = y;
                  alive = a;
                  target = false;
                  xdest = 100;
                  ydest = 100;
             }
             
             
             
             
             
}TCells[5];

class AntiBody
{
      public:
      int xpos; //current position
      int ypos;
      bool alive;
      int xdir; //direction
      int ydir;
      int xdest; //destination
      int ydest;
      int tarVirus;
      
      
      
      void displayAntiBody()
      {
           setcolor(RED);
           if (alive == true)
           {
           line(xpos - 5, ypos , xpos + 5, ypos);
           }
      }
             
      void move()
      {
      xpos += xdir;
      ypos += ydir;
      }
      void getDirection()
      {
             
             if(xpos < xdest)
             xdir = 5;
                  
             if(xpos > xdest)
             xdir = -5;
                  
             if (ypos < ydest)
             ydir = 5;
                  
             if (ypos > ydest)
             ydir = -5;
               
      }
             void bind()
             {
                  if(abs(xdest - xpos) < 20 && abs(ydest - ypos) < 20 && virus[tarVirus].alive == true)
                  {
                          virus[tarVirus].caught = true;
                          alive = false;            
                          
                  }     
             }
             
             void getClosestVirus()
             {
                  double z = 0;
                  double closest = 10000;
                  for (int i = 0; i < numVirus; i++)
                  {
                    if (checkAntiBodyTargets(i) == true)
                    {
                       //
                       //
                       //
                    }
                    else
                    {
                     if(virus[i].alive == true)
                     {  
                       
                       
                       z = sqrt(pow(abs(xpos - virus[i].xpos),2) + pow(abs(ypos - virus[i].ypos),2));
                            if(z < closest)
                            {
                            xdest = virus[i].xpos;
                            ydest = virus[i].ypos;
                            tarVirus = i;
                            closest = z;
                            }
                       }
                    }
                  }
             }
             void death()
             {
                  alive = false;
             }             
             void setValues(int x, int y, bool a)
             {
                           xpos = x;
                           ypos = y;
                           alive = a;
             }
}Anti[2000];

class BCell
{
     public:
      int xpos;
      int ypos;
      bool alerted;
      bool alive;
      int counter;
      
      /*void checkAlert()
      {
           for (int i = 0; i < numHTCells; i++)
           */
      void makeAntiBodies()
      {
           if(alerted == true)
           {
                      int a = 0;
                      while (numVirus > numAntiBody && a < 7)
                      {
                      spawnAntiBody(xpos,ypos);
                      a++;
                      }
           }
      }
      void displayBCell()
      {
           setfillstyle(SOLID_FILL, GREEN);
           setcolor(GREEN);
           circle(xpos, ypos, 30);
           floodfill(xpos, ypos, GREEN);
      }
      void setValues(int x, int y, bool a)
      {
           xpos = x;
           ypos = y;
           alive = a;
           alerted = false;
      }
      
}BCells[4];

class HelperT
{
      public:
             
      int xpos;
      int ypos;
      int xdir;
      int ydir;
      int xdest;
      int ydest;
      int tarBCell;
      bool alive;
      bool goAlert;
      
      
      void displayHelperT()
      {
           setcolor(CYAN);
           circle(xpos,ypos,10);
           setfillstyle(SOLID_FILL,CYAN);
           floodfill(xpos,ypos,CYAN);
           setcolor(MAGENTA);
           circle(xpos+5,ypos+2,3);
           setfillstyle(SOLID_FILL,MAGENTA);
           floodfill(xpos+5,ypos+2,MAGENTA);
                                           
           if (goAlert == true)
           {
                       setcolor(BLUE);
                       line(xpos - 5, ypos,xpos + 5, ypos);
           }
      }
      void move()
      {
      xpos += xdir;
      ypos += ydir;
      }
      void getDirection()
      {
             
             if(xpos < xdest)
             xdir = 8;
                  
             if(xpos > xdest)
             xdir = -8;
                  
             if (ypos < ydest)
             ydir = 8;
                  
             if (ypos > ydest)
             ydir = -8;
               
      }
      void checkAlert()
      {
           if (macro[0].killer == true && goAlert == false)
           {
                               double distance;
                               distance = sqrt(pow(abs(xpos - macro[0].xpos),2) + pow(abs(ypos - macro[0].ypos),2));
                               if (distance < 10)
                               goAlert = true;
                               
                               xdest = macro[0].xpos;
                               ydest = macro[0].ypos;
                               
           }
           else if (goAlert == true)
           {
                for (int i = 0; i < 4; i++)
                {
                    if (BCells[i].alerted == false && BCells[i].alive == true)
                    {
                                          xdest = BCells[i].xpos;
                                          ydest = BCells[i].ypos;
                                          tarBCell = i;
                    }
                }
           }
           else
           {
               xdest = 300;
               ydest = 300;
           }
      }
      
      void makeAlert()
      {
           double distance;
           distance = sqrt(pow(abs(xpos - BCells[tarBCell].xpos),2) + pow(abs(ypos - BCells[tarBCell].ypos),2));
           
           if (distance < 20)
           {
                        BCells[tarBCell].alerted = true;
           }
      }
      void setValues(int x, int y, bool a)
      {
          xpos = x;
          ypos = y;
          alive = a;
          goAlert = false;
      } 
}HTCells[10];
      
      
      
      

bool win = false;
bool lose = false;             


int main()
{
    
    
    int xpos = 0;
    int ypos = 0;
    
    initwindow(800,800);   
    mainMenu();
    //______________________________________________________________________
    
    
    runTheGame();
    
    //waitForLeftMouseClick(); 
    
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
void displayCells()
{
    for (int i = 0; i < 500; i++)
    {
                  cells[i].getInfected();
                  cells[i].display();
    }
} 
void displayMacrophage()
{
     for (int i = 0; i < 1; i++)
     {
         if (macro[i].alive == true)
         {
                            macro[i].display();
                            macro[i].getDirection();
                            macro[i].move();
                            macro[i].kill();
         }
     }
}
void clearScreen()
{
     //cleardevice();
     
     setcolor(WHITE);
     setfillstyle(SOLID_FILL, WHITE);
     floodfill(1,1, BROWN);
     
     
}

void displayViruses()
{
     for (int i = 0; i < 1300; i++)
     {
         if (virus[i].alive == true)
         {
                            virus[i].getClosestCell();
                            virus[i].getDirection();
                            if (virus[i].caught == false)
                            {
                            virus[i].move();
                            virus[i].infect();
                            }
                            virus[i].display();
         }
     }
}

void runTheGame()
{
    
     //for (int i = 0; i < 4; i++)
     while(wave < 5)
     {
         setGame();
         win = false;
         lose = false;
         
         while(lose == false && win == false)
         {
                   displayMacrophage();
                   displayCells();
                   displayViruses();
                   displayTCells();
                   displayAntiBodies();
                   displayBCells();
                   displayHTCells();
                   checkWin();
                   clearScreen();
                   delay(0);
                      
    
         }
         if (lose == true)
         {
         outtextxy(100,100, "YOU LOSE");
         break;
         }
         question(wave);
         cleardevice();
         outtextxy(100,100, "NICE WORK, CLICK WHEN READY FOR NEXT WAVE...");
         
         waitForLeftMouseClick();
         wave++;
     }
     
     if (win == true)
     outtextxy(100,100, "YOU WIN");
     
     
     
     waitForLeftMouseClick(); 
}
void displayTCells()
{
     for (int i = 0; i <= numTCells; i++)
     {
         TCells[i].getCell();
         //TCells[i].checkAlive();
         TCells[i].getDirection();
         TCells[i].move();
         TCells[i].killCell();
         TCells[i].display();
     }
}

void spawnVirus(int tarCell)
{
     virus[numVirus].alive = true;
     virus[numVirus].xpos = cells[tarCell].xpos + 5;
     virus[numVirus].ypos = cells[tarCell].ypos - 20; 
     virus[numVirus].caught = false;
     numVirus++;
}
void spawnTCell()
{
     numTCells++;
     TCells[numTCells].alive = true;
     TCells[numTCells].xpos = 100;
     TCells[numTCells].ypos = 400;
     TCells[numTCells].target = false;
     
}
void doNothing()
{
}
void checkWin()
{
     win = true;
     for (int i = 0; i <= 1300; i++)
     {
         if (virus[i].alive == true)
         {
         win = false;
         }
     }
     lose = true;
     for (int z = 0; z < 500; z++)
     {
         if (cells[z].alive == true)
         {
                 lose = false;
         }
     }           
         
         
         
}

void mainMenu()
{      
       bool exitMenu = false;
       
       while(exitMenu != true)
       {
       cleardevice();
       settextstyle(10,0,2);
       outtextxy(260,100, "The Immune System Game");
       outtextxy(300,300, "START THE GAME");
       outtextxy(310,350, "Instructions");
       outtextxy(350,400, "Exit");
       
       waitForLeftMouseClick();
       
       if (mousex() > 300 && mousex() < 400)
       {
                    if (mousey() > 290 && mousey() < 350)
                    {
                    exitMenu = true;
                    selectDifficulty();
                    }
                    else if(mousey() > 350 && mousey() < 400)
                    {
                         cleardevice();
                         //outtextxy(300,300, "INSTRUCTIONS");
                         instructions();
                         waitForLeftMouseClick();
                    }
                    else if(mousey() > 400 && mousey() < 450)
                    {
                         exit(1);
                    }
                    else
                    {
                    }
       }
                    
       } 
} 

void spawnAntiBody(int x, int y)
{
     Anti[numAntiBody].alive = true;
     Anti[numAntiBody].xpos = x;
     Anti[numAntiBody].ypos = y;
     numAntiBody++;
} 
bool checkAntiBodyTargets(int a)
{
     for (int i = 0; i < numVirus; i++)
     {
         if (Anti[i].tarVirus == a)
         {
               return true;
         }
     }
     return false;
}

void displayAntiBodies()
{
     for (int i = 0; i <= numAntiBody; i++)
     {
              if (Anti[i].alive == true)
              {
                        Anti[i].getClosestVirus();
                        Anti[i].getDirection();
                        Anti[i].move();
                        Anti[i].bind();
                        Anti[i].displayAntiBody();
              }
     }
}

void displayBCells()
{
     for (int i = 0; i < 4; i++)
     {
         if (BCells[i].alive == true)
         {
                             BCells[i].displayBCell();
                             BCells[i].makeAntiBodies();
         }
     }
}
void selectDifficulty()
{
     cleardevice();
     outtextxy(350,300, "EASY");
     outtextxy(350,350, "MEDIUM");
     outtextxy(350,400, "HARD");
     outtextxy(350,450, "VERY VERY HARD");
     
     bool exit = false;
     while (exit == false)
     {
     waitForLeftMouseClick();
      if (mousex() > 300 && mousex() < 400)
      {
                  if (mousey() > 300 && mousey() < 350)
                  {
                       initialHealth = 1500;
                       virusSpeed = 3;
                       macroSpeed = 5;
                       exit = true;
                  }
                  else if(mousey() > 350 && mousey() < 400)
                  {
                       initialHealth = 800;
                       virusSpeed = 5;
                       macroSpeed = 4;
                       exit = true;
                  }
                  else if (mousey() > 400 && mousey() < 450)
                  {
                       initialHealth = 500;
                       virusSpeed = 5;
                       macroSpeed = 3;
                       exit = true;
                  }
                  else if (mousey() > 450 && mousey() < 500)
                  {
                      initialHealth = 250;
                      virusSpeed = 7;
                      macroSpeed = 3;
                      exit = true;
                  }
      }
     }                        
}
void displayHTCells()
{
     for (int i = 0; i <= numHTCells; i++)
     {
         if(HTCells[i].alive == true)
         {
                             HTCells[i].checkAlert();
                             HTCells[i].getDirection();
                             HTCells[i].move();
                             HTCells[i].makeAlert();
                             HTCells[i].displayHelperT();
         }
     }
}

void setGame()
{
    srand(time(0));
    int xpos = 0;
    int ypos = 0;
    
    int v = rand() % 1000 + 250;
    
    int z = rand() % 1000 + 250;
    cout << v << endl;
    cout << z << endl;
    
    for (int i = 0; i < 2000; i++)
    {
        virus[i].setValues(0,0, false);
        Anti[i].setValues(0,0, false);
    }
    
    
    //Sets Cell Values
    // ______________________________________________________________________
    for (int i = 0; i < 500; i++)
    {
        cells[i].setValues(0, 0, true, false, initialHealth);
    }
    //_______________________________________________________________________
    
    // Sets Up Random Cells
    //______________________________________________________________________
    for(int j = 0; j < 500; j++)
    {
            
            xpos = rand() % 500 + 250;
            ypos = rand() % 500 + 250;
            
            redoPositions:       
            for (int z = 0; z <= j; z++)
            {
                if(abs(xpos - cells[z].xpos) < 15 && abs(ypos - cells[z].ypos) < 15)
                {
                xpos = rand() % 500 + 250;
                ypos = rand() % 500 + 250;
                goto redoPositions;
                }
            }
            cells[j].xpos = xpos;
            cells[j].ypos = ypos;
            cout << xpos << " " << ypos << endl;
            
    }
    
    for (int i = 0; i < wave + 1; i++)
    {
    spawnVirus(i);
    }
    
    
    BCells[0].setValues(100,200,true);
    BCells[1].setValues(100,600,true);
    BCells[2].setValues(500,200,true);
    BCells[3].setValues(700,100,true);
    
    HTCells[0].setValues(100,100,true);
    
    TCells[0].setValues(800,800, true);
    numTCells++;
    numAntiBody = 0;
    numVirus = 0;
    numTCells = 0;
    numHTCells = 0;
    //spawnAntiBody();
    
    macro[0].setValues(100, 500, true);
}  

void instructions()
{
     settextstyle(0,0,2);
    setcolor(WHITE);
    outtextxy(20,100,"Instructions: ");
    outtextxy(20,125, "Today you will be playing the immune system ");
    outtextxy(20,150, "video game. Before you begin there are a few ");
    outtextxy(20,175, "things you should know: ");
    
    settextstyle(0,0,1);
    outtextxy(100,300,"THIS IS A MACROPHAGE:");
    int xpos = 200;
    int ypos = 350;
    setcolor(YELLOW);
    circle(xpos, ypos, 30);
    setfillstyle(SOLID_FILL,YELLOW);
    floodfill(xpos,ypos,YELLOW);
    setcolor(WHITE);
    circle(xpos,ypos,30);
    line(xpos,ypos,xpos+22,ypos+22);
    line(xpos,ypos,xpos+22,ypos-22);
    setfillstyle(SOLID_FILL,WHITE);
    floodfill(xpos+1,ypos,WHITE);
           
    outtextxy(100,400,"THIS IS A CELL:");
    setcolor(BLUE);
    circle(200,430,10);
    
    setcolor(WHITE);
    outtextxy(100,450,"THIS IS A VIRUS:");
    setcolor(BLUE);
    line(200 - 10, 480 - 5, 200 + 10, 480 - 5);
    
    setcolor(WHITE);
    outtextxy(100,500,"THIS IS A KILLER T CELL:");
    xpos = 200;
    ypos = 520;
    setcolor(BLUE);
    line(xpos - 5, ypos - 5, xpos + 5, ypos - 5);
    line(xpos, ypos - 5, xpos, ypos + 5);   
    
    setcolor(WHITE);
    outtextxy(100,550, "THIS IS AN ANTIBODY");
    xpos = 200;
    ypos = 570;
    setcolor(RED);
    line(xpos - 5, ypos , xpos + 5, ypos);
    
    setcolor(WHITE);
    outtextxy(100,600, "THIS IS A B CELL");
    xpos = 200;
    ypos = 650;
    setfillstyle(SOLID_FILL, GREEN);
    setcolor(GREEN);
    circle(xpos, ypos, 30);
    floodfill(xpos, ypos, GREEN);
    
    setcolor(WHITE);
    outtextxy(100,700, "THIS IS A HELPER T CELL");
    xpos = 200;
    ypos = 730;
    setcolor(CYAN);
    circle(xpos,ypos,10);
    setfillstyle(SOLID_FILL,CYAN);
    floodfill(xpos,ypos,CYAN);
    setcolor(MAGENTA);
    circle(xpos+5,ypos+2,3);
    setfillstyle(SOLID_FILL,MAGENTA);
    floodfill(xpos+5,ypos+2,MAGENTA);
    
    setcolor(WHITE);
    outtextxy(450,450, "CLICK FOR NEXT PAGE");
    
    waitForLeftMouseClick();
    cleardevice();
    
    outtextxy(20,100,"Instructions (Part 2): ");
    outtextxy(20,125,"You're in control of the macrophage. The macrophage eats viruses and displays a part of the virus");  
    outtextxy(20,150,"on itself. From there Helper T Cells take a part of the virus and gives the virus to the B Cell"); 
    outtextxy(20,175,"to begin making antibodies. Antibodies will then immobolize the virus thus neutralizing the threat");
    outtextxy(20,200,"of the virus and making it a sitting duck for the macrophage. In addition Killer T cells may be");
    outtextxy(20,225,"activated in order to preemptively kill infected cells in order to stop the spread of the virus.");
    
    outtextxy(20,300,"Game Controls: ");
    outtextxy(20,325,"To control the macrophage all you need to do is move the mouse to the location where you want the");
    outtextxy(20,350,"macrophage to be. In order to control the T Cells one must click on the cells which they wish to");
    outtextxy(20,375,"kill. Killer T Cells are more effective in the beginning of the game, use this wisely");
    
    settextstyle(0,0,2);
    outtextxy(100,450, "Click anywhere to continue, HAVE FUN!");
    
    waitForLeftMouseClick();
}

void question(int wave)
{
     switch(wave)
     {
                 case 1:
                      question1();
                      break;
                 case 2:
                      question2();
                      break;
                 case 3:
                      question3();
                      break;
                 case 4: 
                      question4();
                      break;
                 case 5:
                      question5();
                      break;
     }
}
void question1()
{
     cleardevice();
           settextstyle(0,0,2);
           
           outtextxy(5,75, "What is a foreign molecule that is specifically");
           outtextxy(5,100, "recognized by lymphocytes and can elicit responses?");
           outtextxy(5,125, "from them?");
           outtextxy(100,150, "a) Antigen");
           outtextxy(100,175, "b) Antibodies");
           outtextxy(100,200, "c) Epitope");
           outtextxy(100,225, "d) Histamine");
           bool exitScreen = false;
           int correctAnswer = 1;
           while (exitScreen == false)
           {
           waitForLeftMouseClick();
           if (mousex() > 100 && mousex() < 300)
           {
                        if (mousey() > 150 && mousey() < 175)
                        {
                                     if (correctAnswer == 1)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 175 && mousey() < 200)
                        {
                                     if (correctAnswer == 2)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 200 && mousey() < 225)
                        {
                                     if (correctAnswer == 3)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 225 && mousey() < 250)
                        {
                                     if (correctAnswer == 4)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
           }
           }  
           cleardevice();     
}           
void question2()
{
     cleardevice();
     outtextxy(5,75, "What are killer T cells also called?");
     //outtextxy(5,100, "recognized by lymphocytes and can elicit responses");
     //outtextxy(5,125, "from them?");
     outtextxy(100,150, "a) Helper T Cells");
     outtextxy(100,175, "b) B Cells");
     outtextxy(100,200, "c) Interleukins");
     outtextxy(100,225, "d) Cytotoxic");
     bool exitScreen = false;
     int correctAnswer = 4;
     while (exitScreen == false)
           {
           waitForLeftMouseClick();
           if (mousex() > 100 && mousex() < 300)
           {
                        if (mousey() > 150 && mousey() < 175)
                        {
                                     if (correctAnswer == 1)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 175 && mousey() < 200)
                        {
                                     if (correctAnswer == 2)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 200 && mousey() < 225)
                        {
                                     if (correctAnswer == 3)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 225 && mousey() < 250)
                        {
                                     if (correctAnswer == 4)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
           }
           }  
           cleardevice();                
}
void question3()
{
     cleardevice();
           settextstyle(0,0,2);
           
           outtextxy(5,75, "What is the cell that absorbs pathogens that have");
           outtextxy(5,100, "been marked by antibodies?");
           //outtextxy(5,125, "from them?");
           outtextxy(100,150, "a) Macrophage");
           outtextxy(100,175, "b) Killer T Cells");
           outtextxy(100,200, "c) CD4 Protein");
           outtextxy(100,225, "d) Memory Cells");
           bool exitScreen = false;
           int correctAnswer = 1;
           while (exitScreen == false)
           {
           waitForLeftMouseClick();
           if (mousex() > 100 && mousex() < 300)
           {
                        if (mousey() > 150 && mousey() < 175)
                        {
                                     if (correctAnswer == 1)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 175 && mousey() < 200)
                        {
                                     if (correctAnswer == 2)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 200 && mousey() < 225)
                        {
                                     if (correctAnswer == 3)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 225 && mousey() < 250)
                        {
                                     if (correctAnswer == 4)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
           }
           }  
           cleardevice();                
}
void question4()
{
     cleardevice();
           settextstyle(0,0,2);
           
           outtextxy(5,75, "This is a weakened or dead pathogen");
           outtextxy(5,100, "that is used to build up immunity?");
           //outtextxy(5,125, "from them?");
           outtextxy(100,150, "a) Lymphocyte ");
           outtextxy(100,175, "b) Immunogobulin");
           outtextxy(100,200, "c) Vaccine");
           outtextxy(100,225, "d) Exon Shuffling");
           bool exitScreen = false;
           int correctAnswer = 3;
           while (exitScreen == false)
           {
           waitForLeftMouseClick();
           if (mousex() > 100 && mousex() < 300)
           {
                        if (mousey() > 150 && mousey() < 175)
                        {
                                     if (correctAnswer == 1)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 175 && mousey() < 200)
                        {
                                     if (correctAnswer == 2)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 200 && mousey() < 225)
                        {
                                     if (correctAnswer == 3)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 225 && mousey() < 250)
                        {
                                     if (correctAnswer == 4)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
           }
           }  
           cleardevice();                
}
void question5()
{
     cleardevice();
           settextstyle(0,0,2);
           
           outtextxy(5,75, "Autoimmune diseases are an inability");
           outtextxy(5,100, "for which cell to do its job properly?");
           //outtextxy(5,125, "from them?");
           outtextxy(100,150, "a) B Cells ");
           outtextxy(100,175, "b) Antibodies");
           outtextxy(100,200, "c) Killer T Cells");
           outtextxy(100,225, "d) Helper T Cells");
           bool exitScreen = false;
           int correctAnswer = 3;
           while (exitScreen == false)
           {
           waitForLeftMouseClick();
           if (mousex() > 100 && mousex() < 300)
           {
                        if (mousey() > 150 && mousey() < 175)
                        {
                                     if (correctAnswer == 1)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 175 && mousey() < 200)
                        {
                                     if (correctAnswer == 2)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 200 && mousey() < 225)
                        {
                                     if (correctAnswer == 3)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
                        else if(mousey() > 225 && mousey() < 250)
                        {
                                     if (correctAnswer == 4)
                                     {
                                                       correct++;
                                                       exitScreen = true;
                                     }
                        }
           }
           }  
           cleardevice();                
}
/*bool checkTarget(int z)
{
     for (int i = 0; i <= numTCells; i++)
     {
         if(TCells[i].tarCell == z)
         return true;
     }
     return false;
}
*/
