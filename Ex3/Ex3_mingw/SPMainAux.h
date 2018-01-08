#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPFIARGame.h"
#include "SPFIARParser.h"
#include "SPMinimax.h"
#include "SPMinimaxNode.h"

#define HISTORY_SIZE 20

//SPFIARGame:

/**
* Given a game state, this function checks the currentPlayer and returns symbol of the other player
*
* @param currentGame - The current game state
* @return
* PLAYER_2_SYMBOL if the currentPlayer is PLAYER_1_SYMBOL and returns  PLAYER_1_SYMBOL otherwise.
*/
char getOtherPlayer(SPFiarGame* src);

/**
* Given a game state, this function checks if all the columns of the gameboard are full.
*
* @param currentGame - The current game state
* @return
* true if all the gameboard columns are full and false otherwise
*/
bool isBoardFull(SPFiarGame* src);

/**
* Given a game state, this function checks how many columns of the gameboard are full.
*
* @param currentGame - The current game state
* @return
* number of the gameboard's columns that are full.
*/
int fullColumnsCount(SPFiarGame* src);


//SPMinimax:

/**
* Given a game state, this function creats a root for the minimax tree.
*
* @param currentGame - The current game state
* @return
* A Node* for the minimax tree.
*/
Node* createRoot(SPFiarGame* game);

/**
* Given a Node*, this function recursively creats the minimax tree with the given Node* as his root.
* for each node in the tree the function initiates his children field.
* for each node the function checks if the node takes max or min score from his children.
* the function recieves a pointer to a boolean variable named mallocError and if a malloc error occur the function
updates mallocError
*
* @param node - The tree root
* @param depth - minimax tree depth
** @param mallocError - boolean pointer updated in case a malloc error occurred.
*/
void createTree(Node* node, unsigned int depth, bool* mallocError);

/**
* Given a tree's root , the function delete the tree and free all the memory related to the tree.
* @param node - The tree root
*/
void destroyTree(Node* node);

/**
* Given a game state ,  this function calculates the game histogram using the calcHist function and returns the
* scoring result of the curent game state by multiplying the weight vector with the histogram.
* @param currentGame - The current game state
* @return
* The scoring function result for currentGame game board.
*/
int getScore(SPFiarGame* currentGame);

/**
* Given a game state and the histogram of the scoring function,  this function updates the
* histogram for the scoring function according to the current gameboard
* using the functions : scanVerticalBlocks, scanHorizontalBlocks, scanDiag1Blocks, scanDiag2Blocks .
* @param currentGame - The current game state
* @param hist- The current game state histogram for the scoring function.
*/
void calcHist(SPFiarGame* currentGame, int* hist);

/**
* Given a game state and the histogram of the scoring function,  this function updates the
* histogram for the scoring function according to the vertical blocks  in the gameboard.
* @param currentGame - The current game state
* @param hist- The current game state histogram for the scoring function.
*/
void scanVerticalBlocks(SPFiarGame* src, int* hist);

/**
* Given a game state and the histogram of the scoring function,  this function updates the
* histogram for the scoring function according to the horizontal blocks  in the gameboard.
* @param currentGame - The current game state
* @param hist- The current game state histogram for the scoring function.
*/
void scanHorizontalBlocks(SPFiarGame* src, int* hist);

/**
* Given a game state and the histogram of the scoring function,  this function updates the
* histogram for the scoring function according to the diagonals(going down and left direction)  in the gameboard.
* @param currentGame - The current game state
* @param hist- The current game state histogram for the scoring function.
*/
void scanDiag1Blocks(SPFiarGame* src, int* hist);

/**
* Given a game state and the histogram of the scoring function,  this function updates the
* histogram for the scoring function according to the diagonals(going down and right direction)  in the gameboard.
* @param currentGame - The current game state
* @param hist- The current game state histogram for the scoring function.
*/
void scanDiag2Blocks(SPFiarGame* src, int* hist);

/**
* Given an array representing 4 straight cells from the game board,  the function counts player A’s discs
* as positive: +1, and player B’s discs as negative: −1 , and returns a score number between −4 and +4.
* @param arr - an array represents 4 staight cells from the game board.
* @return
* Scoring result calculated for the input arr.
*/
int calcBlockScore(char* arr);


// SPMainAux:

/**
* Given char winner, prints a string declaring the winner (or tie) and further instructions after the game ends.
* @param winner - result of spFiarCheckWinner
*
*/
void printWinner(char winner);

/**
* Given a game state, this function free all the memory used by the game and exit the game window.
* @param currentGame - The current game state
*/
void exitGame(SPFiarGame* game, bool isMallocError);

/**
* Given a string, The my_strdup_main function shall return a pointer to a new string,
* which is a duplicate of the string pointed to by s. A null pointer is returned if the new string cannot be created.
* @param s - A string
*/
char* my_strdup_main(const char* s);

/**
* Given a game state and the max depth of the minimax tree, the function recieves from the player a command to execute.
* if the command is valid the function will execute it , else it will print "invalid command" and call makeUserTurn recursively.
* @param game - The current game state
* @param depth- The max depth of the minimax tree
* @return
* -1 in case the recieved command is restart
* 0 in case other valid command recieved(excepet exit).
*/
int makeUserTurn(SPFiarGame* game, int depth);

/**
* The function gets a string representing the user command from the player and returns the correct SPCommand
*@return
* cmd- SPCOMMAND matching the command from the user.
*/
SPCommand getCommand();

/**
* This function asks the player to enter a difficulty level for the game and if the player
* entered a valid integer between 1 to 7 (included) it returns this number,
else it will print that it is not a valid number between 1-7 and call getMaxDepth() recursively.
* @return
* The difficult level between 1-7 that the player chose.
*/
int getMaxDepth();

/**
* Given a game state and the max depth of the minimax tree, the function executes the computer move
using spMinimaxSuggestMove function and prints a string describes the computer move.
* @param game - The current game state
* @param depth- The max depth of the minimax tree
*
*/
void makeComputerTurn(SPFiarGame* game, int depth);

#endif