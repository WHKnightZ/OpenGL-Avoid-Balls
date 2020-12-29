#include "main.h"

// #define GEN_DATA

#ifdef GEN_DATA
int INTERVAL = 0;
#else
int INTERVAL = 10;
#endif

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

int sender, receiver;
sockaddr_in addr_out, addr_in;
int opt = 1;

#ifdef GEN_DATA
#include "ai.cpp"
#endif

void send_and_receive() {
    float arr[67][100];
    for (int i = 0; i < 67; i++)
        for (int j = 0; j < 100; j++)
            arr[i][j] = 0.0f;
    int n, m;
    n = (int)(player.x / 6);
    for (int i = 0; i < 16; i++)
        for (int j = n - 5; j <= n + 5; j++)
            arr[i][j] = 1.0f;
    if (item.is_alive) {
        n = (int)(item.x / 6);
        for (int i = 0; i <= 6; i++)
            for (int j = n - 3; j <= n + 3; j++)
                arr[i][j] = 0.3f;
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
                arr[i][j] = 0.6f;
    }
    send(sender, arr, sizeof(arr), 0);
    int value;
    recv(receiver, &value, sizeof(value), 0);
    switch (value) {
    case 0:
        player.stand();
        break;
    case 1:
        player.run_left();
        break;
    case 2:
        player.run_right();
        break;
    }
}

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

#ifndef GEN_DATA
    sender = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sender, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    addr_out.sin_family = AF_INET;
    addr_out.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_out.sin_port = htons(9003);
    connect(sender, (sockaddr *)&addr_out, sizeof(addr_out));

    receiver = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(receiver, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_in.sin_port = htons(9004);
    bind(receiver, (sockaddr *)&addr_in, sizeof(addr_in));
#else
    init_ai();
#endif
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

void timer(int value) {
    if (player.life > 0) {
        Miniball::updates();
        Ball::updates();
        item.update();
        player.update();
#ifdef GEN_DATA
        calculate_score();
#else
        send_and_receive();
        glutPostRedisplay();
#endif
        glutTimerFunc(INTERVAL, timer, 0);
    } else {
#ifdef GEN_DATA
        exit(0);
#endif
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
