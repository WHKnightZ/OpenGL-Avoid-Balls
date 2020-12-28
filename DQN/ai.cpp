#include "main.h"

#define MAX_DEPTH 100
#define MAX_CHOICE 3

enum CHOICE_MOVE { CHOICE_STAND, CHOICE_LEFT, CHOICE_RIGHT };

// extern Player player;
// extern Item item;

float choice_pos[MAX_CHOICE];
float choice_offset[] = {0.0f, -6.0f, 6.0f};
bool choice_done[MAX_CHOICE];
int choice_score[MAX_CHOICE];

std::vector<Ball> balls;
FILE *f;

void init_ai() { f = fopen("data.csv", "w"); }

bool check_collision_edge_stand() { return false; }

bool check_collision_edge_left() {
    if (choice_pos[CHOICE_LEFT] > 30.0f)
        return false;
    return true;
}

bool check_collision_edge_right() {
    if (choice_pos[CHOICE_RIGHT] < 570.0f)
        return false;
    return true;
}

bool (*check_collision_edge[])() = {check_collision_edge_stand, check_collision_edge_left, check_collision_edge_right};

int find_max() {
    int n = CHOICE_STAND;
    if (choice_score[n] < choice_score[CHOICE_LEFT])
        n = CHOICE_LEFT;
    if (choice_score[n] < choice_score[CHOICE_RIGHT])
        n = CHOICE_RIGHT;
    return n;
}

void gen_map(int choice) {
    float arr[67][100];
    for (int i = 0; i < 67; i++)
        for (int j = 0; j < 100; j++)
            arr[i][j] = -1.0f;
    int n, m;
    n = (int)(player.x / 6);
    for (int i = 0; i < 16; i++)
        for (int j = n - 5; j <= n + 5; j++)
            arr[i][j] = 0.0f;
    if (item.is_alive) {
        n = (int)(item.x / 6);
        for (int i = 0; i <= 6; i++)
            for (int j = n - 3; j <= n + 3; j++)
                arr[i][j] = 0.5f;
    }
    for (Ball ball : Ball::balls) {
        m = (int)(ball.x / 6);
        n = (int)(ball.y / 6);
        if (m < 3)
            m = 3;
        else if (m > 96)
            m = 96;
        for (int i = n; i <= n + 6; i++)
            for (int j = m - 3; j <= m + 3; j++)
                arr[i][j] = 1.0f;
    }
    for (int i = 0; i < 67; i++)
        for (int j = 0; j < 100; j++)
            fprintf(f, "%.1f,", arr[i][j]);
    fprintf(f, "%d\n", choice);
}

void move_player() {
    int n = find_max();
    switch (n) {
    case CHOICE_STAND:
        player.stand();
        break;
    case CHOICE_LEFT:
        player.run_left();
        break;
    case CHOICE_RIGHT:
        player.run_right();
        break;
    }
    gen_map(n);
}

int is_collision(float p) {
    for (Ball ball : balls)
        if (ball.is_collision_postion(p))
            return true;
    return false;
}

void calculate_score() {
    balls.clear();
    for (Ball ball : Ball::balls)
        balls.push_back(ball);
    int n = 0, count_done = 0;
    float *p;
    for (int i = 0; i < MAX_CHOICE; i++) {
        choice_pos[i] = player.x;
        choice_score[i] = 0;
        choice_done[i] = false;
    }
    while (n < MAX_DEPTH && count_done < MAX_CHOICE) {
        for (int i = 0; i < balls.size(); i++)
            balls[i].update();
        for (int i = 0; i < MAX_CHOICE; i++) {
            if (!choice_done[i]) {
                p = &choice_pos[i];
                *p += choice_offset[i];
                if (!is_collision(*p) && !check_collision_edge[i]()) {
                    if (item.is_alive && *p > item.rct.Left && *p < item.rct.Right) {
                        choice_score[i] += 40;
                        // choice_done[i] = 1;
                        // count_done += 1;
                    }
                    choice_score[i] += 1;
                } else {
                    choice_done[i] = 1;
                    count_done += 1;
                }
            }
        }
        n += 1;
    }
    // printf("%d %d %d\n", choice_score[0], choice_score[1], choice_score[2]);

    move_player();
}
