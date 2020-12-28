#ifndef MAIN_H
#define MAIN_H

#include <math.h>
#include <time.h>

#include <GL/glut.h>

#include "../Library/loadpng.h"
#include "../Library/process_image.h"

#include "../Library/gl_texture.h"

/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

#define WIDTH 600
#define HEIGHT 600

#define ITEM_MAX 3
#define LIFE_MAX 3

int POS_X, POS_Y;
int Interval = 15;

enum GAME_STATE {
    GAME_STT_LOGO,
    GAME_STT_MENU,
    GAME_STT_PREPARE,
    GAME_STT_PLAY,
    GAME_STT_OVER
};

enum PLAYER_STATE {
    PLAYER_STT_STAND,
    PLAYER_STT_RUN,
    PLAYER_STT_DEATH
};

enum DIRECTION {
    LEFT,
    RIGHT
};

enum ITEM_NAME {
    ITEM_COIN,
    ITEM_BOMB,
    ITEM_SHIELD
};

struct s_Logo {
    Rect Rct;
    Image Img;
    unsigned int k;
} Logo[10];

struct s_Player {
    int w, h, Shield_w, Shield_h;
    int Life, Stt, Drt, Num, Shield_Num, Invulnerable;
    int Shield_Count, Bomb_Count;
    int Is_Draw, Has_Shield;
    Rect Rct, Rct_Shield;
    float p;
} Player;

typedef struct s_Ball {
    Rect Rct;
    float ay, vx, vy;
    float Offset;
    int Has_Offset;
    struct s_Ball *Next;
} s_Ball;

struct s_Miniball {
    Rect Rct;
    float vx, vy, ay;
} Miniball[15];

struct s_Item {
    int w, h;
    Rect Rct;
    float p;
    int Num, Timer;
    int Is_Alive, Type;
} Item;

Image Img_Ball, Img_Miniball;
Image Img_Life;
Image Img_Player[3][2][6], Img_Shield[20], Img_Item[ITEM_MAX]; // State - Direction - Number Animation

int Game_State, Game_Timer;

s_Ball *List_Ball = NULL;
s_Ball *Tmp_Ball;

int Flick_Text = 1, Logo_Num;
int Loop[] = {1, 2, 3, 4, 5, 0};
int Loop20[20];
int Loop64[64];
int Explode;
float Explode_Alpha;
int Timer1, Timer2, Timer3;
Rect Rct_Life[3];
int Inc_Score;
float Inc_Score_Alpha, Inc_Score_x, Inc_Score_y;
float Item_Offset[64];
float Logo_Alpha = 1.0f, Menu_Alpha = 0.0f;
float Ground_Height = 150.0f;
float Offset_Angle = 3.141592653f * 2 / 15;
char Text_Game_Over[] = "GAME OVER";
char Text_Press_Any_Key[] = "Press Any Key";
char Score[3];
char Str[25];

// ~ Init Prototype

// image.h
void Load_Image_Logo();
void Load_Image_Player();
void Load_Image_Shield();
void Load_Image_Item();
void Load_Image();

// init.h
s_Ball *Create_Ball();
void Insert_Ball();
void Delete_Ball(int pos);
void Move_Ball(s_Ball *Ball);
int Collision_Ball(s_Ball *Ball);
void Increase_Score();
void Inc_Score_By_Item();
void Take_Item_Shield();
void Take_Item_Coin();
void Take_Item_Bomb();
void Init_Item();
void Create_Item();
void Reload_Player();
void Init_Global();
void Init_Game();

// afunc.h
void Game_Display_Logo();
void Game_Display_Menu();
void Game_Display_Prepare();
void Game_Display_Play();
void Game_Display_Over();
void Game_Process_Logo();
void Game_Process_Menu();
void Game_Process_Prepare();
void Game_Process_Play();
void Game_Process_Over();

// main.h
void Display();
void Timer(int value);
void Keyboard_Menu(unsigned char key, int x, int y);
void Keyboard_Restart(unsigned char key, int x, int y);
void Special_Down(int key, int x, int y);
void Special_Up(int key, int x, int y);

// ~ End Init

void (*Take_Item_Func[])() = {Take_Item_Coin, Take_Item_Bomb, Take_Item_Shield};
void (*Game_Display_Func[])() = {Game_Display_Logo, Game_Display_Menu, Game_Display_Prepare, Game_Display_Play, Game_Display_Over};
void (*Game_Process_Func[])() = {Game_Process_Logo, Game_Process_Menu, Game_Process_Prepare, Game_Process_Play, Game_Process_Over};

#include "afunc.c"
#include "image.c"
#include "init.c"

#endif
