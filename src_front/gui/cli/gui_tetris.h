/**
 * @file gui_tetris.h
 * @author jaycemar@student.21-school.ru
 * @brief s21 tetris gui header
 * @version 1.0
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef GUI_TETRIS_H
#define GUI_TETRIS_H

#include <locale.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tetris.h"

#define GAME_ROW 20
#define GAME_COLUMN 10
#define GAME_WIDTH 20

#define INFO_ROW 10
#define INFO_COLUMN 11
#define INFO_WIDTH 22

#define NEXT_ROW 2
#define NEXT_COLUMN 4
#define NEXT_WIDTH 8
#define NEXT_X 6

#define VAL_COLUMN 5
#define VAL_X 12

#define VDIVIDER_X GAME_WIDTH + 1
#define NEXT_Y 1
#define HSCORE_Y NEXT_Y + 2
#define VALUES_Y HSCORE_Y + 2
#define SCORE_Y HSCORE_Y + 2
#define LEVEL_Y SCORE_Y + 2
#define SPEED_Y LEVEL_Y + 2
#define STATE_Y VALUES_Y + 7

#define KEYS_ROW 7

#define S21_ENTER 10
#define S21_ESC 27
#define S21_SPACE 32

#define WIN_INIT           \
  {                        \
    initscr();             \
    cbreak();              \
    noecho();              \
    nodelay(stdscr, TRUE); \
    curs_set(0);           \
    keypad(stdscr, TRUE);  \
  }

/**
 * @struct Thread_args_t
 * @brief Structure to hold thread arguments for managing thread state.
 *
 * This structure contains a single member:
 * - `running`: A volatile boolean flag indicating whether the thread is active.
 *
 * The `volatile` keyword is used to inform the compiler that this variable
 * may be changed by external factors (such as an interrupt or another thread),
 * preventing the compiler from optimizing its access.
 */
typedef struct {
  volatile bool running;  ///< Indicates if the thread is currently running.
} Thread_args_t;

/**
 * @enum State_gui_t
 * @brief Enum representing the various GUI states in the game.
 *
 * This enumeration defines the different states that the game can be in during
 * its execution. Each state corresponds to a specific phase or action within
 * the game, facilitating easier management of game logic. The possible GUI
 * states are:
 */
typedef enum {
  state_gui_start,     ///< The initial state before the game begins.
  state_gui_game,      ///< The state in which the game elements are in motion.
  state_gui_pause,     ///< The state indicating that the game is paused.
  state_gui_game_over  ///< The state that signifies the end of the game.
} State_gui_t;

/**
 * @enum s21_colors
 * @brief Enumeration of basic colors.
 *
 * This enumeration defines a set of basic colors, each associated
 * with a unique integer value. The values can be used to represent
 * colors in various applications, such as graphics rendering or
 * visual representation of data.
 */
enum s21_colors {
  s21_red = 201,
  s21_orange,
  s21_yellow,
  s21_pinc,
  s21_green,
  s21_blue,
  s21_magenta
};

/**
 * @struct Wins_t
 * @brief Structure to hold window pointers for the game interface.
 *
 * This structure organizes various windows used in the game, allowing for easy
 * management and access to different parts of the user interface.
 */
typedef struct {
  WINDOW *main_win;  ///< Pointer to the main game window.
  WINDOW *game_win;  ///< Pointer to the game field window.
  WINDOW
  *next_win;  ///< Pointer to the window for displaying upcoming game items.
  WINDOW
  *values_win;  ///< Pointer to the window for showing game-related values.
  WINDOW
  *state_win;  ///< Pointer to the window indicating the current game state.
  WINDOW
  *info_win;  ///< Pointer to the information window for additional context.
  WINDOW *keys_win;  ///< Pointer to the window for displaying control keys.
} Wins_t;

/**
 * @brief Initializes color pairs and custom colors for the terminal.
 *
 * This function defines a set of custom colors and their corresponding
 * color pairs for use in terminal-based applications. It utilizes the
 * `init_color` function to specify RGB values for each custom color
 * and the `init_pair` function to create color pairs that can be used
 * for text attributes in the interface.
 *
 * @param[in] bright sets bright options for each color
 */
void init_colors(bool bright);

/**
 * @brief Retrieves a pointer to a static Wins_t instance.
 *
 * This function returns a pointer to a static variable of type Wins_t.
 * The static variable is initialized to zero the first time the function
 * is called and retains its value between subsequent calls.
 * This allows for shared state across multiple calls to this function,
 * providing a single instance of Wins_t that can be accessed globally.
 *
 * @return A pointer to the static Wins_t instance.
 */
Wins_t *get_wins();

/**
 * @brief Creates and initializes the game windows.
 *
 * This function sets up the necessary windows for the game user interface using
 * the ncurses library. It creates the main window and various sub-windows
 * needed to display game information, state, and controls. The windows are
 * positioned based on the terminal size, ensuring they are centered on the
 * screen.
 */
void create_wins();

/**
 * @brief Main loop function that manages a separate thread for key listening.
 *
 * It creates a new thread that runs the key_listener() function, passing
 * a structure containing a boolean flag to indicate running state.
 * The loop continuously updates the window and sleeps for an interval
 * based on the game speed, until the running flag is set to false. Once the
 * loop ends, it cancels the key listener thread and waits for its termination
 * before exiting.
 */
void loop();

/**
 * @brief Deletes all windows associated with the Wins_t structure.
 *
 * This function retrieves the Wins_t object containing multiple windows
 * associated with the game state and then calls the `delwin` function
 * to deallocate each of these windows. This is essential for freeing
 * up resources and avoiding memory leaks when the windows are no longer
 * needed.
 */
void delete_wins();

