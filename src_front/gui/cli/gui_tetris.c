#include "gui_tetris.h"

int main(int argc, char *argv[]) {
  WIN_INIT;
  if (argc == 1 || (argc > 1 && *argv[1] != '0'))
    start_color(); 

  setlocale(LC_ALL, "");
  srand(time(NULL));

  if (argc > 2)
    init_colors(*argv[2] - '0');
  else
    init_colors(false);

  refresh();

  create_wins();
  loop();
  delete_wins();
  endwin();

  return 0;
}

void init_colors(bool bright) {
  init_color(s21_red, 996, 0, 0);
  init_color(s21_orange, 1000, 396, 4);
  init_color(s21_yellow, 1000, 1000, 0);
  init_color(s21_pinc, 1000, 600, 796);
  init_color(s21_green, 0, 502, 4);
  init_color(s21_blue, 0, 0, 996);
  init_color(s21_magenta, 506, 0, 498);

  init_pair(1, bright * s21_red, s21_red);
  init_pair(2, bright * s21_orange, s21_orange);
  init_pair(3, bright * s21_yellow, s21_yellow);
  init_pair(4, bright * s21_pinc, s21_pinc);
  init_pair(5, bright * s21_green, s21_green);
  init_pair(6, bright * s21_blue, s21_blue);
  init_pair(7, bright * s21_magenta, s21_magenta);
  init_pair(8, s21_red, 0);
  init_pair(9, COLOR_WHITE, 0);
}

Wins_t *get_wins() {
  static Wins_t wins;
  return &wins;
}

void create_wins() {
  Wins_t *wins = get_wins();
  int starty = (LINES - GAME_ROW + 2) / 2;
  int startx = (COLS - (GAME_WIDTH + INFO_WIDTH + 5)) / 2;

  wins->main_win =
      newwin(GAME_ROW + 2, GAME_WIDTH + INFO_WIDTH + 5, starty, startx);
  draw_frame(wins->main_win);
  wins->game_win = derwin(wins->main_win, GAME_ROW, GAME_WIDTH, 1, 1);
  wins->state_win =
      derwin(wins->main_win, 1, INFO_WIDTH, STATE_Y, GAME_COLUMN * 2 + 3);
  wins->keys_win =
      derwin(wins->main_win, KEYS_ROW, INFO_WIDTH, STATE_Y + 2, GAME_WIDTH + 3);
  update_keys_win(wins->keys_win);
  wins->info_win =
      derwin(wins->main_win, INFO_ROW, INFO_WIDTH, 1, GAME_WIDTH + 3);
  update_info_win(wins->info_win);
  wins->next_win = derwin(wins->info_win, NEXT_ROW, NEXT_WIDTH, 0, NEXT_X);
  wins->values_win = derwin(wins->info_win, 7, 5, HSCORE_Y, VAL_X);
  doupdate();
}

void loop() {
  pthread_t thread;
  Thread_args_t args = {true};

  pthread_create(&thread, NULL, key_listener, &args);
  while (args.running) napms((14 - update_wins()) * 50);
  pthread_cancel(thread);
  pthread_join(thread, NULL);
}

void delete_wins() {
  Wins_t *wins = get_wins();

  delwin(wins->values_win);
  delwin(wins->next_win);
  delwin(wins->info_win);
  delwin(wins->keys_win);
  delwin(wins->state_win);
  delwin(wins->game_win);
  delwin(wins->main_win);
}

void *key_listener(void *arg) {
  Thread_args_t *args = (Thread_args_t *)arg;

  while (args->running) {
    int ch = getch();
    if (get_backend(ch)) {
      if (ch == S21_ESC)
        args->running = false;
      else
        update_wins();
    }
  }
  return NULL;
}

bool get_backend(int ch) {
  bool result = true;
  switch (ch) {
    case S21_ENTER:
      userInput(Start, false);
      break;
    case KEY_LEFT:
      userInput(Left, false);
      break;
    case KEY_RIGHT:
      userInput(Right, false);
      break;
    case S21_ESC:
      userInput(Terminate, false);
      break;
    case 'p':
    case 'P':
      userInput(Pause, false);
      break;
    case S21_SPACE:
    case KEY_UP:
      userInput(Action, false);
      break;
    case KEY_DOWN:
      userInput(Down, false);
      break;
    default:
      result = false;
      break;
  }

  return result;
}

