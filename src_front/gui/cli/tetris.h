/**
 * @file tetris.h
 * @author jaycemar@student.21-school.ru
 * @brief s21 tetris main header
 * @version 1.0
 * @date 2024-09-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>

/**
 * @enum UserAction_t
 * @brief Represents various actions that a user can perform.
 *
 * This enumeration defines a set of user actions that are used
 * to control the flow of a game.
 */
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

/**
 * @struct GameInfo_t
 * @brief Structure to hold the game information.
 *
 * This structure encapsulates all the necessary information to manage the
 * current game, including the game board, next figure, score, high score,
 * level, speed, pause state.
 */
typedef struct {
  int **field;     ///< Game field (20x10).
  int **next;      ///< Next figure (2x4).
  int score;       ///< Current score.
  int high_score;  ///< Saved high score.
  int level;       ///< Game level (from 1 to 10).
  int speed;       ///< Game speed (from 1 to 10).
  int pause;       ///< Pause state (on/off).
} GameInfo_t;

/**
 * @brief Handles user input actions based on the current game state.
 *
 * This function processes a user action and updates the game state accordingly.
 * Based on the game state `State_t`, it retrieves the corresponding finite
 * state machine (FSM) function pointer from the FSM table. If the function
 * pointer is valid, it calls the associated FSM function and marks the user
 * input as processed.
 *
 * @param[in] action The user action to be processed.
 * @param[in] hold A boolean indicating if the input (not used).
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Updates the current state of the game based on user input and game
 * conditions.
 *
 * The function checks whether the userInput() function has just been executed.
 * If no user input is present, the function determines whether the game is in a
 * paused state or normally running, and executes the corresponding finite state
 * machine (FSM) function, if available. In any case, the function returns the
 * current data in a variable of type GameInfo_t.
 *
 * @return GameInfo_t Contains the updated game state information.
 */
GameInfo_t updateCurrentState();

#endif