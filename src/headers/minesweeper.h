#ifndef SAPER_MINESWEEPER_H
#define SAPER_MINESWEEPER_H

#include <boost/program_options.hpp>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

struct size_count {
    size_t count_of_mines;
    size_t size_of_field;
};

enum field_options {
    MIN_BOMB_COUNT = 5,
    MIN_FIELD_SIZE = 5,
    MAX_FIELD_SIZE = 26 //count of letters in english alphabet
};

size_count input_params();

size_count parse_params(int argc, char *argv[]);

[[noreturn]]void playGame(int argc, char *argv[]);

void setMineField(std::vector<std::vector<bool>> &mineField, size_t sizeField, size_t countOfBomb);

void setShowField(std::vector<std::vector<char>> &field, size_t sizeField);

void consoleOutputShowField(const std::vector<std::vector<char>> &field);

void consoleOutputMineField(const std::vector<std::vector<bool>> &mineField,
                            const std::vector<std::vector<char>> &field);

void outputResultOfCheckingBomb(const std::vector<std::vector<bool>> &mineField,
                                std::vector<std::vector<char>> &field);

void showEmptyCells(const std::vector<std::vector<bool>> &mineField, std::vector<std::vector<char>> &field, char xCoord,
                    char yCoord);

bool isWin(const std::vector<std::vector<bool>> &mineField, const std::vector<std::vector<char>> &field);

void frameOfField(size_t sizeField);

void inputCoordinates(char &xCoord, char &yCoord, size_t sizeField);

void looseCheck(const std::vector<std::vector<bool>> &mineField,
                const std::vector<std::vector<char>> &field, char xCoord,
                char yCoord);

size_t countOfMinesAroundCell(const std::vector<std::vector<bool>> &mineField, char xCoord, char yCoord);

#endif
