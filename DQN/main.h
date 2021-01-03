#ifndef MAIN_H
#define MAIN_H

#include <math.h>
#include <time.h>

#include <GL/glut.h>

#include <arpa/inet.h>

#include "Library/loadpng.h"
#include "Library/process_image.h"

#include "Library/gl_texture.h"

#define WIDTH 600
#define HEIGHT 600

#define GROUND_OFFSET 150.0f

#define ITEM_MAX 3
#define LIFE_MAX 3

#define INTERVAL 0

enum PLAYER_STATE { PLAYER_STT_STAND, PLAYER_STT_RUN, PLAYER_STT_DEATH };

enum DIRECTION { LEFT, RIGHT };

enum ITEM_NAME { ITEM_COIN, ITEM_BOMB, ITEM_SHIELD };

class Player {
  public:
    Image img[3][2][6], img_shield[20], img_life;

    float x;
    int w, h, shield_w, shield_h;
    int stt, drt, num, life, shield_num;
    int shield_count, bomb_count;
    bool is_draw, has_shield;
    int inc_score_count, invul_count;
    float inc_score_alpha, inc_score_x, inc_score_y;
    int timer_num, timer_score;
    char score[3];

    float reward, lose;

    Rect rct, rct_life[3], rct_shield;

    void init() {
        int pos[36][4];
        int *pos_ptr;
        Image img_full, img_tmp;
        FILE *f;

        f = fopen("Images/Player.pos", "r");
        for (int i = 0; i < 36; i++)
            fscanf(f, "%d %d %d %d\n", &pos[i][0], &pos[i][1], &pos[i][2], &pos[i][3]);
        fclose(f);

        Load_Png(&img_full.img, &img_full.w, &img_full.h, "Images/Player.png");
        for (int i = 0; i < 6; i++) {
            pos_ptr = &pos[i][0];
            Crop_Image(&img_full, &img_tmp, *pos_ptr, *(pos_ptr + 1), *(pos_ptr + 2), *(pos_ptr + 3));
            Swap_Image(img_tmp.img, img_tmp.w, img_tmp.h);
            img[PLAYER_STT_STAND][LEFT][i] = img_tmp;
            pos_ptr = &pos[i + 6][0];
            Crop_Image(&img_full, &img_tmp, *pos_ptr, *(pos_ptr + 1), *(pos_ptr + 2), *(pos_ptr + 3));
            Swap_Image(img_tmp.img, img_tmp.w, img_tmp.h);
            img[PLAYER_STT_STAND][RIGHT][i] = img_tmp;
            pos_ptr = &pos[i + 12][0];
            Crop_Image(&img_full, &img_tmp, *pos_ptr, *(pos_ptr + 1), *(pos_ptr + 2), *(pos_ptr + 3));
            Swap_Image(img_tmp.img, img_tmp.w, img_tmp.h);
            img[PLAYER_STT_RUN][LEFT][i] = img_tmp;
            pos_ptr = &pos[i + 18][0];
            Crop_Image(&img_full, &img_tmp, *pos_ptr, *(pos_ptr + 1), *(pos_ptr + 2), *(pos_ptr + 3));
            Swap_Image(img_tmp.img, img_tmp.w, img_tmp.h);
            img[PLAYER_STT_RUN][RIGHT][i] = img_tmp;
            pos_ptr = &pos[i + 24][0];
            Crop_Image(&img_full, &img_tmp, *pos_ptr, *(pos_ptr + 1), *(pos_ptr + 2), *(pos_ptr + 3));
            Swap_Image(img_tmp.img, img_tmp.w, img_tmp.h);
            img[PLAYER_STT_DEATH][LEFT][i] = img_tmp;
            pos_ptr = &pos[i + 30][0];
            Crop_Image(&img_full, &img_tmp, *pos_ptr, *(pos_ptr + 1), *(pos_ptr + 2), *(pos_ptr + 3));
            Swap_Image(img_tmp.img, img_tmp.w, img_tmp.h);
            img[PLAYER_STT_DEATH][RIGHT][i] = img_tmp;
        }
        w = img[PLAYER_STT_STAND][LEFT][0].w;
        h = img[PLAYER_STT_STAND][LEFT][0].h;
        free(img_full.img);

        f = fopen("Images/Shield.pos", "r");
        for (int i = 0; i < 20; i++)
            fscanf(f, "%d %d %d %d\n", &pos[i][0], &pos[i][1], &pos[i][2], &pos[i][3]);
        fclose(f);

        Load_Png(&img_full.img, &img_full.w, &img_full.h, "Images/Shield.png");
        for (int i = 0; i < 20; i++) {
            pos_ptr = &pos[i][0];
            Crop_Image(&img_full, &img_tmp, *pos_ptr, *(pos_ptr + 1), *(pos_ptr + 2), *(pos_ptr + 3));
            Swap_Image(img_tmp.img, img_tmp.w, img_tmp.h);
            img_shield[i] = img_tmp;
        }
        shield_w = img_shield[0].w;
        shield_h = img_shield[0].h;
        free(img_full.img);

        Load_Texture_Swap(&img_life, "Images/Life.png");

        for (int i = 0; i < LIFE_MAX; i++) {
            rct_life[i].Left = 42.0f + (img_life.w + 6) * i;
            rct_life[i].Right = rct_life[i].Left + img_life.w;
            rct_life[i].Bottom = 537.0f;
            rct_life[i].Top = rct_life[i].Bottom + img_life.h;
        }

        rct.Bottom = 120.0f;
        rct.Top = rct.Bottom + h;

        rct_shield.Bottom = rct.Bottom;
        rct_shield.Top = rct_shield.Bottom + shield_h;

        // srand(time(0));

        reload();
    }