/**
 * @brief Listens for keyboard input on a separate thread.
 *
 * This function runs in an infinite loop while the `args->running` flag is set
 * to true. It captures keyboard input using `getch()` and processes it by
 * calling `get_backend()`. If the user presses the Esc key (ASCII value 27), it
 * sets the `args->running` flag to false, which signals the listener to stop.
 * If a valid key is detected, it updates the windows by calling
 * `update_wins()`.
 *
 * @param[in] arg A pointer to a `Thread_args_t` structure containing thread
 * arguments.
 *
 * @return Returns NULL upon completion of the function, indicating that the
 * thread should exit.
 */
void *key_listener(void *arg);

/**
 * @brief Processes the user input based on the provided character.
 *
 * This function interprets the input character `ch` and calls the
 * appropriate userInput() function with corresponding actions. It
 * handles various control commands such as entering, navigating
 * left or right, exiting the application, pausing, and performing
 * actions based on specific key inputs.
 *
 * @param[in] ch The character input from the user, representing a command.
 * @return Returns true if a valid command was processed,
 *               otherwise returns false.
 */
bool get_backend(int ch);

/**
 * @brief Updates the game state and graphical windows based on the current game
 * information.
 *
 * This function retrieves the current state of the game and updates various
 * windows that display information to the user. It checks for specific
 * conditions such as game pause, game over, and the current game state, then
 * updates the corresponding graphical windows accordingly.
 *
 * @return The current speed of the game if it is still active; 0 if the game is
 * over.
 */
int update_wins();

/**
 * @brief Updates the specified window with the current state of the game field.
 *
 * This function clears the given window and redraws the field based on the
 * provided 2D array of integers.
 *
 * @param[in] field_win A pointer to the NCurses window that will be updated.
 * @param[in] field A pointer to a 2D array representing the game field.
 *              Each element is an integer that indicates the color to use for
 *              drawing (0 for no block).
 * @param[in] rows The number of rows in the game field.
 * @param[in] columns The number of columns in the game field.
 */
void update_field_win(WINDOW *field_win, int **field, int rows, int columns);

/**
 * @brief Updates the value window with the current game information.
 *
 * This function takes a pointer to a WINDOW structure and a GameInfo_t
 * structure, then updates the specified window to display the high score,
 * current score, level, and speed of the game.
 *
 * @param[in] val_win A pointer to the WINDOW structure to be updated.
 * @param[in] game_info A GameInfo_t structure containing game statistics such
 *                  as high score, current score, level, and speed.
 */
void update_val_win(WINDOW *val_win, GameInfo_t game_info);

/**
 * @brief Updates the state display window based on the current game state.
 *
 * This function clears the specified window and displays a message
 * corresponding to the current state of the game. It handles the
 * following states:
 * - state_gui_start: Prompts the user to press ENTER to start the game.
 * - state_gui_game: Displays "GAME" indicating that the game is currently
 * running.
 * - state_gui_pause: Displays "PAUSE" indicating that the game is currently
 * paused.
 * - state_gui_game_over: Displays "GAME OVER" when the game is over, and
 * prompts the user to press ENTER to restart if the game has already been over.
 *
 * @param[in] state_win Pointer to the WINDOW structure representing the state
 * window.
 * @param[in] state The current state of the game represented by the State_gui_t
 * enumeration.
 */
void update_state_win(WINDOW *state_win, State_gui_t state);

/**
 * @brief Updates the information window with current game statistics.
 *
 * This function prints the labels for the next game figure, high score,
 * current score, level, and speed onto the specified window. The
 * information is displayed in a formatted manner at predefined
 * positions within the window.
 *
 * @param[in] info_win A pointer to the WINDOW structure representing the
 *                 information window to be updated.
 */
void update_info_win(WINDOW *info_win);

/**
 * @brief Updates the keys window with control instructions.
 *
 * This function draws a list of controls on the specified
 * window for user reference. It displays keyboard commands
 * that can be used during the application, including
 * actions to pause, exit, and move in various directions.
 *
 * @param[in] keys_win A pointer to the WINDOW structure that
 *        represents the keys window where the controls
 *        will be displayed.
 */
void update_keys_win(WINDOW *keys_win);

/**
 * @brief Draws a vertical divider in the specified window.
 *
 * This function creates a vertical line in the provided `WINDOW` starting
 * at the specified coordinates (start_y, start_x).
 *
 * @param[in] my_win Pointer to the WINDOW where the divider will be drawn.
 * @param[in] start_y The starting row position for the divider.
 * @param[in] start_x The starting column position for the divider.
 * @param[in] length The total length of the divider. If set to 0, it is
 *               automatically determined based on the window size.
 */
void draw_vdivider(WINDOW *my_win, int start_y, int start_x, int length);

/**
 * @brief Draws a horizontal divider line in the specified window.
 *
 * This function draws a horizontal divider line starting from the specified
 * coordinates (start_y, start_x) within the given window.
 *
 * @param[in] my_win Pointer to the WINDOW structure where the divider will be
 * drawn.
 * @param[in] start_y The y-coordinate for the start position of the divider.
 * @param[in] start_x The x-coordinate for the start position of the divider.
 * @param[in] length The total length of the divider. If zero, it will be set to
 *               the maximum available length from the starting x-coordinate.
 */
void draw_hdivider(WINDOW *my_win, int start_y, int start_x, int length);

/**
 * @brief Draws a frame around the specified window.
 *
 * This function adds a border box to the provided window using the `box`
 * function, and draws vertical and horizontal dividers at predefined positions.
 *
 * @param[in] my_win A pointer to the WINDOW structure representing the window
 * to be drawn on. It must be initialized before calling this function.
 */
void draw_frame(WINDOW *my_win);

#endif  // GUI_TETRIS_H