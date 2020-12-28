#include "main.h"

// Display

void Game_Display_Logo() {
    int i;
    glColor3f(1.0f, 1.0f, 1.0f);
    for (i = 0; i <= Logo_Num; i++) {
        Map_Texture(&Logo[i].Img);
        Draw_Rect(&Logo[i].Rct);
    }
}

void Game_Display_Menu() {
    int i;
    glColor3f(1.0f, 1.0f, 1.0f);
    for (i = 0; i <= Logo_Num; i++) {
        Map_Texture(&Logo[i].Img);
        Draw_Rect(&Logo[i].Rct);
    }
    glColor4f(1.0f, 1.0f, 1.0f, Menu_Alpha);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
    glVertex2f(0, 147.0f);
    glVertex2f(WIDTH, 147.0f);
    glEnd();
    if (Flick_Text) {
        glTranslatef(180.0f, 280.0f, 0.0f);
        glScalef(0.18f, 0.18f, 1.0f);
        for (i = 0; i < 13; i++)
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, Text_Press_Any_Key[i]);
        glLoadIdentity();
    }
    glEnable(GL_TEXTURE_2D);
    Map_Texture(&Img_Player[Player.Stt][Player.Drt][Player.Num]);
    Draw_Rect(&Player.Rct);
}

void Game_Display_Prepare() {
    int i;
    glColor4f(1.0f, 1.0f, 1.0f, Logo_Alpha);
    for (i = 0; i <= Logo_Num; i++) {
        Map_Texture(&Logo[i].Img);
        Draw_Rect(&Logo[i].Rct);
    }
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - Logo_Alpha);
    Map_Texture(&Img_Life);
    for (i = 0; i < Player.Life; i++)
        Draw_Rect(&Rct_Life[i]);
    glDisable(GL_TEXTURE_2D);
    glTranslatef(270.0f, 540.0f, 0.0f);
    glScalef(0.3f, 0.3f, 1.0f);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, Score[0]);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, Score[1]);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, Score[2]);
    glLoadIdentity();
    glColor4f(1.0f, 1.0f, 1.0f, Menu_Alpha);
    glBegin(GL_LINES);
    glVertex2f(0, 147.0f);
    glVertex2f(WIDTH, 147.0f);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    Map_Texture(&Img_Player[Player.Stt][Player.Drt][Player.Num]);
    Draw_Rect(&Player.Rct);
}

void Game_Display_Play() {
    int i;
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
    glVertex2f(0, 147.0f);
    glVertex2f(WIDTH, 147.0f);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    if (Item.Is_Alive) {
        Map_Texture(&Img_Item[Item.Type]);
        Draw_Rect(&Item.Rct);
    }
    if (Explode) {
        Map_Texture(&Img_Miniball);
        glColor4f(1.0f, 1.0f, 1.0f, Explode_Alpha);
        for (i = 0; i < 15; i++) {
            Draw_Rect(&Miniball[i].Rct);
        }
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    if (Player.Is_Draw) {
        Map_Texture(&Img_Player[Player.Stt][Player.Drt][Player.Num]);
        Draw_Rect(&Player.Rct);
        if (Player.Has_Shield) {
            Player.Rct_Shield.Left = Player.Rct.Left - 3.0f;
            Player.Rct_Shield.Right = Player.Rct_Shield.Left + Player.Shield_w;
            Map_Texture(&Img_Shield[Player.Shield_Num]);
            Draw_Rect(&Player.Rct_Shield);
        }
    }
    Map_Texture(&Img_Ball);
    Tmp_Ball = List_Ball;
    while (Tmp_Ball != NULL) {
        Draw_Rect(&Tmp_Ball->Rct);
        Tmp_Ball = Tmp_Ball->Next;
    }
    Map_Texture(&Img_Life);
    for (i = 0; i < Player.Life; i++)
        Draw_Rect(&Rct_Life[i]);
    glDisable(GL_TEXTURE_2D);
    glTranslatef(270.0f, 540.0f, 0.0f);
    glScalef(0.3f, 0.3f, 1.0f);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, Score[0]);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, Score[1]);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, Score[2]);
    if (Inc_Score) {
        glLoadIdentity();
        glColor4f(1.0f, 1.0f, 1.0f, Inc_Score_Alpha);
        glTranslatef(Inc_Score_x, Inc_Score_y, 0.0f);
        glScalef(0.12f, 0.12f, 1.0f);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, '+');
        glutStrokeCharacter(GLUT_STROKE_ROMAN, '2');
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glEnable(GL_TEXTURE_2D);
}

