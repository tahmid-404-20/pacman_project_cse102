/*
	Developed by: Nafis Tahmid, CSE BUET'19
	Last Modified: 19 December, 2020
*/
# include "iGraphics.h"
# include "gl.h"
#include<math.h>
#include<string.h>
#include<Windows.h>
#include<mmsystem.h>

#define SCREENWIDTH 800
#define SCREENHEIGHT 504
#define blockdim 24
#define mapHeight 21
#define mapWidth 19
#define nGhost 4
#define MAXSPEED 6
#define MAXSAVE 7

void setpacman(void);
void setghost(void);
void turnaround(int i);
void frightened(void);
void revive(int i);
int getghostdir(int i);
void moveghost(void);
void halloffame(void);
void seehalloffame(void);
void savegame(void);
void loadlog(void);
void loadgame(int i);
void movepacman(void);
void moveghost(void);
void changemode(void);
void loadreset(void);
void changedir(int up,int down,int right, int left, int i);
void deletegame(int);
void restart(void);

int menu,playgame;

//mode: scatter, chase, frightened, eaten
int mode,showtracking,score,foodcount,ghostspeed,ghosteaten,ghoststate;
int win,speedchange;
int reset;
int sec, minute,paused;
int deathstate,pacmandies;
int hallcount;
char scoredisplay[10],foodisplay[5];
char minutedisplay[4],secondisplay[4];
char playername[80] = {0};
int len,savelen,loadlen,dellen;//name length
char savedname[10][60];
char nowsave[60];
int savednum;
char loadnum[5],delnum[5];
int flag,loadwarning,delwarning;
int showeat,showeatx,showeaty,callcount;

struct _HALL{
    char name[80];
    int score;
}hall[10];

int brickmap[mapHeight][mapWidth] =
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1,
		1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1,
		1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1,
		1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1,
		1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1,
		1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1,
		1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1,
		1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1,
		1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

int food[mapHeight][mapWidth] =
{
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
		0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,
		0 ,1 ,0 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,0 ,1 ,0 ,
		0 ,2 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,2 ,0 ,
		0 ,1 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,1 ,0 ,
		0 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,2 ,0 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,2 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,
		0 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,0 ,2 ,0 ,
		0 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,0 ,
		0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,
		0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,
		0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0
};

int foodbackup[mapHeight][mapWidth] =
{
        0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,
		0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,
		0 ,1 ,0 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,0 ,1 ,0 ,
		0 ,2 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,2 ,0 ,
		0 ,1 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,1 ,0 ,
		0 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,2 ,0 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,2 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,
		0 ,2 ,0 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,1 ,0 ,0 ,2 ,0 ,
		0 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,0 ,
		0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,1 ,0 ,0 ,
		0 ,1 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,0 ,1 ,1 ,1 ,1 ,0 ,
		0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,
		0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,
		0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0
};

enum spritedir{up,down,right,left}pacdir;

char pacsprite[4][4][50] =
{
    "Images\\Pacman\\up1.bmp","Images\\Pacman\\up2.bmp","Images\\Pacman\\up3.bmp","Images\\Pacman\\up4.bmp",
    "Images\\Pacman\\down1.bmp","Images\\Pacman\\down2.bmp","Images\\Pacman\\down3.bmp","Images\\Pacman\\down4.bmp",
    "Images\\Pacman\\right1.bmp","Images\\Pacman\\right2.bmp","Images\\Pacman\\right3.bmp","Images\\Pacman\\right4.bmp",
    "Images\\Pacman\\left1.bmp","Images\\Pacman\\left2.bmp","Images\\Pacman\\left3.bmp","Images\\Pacman\\left4.bmp"
};

char menuop[6][50]=
{
    "Images\\MainMenu\\NewGame.bmp","Images\\MainMenu\\LoadGame.bmp", "Images\\MainMenu\\Instructions.bmp",
    "Images\\MainMenu\\HighScores.bmp","Images\\MainMenu\\Credits.bmp","Images\\MainMenu\\Exit.bmp"
};

char menudisplay[5][50] =
{
    "Images\\MainMenu\\MenuScreen.bmp","Images\\MainMenu\\LoadGamePage.bmp",
    "Images\\MainMenu\\InstructionsPage.bmp","Images\\MainMenu\\HighScoresPage.bmp",
    "Images\\MainMenu\\CreditsPage.bmp"
};

char ghostsprite[4][4][2][30] =
{
    "Images\\Ghost\\blinky01.bmp","Images\\Ghost\\blinky02.bmp",
    "Images\\Ghost\\blinky05.bmp","Images\\Ghost\\blinky06.bmp",
    "Images\\Ghost\\blinky07.bmp","Images\\Ghost\\blinky08.bmp",
    "Images\\Ghost\\blinky03.bmp","Images\\Ghost\\blinky04.bmp",
    "Images\\Ghost\\pinky01.bmp","Images\\Ghost\\pinky02.bmp",
    "Images\\Ghost\\pinky05.bmp","Images\\Ghost\\pinky06.bmp",
    "Images\\Ghost\\pinky07.bmp","Images\\Ghost\\pinky08.bmp",
    "Images\\Ghost\\pinky03.bmp","Images\\Ghost\\pinky04.bmp",
    "Images\\Ghost\\inky01.bmp","Images\\Ghost\\inky02.bmp",
    "Images\\Ghost\\inky05.bmp","Images\\Ghost\\inky06.bmp",
    "Images\\Ghost\\inky07.bmp","Images\\Ghost\\inky08.bmp",
    "Images\\Ghost\\inky03.bmp","Images\\Ghost\\inky04.bmp",
    "Images\\Ghost\\clyde01.bmp","Images\\Ghost\\clyde02.bmp",
    "Images\\Ghost\\clyde05.bmp","Images\\Ghost\\clyde06.bmp",
    "Images\\Ghost\\clyde07.bmp","Images\\Ghost\\clyde08.bmp",
    "Images\\Ghost\\clyde03.bmp","Images\\Ghost\\clyde04.bmp"
};

typedef struct _MOVE{
    unsigned int up,down,right,left;
}MOVE;

struct _PACMAN{

    int x,y,dx,dy;
    int speed;
    int tilex,tiley;
    int xoffset,yoffset;
    int spritestate;
    MOVE movepac;
    int life;
}pacman;

struct _GHOST{

    int x,y,dx,dy;
    int speed;
    int tilex,tiley;
    int targetx,targety;
    int xoffset,yoffset;
    MOVE moveghost;
    int eaten;

}ghost[nGhost];