    void reload() {
        // srand(0);

        score[0] = score[1] = score[2] = '0';
        stt = 0;
        drt = 1;
        num = 0;
        x = WIDTH / 2.0f;
        rct.Left = x - w / 2;
        rct.Right = rct.Left + w;
        is_draw = true;
        has_shield = false;
        life = 3;
        shield_num = shield_count = bomb_count = 0;
        invul_count = 0;
        inc_score_count = 0;
        timer_num = timer_score = 0;

        reward = 0.0f;
        lose = -1.0f;
    }

    void inc_score() {
        score[2] += 1;
        if (score[2] > '9') {
            score[2] = '0';
            score[1] += 1;
            if (score[1] > '9') {
                score[1] = '0';
                score[0] += 1;
            }
        }

        reward += 0.02f;
    }

    void inc_score_item(float item_left, float item_top) {
        inc_score();
        inc_score();
        inc_score_alpha = 1.0f;
        inc_score_count = 20;
        inc_score_x = item_left + 9.0f;
        inc_score_y = item_top + 18.0f;
    }

    void run_left() {
        stt = PLAYER_STT_RUN;
        if (drt != LEFT) {
            drt = LEFT;
            num = 0;
        }
    }

    void run_right() {
        stt = PLAYER_STT_RUN;
        if (drt != RIGHT) {
            drt = RIGHT;
            num = 0;
        }
    }

    void stand() {
        if (stt != PLAYER_STT_STAND) {
            stt = PLAYER_STT_STAND;
            num = 0;
        }
    }

    void collide_ball() {
        if (has_shield) {
            has_shield = 0;
        } else {
            life--;

            reward = 0.0f;
            lose = 1.0f;
        }
        if (life == 0) {
            stt = PLAYER_STT_DEATH;
            num = 0;
            // Lose
        } else
            invul_count = 14;
    }

    void render() {
        if (is_draw) {
            Map_Texture(&img[stt][drt][num]);
            Draw_Rect(&rct);
            if (has_shield) {
                rct_shield.Left = rct.Left - 3.0f;
                rct_shield.Right = rct_shield.Left + shield_w;
                Map_Texture(&img_shield[shield_num]);
                Draw_Rect(&rct_shield);
            }
        }

        Map_Texture(&img_life);
        for (int i = 0; i < life; i++)
            Draw_Rect(&rct_life[i]);

        glDisable(GL_TEXTURE_2D);
        glTranslatef(270.0f, 540.0f, 0.0f);
        glScalef(0.3f, 0.3f, 1.0f);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, score[0]);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, score[1]);
        glutStrokeCharacter(GLUT_STROKE_ROMAN, score[2]);
        if (inc_score_count > 0) {
            glLoadIdentity();
            glColor4f(1.0f, 1.0f, 1.0f, inc_score_alpha);
            glTranslatef(inc_score_x, inc_score_y, 0.0f);
            glScalef(0.12f, 0.12f, 1.0f);
            glutStrokeCharacter(GLUT_STROKE_ROMAN, '+');
            glutStrokeCharacter(GLUT_STROKE_ROMAN, '2');
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        glEnable(GL_TEXTURE_2D);
    }

    void update() {
        if (inc_score_count > 0) {
            inc_score_alpha -= 0.05f;
            inc_score_y += 3.0f;
            inc_score_count -= 1;
        }
        shield_num += 1;
        if (shield_num == 20)
            shield_num = 0;
        timer_num++;
        if (timer_num == 4) {
            timer_num = 0;
            if (life > 0) {
                if (invul_count > 0) {
                    is_draw = !is_draw;
                    invul_count--;
                }
                num += 1;
                if (num == 6)
                    num = 0;
                timer_score++;
                if (timer_score == 10) {
                    timer_score = 0;
                    inc_score();
                }
            }
        }
        if (stt == PLAYER_STT_RUN) {
            if (drt == LEFT) {
                if (x > 37.0f) {
                    x -= 6.0f;
                    rct.Left -= 6.0f;
                    rct.Right -= 6.0f;
                }
            } else {
                if (x < 563.0f) {
                    x += 6.0f;
                    rct.Left += 6.0f;
                    rct.Right += 6.0f;
                }
            }
        }
    }
};

