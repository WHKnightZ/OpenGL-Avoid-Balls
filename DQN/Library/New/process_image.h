#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

#include <GL/glut.h>

#include "lodepng.h"

struct Image {
    unsigned char *img;
    unsigned w, h;
};

struct Rect {
    float left, right, bottom, top;
};

Image *load_image(const char *path) {
    Image *img;
    lodepng_decode_file(img->img, img->w, img->h, path, LCT_RGBA, 8);
    return img;
}

GLuint create_texture(Image *img) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void bind_texture(texture) { glBindTexture(GL_TEXTURE_2D, texture); }

void draw_rect(Rect *rct) {
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f((int)rct->left, (int)rct->bottom);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f((int)rct->right, (int)rct->bottom);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f((int)rct->right, (int)rct->top);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f((int)rct->left, (int)rct->top);
    glEnd();
}

void draw_rect_without_texture(Rect *rct) {
    glBegin(GL_POLYGON);
    glVertex2f((int)rct->left, (int)rct->bottom);
    glVertex2f((int)rct->right, (int)rct->bottom);
    glVertex2f((int)rct->right, (int)rct->top);
    glVertex2f((int)rct->left, (int)rct->top);
    glEnd();
}

void Create_Image(Image *Img, int w, int h);
void Create_Image_Color(Image *Img, int w, int h, unsigned char color[]);
void Delete_Image(Image *Img);
void Clone_Image(Image *Source, Image *Dest);
void Clone_Image_Shadow(Image *Source, Image *Dest, unsigned char color[]);
void Crop_Image(Image *Source, Image *Dest, int x, int y, int w, int h);
void Flip_Horizontal(Image *Source, Image *Dest);
void Flip_Vertical(Image *Source, Image *Dest);
void Rotate_180(Image *Source, Image *Dest);
void Rotate_Left(Image *Source, Image *Dest);
void Rotate_Right(Image *Source, Image *Dest);
void Mix_Image(Image *BG, Image *FG, int x, int y);
void Mix_Image_NoBG(Image *BG, Image *FG, int x, int y);
void Mix_Image_Color(Image *BG, Image *FG, int x, int y, unsigned char color[]);
void Mix_Image_Color_NoBG(Image *BG, Image *FG, int x, int y, unsigned char color[]);
void Zoom_Image(Image *Img, int Scale);
void Blur_Image(Image *Src, Image *Dst, int Radius);

#endif
