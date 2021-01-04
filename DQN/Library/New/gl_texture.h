#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "lodepng.h"

#include <GL/glut.h>

struct Image {
    GLubyte *img;
    unsigned w, h;
};

struct Rect {
    float left, right, bottom, top;
};

GLuint create_texture(Image *img);
void bind_texture(GLuint texture);
void draw_rect(Rect *rct);
void draw_rect_without_texture(Rect *rct);

Image load_image(const char *path);
void delete_image(Image *img);
void create_image(Image *img, int w, int h);
void create_image_color(Image *img, int w, int h, GLubyte color[]);
void clone_image(Image *source, Image *dest);
void clone_image_shadow(Image *source, Image *dest, GLubyte color[]);
void crop_image(Image *source, Image *dest, int x, int y, int w, int h);
void flip_horizontal(Image *source, Image *dest);
void flip_vertical(Image *source, Image *dest);
void rotate_180(Image *source, Image *dest);
void rotate_left(Image *source, Image *dest);
void rotate_right(Image *source, Image *dest);
void mix_image(Image *bg, Image *fg, int x, int y);
void mix_image_nobg(Image *bg, Image *fg, int x, int y);
void mix_image_color(Image *bg, Image *fg, int x, int y, GLubyte color[]);
void mix_image_color_nobg(Image *bg, Image *fg, int x, int y, GLubyte color[]);
void zoom_image(Image *img, int scale);
void blur_image(Image *src, Image *dst, int radius);

#endif
