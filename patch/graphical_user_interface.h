#pragma once

#include <CSFML/Graphics.h>

#define WIDTH 800
#define HEIGHT 600
#define FRAME_COUNT 49
#define FRAME_DELAY 0.1f

typedef struct
{
    sfRectangleShape *shape;
    sfText *text;
    sfFloatRect bounds;
    int is_hovered;
} Button;

typedef struct
{
    sfTexture **textures;
    sfSprite *sprite;
    int current_frame;
    float timer;
    int frame_count;
    int is_playing;
} Animation;

Button *CreateButton(const char *label, sfFont *font, float x, float y, float width, float height);
void UpdateButtonHover(Button *btn, sfVector2i mouse_pos);
int IsButtonClicked(Button *btn, sfVector2i mouse_pos);
void DestroyButton(Button *btn);
Animation *CreateAnimation(const char *folder, int start, int count);
void DestroyAnimation(Animation *anim);
void UpdateAnimation(Animation *anim, float delta_time);