void Game_Display_Over() {
    int i;
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
    glVertex2f(0, 147.0f);
    glVertex2f(WIDTH, 147.0f);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    if (Item.Is_Alive) {
        Map_Texture(&Img_Item[Item.Type]);
        Draw_Rect(&Item.Rct);
    }
    if (Explode) {
        Map_Texture(&Img_Miniball);
        glColor4f(1.0f, 1.0f, 1.0f, Explode_Alpha);
        for (i = 0; i < 15; i++) {
            Draw_Rect(&Miniball[i].Rct);
        }
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    Map_Texture(&Img_Player[Player.Stt][Player.Drt][Player.Num]);
    Draw_Rect(&Player.Rct);
    Map_Texture(&Img_Ball);
    Tmp_Ball = List_Ball;
    while (Tmp_Ball != NULL) {
        Draw_Rect(&Tmp_Ball->Rct);
        Tmp_Ball = Tmp_Ball->Next;
    }
    Map_Texture(&Img_Life);
    for (i = 0; i < Player.Life; i++)
        Draw_Rect(&Rct_Life[i]);
    glDisable(GL_TEXTURE_2D);
    glTranslatef(270.0f, 540.0f, 0.0f);
    glScalef(0.3f, 0.3f, 1.0f);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, Score[0]);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, Score[1]);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, Score[2]);
    glLoadIdentity();
    glTranslatef(66.0f, 360.0f, 0.0f);
    glScalef(0.6f, 0.6f, 1.0f);
    glLineWidth(4.0f);
    for (i = 0; i < 9; i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, Text_Game_Over[i]);
    glLineWidth(2.0f);
    if (Flick_Text) {
        glLoadIdentity();
        glTranslatef(180.0f, 270.0f, 0.0f);
        glScalef(0.18f, 0.18f, 1.0f);
        for (i = 0; i < 13; i++)
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, Text_Press_Any_Key[i]);
    }
    glEnable(GL_TEXTURE_2D);
}

// Process

void Game_Process_Logo() {
    Logo[Logo_Num].k++;
    if (Logo[Logo_Num].k == 20) {
        Logo_Num++;
        if (Logo_Num == 10) {
            Logo_Num = 9;
            Reload_Player();
            Game_State = GAME_STT_MENU;
            Interval = 25;
            Timer1 = 0;
            return;
        }
    }
    Logo[Logo_Num].Rct.Bottom -= 9.0f;
    Logo[Logo_Num].Rct.Top -= 9.0f;
}

void Game_Process_Menu() {
    if (Menu_Alpha < 1.0f) {
        Menu_Alpha += 0.05f;
        if (Menu_Alpha >= 1.0f)
            glutKeyboardFunc(Keyboard_Menu);
    }
    Timer1++;
    if (Timer1 == 4) {
        Timer1 = 0;
        Game_Timer++;
        if (Game_Timer == 3) {
            Flick_Text = 1 - Flick_Text;
            Game_Timer = 0;
        }
        Player.Num = Loop[Player.Num];
    }
}

void Game_Process_Prepare() {
    Timer1++;
    if (Timer1 == 4) {
        Timer1 = 0;
        Player.Num = Loop[Player.Num];
    }
    if (Logo_Alpha > 0.0f) {
        Logo_Alpha -= 0.05f;
    } else {
        Init_Game();
    }
}

void Game_Process_Play() {
    int i;
    if (Item.Is_Alive) {
        Item.Num = Loop64[Item.Num];
        Item.Rct.Bottom = Item_Offset[Item.Num];
        Item.Rct.Top = Item.Rct.Bottom + Item.h;
        if (Player.p > Item.Rct.Left && Player.p < Item.Rct.Right) {
            Take_Item_Func[Item.Type]();
            Item.Is_Alive = 0;
        }
    } else {
        Item.Timer++;
        if (Item.Timer == 100) {
            Item.Timer = 0;
            Create_Item();
        }
    }
    if (Inc_Score) {
        Inc_Score_Alpha -= 0.05f;
        Inc_Score_y += 3.0f;
        Inc_Score--;
    }
    Player.Shield_Num = Loop20[Player.Shield_Num];
    Timer1++;
    if (Timer1 == 4) {
        Timer1 = 0;
        if (Player.Life > 0) {
            if (Player.Invulnerable > 0) {
                Player.Is_Draw = !Player.Is_Draw;
                Player.Invulnerable--;
            }
            Player.Num = Loop[Player.Num];
            Timer2++;
            if (Timer2 == 10) {
                Timer2 = 0;
                Increase_Score();
                Timer3++;
                if (Timer3 == 4) {
                    Timer3 = 0;
                    Insert_Ball();
                }
            }
        } else {
            Game_Timer++;
            if (Game_Timer == 4) {
                Flick_Text = 1 - Flick_Text;
                Game_Timer = 0;
            }
            if (Player.Num < 5) {
                if (Player.Num == 3) {
                    Player.Rct.Bottom -= 12.0f;
                    Player.Rct.Top -= 12.0f;
                }
                Player.Num++;
            }
        }
    }
    if (Player.Stt == PLAYER_STT_RUN) {
        if (Player.Drt == LEFT) {
            if (Player.p > 30.0f) {
                Player.p -= 6.0f;
                Player.Rct.Left -= 6.0f;
                Player.Rct.Right -= 6.0f;
            }
        } else {
            if (Player.p < 570.0f) {
                Player.p += 6.0f;
                Player.Rct.Left += 6.0f;
                Player.Rct.Right += 6.0f;
            }
        }
    }
    Tmp_Ball = List_Ball;
    int n = 0;
    while (Tmp_Ball != NULL) {
        Move_Ball(Tmp_Ball);
        if (Player.Life > 0 && Player.Invulnerable == 0) {
            if (Collision_Ball(Tmp_Ball)) {
                Delete_Ball(n);
                if (Player.Has_Shield) {
                    Player.Has_Shield = 0;
                } else
                    Player.Life--;
                if (Player.Life == 0) {
                    glutSpecialFunc(NULL);
                    glutSpecialUpFunc(NULL);
                    glutKeyboardFunc(Keyboard_Restart);
                    Game_Timer = 0;
                    Game_State = GAME_STT_OVER;
                    Player.Stt = PLAYER_STT_DEATH;
                    Player.Num = 0;
                } else
                    Player.Invulnerable = 14;
            }
        }
        n++;
        if (Tmp_Ball != NULL)
            Tmp_Ball = Tmp_Ball->Next;
        else
            break;
    }
    if (Explode) {
        Explode--;
        Explode_Alpha -= 0.01f;
        for (i = 0; i < 15; i++) {
            Miniball[i].vy += Miniball[i].ay;
            Miniball[i].Rct.Left += Miniball[i].vx;
            Miniball[i].Rct.Right += Miniball[i].vx;
            Miniball[i].Rct.Bottom += Miniball[i].vy;
            Miniball[i].Rct.Top += Miniball[i].vy;
            if (Miniball[i].Rct.Bottom < Ground_Height) {
                Miniball[i].Rct.Bottom = Ground_Height;
                Miniball[i].Rct.Top = Miniball[i].Rct.Bottom + Img_Miniball.h;
                Miniball[i].vy = Miniball[i].vy * -0.6f;
            }
        }
    }
}

