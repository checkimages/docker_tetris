# Tерминальный интерфейс для проекта BrickGame v1.0 aka Tetris

Образ создан на базе ubuntu:22.04.

Скачать образ:
```bash copy
docker docker pull checkimages/tetris
```

## Применение

### Создание контейнера 
Выполняется в основной директории проекта (C7_BrickGame_v1.0):
```bash
docker run -it --rm -v .:/project --device /dev/tty checkimages/tetris
```
Здесь: 
 - --rm - автоудаление контейнера по завершении работы;
 - -v .:/project - монтирование текущей директории в контейнер;

Контейнер запустится, скопирует backend пользователя, соберет и запустит приложение Tetris.
Сначала проверяется наличие библиотеки src/tetris_fsm.a. Если готовой библиотеки нет, она собирается из исходников src/brick_game/tetris/.

### Настройки

По умолчанию цвета не поддерживаются. Для поддержки цветов необходимо ячейки фигур в бэкэнде кодировать числами, которые при взятии остатка от деления на 10 (% 10) дадут цифры от 1 до 7.

А также при создании контейнера добавить переменную:
```bash
-e TCOLOR=1
```

Для яркости цветов можно добавить переменную:
```bash
-e TBRIGHT=1
```

Сохранение рекорда между сеансами игры при помощи СУБД не гарантируется. Сохранение в файл возможно, для этого файл необходимо сохранять в директорию /project.

Взаимодействие фронтэнда и бэкэнда осуществляется в соответствии со спецификацией:
```bash
typedef enum {
  Start,      ///< Initiates the game.
  Pause,      ///< Pause on/off.
  Terminate,  ///< End the game and exit.
  Left,       ///< Move the figure to the left.
  Right,      ///< Move the figure to the right.
  Up,         ///< Not used.
  Down,       ///< Move the figure all the way down.
  Action      ///< Rotate the figure.
} UserAction_t;

typedef struct {
  int **field;     ///< Game field (20x10).
  int **next;      ///< Next figure (2x4).
  int score;       ///< Current score.
  int high_score;  ///< Saved high score.
  int level;       ///< Game level (from 1 to 10).
  int speed;       ///< Game speed (from 1 to 10). Game over state (value 0).
  int pause;       ///< Pause state (on/off).
} GameInfo_t;

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();
```

Если бэкэнд передаст параметр speed=0 для фронтенда это будет означать "game over".

Пример создания контейнера с яркими цветами:
```bash
docker run -it --rm -v .:/project --device /dev/tty -e TCOLOR=1 -e TBRIGHT=1 checkimages/tetris
```

[Гит-репозиторий](https://github.com/checkimages/docker_tetris)