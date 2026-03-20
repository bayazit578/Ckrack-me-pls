#include "graphical_user_interface.h"

#include <stdlib.h>
#include <stdio.h>
#include <CSFML/Graphics.h>



Button *CreateButton(const char *label, sfFont *font, float x, float y, float width, float height) {
    Button *btn = (Button *)malloc(sizeof(Button));
    if (!btn)
        return NULL;

    btn->shape = sfRectangleShape_create(); //rectangle for knopkas
    sfRectangleShape_setSize(btn->shape, (sfVector2f){width, height});
    sfRectangleShape_setPosition(btn->shape, (sfVector2f){x, y});
    sfRectangleShape_setFillColor(btn->shape, (sfColor){50, 50, 50, 255});
    sfRectangleShape_setOutlineColor(btn->shape, sfGreen);
    sfRectangleShape_setOutlineThickness(btn->shape, 2.0f);

    btn->text = sfText_create(font); //text_create
    sfText_setString(btn->text, label);
    sfText_setCharacterSize(btn->text, 40);
    sfText_setFillColor(btn->text, sfWhite);

    sfFloatRect text_bounds = sfText_getLocalBounds(btn->text); //text centring
    float text_x = x + (width - text_bounds.size.x) / 2.0f;
    float text_y = y + (height - text_bounds.size.y) / 2.0f - 5;
    sfText_setPosition(btn->text, (sfVector2f){text_x, text_y});

    btn->bounds.position = (sfVector2f){x, y}; // button limits
    btn->bounds.size = (sfVector2f){width, height};
    btn->is_hovered = 0;

    return btn;
}

void UpdateButtonHover(Button *btn, sfVector2i mouse_pos) {
    if (!btn)
        return;

    int inside = (mouse_pos.x >= btn->bounds.position.x &&
                  mouse_pos.x <= btn->bounds.position.x + btn->bounds.size.x &&
                  mouse_pos.y >= btn->bounds.position.y &&
                  mouse_pos.y <= btn->bounds.position.y + btn->bounds.size.y);

    if (inside && !btn->is_hovered)
    {
        btn->is_hovered = 1;
        sfRectangleShape_setFillColor(btn->shape, (sfColor){100, 100, 100, 255});
    }
    else if (!inside && btn->is_hovered)
    {
        btn->is_hovered = 0;
        sfRectangleShape_setFillColor(btn->shape, (sfColor){50, 50, 50, 255});
    }
}

int IsButtonClicked(Button *btn, sfVector2i mouse_pos) {
    if (!btn)
        return 0;

    return (mouse_pos.x >= btn->bounds.position.x &&
            mouse_pos.x <= btn->bounds.position.x + btn->bounds.size.x &&
            mouse_pos.y >= btn->bounds.position.y &&
            mouse_pos.y <= btn->bounds.position.y + btn->bounds.size.y);
}

void DestroyButton(Button *btn) {
    if (!btn)
        return;
    sfRectangleShape_destroy(btn->shape);
    sfText_destroy(btn->text);
    free(btn);
}

Animation *CreateAnimation(const char *folder, int start, int count) {
    Animation *anim = (Animation *)malloc(sizeof(Animation));
    if (!anim)
        return NULL;

    anim->textures = (sfTexture **)malloc(sizeof(sfTexture *) * count);
    anim->current_frame = 0;
    anim->timer = 0.0f;
    anim->frame_count = count;
    anim->is_playing = 0;

    char filename[256];
    for (int i = 0; i < count; i++) {
        snprintf(filename, sizeof(filename), "%s/IMG_6195_%03d.jpg", folder, start + i);

        anim->textures[i] = sfTexture_createFromFile(filename, NULL);
        if (!anim->textures[i]) {
            printf("Failed to load: %s\n", filename);
            for (int j = 0; j < i; j++) {
                sfTexture_destroy(anim->textures[j]);
            }

            free(anim->textures);
            free(anim);
            return NULL;
        }

        sfTexture_setSmooth(anim->textures[i], false);
        printf("Loaded: %s\n", filename);
    }

    anim->sprite = sfSprite_create(anim->textures[0]);
    if (!anim->sprite) {
        printf("Failed to create sprite\n");
        for (int i = 0; i < count; i++) {
            sfTexture_destroy(anim->textures[i]);
        }
        
        free(anim->textures);
        free(anim);
        return NULL;
    }

    return anim;
}

void DestroyAnimation(Animation *anim) {
    if (!anim) {
        return;
    }

    if (anim->sprite) {
        sfSprite_destroy(anim->sprite);
    }

    for (int i = 0; i < anim->frame_count; i++) {
        if (anim->textures[i]) {
            sfTexture_destroy(anim->textures[i]);
        }
    }

    free(anim->textures);
    free(anim);
}

void UpdateAnimation(Animation *anim, float delta_time) {
    if (!anim || !anim->is_playing) {
        return;
    }

    anim->timer += delta_time;

    if (anim->timer >= FRAME_DELAY) {
        anim->timer = 0.0f;
        anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
        sfSprite_setTexture(anim->sprite, anim->textures[anim->current_frame], true);
    }
}