/*
	function iDraw() is called again and again by the system.
*/
void iDraw()
{
	//place your drawing codes here
    int i,j;
    iClear();

    if(menu == 0)
    {
        iShowBMP(0,0,"Images\\MainMenu\\MenuScreen.bmp");
        for(i=0;i<6;i++)
        {
            iShowBMP2((mapWidth/2-2)*blockdim,(mapHeight/2+5-3*i)*blockdim,menuop[i],0x000000);
        }

    }

    if(menu == 1)
    {
        iSetColor(255,0,0);
        for(i=0;i<mapHeight;i++)
        {
            for(j=0;j<mapWidth;j++)
            {
                if(brickmap[i][j] ==1)
                {
                    iShowBMP(j*blockdim,(mapHeight-1-i)*blockdim,"Images\\brick2.bmp");
                }

                if(food[i][j]==1)
                {
                    iFilledCircle(j*blockdim+blockdim/2+1,(mapHeight-1-i)*blockdim+blockdim/2+1,2);
                }

                else if(food[i][j]==2)
                {
                    iFilledCircle(j*blockdim+blockdim/2+1,(mapHeight-1-i)*blockdim+blockdim/2+1,6,100);
                }

            }
        }

//DRAW Pac Life
        iText((mapWidth+2)*blockdim,(mapHeight-8)*blockdim-20,"LIFE: ",GLUT_BITMAP_TIMES_ROMAN_24);
        for(i=0;i<pacman.life;i++)
            iShowBMP2((mapWidth+2+2*i)*blockdim,(mapHeight/2+1)*blockdim,"Images\\Pacman\\right2.bmp",0x070707);

//DrawPacman
        iShowBMP2(pacman.x,pacman.y,pacsprite[pacdir][pacman.spritestate],0x070707);

//Show eating score
        if(showeat == 1)
        {
            if(ghosteaten == 1)
                iShowBMP2(showeatx*blockdim,(mapHeight-showeaty-1)*blockdim,"Images\\Ghost\\eat1.bmp",0x070707);

            else if(ghosteaten == 2)
                iShowBMP2(showeatx*blockdim,(mapHeight-showeaty-1)*blockdim,"Images\\Ghost\\eat2.bmp",0x070707);

            else if(ghosteaten == 3)
                iShowBMP2(showeatx*blockdim,(mapHeight-showeaty-1)*blockdim,"Images\\Ghost\\eat3.bmp",0x070707);

            else if(ghosteaten == 4)
                iShowBMP2(showeatx*blockdim,(mapHeight-showeaty-1)*blockdim,"Images\\Ghost\\eat4.bmp",0x070707);

            else if(ghosteaten == 5)
                iShowBMP2(showeatx*blockdim,(mapHeight-showeaty-1)*blockdim,"Images\\Ghost\\eat5.bmp",0x070707);

            else if(ghosteaten >= 6)
                iShowBMP2(showeatx*blockdim,(mapHeight-showeaty-1)*blockdim,"Images\\Ghost\\eat6.bmp",0x070707);
        }

//Draw Ghost
        for(i=0;i<nGhost;i++)
        {
            if(ghost[i].eaten == 1)
            {
                int d;
                d = getghostdir(i);

                if(d==0)
                    iShowBMP2(ghost[i].x,ghost[i].y,"Images\\Ghost\\eaten01.bmp",0x070707);
                else if(d==1)
                    iShowBMP2(ghost[i].x,ghost[i].y,"Images\\Ghost\\eaten03.bmp",0x070707);
                else if(d==2)
                    iShowBMP2(ghost[i].x,ghost[i].y,"Images\\Ghost\\eaten04.bmp",0x070707);
                else
                    iShowBMP2(ghost[i].x,ghost[i].y,"Images\\Ghost\\eaten02.bmp",0x070707);
            }

            else
            {
                if(mode == 2 || mode == 3)
                {
                    if(ghoststate == 0)
                        iShowBMP2(ghost[i].x,ghost[i].y,"Images\\Ghost\\frightened01.bmp",0x070707);

                    else if(ghoststate == 1)
                        iShowBMP2(ghost[i].x,ghost[i].y,"Images\\Ghost\\frightened02.bmp",0x070707);
                }


                else
                {
                    if(ghost[0].eaten == 0)
                        iShowBMP2(ghost[0].x,ghost[0].y,ghostsprite[0][getghostdir(0)][ghoststate],0x070707);

                    if(ghost[1].eaten == 0)
                        iShowBMP2(ghost[1].x,ghost[1].y,ghostsprite[1][getghostdir(1)][ghoststate],0x070707);

                    if(ghost[2].eaten == 0)
                        iShowBMP2(ghost[2].x,ghost[2].y,ghostsprite[2][getghostdir(2)][ghoststate],0x070707);

                    if(ghost[3].eaten == 0)
                        iShowBMP2(ghost[3].x,ghost[3].y,ghostsprite[3][getghostdir(3)][ghoststate],0x070707);
                }

            }

            if(showtracking)
                iLine(ghost[i].x,ghost[i].y,ghost[i].targetx*blockdim,(mapHeight-ghost[i].targety-1)*blockdim);
        }

        //clyde tracking
        if(showtracking)
            iCircle(pacman.x+blockdim/2,pacman.y+blockdim/2,8*blockdim);

        iSetColor(255,255,255);
        iText((mapWidth+2)*blockdim,(mapHeight-2)*blockdim+2,"POINTS:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText((mapWidth+2)*blockdim,(mapHeight-3)*blockdim,scoredisplay,GLUT_BITMAP_TIMES_ROMAN_24);

        iText((mapWidth+2)*blockdim,(mapHeight-5)*blockdim+2,"REMAINING FOOD: ",GLUT_BITMAP_TIMES_ROMAN_24);
        iText((mapWidth+2)*blockdim,(mapHeight-6)*blockdim,foodisplay,GLUT_BITMAP_TIMES_ROMAN_24);

        iText((mapWidth+2)*blockdim,(mapHeight/2-1)*blockdim+2,"ELAPSED TIME:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText((mapWidth+3)*blockdim,(mapHeight/2-2)*blockdim+2,minutedisplay,GLUT_BITMAP_TIMES_ROMAN_24);
        iText((mapWidth+4)*blockdim,(mapHeight/2-2)*blockdim+2,":",GLUT_BITMAP_TIMES_ROMAN_24);
        iText((mapWidth+4)*blockdim+5,(mapHeight/2-2)*blockdim+2,secondisplay,GLUT_BITMAP_TIMES_ROMAN_24);

        iText((mapWidth+2)*blockdim,(mapHeight/2-6)*blockdim+2,"Press T/t to toggle tracing visualizer",GLUT_BITMAP_HELVETICA_12);
        iText((mapWidth+2)*blockdim,(mapHeight/2-7)*blockdim+2,"Press P/p to Pause. R/r to resume",GLUT_BITMAP_HELVETICA_12);
        iText((mapWidth+2)*blockdim,(mapHeight/2-8)*blockdim+2,"Press S/s to SAVE GAME",GLUT_BITMAP_HELVETICA_12);
        iText((mapWidth+2)*blockdim,(mapHeight/2-9)*blockdim+2,"Press END to exit without saving",GLUT_BITMAP_HELVETICA_12);

        if(paused)
        {
            iText((mapWidth/2-3)*blockdim,(mapHeight/2)*blockdim,"PAUSED. R/r TO RESUME",GLUT_BITMAP_TIMES_ROMAN_24);
        }

        if(reset)
        {
            iText((mapWidth/2-3)*blockdim,(mapHeight/2)*blockdim,"TAP SPACE TO START",GLUT_BITMAP_TIMES_ROMAN_24);
        }

        if(win)
        {
            iText((mapWidth/2)*blockdim,(mapHeight/2)*blockdim,"CONGRATULATIONS!!! YOU WIN!!",GLUT_BITMAP_TIMES_ROMAN_24);
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);
            menu = 6;
        }
        if(pacman.life == 0)
        {
            iText((mapWidth/2)*blockdim,(mapHeight/2)*blockdim,"YOU HAVE LOST :(\nTap Space To Close",GLUT_BITMAP_TIMES_ROMAN_24);
            iPauseTimer(0);
            iPauseTimer(1);
            iPauseTimer(2);
            iPauseTimer(3);
            menu = 6;
        }

    }

//Load Game
    if(menu == 2)
    {
        int i;
        char num[3];

        iSetColor(255,0,0);
        iText((mapWidth/2+2)*blockdim,(mapHeight/2+10)*blockdim,"LOAD GAME",GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(78,121,255);
        for(i=0;i<savednum;i++)
        {
            sprintf(num,"%d",i+1);
            iText((mapWidth/2-1)*blockdim,(mapHeight/2+5-i)*blockdim,num,GLUT_BITMAP_HELVETICA_18);
            iText((mapWidth/2)*blockdim,(mapHeight/2+5-i)*blockdim,savedname[i],GLUT_BITMAP_HELVETICA_18);
        }


        iSetColor(255,255,255);
        iText(11*blockdim,3*blockdim,"Enter the file number to load the file",GLUT_BITMAP_HELVETICA_12);
        iRectangle(11*blockdim,4*blockdim,10*blockdim,2*blockdim);
        iSetColor(255,0,0);
        iText(11*blockdim+5,4*blockdim+15,loadnum,GLUT_BITMAP_TIMES_ROMAN_24);

        if(loadwarning == 1)
            iText(11*blockdim,7*blockdim+15,"Enter a valid number",GLUT_BITMAP_HELVETICA_12);

        iShowBMP2(blockdim,19*blockdim,"Images\\MainMenu\\Back.bmp",0x000000);
        iSetColor(255,255,255);
        iRectangle(blockdim,19*blockdim,120,28);
    }

//Instruction
    if(menu == 3)
    {
        iShowBMP(0,0,"Images\\MainMenu\\InstructionsPage.bmp");

        iShowBMP2(27*blockdim,19*blockdim,"Images\\MainMenu\\Back.bmp",0x000000);
        iSetColor(255,255,255);
        iRectangle(27*blockdim,19*blockdim,120,28);
    }

//High Scores
    if(menu == 4)
    {

        int i;
        char dishighscore[10], num[3];
        iShowBMP(0,0,"Images\\MainMenu\\HighScoresPage.bmp");

        iSetColor(255,0,0);
        iText((mapWidth/2+2)*blockdim,(mapHeight/2+10)*blockdim,"HALL OF FAME",GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(78,255,121);
        iText((mapWidth/2-2)*blockdim,(mapHeight/2+7)*blockdim,"NAME",GLUT_BITMAP_TIMES_ROMAN_24);
        iText((mapWidth/2+8)*blockdim,(mapHeight/2+7)*blockdim,"SCORE",GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(78,121,255);
        for(i=0;i<hallcount;i++)
        {
            sprintf(num,"%d",i+1);
            iText((mapWidth/2-3)*blockdim,(mapHeight/2+5-i)*blockdim,num,GLUT_BITMAP_HELVETICA_18);
            iText((mapWidth/2-2)*blockdim,(mapHeight/2+5-i)*blockdim,hall[i].name,GLUT_BITMAP_HELVETICA_18);
            sprintf(dishighscore,"%d",hall[i].score);
            iText((mapWidth/2+9)*blockdim,(mapHeight/2+5-i)*blockdim,dishighscore,GLUT_BITMAP_HELVETICA_18);
        }

        iShowBMP2(blockdim,19*blockdim,"Images\\MainMenu\\Back.bmp",0x000000);
        iSetColor(255,255,255);
        iRectangle(blockdim,19*blockdim,120,28);
    }

//Credits
    if(menu == 5)
    {
        iShowBMP(0,0,"Images\\MainMenu\\CreditsPage.bmp");

        iShowBMP2(blockdim,19*blockdim,"Images\\MainMenu\\Back.bmp",0x000000);
        iSetColor(255,255,255);
        iRectangle(blockdim,19*blockdim,120,28);
    }

//SavePlayerScore
    if(menu == 6)
    {
        iShowBMP(0,0,"Images\\MainMenu\\LoadGamePage.bmp");
        iSetColor(255,0,0);
        if(win == 1)
            iText((mapWidth/2+2)*blockdim,(mapHeight/2+9)*blockdim,"YOU WIN. SAVE SCORE",GLUT_BITMAP_TIMES_ROMAN_24);
        else if(pacman.life == 0)
            iText((mapWidth/2+2)*blockdim,(mapHeight/2+9)*blockdim,"YOU'VE LOST. SAVE SCORE",GLUT_BITMAP_TIMES_ROMAN_24);
        iRectangle(5*blockdim,10*blockdim,10*blockdim,3*blockdim);
        iText(5*blockdim+2,10*blockdim+2,playername,GLUT_BITMAP_TIMES_ROMAN_24);
    }

//Save Game
    if(menu == 7)
    {
        iShowBMP(0,0,"Images\\MainMenu\\LoadGamePage.bmp");
        iSetColor(255,0,0);
        iText((mapWidth/2+2)*blockdim,(mapHeight/2+10)*blockdim,"SAVE YOUR GAME",GLUT_BITMAP_TIMES_ROMAN_24);
        iRectangle(5*blockdim,10*blockdim,10*blockdim,3*blockdim);
        iText(5*blockdim+2,10*blockdim+2,nowsave,GLUT_BITMAP_TIMES_ROMAN_24);
    }

//delete game
    if(menu == 8)
    {
        int i;
        char num[3];

        iSetColor(255,0,0);
        iText((mapWidth/2-6)*blockdim,(mapHeight/2+7)*blockdim,"SAVE LIMIT EXCEEDED. DELETE A GAME TO SAVE YOURS",GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(78,121,255);
        for(i=0;i<savednum;i++)
        {
            sprintf(num,"%d",i+1);
            iText((mapWidth/2-1)*blockdim,(mapHeight/2+5-i)*blockdim,num,GLUT_BITMAP_HELVETICA_18);
            iText((mapWidth/2)*blockdim,(mapHeight/2+5-i)*blockdim,savedname[i],GLUT_BITMAP_HELVETICA_18);
        }

        if(delwarning == 1)
            iText(11*blockdim,7*blockdim+15,"Enter a valid number",GLUT_BITMAP_HELVETICA_12);

        iSetColor(255,255,255);
        iText(11*blockdim,3*blockdim,"Enter the file number to delete the file",GLUT_BITMAP_HELVETICA_12);
        iRectangle(11*blockdim,4*blockdim,10*blockdim,2*blockdim);
        iSetColor(255,0,0);
        iText(11*blockdim+5,4*blockdim+15,delnum,GLUT_BITMAP_TIMES_ROMAN_24);

        iShowBMP2(blockdim,19*blockdim,"Images\\MainMenu\\Back.bmp",0x000000);
        iSetColor(255,255,255);
        iRectangle(blockdim,19*blockdim,120,28);
    }
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//place your codes here
		if(menu == 0)
        {
            if((mx >= ((mapWidth/2-2)*blockdim) && mx <= ((mapWidth/2+16)*blockdim)) && (my>= ((mapHeight/2+5)*blockdim) && my<=((mapHeight/2+7)*blockdim+12)))
            {
                restart();
                menu = 1; // Play Game from begining
            }

            if((mx >= ((mapWidth/2-2)*blockdim) && mx <= ((mapWidth/2+16)*blockdim)) && (my>= ((mapHeight/2+5-3)*blockdim) && my<=((mapHeight/2+7-3)*blockdim+12)))
            {
                menu = 2; //LOAD GAME
            }

            if((mx >= ((mapWidth/2-2)*blockdim) && mx <= ((mapWidth/2+16)*blockdim)) && (my>= ((mapHeight/2+5-6)*blockdim) && my<=((mapHeight/2+7-6)*blockdim+12)))
            {
                menu = 3; //Instructions
            }

            if((mx >= ((mapWidth/2-2)*blockdim) && mx <= ((mapWidth/2+16)*blockdim)) && (my>= ((mapHeight/2+5-9)*blockdim) && my<=((mapHeight/2+7-9)*blockdim+12)))
            {
                menu = 4; //High Scores
            }

            if((mx >= ((mapWidth/2-2)*blockdim) && mx <= ((mapWidth/2+16)*blockdim)) && (my>= ((mapHeight/2+5-12)*blockdim) && my<=((mapHeight/2+7-12)*blockdim+12)))
            {
                menu = 5; // Credits
            }

            if((mx >= ((mapWidth/2-2)*blockdim) && mx <= ((mapWidth/2+16)*blockdim)) && (my>= ((mapHeight/2+5-15)*blockdim) && my<=((mapHeight/2+7-15)*blockdim+12)))
            {
                exit(0);
            }
		}

		else if(menu == 2)
        {
            if((mx>=blockdim && mx<=(blockdim+120)) && (my>=19*blockdim && my<=(19*blockdim+28)))
                menu = 0;
        }

        else if(menu == 3)
        {
            if((mx>=27*blockdim && mx<=(27*blockdim+120)) && (my>=19*blockdim && my<=(19*blockdim+28)))
                menu = 0;
        }

        else if(menu == 4)
        {
            if((mx>=blockdim && mx<=(blockdim+120)) && (my>=19*blockdim && my<=(19*blockdim+28)))
                menu = 0;
        }

        else if(menu == 5)
        {
            if((mx>=blockdim && mx<=(blockdim+120)) && (my>=19*blockdim && my<=(19*blockdim+28)))
                menu = 0;
        }

        else if(menu == 8)
        {
            if((mx>=blockdim && mx<=(blockdim+120)) && (my>=19*blockdim && my<=(19*blockdim+28)))
                menu = 0;
        }
	}



	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

    }
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//place your codes here
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if(menu == 1)
    {
        if(reset == 0)
        {
            if(key == 't' || key =='T')
            {
                if(showtracking)
                    showtracking = 0;
                else
                    showtracking = 1;
            }

            if(key == 's' || key == 'S')
            {
                iPauseTimer(0);
                iPauseTimer(1);
                iPauseTimer(2);
                iPauseTimer(3);
                if(savednum<=6)
                {
                    savelen = 0;
                    flag = 0;
                    menu = 7;
                }

                else
                {
                    dellen = 0;
                    flag = 0;
                    menu = 8;
                }
            }

            if((key == 'p' || key == 'P') && paused == 0)
            {
                iPauseTimer(0);
                iPauseTimer(1);
                iPauseTimer(2);
                iPauseTimer(3);
                paused = 1;
            }
            if((key == 'r' || key == 'R') && paused == 1)
            {
                iResumeTimer(0);
                iResumeTimer(1);
                iResumeTimer(2);
                iResumeTimer(3);
                paused = 0;
            }
        }

        if(key == ' ' && paused == 0)
        {
            iResumeTimer(0);
            iResumeTimer(1);
            iResumeTimer(2);
            iResumeTimer(3);
            reset = 0;
        }
    }

    if(menu == 2)
    {
        if(key == '\r')
        {
            int num;
            num = atoi(loadnum);
            char *p = loadnum;
            while(*p) *p++ = '\0';
            loadlen = 0;

            if(num>savednum || num <= 0)
            {
                loadwarning = 1;
            }

            else
            {
                loadwarning = 0;
                loadgame(num);
                menu = 1;
            }
        }

        else if(key == '\b')
        {
            if(loadlen>=1)
            {
                loadlen--;
                delnum[loadlen] = '\0';
            }
        }

        else
        {
            loadnum[loadlen] = key;
            loadlen++;
        }
    }
    if(menu == 6)
    {
        if(key == '\r')
		{
		    halloffame();
		}

		else
		{
			playername[len] = key;
			len++;
		}
    }

    if(menu == 7)
    {
        if(key == '\r')
        {
            savegame();
            menu = 0;
        }

        if(key == ' ')
        {
            return;
        }
        else
        {
            if(flag ==0)
            {
                nowsave[savelen] = '\0';
                flag = 1;
            }
            else
            {
                nowsave[savelen] = key;
                savelen++;
            }
        }
    }

    if(menu == 8)
    {
        if(key == '\r')
        {
            int num;
            num = atoi(delnum);
            char *p = delnum;
            while(*p) *p++ = '\0';
            dellen = 0;

            if(num>savednum || num <=0)
            {
                delwarning = 1;
            }

            else
            {
                delwarning = 0;
                deletegame(num);
                flag = 1;
                menu = 7;
            }
        }

        else if(key == '\b')
        {
            if(dellen>=1)
            {
                dellen--;
                delnum[dellen] = '\0';
            }
        }

        else
        {
            if(flag ==0)
            {
                delnum[dellen] = '\0';
                flag = 1;
            }

            else
            {
            delnum[dellen] = key;
            dellen++;
            }
        }

    }
	//place your codes for other keys here
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
    if(menu == 1 && reset == 0)
    {
        if(paused == 0)
        {
            if(key == GLUT_KEY_RIGHT)
            {
                if(brickmap[pacman.tiley][pacman.tilex+1]!=1 && !(pacman.y%blockdim))
                {
                    if(pacman.movepac.right == 0)
                    {
                        pacman.movepac.left = 0;
                        pacman.movepac.right = 1;
                        pacman.movepac.up = 0;
                        pacman.movepac.down = 0;

                        pacman.dx = pacman.speed;
                        pacman.dy = 0;

                        pacdir = right;
                    }
            }
            }

            if(key == GLUT_KEY_LEFT)
            {
                if(brickmap[pacman.tiley][pacman.tilex-1]!=1 && !(pacman.y%blockdim))
                {
                    if(pacman.movepac.left == 0)
                    {
                        pacman.movepac.left = 1;
                        pacman.movepac.right = 0;
                        pacman.movepac.up = 0;
                        pacman.movepac.down = 0;

                        pacman.dx = -pacman.speed;
                        pacman.dy = 0;

                        pacdir = left;
                    }
                }
            }

            if(key == GLUT_KEY_UP)
            {
                if(brickmap[pacman.tiley-1][pacman.tilex]!=1 && !(pacman.x%blockdim))
                {
                    if(pacman.movepac.up == 0)
                    {
                        pacman.movepac.left = 0;
                        pacman.movepac.right = 0;
                        pacman.movepac.up = 1;
                        pacman.movepac.down = 0;

                        pacman.dx = 0;
                        pacman.dy = pacman.speed;

                        pacdir = up;
                    }
                }
            }

            if(key == GLUT_KEY_DOWN)
            {
                if(brickmap[pacman.tiley+1][pacman.tilex]!=1 && !(pacman.x%blockdim))
                {
                    if(pacman.movepac.down == 0)
                    {
                        pacman.movepac.left = 0;
                        pacman.movepac.right = 0;
                        pacman.movepac.up = 0;
                        pacman.movepac.down = 1;

                        pacman.dx = 0;
                        pacman.dy = -pacman.speed;

                        pacdir = down;
                    }

                }
            }
        }
//exit without saving
        if(key == GLUT_KEY_END)
            exit(0);
    }
	//place your codes for other keys here
}

void loadlog(void)
{
    FILE *fin;
    savednum = 0;

    int i;
    fin  = fopen("GameData\\SaveLog.txt","r");

    if(fin==NULL)
    {
        printf("Error Opening");
        return;
    }

    int cnt = 0;
    for(i=0;i<=MAXSAVE;i++)
    {
        if(EOF != (fscanf(fin,"%s",savedname[i])))
            cnt++;
    }

    fclose(fin);
    savednum = cnt;
}


void seehalloffame(void)
{
    FILE *fin;
    hallcount = 0;
    int i;
    fin  = fopen("GameData\\halloffame.txt","r");

    if(fin==NULL)
    {
        printf("Error Opening");
        return;
    }

    int cnt = 0;
    for(i=0;i<10;i++)
    {
        if(EOF != (fscanf(fin,"%s %d",hall[i].name,&hall[i].score)))
            cnt++;
    }

    hallcount = cnt;
    fclose(fin);
}

void halloffame(void)
{
    typedef struct _SCORE
    {
        char name[80];
        int score;
    }SCORE;

    SCORE scr[11];
    FILE *fin,*fout;
    int i,j;
    int cnt = 0;

    fin  = fopen("GameData\\halloffame.txt","r");

    for(i=0;i<10;i++)
    {
        if(EOF != (fscanf(fin,"%s %d",scr[i].name,&scr[i].score)))
            cnt++;
    }

    fclose(fin);

    strcpy(scr[cnt].name,playername);
    scr[cnt].score = score;

//sorting with the new one
    cnt++;

    for(i=0;i<cnt;i++)
    {
        for(j=0;j<cnt-1-i;j++)
        {
            if(scr[j].score<scr[j+1].score)
            {
                SCORE temp;
                temp = scr[j];
                scr[j] = scr[j+1];
                scr[j+1] = temp;
            }
        }
    }

//Stores Top 10
//Now data is one more
    if(cnt>10)
        cnt = 10;

    fout  = fopen("GameData\\halloffame.txt","w");

    for(i=0;i<cnt;i++)
    {
        fprintf(fout,"%s %d\n",scr[i].name,scr[i].score);
    }

    fclose(fout);

    for(i = 0; i < len; i++)
        playername[i] = 0;

    len = 0;

    seehalloffame();
    menu = 4;
}

int getghostdir(int i)
{
    return 0*ghost[i].moveghost.up+1*ghost[i].moveghost.down+2*ghost[i].moveghost.right+3*ghost[i].moveghost.left;
}

void savegame(void)
{
        FILE *fout,*flog;
        int i,j;

        char fsave[80] = "GameData\\";
        char log[30] = "GameData\\SaveLog.txt";
        strcat(fsave,nowsave);
        strcat(fsave,".txt");
        char *p = nowsave;

        fout = fopen(fsave,"w");

//Pacman savings
        fprintf(fout,"%d %d %d %d %d\n",pacman.life,score,60*minute+sec,foodcount,pacdir);
        fprintf(fout,"%d %d %d %d %d %d\n",pacman.x,pacman.y,pacman.dx,pacman.dy,pacman.xoffset,pacman.yoffset);
        fprintf(fout,"%d %d %d %d\n",pacman.movepac.up,pacman.movepac.down,pacman.movepac.right,pacman.movepac.left);
        fprintf(fout,"%d %d %d\n",pacman.tilex,pacman.tiley,pacman.spritestate);
        fprintf(fout,"%d %d\n",mode,ghosteaten);

        fflush(fout);

        for(i=0;i<nGhost;i++)
        {
            fprintf(fout,"%d %d %d %d %d ",ghost[i].x,ghost[i].y,ghost[i].tilex,ghost[i].tiley,ghost[i].eaten);
            fprintf(fout,"%u %u %u %u ",ghost[i].moveghost.up,ghost[i].moveghost.down,ghost[i].moveghost.right,ghost[i].moveghost.left);
            fprintf(fout,"%d %d %d ",ghost[i].speed,ghost[i].targetx,ghost[i].targety);
            fprintf(fout,"%d %d\n",ghost[i].xoffset,ghost[i].yoffset);
            fflush(fout);

        }

        for(i=0;i<mapHeight;i++)
        {
            for(j=0;j<mapWidth;j++)
                fprintf(fout,"%d ",food[i][j]);
            fprintf(fout,"\n");
        }

        fclose(fout);
        int flag = 0;

//same name input test
        for(i=0;i<savednum;i++)
        {
            if(strcmp(savedname[i],nowsave)==0)
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            strcpy(savedname[savednum],nowsave);
            savednum++;
        }

        flog = fopen(log,"w");

        for(i=0;i<savednum;i++)
        {
            fprintf(flog,"%s\n",savedname[i]);
        }

        for(i=0;i<savelen;i++)
            nowsave[i] = '\0';

        savelen = 0;

        fclose(flog);

        while(*p) *p++ = '\0';

        loadlog();
}

void deletegame(int n)
{
    int i;
    FILE *flog;
    char todelete[80] = "GameData\\";

    strcat(todelete,savedname[n-1]);
    strcat(todelete,".txt");
    remove(todelete);

    for(i=n-1;i<savednum-1;i++)
    {
        strcpy(savedname[i],savedname[i+1]);
    }

    flog = fopen("GameData\\SaveLog.txt","w");

    for(i=0;i<savednum-1;i++)
    {
        fprintf(flog,"%s\n",savedname[i]);
    }

    fclose(flog);
    loadlog();
}

void loadgame(int n)
{
    int i,j,time,dir;
    FILE *fout;
    char toload[80] = "GameData\\";

    strcat(toload,savedname[n-1]);
    strcat(toload,".txt");

    fout = fopen(toload,"r");

    if(fout == NULL)
    {
        if(ferror(fout))
            printf("Error openingin Loadgame");
       return;
    }

    fscanf(fout,"%d%d%d%d%d",&pacman.life,&score,&time,&foodcount,&pacdir);
    fscanf(fout,"%d%d%d%d",&pacman.x,&pacman.y,&pacman.dx,&pacman.dy);
    fscanf(fout,"%d%d",&pacman.xoffset,&pacman.yoffset);
    fscanf(fout,"%d%d%d%d",&pacman.movepac.up,&pacman.movepac.down,&pacman.movepac.right,&pacman.movepac.left);
    fscanf(fout,"%d%d%d",&pacman.tilex,&pacman.tiley,&pacman.spritestate);
    fscanf(fout,"%d%d",&mode,&ghosteaten);


    minute = time/60;
    sec = time%60;

    for(i=0;i<nGhost;i++)
    {
        fscanf(fout,"%d%d%d%d%d",&ghost[i].x,&ghost[i].y,&ghost[i].tilex,&ghost[i].tiley,&ghost[i].eaten);
        fscanf(fout,"%u%u%u%u",&ghost[i].moveghost.up,&ghost[i].moveghost.down,&ghost[i].moveghost.right,&ghost[i].moveghost.left);
        fscanf(fout,"%d%d%d",&ghost[i].speed,&ghost[i].targetx,&ghost[i].targety);
        fscanf(fout,"%d%d",&ghost[i].xoffset,&ghost[i].yoffset);
        changedir(ghost[i].moveghost.up,ghost[i].moveghost.down,ghost[i].moveghost.right,ghost[i].moveghost.left,i);
        dir = getghostdir(i);

        if(dir == 0)
        {
            ghost[i].dx = 0;
            ghost[i].dy = ghost[i].speed;
        }
        else if(dir == 1)
        {
            ghost[i].dx = 0;
            ghost[i].dy = -ghost[i].speed;
        }
        else if(dir == 2)
        {
            ghost[i].dx = ghost[i].speed;
            ghost[i].dy = 0;
        }
        else if(dir == 3)
        {
            ghost[i].dx = -ghost[i].speed;
            ghost[i].dy = 0;
        }
    }

    for(i=0;i<mapHeight;i++)
    {
        for(j=0;j<mapWidth;j++)
            fscanf(fout,"%d",&food[i][j]);
    }

    fclose(fout);

    sprintf(scoredisplay, "%d", score);
	sprintf(foodisplay, "%d", foodcount);
	sprintf(minutedisplay,"%02d",minute);
    sprintf(secondisplay,"%02d",sec);

    reset = 1;
    paused = 0;
    showtracking = 0;
}

void updateTime(void)
{
    sec++;
    if(sec>0 && sec%30 == 0)
        speedchange = 1;

    if(sec%60 == 0){
        minute++;
        sec = 0;
    }

    score--;

    sprintf(minutedisplay,"%02d",minute);
    sprintf(secondisplay,"%02d",sec);

    if(showeat == 1)
    {
        callcount++;
        if(callcount == 2)
        {
            callcount = 0;
            showeat = 0;
        }
    }

}

void changemode(void)
{
    if(reset == 1)
    {
        reset = 0;
        return;
    }

    if(mode == 0 || mode == 1)
    {
        mode = (mode + 1)%2;
    }

    if(mode == 3)
    {
        mode = 0;
        ghosteaten = 0;
    }

    if(mode == 2)
    {
        mode = 3;
    }
}

void movepacman(void)
{
    int i;

    pacman.x += pacman.dx;
    pacman.y += pacman.dy;

    pacman.tilex = (pacman.x)/(int)blockdim;
    pacman.tiley = mapHeight-(pacman.y)/(int)blockdim-1;

    if(pacman.movepac.right)
    {
        if(brickmap[pacman.tiley][pacman.tilex+1]==1)
            pacman.dx = 0;

        else
            pacman.dx = pacman.speed;
    }

    if(pacman.movepac.left)
    {
        pacman.xoffset = pacman.x - pacman.tilex*blockdim;
        if(pacman.xoffset == 0 && brickmap[pacman.tiley][pacman.tilex-1]==1 )
            pacman.dx = 0;

        else
            pacman.dx = -pacman.speed;
    }


    if(pacman.movepac.up)
    {
        if(brickmap[pacman.tiley-1][pacman.tilex]==1)
            pacman.dy = 0;

        else
            pacman.dy = pacman.speed;
    }

    if(pacman.movepac.down)
    {
        pacman.yoffset = pacman.y - (mapHeight-1-pacman.tiley)*blockdim;

        if(pacman.yoffset == 0 && brickmap[pacman.tiley+1][pacman.tilex]==1)
            pacman.dy = 0;

        else
            pacman.dy = -pacman.speed;
    }

//FOOD Getting invisible
    if(pacman.movepac.right || pacman.movepac.up)
    {
        if(food[pacman.tiley][pacman.tilex]==1)
        {
            food[pacman.tiley][pacman.tilex]=0;
            foodcount--;
        }

        else if(food[pacman.tiley][pacman.tilex]==2)
        {
            food[pacman.tiley][pacman.tilex]=0;
            PlaySound("Tracks\\pacman_eatfruit.wav",NULL,SND_ASYNC);
            frightened();
            foodcount--;
        }
    }

    if(pacman.movepac.left)
    {
        if(food[pacman.tiley][pacman.tilex+1]==1)
        {
            food[pacman.tiley][pacman.tilex+1]=0;
            foodcount--;
        }

        else if(food[pacman.tiley][pacman.tilex+1]==2)
        {
            food[pacman.tiley][pacman.tilex+1]=0;
            PlaySound("Tracks\\pacman_eatfruit.wav",NULL,SND_ASYNC);
            frightened();
            foodcount--;
        }
    }

    if(pacman.movepac.down)
    {
        if(food[pacman.tiley-1][pacman.tilex]==1){
            food[pacman.tiley-1][pacman.tilex]=0;
            foodcount--;
        }

        else if(food[pacman.tiley-1][pacman.tilex]==2)
        {
            food[pacman.tiley-1][pacman.tilex]=0;
            PlaySound("Tracks\\pacman_eatfruit.wav",NULL,SND_ASYNC);
            frightened();
            foodcount--;
        }
    }
//End of Food testing

   if(pacman.dx == 0 && pacman.dy == 0)
        pacman.spritestate = 2;

    else if(++pacman.spritestate>3)
        pacman.spritestate = 0;

//Ghost Area
    for(i=0;i<nGhost;i++)
    {
        if((pacman.x == ghost[i].x && pacman.y == ghost[i].y) || (pacman.tilex == ghost[i].tilex && pacman.tiley == ghost[i].tiley))
        {
            if(mode == 0 || mode == 1)
            {
                if(ghost[i].eaten == 0)
                {
                    if(pacmandies == 0)
                        pacman.life--;

                    PlaySound("Tracks\\pacman_death.wav",NULL,SND_ASYNC);

                    iPauseTimer(0);
                    iPauseTimer(1);
                    iPauseTimer(2);
                    iPauseTimer(3);
                    if(pacman.life != 0)
                    {
                        setpacman();

                        if(pacman.life!=0)
                            reset = 1;
                    }


                }
            }

            else if(mode == 2 || mode == 3)
            {
                if(ghost[i].eaten == 0)
                {
                    score += (ghosteaten+1)*200;
                    ghosteaten++;
                    if(showeat == 0)
                        showeat = 1;
                    showeatx = ghost[i].tilex;
                    showeaty = ghost[i].tiley;
                    ghost[i].eaten = 1;
                    PlaySound("Tracks\\pacman_eatghost.wav",NULL,SND_ASYNC);
                }
            }
        }
    }

    if(foodcount == 0)
    {
        win = 1;
    }

    sprintf(scoredisplay, "%d", score);
    sprintf(foodisplay,"%d",foodcount);
}

void frightened(void)
{
    int i;

    for(i=0;i<nGhost;i++){
        if(ghost[i].eaten == 0)
            turnaround(i);
    }

    mode = 2;
}

void turnaround(int i)
{
    if(ghost[i].dx != 0)
    {
        ghost[i].dx = -ghost[i].dx;
        ghost[i].moveghost.right = ghost[i].moveghost.right^1;
        ghost[i].moveghost.left = ghost[i].moveghost.left^1;
    }

    else if(ghost[i].dy != 0)
    {
        ghost[i].dy = -ghost[i].dy;
        ghost[i].moveghost.up = ghost[i].moveghost.up^1;
        ghost[i].moveghost.down = ghost[i].moveghost.down^1;
    }
}

void setpacman(void)
{
    pacman.x = (mapWidth/2)*blockdim;
    pacman.y = blockdim;
    pacman.tilex = pacman.x/(int)blockdim; //Map column
    pacman.tiley = mapHeight-pacman.y/(int)blockdim-1; //Map row
    pacman.speed = 6;

    pacman.movepac.right = 1;
    pacman.movepac.left = 0;
    pacman.movepac.up = 0;
    pacman.movepac.down = 0;

    pacman.dy = 0;
    pacman.dx = pacman.speed;

    pacdir = right;
    pacman.spritestate = 2;

    deathstate = 0;
    pacmandies = 0;

    reset = 1;
    mode = 0;
    paused = 0;

    setghost();
}


void changedir(int up,int down,int right, int left, int i)
{
    ghost[i].moveghost.up=up;
    ghost[i].moveghost.down=down;
    ghost[i].moveghost.right=right;
    ghost[i].moveghost.left=left;
}

void revive(int i)
{
    ghost[i].x = (mapWidth/2)*blockdim - ghost[i].speed;
    ghost[i].y = (mapHeight-1-7)*blockdim;

    ghost[i].dx = ghost[i].speed;
    ghost[i].dy = 0;

    ghost[i].eaten = 0;
    changedir(0,0,1,0,i);
}

void setghost(void)
{
//  GhostName{blinky, pinky, inky , clyde}name;
    int i;

    ghost[0].x = (mapWidth/2)*blockdim;
    ghost[0].y = (mapHeight-1-7)*blockdim;

    ghost[1].x = (mapWidth/2-1)*blockdim;
    ghost[1].y = (mapHeight-1-8)*blockdim;

    ghost[2].x = (mapWidth/2-3)*blockdim;
    ghost[2].y = (mapHeight-1-8)*blockdim;

    ghost[3].x = (mapWidth/2+5)*blockdim;
    ghost[3].y = (mapHeight-1-7)*blockdim;

//Targets are same as Scatter Mode. where mode = 0
    ghost[0].targetx = 0;
    ghost[0].targety = 0;

    ghost[1].targetx = 19;
    ghost[1].targety = 0;

    ghost[2].targetx = 19;
    ghost[2].targety = 21;

    ghost[3].targetx = 0;
    ghost[3].targety = 21;

    for(i=0;i<nGhost;i++)
    {
        ghost[i].tilex = ghost[i].x/(int)blockdim; //Map column
        ghost[i].tiley = mapHeight-ghost[i].y/(int)blockdim-1; //Map row
        ghost[i].eaten = 0;
        ghost[i].xoffset = 0;
        ghost[i].yoffset = 0;
        ghost[i].moveghost.up = 0;
        ghost[i].moveghost.down = 0;
        ghost[i].moveghost.right = 0;
        ghost[i].moveghost.left = 0;
    }

    ghost[0].moveghost.left=1;
    ghost[0].dx = -ghost[0].speed;
    ghost[0].dy = 0;

    ghost[1].moveghost.right=1;
    ghost[1].dx = ghost[1].speed;
    ghost[1].dy = 0;

    ghost[2].moveghost.up=1;
    ghost[2].dy = ghost[2].speed;
    ghost[2].dx = 0;

    ghost[3].moveghost.down=1;
    ghost[3].dy = -ghost[3].speed;
    ghost[3].dx = 0;

    moveghost();
    iDraw();
    ghoststate = 0;
}

void moveghost(void)
{
    int i;
//  GhostName{blinky, pinky, inky , clyde}name;
    if(mode == 0)
    {
        if(ghost[0].eaten == 0)
        {
            ghost[0].targetx = 0;
            ghost[0].targety = 0;
        }

        if(ghost[1].eaten == 0)
        {
            ghost[1].targetx = 19;
            ghost[1].targety = 0;
        }

        if(ghost[2].eaten == 0)
        {
            ghost[2].targetx = 19;
            ghost[2].targety = 21;
        }

        if(ghost[3].eaten == 0)
        {
            ghost[3].targetx = 0;
            ghost[3].targety = 21;
        }
    }

    if(mode == 1)
    {
    //blinky, pinky, inky , clyde

        if(ghost[0].eaten == 0)
        {
            ghost[0].targetx = pacman.tilex;
            ghost[0].targety = pacman.tiley;
        }

        if(ghost[1].eaten == 0)
        {
            ghost[1].targetx = pacman.tilex;
            ghost[1].targety = pacman.tiley;
        }


//Setting Target for inky

        if(ghost[2].eaten == 0)
        {
            if(pacman.movepac.right)
            {
                ghost[2].targetx = pacman.tilex+1;
                ghost[2].targety = pacman.tiley;
            }

            else if(pacman.movepac.left)
            {
                ghost[2].targetx = pacman.tilex-1;
                ghost[2].targety = pacman.tiley;
            }

            else if(pacman.movepac.up)
            {
                ghost[2].targetx = pacman.tilex-1;
                ghost[2].targety = pacman.tiley-1;
            }

            else if(pacman.movepac.up)
            {
                ghost[2].targetx = pacman.tilex;
                ghost[2].targety = pacman.tiley+1;
            }
        }
//Clyde The ODD one
        if(ghost[3].eaten == 0)
        {
            int dist;
            dist = pow((ghost[3].tiley-pacman.tiley),2)+pow((ghost[3].tilex-pacman.tilex),2);

            if(dist<=64)
            {
                ghost[3].targetx = pacman.tilex;
                ghost[3].targety = pacman.tiley;
            }

            else
            {
            //Target will be same as scatter here
                ghost[3].targetx = 0;
                ghost[3].targety = 21;
            }
        }
    }


    if(mode == 2 || mode == 3)
    {
        int i;
        for(i=0;i<nGhost;i++)
        {
            if(!(ghost[i].x%blockdim) && !(ghost[i].y%blockdim))
            {
                ghost[i].targetx = rand()%mapWidth;
                ghost[i].targety = rand()%mapHeight;
            }
        }
    }


    for(i=0;i<nGhost;i++)
    {
        if(ghost[i].eaten == 1)
        {
            if((ghost[i].x%blockdim==0) && (ghost[i].y%blockdim==0))
            {
                ghost[i].targetx = (mapWidth/2);
                ghost[i].targety = 7;
            }

            if(ghost[i].tilex == ghost[i].targetx && ghost[i].tiley == ghost[i].targety)
            {
                if(!(ghost[i].x%blockdim) && !(ghost[i].y%blockdim))
                    revive(i);
            }
        }

        ghost[i].x += ghost[i].dx;
        ghost[i].y += ghost[i].dy;

        ghost[i].tilex = (ghost[i].x)/(int)blockdim;
        ghost[i].tiley = mapHeight-(ghost[i].y)/(int)blockdim-1;

        int dish,disv;

        if(ghost[i].moveghost.right)
        {
            if(brickmap[ghost[i].tiley][ghost[i].tilex+1]==1)
            {
                ghost[i].dx = 0;
                //Testing corners
                if(brickmap[ghost[i].tiley-1][ghost[i].tilex]==1)
                {
                    changedir(0,1,0,0,i);
                    ghost[i].dy = -ghost[i].speed;
                }

                else if(brickmap[ghost[i].tiley+1][ghost[i].tilex]==1)
                {
                    changedir(1,0,0,0,i);
                    ghost[i].dy = ghost[i].speed;
                }

                else
                {
                    if(ghost[i].targety>=ghost[i].tiley) //[acman is down
                    {
                        changedir(0,1,0,0,i);
                        ghost[i].dy = -ghost[i].speed;
                    }

                    else
                    {
                        changedir(1,0,0,0,i);
                        ghost[i].dy = ghost[i].speed;
                    }
                }
            }

            else
            {
                if(!(ghost[i].x%blockdim))
                {

                    if(brickmap[ghost[i].tiley+1][ghost[i].tilex]==1 && brickmap[ghost[i].tiley-1][ghost[i].tilex]==1)
                        ghost[i].dx = ghost[i].speed;


                    else{
                        //Check Up for brick
                        if(brickmap[ghost[i].tiley-1][ghost[i].tilex]==1)
                        {
                            //Go Down OR Right
                            disv = pow((ghost[i].tiley+1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex+1-ghost[i].targetx),2);
                            if(disv<=dish)
                            {
                                changedir(0,1,0,0,i);
                                ghost[i].dx = 0;
                                ghost[i].dy = -ghost[i].speed;
                            }
                        }

                        //Check DOWN for brick
                        else if(brickmap[ghost[i].tiley+1][ghost[i].tilex]==1)
                        {
                            //Go UP OR RIGHT
                            disv = pow((ghost[i].tiley-1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex+1-ghost[i].targetx),2);
                            if(disv<=dish)
                            {
                                changedir(1,0,0,0,i);
                                ghost[i].dx = 0;
                                ghost[i].dy = ghost[i].speed;
                            }
                        }

                        else
                        {
                            //GO UP. DOWN OR RIGHT
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex+1-ghost[i].targetx),2);

                            if(ghost[i].targety>ghost[i].tiley) //pacman is down
                            {
                                disv = pow((ghost[i].tiley+1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                                if(disv<=dish)
                                {
                                    changedir(0,1,0,0,i);
                                    ghost[i].dx = 0;
                                    ghost[i].dy = -ghost[i].speed;
                                }
                            }

                            else if(ghost[i].targety<ghost[i].tiley) // pacman is up
                            {
                                disv = pow((ghost[i].tiley-1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                                if(disv<=dish)
                                {
                                    changedir(1,0,0,0,i);
                                    ghost[i].dx = 0;
                                    ghost[i].dy = ghost[i].speed;
                                }
                            }
                        }
                    }
                }
            }
        }

        else if(ghost[i].moveghost.left)
        {
            ghost[i].xoffset = ghost[i].x - ghost[i].tilex*blockdim;
            if(ghost[i].xoffset == 0 && brickmap[ghost[i].tiley][ghost[i].tilex-1]==1 )
            {
                ghost[i].dx = 0;
                //Testing corners
                if(brickmap[ghost[i].tiley-1][ghost[i].tilex]==1)
                {
                    changedir(0,1,0,0,i);
                    ghost[i].dy = -ghost[i].speed;
                }

                else if(brickmap[ghost[i].tiley+1][ghost[i].tilex]==1)
                {
                    changedir(1,0,0,0,i);
                    ghost[i].dy = ghost[i].speed;
                }

                else
                {
                    if(ghost[i].targety>=ghost[i].tiley) //pacman is down
                    {
                        changedir(0,1,0,0,i);
                        ghost[i].dy = -ghost[i].speed;
                    }

                    else                            //pacman is up
                    {
                        changedir(1,0,0,0,i);
                        ghost[i].dy = ghost[i].speed;
                    }
                }
            }

            //CHange from here
            else if(ghost[i].xoffset == 0)
            {
                if(!(ghost[i].x%blockdim))
                {
                    if(brickmap[ghost[i].tiley+1][ghost[i].tilex]==1 && brickmap[ghost[i].tiley-1][ghost[i].tilex]==1)
                        ghost[i].dx = -ghost[i].speed;


                    else{
                        //Check Up for brick
                        if(brickmap[ghost[i].tiley-1][ghost[i].tilex]==1)
                        {
                            //Go Down OR Left
                            disv = pow((ghost[i].tiley+1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex-1-ghost[i].targetx),2);
                            if(disv<=dish)
                            {
                                changedir(0,1,0,0,i);
                                ghost[i].dx = 0;
                                ghost[i].dy = -ghost[i].speed;
                            }
                        }

                        //Check DOWN for brick
                        else if(brickmap[ghost[i].tiley+1][ghost[i].tilex]==1)
                        {
                            //Go UP OR LEFT
                            disv = pow((ghost[i].tiley-1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex-1-ghost[i].targetx),2);
                            if(disv<=dish)
                            {
                                changedir(1,0,0,0,i);
                                ghost[i].dx = 0;
                                ghost[i].dy = ghost[i].speed;
                            }
                        }

                        else
                        {
                            //GO UP. DOWN OR Left
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex-1-ghost[i].targetx),2);

                            if(ghost[i].targety>ghost[i].tiley) //pacman is down
                            {
                                disv = pow((ghost[i].tiley+1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                                if(disv<=dish)
                                {
                                    changedir(0,1,0,0,i);
                                    ghost[i].dx = 0;
                                    ghost[i].dy = -ghost[i].speed;
                                }
                            }

                            else if(ghost[i].targety<ghost[i].tiley) // pacman is up
                            {
                                disv = pow((ghost[i].tiley-1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                                if(disv<=dish)
                                {
                                    changedir(1,0,0,0,i);
                                    ghost[i].dx = 0;
                                    ghost[i].dy = ghost[i].speed;
                                }
                            }
                        }
                    }
                }
            }
        }


        else if(ghost[i].moveghost.up)
        {
            if(brickmap[ghost[i].tiley-1][ghost[i].tilex]==1)
            {
                ghost[i].dy = 0;
                //Testing corners
                if(brickmap[ghost[i].tiley][ghost[i].tilex+1]==1)
                {
                    changedir(0,0,0,1,i);
                    ghost[i].dx = -ghost[i].speed;
                }

                else if(brickmap[ghost[i].tiley+1][ghost[i].tilex-1]==1)
                {
                    changedir(0,0,1,0,i);
                    ghost[i].dx = ghost[i].speed;
                }

                else
                {
                    if(ghost[i].targetx>=ghost[i].tilex) //pacman is right
                    {
                        changedir(0,0,1,0,i);
                        ghost[i].dx = ghost[i].speed;
                    }

                    else                            //pacman is left
                    {
                        changedir(0,0,0,1,i);
                        ghost[i].dx = -ghost[i].speed;
                    }
                }
            }
    // change is here
            else
            {
                if(ghost[i].y%blockdim==0)
                {

                    if(brickmap[ghost[i].tiley][ghost[i].tilex-1]==1 && brickmap[ghost[i].tiley][ghost[i].tilex+1]==1)
                        ghost[i].dy = ghost[i].speed;


                    else{
                        //Check RIGHT for brick
                        if(brickmap[ghost[i].tiley][ghost[i].tilex+1]==1)
                        {
                            //Go Up OR Left
                            disv = pow((ghost[i].tiley-1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex-1-ghost[i].targetx),2);
                            if(disv>dish)
                            {   //will go left
                                changedir(0,0,0,1,i);
                                ghost[i].dy = 0;
                                ghost[i].dx = -ghost[i].speed;
                            }
                        }

                        //Check LEFT for brick
                        else if(brickmap[ghost[i].tiley][ghost[i].tilex-1]==1)
                        {
                            //Go UP OR RIGHT
                            disv = pow((ghost[i].tiley-1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex+1-ghost[i].targetx),2);
                            if(disv>dish)
                            {
                                changedir(0,0,1,0,i);
                                ghost[i].dy = 0;
                                ghost[i].dx = ghost[i].speed;
                            }
                        }

                        else
                        {
                            //GO UP. LEFT OR RIGHT
                            disv = pow((ghost[i].tiley-1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);

                            if(ghost[i].targetx>ghost[i].tilex) //pacman is right
                            {
                                dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex+1-ghost[i].targetx),2);
                                if(disv>dish)
                                {
                                    changedir(0,0,1,0,i);
                                    ghost[i].dy = 0;
                                    ghost[i].dx = ghost[i].speed;
                                }
                            }

                            else if(ghost[i].targetx<ghost[i].tilex) // pacman is left
                            {
                                dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex-1-ghost[i].targetx),2);
                                if(disv>dish)
                                {
                                    changedir(0,0,0,1,i);
                                    ghost[i].dy = 0;
                                    ghost[i].dx = -ghost[i].speed;
                                }
                            }
                        }
                    }
                }
            }
        }

        else if(ghost[i].moveghost.down)
        {
            ghost[i].yoffset = ghost[i].y - (mapHeight-1-ghost[i].tiley)*blockdim;

            if(ghost[i].yoffset == 0 && brickmap[ghost[i].tiley+1][ghost[i].tilex]==1)
            {
                ghost[i].dy = 0;
                //Testing corners
                if(brickmap[ghost[i].tiley][ghost[i].tilex+1]==1)
                {
                    changedir(0,0,0,1,i);
                    ghost[i].dx = -ghost[i].speed;
                }

                else if(brickmap[ghost[i].tiley+1][ghost[i].tilex-1]==1)
                {
                    changedir(0,0,1,0,i);
                    ghost[i].dx = ghost[i].speed;
                }

                else
                {
                    if(ghost[i].targetx>=ghost[i].tilex) //pacman is right
                    {
                        changedir(0,0,1,0,i);
                        ghost[i].dx = ghost[i].speed;
                    }

                    else                            //pacman is left
                    {
                        changedir(0,0,0,1,i);
                        ghost[i].dx = -ghost[i].speed;
                    }
                }
            }

            else if(ghost[i].yoffset == 0)
            {
                if(!(ghost[i].y%blockdim))
                {

                    if(brickmap[ghost[i].tiley][ghost[i].tilex-1]==1 && brickmap[ghost[i].tiley][ghost[i].tilex+1]==1)
                        ghost[i].dy = -ghost[i].speed;


                    else{
                        //Check RIGHT for brick
                        if(brickmap[ghost[i].tiley][ghost[i].tilex+1]==1)
                        {
                            //Go Up OR Left
                            disv = pow((ghost[i].tiley+1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex-1-ghost[i].targetx),2);
                            if(disv>dish)
                            {   //will go left
                                changedir(0,0,0,1,i);
                                ghost[i].dy = 0;
                                ghost[i].dx = -ghost[i].speed;
                            }
                        }

                        //Check LEFT for brick
                        else if(brickmap[ghost[i].tiley][ghost[i].tilex-1]==1)
                        {
                            //Go UP OR RIGHT
                            disv = pow((ghost[i].tiley+1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);
                            dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex+1-ghost[i].targetx),2);
                            if(disv>dish)
                            {
                                changedir(0,0,1,0,i);
                                ghost[i].dy = 0;
                                ghost[i].dx = ghost[i].speed;
                            }
                        }

                        else
                        {
                            //GO UP. LEFT OR RIGHT
                            disv = pow((ghost[i].tiley+1-ghost[i].targety),2)+pow((ghost[i].tilex-ghost[i].targetx),2);

                            if(ghost[i].targetx>ghost[i].tilex) //pacman is right
                            {
                                dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex+1-ghost[i].targetx),2);
                                if(disv>dish)
                                {
                                    changedir(0,0,1,0,i);
                                    ghost[i].dy = 0;
                                    ghost[i].dx = ghost[i].speed;
                                }
                            }

                            else if(ghost[i].targetx<ghost[i].tilex) // pacman is left
                            {
                                dish = pow((ghost[i].tiley-ghost[i].targety),2)+pow((ghost[i].tilex-1-ghost[i].targetx),2);
                                if(disv>dish)
                                {
                                    changedir(0,0,0,1,i);
                                    ghost[i].dy = 0;
                                    ghost[i].dx = -ghost[i].speed;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ghoststate = (ghoststate+1)%2;

    if(speedchange)
    {
        for(i=0;i<nGhost;i++)
        {
            if(ghost[i].speed<=MAXSPEED)
                ghost[i].speed += 2;
        }

        speedchange = 0;
    }
}

void restart(void)
{
//Full initialization

    int i,j;
	for(i=0;i<nGhost;i++)
        ghost[i].speed = 2;

    for(i=0;i<mapHeight;i++)
    {
        for(j=0;j<mapWidth;j++)
            food[i][j] = foodbackup[i][j];
    }

    seehalloffame();
    loadlog();
	speedchange = 0;
	score = 1000;
	reset = 1;
	paused = 0;
	sec = minute = 0;
	showtracking = 0;
	foodcount = 191;
	sprintf(scoredisplay, "%d", score);
	sprintf(foodisplay, "%d", foodcount);
	sprintf(minutedisplay,"%02d",minute);
    sprintf(secondisplay,"%02d",sec);
    setpacman();
    pacman.life = 3;
}

int main()
{
//    PlaySound((LPCSTR) "C:\\Users\\Public\\Music\\Sample Music\\Sleep Away.mp3", NULL, SND_FILENAME | SND_ASYNC);
//    DWORD dwError = GetLastError();

	//place your own initialization codes here.
	int i;
	for(i=0;i<nGhost;i++)
        ghost[i].speed = 2;

    seehalloffame();
    loadlog();
	speedchange = 0;
	score = 1000;
	sec = minute = 0;
	foodcount = 191;
	ghostspeed = 100;
	sprintf(scoredisplay, "%d", score);
	sprintf(foodisplay, "%d", foodcount);
	sprintf(minutedisplay,"%02d",minute);
    sprintf(secondisplay,"%02d",sec);
    setpacman();
    pacman.life = 3;
    iSetTimer(80,movepacman);
    iSetTimer(80,moveghost);
    iSetTimer(7000,changemode);
    iSetTimer(1000,updateTime);

    ghosteaten = 0;
    showeat = 0;

    menu = 0;

    PlaySound("Tracks\\pacman_beginning.wav",NULL,SND_ASYNC);

    iPauseTimer(0);
    iPauseTimer(1);
    iPauseTimer(2);
    iPauseTimer(3);
    iPauseTimer(4);
	iInitialize(SCREENWIDTH, SCREENHEIGHT, "Pacman");

	return 0;
}
