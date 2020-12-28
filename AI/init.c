#include "main.h"

// Init Func, Keyboard Func

// ~ s_Ball

s_Ball *Create_Ball() {
    s_Ball *tg = (s_Ball *)malloc(sizeof(s_Ball));
    int Drt = rand() % 2;
    int Tmp = 1 - 2 * Drt;
    tg->vx = (6.0f + rand() % 12) * 0.35f * Tmp;
    tg->ay = -(70.0f + rand() % 50) * 0.01f;
    tg->vy = tg->ay;
    tg->Rct.Left = Drt * WIDTH * 1.0f - Img_Ball.w / 2;
    tg->Rct.Right = tg->Rct.Left + Img_Ball.w;
    tg->Rct.Bottom = rand() % 58 * 3.0f + 340.0f;
    //    tg->vx = 8.0f * 0.35f;
    //    tg->ay = -0.9f;
    //    tg->vy = tg->ay;
    //    tg->Rct.Left = 200.0f - Img_Ball.w / 2;
    //    tg->Rct.Right = tg->Rct.Left + Img_Ball.w;
    //    tg->Rct.Bottom = 280.0f;
    tg->Rct.Top = tg->Rct.Bottom + Img_Ball.h;
    tg->Has_Offset = 0;
    tg->Next = NULL;
    return tg;
}

void Insert_Ball() {
    s_Ball *tg = Create_Ball();
    if (List_Ball == NULL) {
        List_Ball = tg;
        return;
    }
    tg->Next = List_Ball;
    List_Ball = tg;
}

void Delete_Ball(int pos) {
    s_Ball *tg = List_Ball;
    if (pos == 0) {
        List_Ball = List_Ball->Next;
        free(tg);
        return;
    }
    int i = 0;
    while (i < pos - 1) {
        i++;
        tg = tg->Next;
    }
    if (tg->Next->Next == NULL) {
        free(tg->Next);
        tg->Next = NULL;
    } else {
        s_Ball *tg2 = tg->Next;
        tg->Next = tg->Next->Next;
        free(tg2);
    }
}

void Move_Ball(s_Ball *Ball) {
    Ball->Rct.Left += Ball->vx;
    Ball->Rct.Right += Ball->vx;
    if (Ball->vx > 0) {
        if (Ball->Rct.Right > WIDTH)
            Ball->vx = -Ball->vx;
    } else {
        if (Ball->Rct.Left < 0.0f)
            Ball->vx = -Ball->vx;
    }
    Ball->Rct.Bottom += Ball->vy;
    Ball->Rct.Top += Ball->vy;
    Ball->vy += Ball->ay;
    if (Ball->Has_Offset) {
        Ball->Rct.Bottom += Ball->Offset;
        Ball->Rct.Top += Ball->Offset;
        Ball->Has_Offset = 0;
    } else if (Ball->Rct.Bottom < Ground_Height) {
        Ball->Offset = Ball->Rct.Bottom - Ground_Height;
        Ball->Has_Offset = 1;
        Ball->Rct.Bottom = Ground_Height;
        Ball->Rct.Top = Ball->Rct.Bottom + Img_Ball.h;
        Ball->vy = Ball->ay - Ball->vy;
    }
}

int Collision_Ball(s_Ball *Ball, float *p) {
    if (Ball->Rct.Bottom < 210.0f)
        if (Ball->Rct.Left - 10.0f < *p && Ball->Rct.Right + 10.0f > *p)
            return 1;
    return 0;
}

// ~ End s_Ball

void Increase_Score() {
    Score[2]++;
    if (Score[2] > '9') {
        Score[2] = '0';
        Score[1]++;
        if (Score[1] > '9') {
            Score[1] = '0';
            Score[0]++;
        }
    }
}

void Inc_Score_By_Item() {
    Increase_Score();
    Increase_Score();
    Inc_Score_Alpha = 1.0f;
    Inc_Score = 20;
    Inc_Score_x = Item.Rct.Left + 9.0f;
    Inc_Score_y = Item.Rct.Top + 18.0f;
}

