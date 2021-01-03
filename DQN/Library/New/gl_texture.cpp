#include "gl_texture.h"

GLuint create_texture(Image *img) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void bind_texture(GLuint texture) { glBindTexture(GL_TEXTURE_2D, texture); }

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

Image load_image(const char *path) {
    Image img;
    lodepng_decode_file(&img.img, &img.w, &img.h, path, LCT_RGBA, 8);
    return img;
}

void delete_image(Image *img) { free(img->img); }

void create_image(Image *img, int w, int h) {
    img->w = w;
    img->h = h;
    int i, size = w * h, size_4 = size * 4;
    img->img = (GLubyte *)malloc(size_4);
    GLubyte *img_ptr = img->img;
    for (i = 0; i < size; i++) {
        *img_ptr++ = 255;
        *img_ptr++ = 255;
        *img_ptr++ = 255;
        *img_ptr++ = 0;
    }
}

void create_image_color(Image *img, int w, int h, GLubyte color[]) {
    img->w = w;
    img->h = h;
    int i, size = w * h, size_4 = size * 4;
    img->img = (GLubyte *)malloc(size_4);
    GLubyte *img_ptr = img->img;
    GLubyte color_0 = color[0], color_1 = color[1], color_2 = color[2], color_3 = color[3];
    for (i = 0; i < size; i++) {
        *img_ptr++ = color_0;
        *img_ptr++ = color_1;
        *img_ptr++ = color_2;
        *img_ptr++ = color_3;
    }
}

void clone_image(Image *source, Image *dest) {
    int i, size = source->w * source->h * 4;
    dest->w = source->w;
    dest->h = source->h;
    dest->img = (GLubyte *)malloc(size);
    GLubyte *img_s = source->img, *img_d = dest->img;
    for (i = 0; i < size; i++)
        *img_d++ = *img_s++;
}

void clone_image_shadow(Image *source, Image *dest, GLubyte color[]) {
    int i, size = source->w * source->h;
    dest->w = source->w;
    dest->h = source->h;
    dest->img = (GLubyte *)malloc(size * 4);
    GLubyte *img_s = source->img, *img_d = dest->img;
    GLubyte r = color[0], g = color[1], b = color[2], a = color[3];
    for (i = 0; i < size; i++) {
        img_d[0] = r;
        img_d[1] = g;
        img_d[2] = b;
        img_d[3] = a * img_s[3] / 255;
        img_s += 4;
        img_d += 4;
    }
}

void crop_image(Image *source, Image *dest, int x, int y, int w, int h) {
    int i, j, w4 = source->w * 4, start;
    dest->w = w;
    dest->h = h;
    dest->img = (GLubyte *)malloc(w * h * 4);
    GLubyte *img_s = source->img, *img_d = dest->img, *img_ptr;
    start = w4 * y + x * 4;
    for (i = 0; i < h; i++) {
        img_ptr = img_s + start;
        start += w4;
        for (j = 0; j < w; j++) {
            *img_d++ = *img_ptr++;
            *img_d++ = *img_ptr++;
            *img_d++ = *img_ptr++;
            *img_d++ = *img_ptr++;
        }
    }
}

void flip_horizontal(Image *source, Image *dest) {
    int i, j, w = source->w, h = source->h, w4 = w * 4, start = w4 - 4;
    dest->w = w;
    dest->h = h;
    dest->img = (GLubyte *)malloc(w * h * 4);
    GLubyte *img_s = source->img, *img_d, *img_ptr = dest->img;
    for (i = 0; i < h; i++) {
        img_d = img_ptr + start;
        for (j = 0; j < w; j++) {
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            img_d -= 8;
        }
        start += w4;
    }
}

void flip_vertical(Image *source, Image *dest) {
    int i, j, w = source->w, h = source->h, w4 = w * 4, start = (h - 1) * w * 4;
    dest->w = w;
    dest->h = h;
    dest->img = (GLubyte *)malloc(w * h * 4);
    GLubyte *img_s = source->img, *img_d, *img_ptr = dest->img;
    for (i = 0; i < h; i++) {
        img_d = img_ptr + start;
        for (j = 0; j < w; j++) {
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
        }
        start -= w4;
    }
}

void rotate_180(Image *source, Image *dest) {
    int i, j, w = source->w, h = source->h;
    dest->w = w;
    dest->h = h;
    dest->img = (GLubyte *)malloc(w * h * 4);
    GLubyte *img_s = source->img, *img_d = dest->img + w * h * 4 - 4;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            img_d -= 8;
        }
    }
}

void rotate_left(Image *source, Image *dest) {
    int i, j, w = source->w, h = source->h, h4 = h * 4 + 4, start = (w - 1) * h * 4;
    dest->w = h;
    dest->h = w;
    dest->img = (GLubyte *)malloc(w * h * 4);
    GLubyte *img_s = source->img, *img_d, *img_ptr = dest->img;
    for (i = 0; i < h; i++) {
        img_d = img_ptr + start;
        for (j = 0; j < w; j++) {
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            img_d -= h4;
        }
        start += 4;
    }
}

void rotate_right(Image *source, Image *dest) {
    int i, j, w = source->w, h = source->h, h4 = h * 4 - 4, start = h4;
    dest->w = h;
    dest->h = w;
    dest->img = (GLubyte *)malloc(w * h * 4);
    GLubyte *img_s = source->img, *img_d, *img_ptr = dest->img;
    for (i = 0; i < h; i++) {
        img_d = img_ptr + start;
        for (j = 0; j < w; j++) {
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            *img_d++ = *img_s++;
            img_d += h4;
        }
        start -= 4;
    }
}