int update_wins() {
  GameInfo_t game_info = updateCurrentState();
  State_gui_t state = state_gui_start;

  if (game_info.pause) state = state_gui_pause;

  if (!state && !game_info.speed) state = state_gui_game_over;

  for (int i = 0; i < GAME_ROW && !state; i++)
    for (int j = 0; j < GAME_COLUMN && !state; j++)
      if (game_info.field[i][j]) state = state_gui_game;

  Wins_t *wins = get_wins();
  update_field_win(wins->game_win, game_info.field, GAME_ROW, GAME_COLUMN);
  update_field_win(wins->next_win, game_info.next, NEXT_ROW, NEXT_COLUMN);
  update_val_win(wins->values_win, game_info);
  update_state_win(wins->state_win, state);
  doupdate();

  return game_info.speed;
}

void update_field_win(WINDOW *field_win, int **field, int rows, int columns) {
  wclear(field_win);
  if (field)
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < columns; j++)
        if (field[i][j]) {
          int color = field[i][j] % 10;
          wattron(field_win, COLOR_PAIR(color));
          mvwaddch(field_win, i, j * 2, ACS_CKBOARD);
          mvwaddch(field_win, i, j * 2 + 1, ACS_CKBOARD);
          wattroff(field_win, COLOR_PAIR(color));
        }
  wnoutrefresh(field_win);
}

void update_val_win(WINDOW *val_win, GameInfo_t game_info) {
  mvwprintw(val_win, 0, 0, "%5d", game_info.high_score);
  mvwprintw(val_win, 2, 0, "%5d", game_info.score);
  mvwprintw(val_win, 4, 0, "%5d", game_info.level);
  mvwprintw(val_win, 6, 0, "%5d", game_info.speed);
  wnoutrefresh(val_win);
}

void update_state_win(WINDOW *state_win, State_gui_t state) {
  static bool restart = false;
  wclear(state_win);
  wattron(state_win, COLOR_PAIR(8));
  if (state == state_gui_start) {
    mvwprintw(state_win, 0, 0, "PRESS ENTER TO START");
  } else if (state == state_gui_game) {
    mvwprintw(state_win, 0, 0, "GAME");
  } else if (state == state_gui_pause) {
    mvwprintw(state_win, 0, 0, "PAUSE");
  } else if (state == state_gui_game_over) {
    if (restart) {
      mvwprintw(state_win, 0, 0, "PRESS ENTER TO RESTART");
      restart = false;
    } else {
      mvwprintw(state_win, 0, 0, "GAME OVER");
      restart = true;
    }
  }
  wattroff(state_win, COLOR_PAIR(8));
  wnoutrefresh(state_win);
}

void update_info_win(WINDOW *info_win) {
  mvwprintw(info_win, NEXT_Y, 0, "NEXT:");
  mvwprintw(info_win, HSCORE_Y, 0, "HIGH SCORE:");
  mvwprintw(info_win, SCORE_Y, 0, "SCORE:");
  mvwprintw(info_win, LEVEL_Y, 0, "LEVEL:");
  mvwprintw(info_win, SPEED_Y, 0, "SPEED:");
  wnoutrefresh(info_win);
}

void update_keys_win(WINDOW *keys_win) {
  mvwprintw(keys_win, 0, 0, "P         - Pause");
  mvwprintw(keys_win, 1, 0, "ESC       - Exit");
  mvwprintw(keys_win, 2, 0, "KEY_LEFT  - Move left");
  mvwprintw(keys_win, 3, 0, "KEY_RIGHT - Move right");
  mvwprintw(keys_win, 4, 0, "KEY_DOWN  - Move down");
  mvwprintw(keys_win, 5, 0, "KEY_UP    - Rotate");
  mvwprintw(keys_win, 6, 0, "SPACE     - Rotate");
  wnoutrefresh(keys_win);
}

void draw_vdivider(WINDOW *my_win, int start_y, int start_x, int length) {
  int rows = getmaxy(my_win);
  if (!length) length = rows - start_y;
  mvwaddch(my_win, start_y, start_x, ACS_TTEE);
  mvwvline(my_win, start_y + 1, start_x, ACS_VLINE, length - 2);
  mvwaddch(my_win, start_y + length - 1, start_x, ACS_BTEE);
}

void draw_hdivider(WINDOW *my_win, int start_y, int start_x, int length) {
  int columns = getmaxx(my_win);
  if (!length) length = columns - start_x;
  mvwaddch(my_win, start_y, start_x, ACS_LTEE);
  mvwhline(my_win, start_y, start_x + 1, ACS_HLINE, length - 2);
  mvwaddch(my_win, start_y, start_x + length - 1, ACS_RTEE);
}

void draw_frame(WINDOW *my_win) {
  box(my_win, 0, 0);
  draw_vdivider(my_win, 0, VDIVIDER_X, 0);
  draw_hdivider(my_win, STATE_Y - 1, VDIVIDER_X, 0);
  draw_hdivider(my_win, STATE_Y + 1, VDIVIDER_X, 0);
  wnoutrefresh(my_win);
}