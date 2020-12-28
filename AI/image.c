#include "main.h"

void Load_Image_Logo() {
    int Pos[][4] = {
        {0, 0, 47, 56},
        {47, 0, 43, 56},
        {90, 0, 40, 56},
        {130, 0, 31, 56},
        {161, 0, 40, 56},
        {201, 0, 41, 56},
        {242, 0, 39, 56},
        {281, 0, 32, 56},
        {313, 0, 34, 56}};
    int *Ptr_Pos;
    Image Img, Img_Tmp;
    Load_Png(&Img.img, &Img.w, &Img.h, "Images/Logo.png");
    int i;
    for (i = 0; i < 8; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Logo[i].Img = Img_Tmp;
    }
    Logo[8].Img = Logo[7].Img;
    Ptr_Pos = &Pos[8][0];
    Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
    Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
    Logo[9].Img = Img_Tmp;
    free(Img.img);
}

void Load_Image_Player() {
    int Pos[][4] = {
        {0, 0, 64, 96},
        {64, 0, 64, 96},
        {128, 0, 64, 96},
        {192, 0, 64, 96},
        {256, 0, 64, 96},
        {320, 0, 64, 96},
        {0, 96, 64, 96},
        {64, 96, 64, 96},
        {128, 96, 64, 96},
        {192, 96, 64, 96},
        {256, 96, 64, 96},
        {320, 96, 64, 96},
        {0, 192, 64, 96},
        {64, 192, 64, 96},
        {128, 192, 64, 96},
        {192, 192, 64, 96},
        {256, 192, 64, 96},
        {320, 192, 64, 96},
        {0, 288, 64, 96},
        {64, 288, 64, 96},
        {128, 288, 64, 96},
        {192, 288, 64, 96},
        {256, 288, 64, 96},
        {320, 288, 64, 96},
        {0, 384, 64, 96},
        {64, 384, 64, 96},
        {128, 384, 64, 96},
        {192, 384, 64, 96},
        {256, 384, 64, 96},
        {320, 384, 64, 96},
        {0, 480, 64, 96},
        {64, 480, 64, 96},
        {128, 480, 64, 96},
        {192, 480, 64, 96},
        {256, 480, 64, 96},
        {320, 480, 64, 96}};
    int *Ptr_Pos;
    Image Img, Img_Tmp;
    Load_Png(&Img.img, &Img.w, &Img.h, "Images/Player.png");
    int i;
    for (i = 0; i < 6; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Player[PLAYER_STT_STAND][LEFT][i] = Img_Tmp;
        Ptr_Pos = &Pos[i + 6][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Player[PLAYER_STT_STAND][RIGHT][i] = Img_Tmp;
        Ptr_Pos = &Pos[i + 12][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Player[PLAYER_STT_RUN][LEFT][i] = Img_Tmp;
        Ptr_Pos = &Pos[i + 18][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Player[PLAYER_STT_RUN][RIGHT][i] = Img_Tmp;
        Ptr_Pos = &Pos[i + 24][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Player[PLAYER_STT_DEATH][LEFT][i] = Img_Tmp;
        Ptr_Pos = &Pos[i + 30][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Player[PLAYER_STT_DEATH][RIGHT][i] = Img_Tmp;
    }
    Player.w = Img_Player[PLAYER_STT_STAND][LEFT][0].w;
    Player.h = Img_Player[PLAYER_STT_STAND][LEFT][0].h;
    free(Img.img);
}

void Load_Image_Shield() {
    int Pos[][4] = {
        {0, 0, 70, 92},
        {70, 0, 70, 92},
        {140, 0, 70, 92},
        {210, 0, 70, 92},
        {280, 0, 70, 92},
        {0, 92, 70, 92},
        {70, 92, 70, 92},
        {140, 92, 70, 92},
        {210, 92, 70, 92},
        {280, 92, 70, 92},
        {0, 184, 70, 92},
        {70, 184, 70, 92},
        {140, 184, 70, 92},
        {210, 184, 70, 92},
        {280, 184, 70, 92},
        {0, 276, 70, 92},
        {70, 276, 70, 92},
        {140, 276, 70, 92},
        {210, 276, 70, 92},
        {280, 276, 70, 92}};
    int *Ptr_Pos;
    Image Img, Img_Tmp;
    Load_Png(&Img.img, &Img.w, &Img.h, "Images/Shield.png");
    int i;
    for (i = 0; i < 20; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Shield[i] = Img_Tmp;
    }
    Player.Shield_w = Img_Shield[0].w;
    Player.Shield_h = Img_Shield[0].h;
    free(Img.img);
}

void Load_Image_Item() {
    int Pos[][4] = {
        {0, 0, 42, 46},
        {42, 0, 42, 46},
        {84, 0, 42, 46}};
    int *Ptr_Pos;
    Image Img, Img_Tmp;
    Load_Png(&Img.img, &Img.w, &Img.h, "Images/Item.png");
    int i;
    for (i = 0; i < ITEM_MAX; i++) {
        Ptr_Pos = &Pos[i][0];
        Crop_Image(&Img, &Img_Tmp, *Ptr_Pos, *(Ptr_Pos + 1), *(Ptr_Pos + 2), *(Ptr_Pos + 3));
        Swap_Image(Img_Tmp.img, Img_Tmp.w, Img_Tmp.h);
        Img_Item[i] = Img_Tmp;
    }
    Item.w = Img_Item[0].w;
    Item.h = Img_Item[0].h;
    free(Img.img);
}

void Load_Image() {
    Load_Image_Logo();
    Load_Image_Player();
    Load_Image_Shield();
    Load_Image_Item();
    Load_Texture_Swap(&Img_Ball, "Images/Ball.png");
    Load_Texture_Swap(&Img_Life, "Images/Life.png");
    Load_Texture_Swap(&Img_Miniball, "Images/Miniball.png");
}