class Ball {
  public:
    static Player *player;
    static Image img;
    static std::vector<Ball> balls;
    static int timer_spawn;

    float x, y, vx, vy, ay, offset;
    bool has_offset;
    Rect rct;

    static void init(Player *_player) {
        player = _player;
        Load_Texture_Swap(&img, "Images/Ball.png");

        reload();
    }

    static void reload() {
        timer_spawn = 0;
        balls.clear();
    }

    static void spawn() {
        Ball ball;
        int drt = rand() % 2;
        int tmp = 1 - 2 * drt;
        ball.x = drt * WIDTH * 1.0f;
        ball.y = rand() % 58 * 3.0f + 190.0f;
        ball.vx = (6.0f + rand() % 12) * 0.35f * tmp;
        ball.ay = -(70.0f + rand() % 50) * 0.01f;
        ball.vy = ball.ay;
        ball.offset = 0.0f;
        ball.has_offset = 0;

        ball.rct.Left = ball.x - img.w / 2;
        ball.rct.Right = ball.rct.Left + img.w;
        ball.rct.Bottom = ball.y + GROUND_OFFSET;
        ball.rct.Top = ball.rct.Bottom + img.h;

        balls.push_back(ball);
    }

    static void renders() {
        Map_Texture(&img);
        for (Ball ball : balls)
            ball.render();
    }

    static void updates() {
        timer_spawn += 1;
        if (timer_spawn == 160) {
            timer_spawn = 0;
            spawn();
        }

        std::vector<Ball>::iterator it = balls.begin();
        while (it != balls.end()) {
            it->update();
            if (it->is_collision()) {
                player->collide_ball();
                it = balls.erase(it);
            } else
                it++;
        }
    }

    void render() { Draw_Rect(&rct); }

    bool is_collision() {
        if (y < 60.0f && rct.Left - 10.0f < player->x && rct.Right + 10.0f > player->x && player->life > 0 &&
            player->invul_count == 0)
            return true;
        return false;
    }

    bool is_collision_postion(float pos) {
        if (y < 60.0f && rct.Left - 10.0f < pos && rct.Right + 10.0f > pos && player->invul_count == 0)
            return true;
        return false;
    }

    void update() {
        x += vx;
        y += vy;
        rct.Left += vx;
        rct.Right += vx;
        rct.Bottom += vy;
        rct.Top += vy;
        if (vx > 0) {
            if (rct.Right > WIDTH)
                vx = -vx;
        } else {
            if (rct.Left < 0.0f)
                vx = -vx;
        }
        vy += ay;
        if (has_offset) {
            y += offset;
            rct.Bottom += offset;
            rct.Top += offset;
            has_offset = false;
        } else if (y < 0.0f) {
            offset = y;
            y = 0.0f;
            has_offset = 1;
            rct.Bottom = GROUND_OFFSET;
            rct.Top = rct.Bottom + img.h;
            vy = ay - vy;
        }
    }
};

class Miniball {
  public:
    static Miniball miniballs[15];
    static Image img;
    static int explode_count;
    static float explode_alpha, offset_angle;

    float vx, vy, ay;
    Rect rct;

    static void init() {
        offset_angle = 3.141592653f * 2 / 15;
        Load_Texture_Swap(&img, "Images/Miniball.png");
    }