void Init_Item() {
    float Max = 1.8f;
    float a = Max / 16, v = Max, x = 0.0f;
    int c = 0, t1 = 0, t2 = 1;
    while (c < 64) {
        v -= a;
        t1 += t2;
        if (t1 == 16) {
            t2 = -t2;
        } else if (t1 == 0) {
            a = -a;
            t2 = -t2;
        }
        x += v;
        Item_Offset[c] = x + 150.0f;
        c++;
    }
}

void Create_Item() {
    Item.Num = 0;
    Item.p = rand() % 561 + 20.0f;
    Item.Rct.Bottom = Item_Offset[Item.Num];
    Item.Rct.Top = Item.Rct.Bottom + Item.h;
    Item.Rct.Left = Item.p - Item.w / 2;
    Item.Rct.Right = Item.Rct.Left + Item.w;
    Item.Type = rand() % 3;
    Item.Is_Alive = 1;
}

void Reload_Player() {
    Player.Stt = 0;
    Player.Drt = 1;
    Player.Num = 0;
    Player.p = WIDTH / 2.0f;
    Player.Rct.Left = Player.p - Player.w / 2;
    Player.Rct.Right = Player.Rct.Left + Player.w;
    Player.Rct.Bottom = 120.0f;
    Player.Rct.Top = Player.Rct.Bottom + Player.h;
}

void Init_Global() {
    // OpenGL
    glClearColor(0.2f, 0.4f, 0.4f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, WIDTH, HEIGHT);
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glutIgnoreKeyRepeat(GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glLineWidth(2.0f);

    // Game
    srand(time(NULL));
    int i;
    for (i = 0; i < 19; i++)
        Loop20[i] = i + 1;
    Loop20[19] = 0;
    for (i = 0; i < 63; i++)
        Loop64[i] = i + 1;
    Loop64[63] = 0;
    Load_Image();
    float Logo_Offset = 60.0f;
    float Logo_Offset_Extra[] = {3.0f, 9.0f, 9.0f, 9.0f, 33.0f, 9.0f, 9.0f, 9.0f, 9.0f, 9.0f};
    for (i = 0; i < 10; i++) {
        Logo[i].k = 0;
        Logo[i].Rct.Left = Logo_Offset;
        Logo[i].Rct.Right = Logo[i].Rct.Left + Logo[i].Img.w;
        Logo_Offset = Logo[i].Rct.Right + Logo_Offset_Extra[i];
        Logo[i].Rct.Bottom = HEIGHT;
        Logo[i].Rct.Top = Logo[i].Rct.Bottom + Logo[i].Img.h;
    }
    Logo[0].k = -1;
    Logo_Num = 0;
    for (i = 0; i < LIFE_MAX; i++) {
        Rct_Life[i].Left = 42.0f + (Img_Life.w + 6) * i;
        Rct_Life[i].Right = Rct_Life[i].Left + Img_Life.w;
        Rct_Life[i].Bottom = 537.0f;
        Rct_Life[i].Top = Rct_Life[i].Bottom + Img_Life.h;
    }
    Reload_Player();
    Player.Rct_Shield.Bottom = Player.Rct.Bottom;
    Player.Rct_Shield.Top = Player.Rct_Shield.Bottom + Player.Shield_h;
    Init_Item();
}

void Init_Game() {
    while (List_Ball != NULL)
        Delete_Ball(0);
    Insert_Ball();
    Score[0] = Score[1] = Score[2] = '0';
    Player.Shield_Count = Player.Bomb_Count = Player.Shield_Num = Player.Invulnerable = Player.Has_Shield = 0;
    Item.Num = Item.Timer = Item.Is_Alive = 0;
    Inc_Score = 0;
    Timer1 = Timer2 = Timer3 = 0;
    Explode = 0;
    Explode_Alpha = 0.0f;
    Player.Is_Draw = 1;
    Player.Life = 3;
    glutSpecialFunc(Special_Down);
    glutSpecialUpFunc(Special_Up);
    Game_State = GAME_STT_PLAY;
}
