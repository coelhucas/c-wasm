emcc -o game.html game.c -Os -Wall ./vendor/libraylib.a -I. -I./vendor/raylib.h -L. -L./vendor/libraylib.a -s USE_GLFW=3 --shell-file shell.html -DPLATFORM_WEB