void Game_Process_Over() {
    int i;
    if (Item.Is_Alive) {
        Item.Num = Loop64[Item.Num];
        Item.Rct.Bottom = Item_Offset[Item.Num];
        Item.Rct.Top = Item.Rct.Bottom + Item.h;
    }
    if (Inc_Score) {
        Inc_Score_Alpha -= 0.05f;
        Inc_Score_y += 3.0f;
        Inc_Score--;
    }
    Timer1++;
    if (Timer1 == 4) {
        Timer1 = 0;
        Game_Timer++;
        if (Game_Timer == 3) {
            Flick_Text = 1 - Flick_Text;
            Game_Timer = 0;
        }
        if (Player.Num < 5) {
            if (Player.Num == 3) {
                Player.Rct.Bottom -= 12.0f;
                Player.Rct.Top -= 12.0f;
            }
            Player.Num++;
        }
    }
    Tmp_Ball = List_Ball;
    while (Tmp_Ball != NULL) {
        Move_Ball(Tmp_Ball);
        if (Tmp_Ball != NULL)
            Tmp_Ball = Tmp_Ball->Next;
        else
            break;
    }
    if (Explode) {
        Explode--;
        Explode_Alpha -= 0.01f;
        for (i = 0; i < 15; i++) {
            Miniball[i].vy += Miniball[i].ay;
            Miniball[i].Rct.Left += Miniball[i].vx;
            Miniball[i].Rct.Right += Miniball[i].vx;
            Miniball[i].Rct.Bottom += Miniball[i].vy;
            Miniball[i].Rct.Top += Miniball[i].vy;
            if (Miniball[i].Rct.Bottom < Ground_Height) {
                Miniball[i].Rct.Bottom = Ground_Height;
                Miniball[i].Rct.Top = Miniball[i].Rct.Bottom + Img_Miniball.h;
                Miniball[i].vy = Miniball[i].vy * -0.6f;
            }
        }
    }
}

// Take Item

void Take_Item_Coin() {
    Inc_Score_By_Item();
}

void Take_Item_Bomb() {
    int i;
    if (List_Ball == NULL)
        Inc_Score_By_Item();
    else {
        Player.Bomb_Count++;
        if (Player.Bomb_Count == 2) {
            Player.Bomb_Count = 0;
            Explode = 100;
            Explode_Alpha = 1.0f;
            for (i = 0; i < 15; i++) {
                Miniball[i].Rct.Left = List_Ball->Rct.Left + 15.0f;
                Miniball[i].Rct.Right = List_Ball->Rct.Left + 24.0f;
                Miniball[i].Rct.Bottom = List_Ball->Rct.Bottom + 15.0f;
                Miniball[i].Rct.Top = List_Ball->Rct.Bottom + 24.0f;
                Miniball[i].vx = 1.8f * cos(i * Offset_Angle) + List_Ball->vx / 2;
                Miniball[i].vy = 1.5f * sin(i * Offset_Angle) + List_Ball->vy / 2;
                Miniball[i].ay = List_Ball->ay / 1.4f;
            }
            Delete_Ball(0);
        }
    }
}

void Take_Item_Shield() {
    if (!Player.Has_Shield)
        Player.Shield_Count++;
    else
        Inc_Score_By_Item();
    if (Player.Shield_Count == 3) {
        Player.Shield_Count = 0;
        Player.Has_Shield = 1;
    }
}
