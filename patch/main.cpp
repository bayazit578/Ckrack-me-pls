#include <CSFML/Graphics.h>
#include <CSFML/Audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphical_user_interface.h"
#include "patch.h"

int main() {
    sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow *window = sfRenderWindow_create(mode, 
                                                   "Animation with Buttons",
                                                    sfClose | sfTitlebar, 
                                                    sfWindowed, NULL);

    if (!window) {
        printf("Failed to create window\n");
        return EXIT_FAILURE;
    }

    sfMusic *music = sfMusic_createFromFile("audio/Napalm_Death_-_You_Suffer.mp3"); // music
    if (!music) {
        printf("Failed to load music\n");
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }

    sfMusic_setVolume(music, 100.f);

    sfFont *font = sfFont_createFromFile("fonts/DS Stamper.ttf"); // font
    if (!font) {
        printf("Failed to load font\n");
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }

    Animation *anim = CreateAnimation("img", 1, FRAME_COUNT); // anime
    if (!anim) {
        printf("Failed to load animation\n");
        sfFont_destroy(font);
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }

    sfVector2u tex_size = sfTexture_getSize(anim->textures[0]); // first frame dim
    float scale_x = (float)WIDTH / (float)tex_size.x;
    float scale_y = (float)HEIGHT / (float)tex_size.y;
    sfSprite_setScale(anim->sprite, (sfVector2f){scale_x, scale_y});

    Button *exit_btn = CreateButton("EXIT", font, 50, HEIGHT - 100, 200, 60);
    Button *start_btn = CreateButton("PATCH", font, WIDTH - 250, HEIGHT - 100, 200, 60);

    sfText *title_text = sfText_create(font); // title
    sfText_setString(title_text, "MIPT PATCHER");
    sfText_setCharacterSize(title_text, 80);
    sfText_setFillColor(title_text, (sfColor){255, 255, 255, 255});

    sfText_setOutlineColor(title_text, sfBlack);
    sfText_setOutlineThickness(title_text, 2.0f);

    sfFloatRect title_bounds = sfText_getLocalBounds(title_text);
    float title_x = (WIDTH - title_bounds.size.x) / 2.0f;
    float title_y = 50.0f;
    sfText_setPosition(title_text, (sfVector2f){title_x, title_y});

    if (!exit_btn || !start_btn) {
        printf("Failed to create buttons\n");
        if (exit_btn) {
            DestroyButton(exit_btn);
        }

        if (start_btn) {
            DestroyButton(start_btn);
        }
        DestroyAnimation(anim);
        sfMusic_destroy(music);
        sfFont_destroy(font);
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }

    sfClock *clock = sfClock_create();

    bool show_buttons = true;

    while (sfRenderWindow_isOpen(window)) {
        float delta_time = sfTime_asSeconds(sfClock_restart(clock));

        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }

            if (show_buttons) {
                if (event.type == sfEvtMouseMoved) {
                    sfVector2i mouse_pos = event.mouseMove.position;
                    UpdateButtonHover(exit_btn, mouse_pos);
                    UpdateButtonHover(start_btn, mouse_pos);
                }

                if (event.type == sfEvtMouseButtonPressed) {
                    sfVector2i mouse_pos = {event.mouseButton.position.x, event.mouseButton.position.y};

                    if (IsButtonClicked(exit_btn, mouse_pos)) {
                        sfRenderWindow_close(window);
                    }

                    if (IsButtonClicked(start_btn, mouse_pos)) {
                        FILE *input = fopen("CRCK_ME2.COM", "rb+");
                        if (!input) {
                            perror("Error opening file!\n");
                            return 1;
                        }

                        if (patch_file(input)) {
                            return 1;
                        }

                        sfMusic_play(music);
                        anim->is_playing = 1;
                        show_buttons = 0;
                    }
                }
            }
        }

        UpdateAnimation(anim, delta_time);

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, anim->sprite, NULL);

        if (show_buttons)
        {
            sfRenderWindow_drawRectangleShape(window, exit_btn->shape, NULL);
            sfRenderWindow_drawRectangleShape(window, start_btn->shape, NULL);
            sfRenderWindow_drawText(window, exit_btn->text, NULL);
            sfRenderWindow_drawText(window, start_btn->text, NULL);

            sfRenderWindow_drawText(window, title_text, NULL);
        }

        sfRenderWindow_display(window);
    }

    sfClock_destroy(clock);
    DestroyButton(exit_btn);
    DestroyButton(start_btn);
    sfText_destroy(title_text);
    DestroyAnimation(anim);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);

    return 0;
}