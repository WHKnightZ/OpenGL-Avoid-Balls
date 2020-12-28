#include "main.h"

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    Game_Display_Func[Game_State]();
    glutSwapBuffers();
}

void Timer(int value) {
    Game_Process_Func[Game_State]();
    glutPostRedisplay();
    glutTimerFunc(Interval, Timer, 0);
}

void Keyboard_Menu(unsigned char key, int x, int y) {
    Score[0] = Score[1] = Score[2] = '0';
    Player.Life = 3;
    Game_State = GAME_STT_PREPARE;
    glutKeyboardFunc(NULL);
}

void Keyboard_Restart(unsigned char key, int x, int y) {
    Flick_Text = 0;
    Reload_Player();
    Init_Game();
    glutKeyboardFunc(NULL);
}

void Special_Down(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        Player.Stt = PLAYER_STT_RUN;
        if (Player.Drt != LEFT) {
            Player.Drt = LEFT;
            Player.Num = 0;
        }
        break;
    case GLUT_KEY_RIGHT:
        Player.Stt = PLAYER_STT_RUN;
        if (Player.Drt != RIGHT) {
            Player.Drt = RIGHT;
            Player.Num = 0;
        }
        break;
    }
}

void Special_Up(int key, int x, int y) {
    if (Player.Stt == PLAYER_STT_RUN)
        switch (key) {
        case GLUT_KEY_LEFT:
            if (Player.Drt == LEFT) {
                Player.Stt = PLAYER_STT_STAND;
                Player.Num = 0;
            }
            break;
        case GLUT_KEY_RIGHT:
            if (Player.Drt == RIGHT) {
                Player.Stt = PLAYER_STT_STAND;
                Player.Num = 0;
            }
            break;
        }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Avoid Balls");
    Init_Global();
    glutTimerFunc(0, Timer, 0);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}