// need fix, has bug
void mix_image(Image *bg, Image *fg, int x, int y) {
    int i, j, w_bg = bg->w, w_fg = fg->w, h_fg = fg->h;
    GLubyte *img1 = bg->img + w_bg * 4 * y + x * 4, *img2 = fg->img;
    GLubyte tmp;
    for (i = 0; i < h_fg; i++) {
        for (j = 0; j < w_fg; j++) {
            tmp = img1[3];
            img1[3] = img1[3] + img2[3] - img1[3] * img2[3] / 255;
            if (img1[3] != 0) {
                img1[0] = (img2[0] * img2[3] + img1[0] * tmp * (255 - img2[3]) / 255) / img1[3];
                img1[1] = (img2[1] * img2[3] + img1[1] * tmp * (255 - img2[3]) / 255) / img1[3];
                img1[2] = (img2[2] * img2[3] + img1[2] * tmp * (255 - img2[3]) / 255) / img1[3];
            } else {
                img1[0] = img2[0];
                img1[1] = img2[1];
                img1[2] = img2[2];
            }
            img1 += 4;
            img2 += 4;
        }
        img1 += (w_bg - w_fg) * 4;
    }
}

void mix_image_nobg(Image *bg, Image *fg, int x, int y) {
    int i, j, w_bg = bg->w, w_fg = fg->w, h_fg = fg->h;
    GLubyte *img1 = bg->img + w_bg * 4 * y + x * 4, *img2 = fg->img;
    for (i = 0; i < h_fg; i++) {
        for (j = 0; j < w_fg; j++) {
            img1[0] = img2[0];
            img1[1] = img2[1];
            img1[2] = img2[2];
            img1[3] = img2[3];
            img1 += 4;
            img2 += 4;
        }
        img1 += (w_bg - w_fg) * 4;
    }
}

void mix_image_color(Image *bg, Image *fg, int x, int y, GLubyte color[]) {
    int i, j, w_bg = bg->w, w_fg = fg->w, h_fg = fg->h;
    GLubyte *img1 = bg->img + w_bg * 4 * y + x * 4, *img2 = fg->img;
    GLubyte tmp;
    GLubyte color_0 = color[0], color_1 = color[1], color_2 = color[2], color_3 = color[3];
    for (i = 0; i < h_fg; i++) {
        for (j = 0; j < w_fg; j++) {
            tmp = img1[3];
            img1[3] = img1[3] + img2[3] - img1[3] * img2[3] / 255;
            if (img1[3] != 0) {
                img1[0] = (color_0 * img2[3] + img1[0] * tmp * (255 - img2[3]) / 255) / img1[3];
                img1[1] = (color_1 * img2[3] + img1[1] * tmp * (255 - img2[3]) / 255) / img1[3];
                img1[2] = (color_2 * img2[3] + img1[2] * tmp * (255 - img2[3]) / 255) / img1[3];
            } else {
                img1[0] = color_0;
                img1[1] = color_1;
                img1[2] = color_2;
            }
            img1[3] = img1[3] * color_3 / 255;
            img1 += 4;
            img2 += 4;
        }
        img1 += (w_bg - w_fg) * 4;
    }
}

void mix_image_color_nobg(Image *bg, Image *fg, int x, int y, GLubyte color[]) {
    int i, j, w_bg = bg->w, w_fg = fg->w, h_fg = fg->h;
    GLubyte *img1 = bg->img + w_bg * 4 * y + x * 4, *img2 = fg->img;
    GLubyte color_0 = color[0], color_1 = color[1], color_2 = color[2], color_3 = color[3];
    for (i = 0; i < h_fg; i++) {
        for (j = 0; j < w_fg; j++) {
            img1[0] = color_0;
            img1[1] = color_1;
            img1[2] = color_2;
            img1[3] = color_3 * img2[3] / 255;
            img1 += 4;
            img2 += 4;
        }
        img1 += (w_bg - w_fg) * 4;
    }
}

void zoom_image(Image *img, int scale) {
    unsigned w = img->w, h = img->h;
    int size = w * h;
    GLubyte *img_ptr = img->img, *img_save = img_ptr;
    GLubyte *img1 = (GLubyte *)malloc(size * scale * scale * 4), *img2;
    int i, j, k, x, y, r, g, b, a;
    int scale4 = 4 * scale, element_per_row = w * scale4, element_per_scale_row = element_per_row * scale;
    for (i = 0; i < size; i++) {
        x = i % w;
        y = i / w;
        r = img_ptr[0];
        g = img_ptr[1];
        b = img_ptr[2];
        a = img_ptr[3];
        img2 = img1 + y * element_per_scale_row + x * scale4;
        for (j = 0; j < scale; j++) {
            for (k = 0; k < scale; k++) {
                img2[0] = r;
                img2[1] = g;
                img2[2] = b;
                img2[3] = a;
                img2 += 4;
            }
            img2 += (element_per_row - scale4);
        }
        img_ptr += 4;
    }
    free(img_save);
    img->img = img1;
    img->w = w * scale;
    img->h = h * scale;
}

void blur_image(Image *src, Image *dst, int radius) {}