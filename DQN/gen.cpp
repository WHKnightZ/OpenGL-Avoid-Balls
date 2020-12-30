#include "main.h"

#define MAX_DEPTH 100
#define MAX_CHOICE 3

enum CHOICE_MOVE { CHOICE_STAND, CHOICE_LEFT, CHOICE_RIGHT };

float choice_pos[MAX_CHOICE];
float choice_offset[] = {0.0f, -6.0f, 6.0f};
bool choice_done[MAX_CHOICE];
int choice_score[MAX_CHOICE];

std::vector<Ball> balls;
FILE *f;

int episode = 0, iteration = 0;

Player player;

Player *Ball::player;
Image Ball::img;
std::vector<Ball> Ball::balls;
int Ball::timer_spawn;

Miniball Miniball::miniballs[15];
Image Miniball::img;
int Miniball::explode_count;
float Miniball::explode_alpha;
float Miniball::offset_angle;

Item item;

void reload() {
    player.reload();
    Ball::reload();
    item.reload();
}

void init_ai() { f = fopen("data.csv", "w"); }

void init() {
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
    glEnable(GL_TEXTURE_2D);
    glLineWidth(2.0f);

    player.init();
    Ball::init(&player);
    Miniball::init();
    item.init(&player, &Ball::balls, Miniball::miniballs);
    init_ai();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
    glVertex2f(0, 147.0f);
    glVertex2f(WIDTH, 147.0f);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    Miniball::renders();
    Ball::renders();
    item.render();
    player.render();

    glutSwapBuffers();
}

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
    float arr[6700];
    for (int i = 0; i < 6700; i++)
        arr[i] = 0.0f;
    int n, m;
    n = (int)(player.x / 6);
    for (int i = 0; i < 16; i++)
        for (int j = n - 5; j <= n + 5; j++)
            arr[i * 100 + j] = 1.0f;
    if (item.is_alive) {
        n = (int)(item.x / 6);
        for (int i = 0; i <= 6; i++)
            for (int j = n - 3; j <= n + 3; j++)
                arr[i * 100 + j] = 0.3f;
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
                arr[i * 100 + j] = 0.6f;
    }
    for (int i = 0; i < 6700; i++)
        fprintf(f, "%.1f,", arr[i]);
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
    if (iteration > 30)
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
                    if (item.is_alive && *p > item.rct.Left && *p < item.rct.Right)
                        choice_score[i] += 40;
                    choice_score[i] += 1;
                } else {
                    choice_done[i] = 1;
                    count_done += 1;
                }
            }
        }
        n += 1;
    }

    move_player();
}

void timer(int v) {
    if (episode < 30) {
        if (player.lose > 0.0f) {
            reload();
            iteration = 0;
            episode += 1;
        } else {
            calculate_score();

            iteration += 1;
            Miniball::updates();
            Ball::updates();
            item.update();
            player.update();
            // glutPostRedisplay();
        }
        glutTimerFunc(0, timer, 0);
    } else {
        printf("%d %d\n", episode, iteration);
        exit(0);
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    int POS_X, POS_Y;
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Avoid Balls");
    init();
    glutTimerFunc(0, timer, 0);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}