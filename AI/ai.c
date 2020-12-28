#include "main.h"

#define AI_MAX_BALL 100
#define AI_MAX_DEPTH 100
#define AI_MAX_CHOICE 3

enum AI_CHOICE_MOVE {
    CHOICE_STAND,
    CHOICE_LEFT,
    CHOICE_RIGHT
};

int AI_Check_Collision_Edge_Stand();
int AI_Check_Collision_Edge_Left();
int AI_Check_Collision_Edge_Right();

int (*AI_Check_Collision_Edge_Func[])() = {AI_Check_Collision_Edge_Stand, AI_Check_Collision_Edge_Left, AI_Check_Collision_Edge_Right};

float AI_Choice_Pos[AI_MAX_CHOICE];
float AI_Choice_Offset_Old[] = {0.0f, -30.0f, 30.0f};
float AI_Choice_Offset[] = {0.0f, -6.0f, 6.0f};

int AI_Choice_Is_Stop[AI_MAX_CHOICE];
int AI_Choice_Score[AI_MAX_CHOICE];

s_Ball AI_Array_Ball[AI_MAX_BALL];
s_Ball *AI_Tmp_Ball;
int AI_Count_Ball;

void AI_Set_Array_Ball(s_Ball *Ball_Dst, s_Ball *Ball_Src) {
    Ball_Dst->Rct = Ball_Src->Rct;
    Ball_Dst->ay = Ball_Src->ay;
    Ball_Dst->vx = Ball_Src->vx;
    Ball_Dst->vy = Ball_Src->vy;
    Ball_Dst->Offset = Ball_Src->Offset;
    Ball_Dst->Has_Offset = Ball_Src->Has_Offset;
}

void AI_Create_Array_Ball() {
    AI_Count_Ball = 0;
    AI_Tmp_Ball = List_Ball;
    while (AI_Tmp_Ball != NULL) {
        AI_Set_Array_Ball(&AI_Array_Ball[AI_Count_Ball], AI_Tmp_Ball);
        AI_Tmp_Ball = AI_Tmp_Ball->Next;
        AI_Count_Ball++;
    }
}

int AI_Collision_Ball(float p) {
    int i;
    for (i = 0; i < AI_Count_Ball; i++)
        if (Collision_Ball(&AI_Array_Ball[i], &p))
            return 1;
    return 0;
}

int AI_Find_Max() {
    // When 3 choices have same score, STAND is high priority
    int n = CHOICE_STAND;
    if (AI_Choice_Score[n] < AI_Choice_Score[CHOICE_LEFT])
        n = CHOICE_LEFT;
    if (AI_Choice_Score[n] < AI_Choice_Score[CHOICE_RIGHT])
        n = CHOICE_RIGHT;
    return n;
}

void AI_Move_Player() {
    int n = AI_Find_Max();
    switch (n) {
    case CHOICE_STAND:
        Player.Stt = PLAYER_STT_STAND;
        break;
    case CHOICE_LEFT:
        Player.Stt = PLAYER_STT_RUN;
        Player.Drt = LEFT;
        break;
    case CHOICE_RIGHT:
        Player.Stt = PLAYER_STT_RUN;
        Player.Drt = RIGHT;
        break;
    }
}

void AI_Calculate_Score_Old() {
/*
	Only check 2 position offset by 30.0f to the left and right
	Find the posposition which can be stood as long as possible
*/
    AI_Create_Array_Ball();
    int i, n = 0, Count_Stop = 0;
    for (i = 0; i < AI_MAX_CHOICE; i++) {
        AI_Choice_Score[i] = 0;
        AI_Choice_Is_Stop[i] = 0;
    }
    while (n < AI_MAX_DEPTH && Count_Stop < AI_MAX_CHOICE) {
        for (i = 0; i < AI_Count_Ball; i++) {
            Move_Ball(&AI_Array_Ball[i]);
        }
        for (i = 0; i < AI_MAX_CHOICE; i++) {
            if (!AI_Choice_Is_Stop[i]) {
                if (!AI_Collision_Ball(Player.p + AI_Choice_Offset_Old[i]))
                    AI_Choice_Score[i]++;
                else {
                    AI_Choice_Is_Stop[i] = 1;
                    Count_Stop++;
                }
            }
        }
        n++;
    }
    AI_Move_Player();
}

void AI_Calculate_Score() {
/*
	Check entire map and item
*/
    AI_Create_Array_Ball();
    int i, n = 0, Count_Stop = 0;
    float *p;
    for (i = 0; i < AI_MAX_CHOICE; i++) {
        AI_Choice_Pos[i] = Player.p;
        AI_Choice_Score[i] = 0;
        AI_Choice_Is_Stop[i] = 0;
    }
    while (n < AI_MAX_DEPTH && Count_Stop < AI_MAX_CHOICE) {
        for (i = 0; i < AI_Count_Ball; i++) {
            Move_Ball(&AI_Array_Ball[i]);
        }
        for (i = 0; i < AI_MAX_CHOICE; i++) {
            if (!AI_Choice_Is_Stop[i]) {
                p = &AI_Choice_Pos[i];
                *p += AI_Choice_Offset[i];
                if (!AI_Collision_Ball(*p) && !AI_Check_Collision_Edge_Func[i]()) {
                    if (Item.Is_Alive && *p > Item.Rct.Left && *p < Item.Rct.Right) {
                        AI_Choice_Score[i] += 100;
                        AI_Choice_Is_Stop[i] = 1;
                        Count_Stop++;
                    }
                    AI_Choice_Score[i]++;
                } else {
                    AI_Choice_Is_Stop[i] = 1;
                    Count_Stop++;
                }
            }
        }
        n++;
    }
    printf("%d %d %d\n", AI_Choice_Score[0], AI_Choice_Score[1], AI_Choice_Score[2]);
    // Can combine two method to improve AI, if STAND is better choice then should check 2 adjacent positions
    // if (AI_Find_Max() == PLAYER_STT_STAND)
    //    AI_Calculate_Score_Old();
    AI_Move_Player();
}

int AI_Check_Collision_Edge_Stand() {
    return 0;
}

int AI_Check_Collision_Edge_Left() {
    if (AI_Choice_Pos[CHOICE_LEFT] > 30.0f)
        return 0;
    return 1;
}

int AI_Check_Collision_Edge_Right() {
    if (AI_Choice_Pos[CHOICE_RIGHT] < 570.0f)
        return 0;
    return 1;
}