    static void renders() {
        Map_Texture(&img);
        glColor4f(1.0f, 1.0f, 1.0f, explode_alpha);
        for (int i = 0; i < 15; i++)
            miniballs[i].render();
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    static void updates() {
        if (explode_count > 0) {
            explode_count -= 1;
            explode_alpha -= 0.01f;
            for (int i = 0; i < 15; i++)
                miniballs[i].update();
        }
    }

    void reload(float left, float bottom, float _vx, float _vy, float _ay) {
        rct.Left = left + 15.0f;
        rct.Right = left + 24.0f;
        rct.Bottom = bottom + 15.0f;
        rct.Top = bottom + 24.0f;
        vx = _vx;
        vy = _vy;
        ay = _ay;
    }

    void render() { Draw_Rect(&rct); }

    void update() {
        vy += ay;
        rct.Left += vx;
        rct.Right += vx;
        rct.Bottom += vy;
        rct.Top += vy;
        if (rct.Bottom < GROUND_OFFSET) {
            rct.Bottom = GROUND_OFFSET;
            rct.Top = rct.Bottom + img.h;
            vy *= -0.6f;
        }
    }
};

class Item {
  public:
    Player *player;
    std::vector<Ball> *balls;
    Miniball *miniballs;
    Image img[ITEM_MAX];

    int w, h;
    float x;
    int num, timer_spawn;
    int type;
    bool is_alive;
    Rect rct;

    float item_offset[64];

    void init(Player *_player, std::vector<Ball> *_balls, Miniball *_miniballs) {
        get_item[0] = &Item::get_coin;
        get_item[1] = &Item::get_bomb;
        get_item[2] = &Item::get_shield;
        player = _player;
        balls = _balls;
        miniballs = _miniballs;
        int pos[ITEM_MAX][4];
        int *pos_ptr;
        Image img_full, img_tmp;
        FILE *f;

        f = fopen("Images/Item.pos", "r");
        for (int i = 0; i < ITEM_MAX; i++)
            fscanf(f, "%d %d %d %d\n", &pos[i][0], &pos[i][1], &pos[i][2], &pos[i][3]);
        fclose(f);

        Load_Png(&img_full.img, &img_full.w, &img_full.h, "Images/Item.png");
        for (int i = 0; i < ITEM_MAX; i++) {
            pos_ptr = &pos[i][0];
            Crop_Image(&img_full, &img_tmp, *pos_ptr, *(pos_ptr + 1), *(pos_ptr + 2), *(pos_ptr + 3));
            Swap_Image(img_tmp.img, img_tmp.w, img_tmp.h);
            img[i] = img_tmp;
        }
        w = img[0].w;
        h = img[0].h;
        free(img_full.img);

        float _max = 1.8f;
        float a = _max / 16, v = _max, x = 0.0f;
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
            item_offset[c] = x;
            c++;
        }

        reload();
    }

    void reload() {
        num = timer_spawn = 0;
        is_alive = false;
    }

    void (Item::*get_item[ITEM_MAX])();

    void get_coin() { player->inc_score_item(rct.Left, rct.Top); }

    void get_bomb() {
        if (balls->size() == 0)
            player->inc_score_item(rct.Left, rct.Top);
        else {
            player->bomb_count += 1;
            if (player->bomb_count == 2) {
                player->bomb_count = 0;
                Miniball::explode_count = 100;
                Miniball::explode_alpha = 1.0f;
                Ball ball = balls->at(0);
                float left = ball.rct.Left;
                float bottom = ball.rct.Bottom;
                for (int i = 0; i < 15; i++) {
                    float _vx = 1.8f * cos(i * Miniball::offset_angle) + ball.vx / 2;
                    float _vy = 1.5f * sin(i * Miniball::offset_angle) + ball.vy / 2;
                    float _ay = ball.ay / 1.4f;
                    miniballs[i].reload(left, bottom, _vx, _vy, _ay);
                }
                balls->erase(balls->begin());
            }
        }
    }

    void get_shield() {
        if (!player->has_shield) {
            player->shield_count++;
            if (player->shield_count == 3) {
                player->shield_count = 0;
                player->shield_num = 0;
                player->has_shield = true;
            }
        } else
            player->inc_score_item(rct.Left, rct.Top);
    }

    void spawn() {
        num = 0;
        x = rand() % 561 + 20.0f;
        rct.Bottom = item_offset[num] + GROUND_OFFSET;
        rct.Top = rct.Bottom + h;
        rct.Left = x - w / 2;
        rct.Right = rct.Left + w;
        type = rand() % ITEM_MAX;
        is_alive = true;
    }

    void render() {
        if (is_alive) {
            Map_Texture(&img[type]);
            Draw_Rect(&rct);
        }
    }

    void update() {
        if (is_alive) {
            num += 1;
            if (num == 64)
                num = 0;
            rct.Bottom = item_offset[num] + GROUND_OFFSET;
            rct.Top = rct.Bottom + h;
            if (player->x > rct.Left && player->x < rct.Right) {
                (this->*get_item[type])();
                is_alive = false;
                
                player->reward += 0.8f;
            }
        } else {
            timer_spawn++;
            if (timer_spawn == 100) {
                timer_spawn = 0;
                spawn();
            }
        }
    }
};

#endif
