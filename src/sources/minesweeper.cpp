#include "../headers/minesweeper.h"

namespace po = boost::program_options;

size_count input_params() {
    size_t size_of_field, count_of_mines;
    while (true) {
        std::cout << std::endl << " Input size of field (square field) : ";
        std::cin >> size_of_field;
        std::cout << std::endl << " Input count of mines (less than square of field) : ";
        std::cin >> count_of_mines;
        if (((size_of_field * size_of_field) > count_of_mines) ||
            (size_of_field <= MAX_FIELD_SIZE)) {
            break;
        } else {
            std::cout << " Invalid value \n Input again" << std::endl;
        }
    }
    return size_count{count_of_mines, size_of_field};
}

size_count parse_params(int argc, char *argv[]) {
    if (argc == 1) {
        return input_params();
    }
    po::options_description description("Allowed options");
    description.add_options()
            ("size", po::value<size_t>(), "set size of field")
            ("mines", po::value<size_t>(), "set count of mines of field");

    po::variables_map variablesMap;
    po::store(po::parse_command_line(argc, argv, description), variablesMap);
    po::notify(variablesMap);

    if ((variablesMap["mines"].as<size_t>() <= MIN_BOMB_COUNT || variablesMap["size"].as<size_t>() <= MIN_FIELD_SIZE) ||
        variablesMap["mines"].as<size_t>() >= (variablesMap["size"].as<size_t>() *
                                               variablesMap["size"].as<size_t>()) ||
        variablesMap["size"].as<size_t>() > MAX_FIELD_SIZE) {
        std::cerr << "invalid params" << std::endl;
        return input_params();
    }

    return size_count{variablesMap["mines"].as<size_t>(), variablesMap["size"].as<size_t>()};
}

void playGame(int argc, char *argv[]) {
    size_count complexity_of_game = parse_params(argc, argv);
    size_t countOfMines = complexity_of_game.count_of_mines;
    size_t sizeField = complexity_of_game.size_of_field;
    std::vector<std::vector<bool>> mineField(sizeField);
    std::vector<std::vector<char>> showField(sizeField);
    setMineField(mineField, sizeField, countOfMines);
    setShowField(showField, sizeField);
    consoleOutputShowField(showField);
    while (true) {
        outputResultOfCheckingBomb(mineField, showField);
    }
}

void setShowField(std::vector<std::vector<char>> &field, size_t sizeField) {
    for (auto &entry: field) {
        entry.resize(sizeField);
    }
    for (auto &filed_i: field) {
        for (auto &filed_j: filed_i) {
            filed_j = '#';
        }
    }
}

void frameOfField(size_t sizeField) {
    std::cout << "\n\n  ";
    for (size_t i = 0; i < sizeField; ++i) {
        std::cout << " " << static_cast<char>(i + 'a');
    }
    std::cout << "\n";
    for (int i = 0; i < (sizeField * 2) + 2; ++i) {
        std::cout << "-";
    }
    std::cout << "\n";
}

void consoleOutputShowField(const std::vector<std::vector<char>> &field) {
#if __unix
    system("clear");
#elif WIN32
    system("cls");
#endif

    frameOfField(field.size());

    for (size_t i = 0; i < field.size(); ++i) {
        std::cout << static_cast<char>(i + 'a') << '|';
        for (const auto filed_j: field[i]) {
            if (filed_j == '_') {
                std::cout << " \033[;32m_\033[0m";
            } else {
                std::cout << ' ' << filed_j;
            }
        }
        std::cout << "\n";
    }
}

void inputCoordinates(char &xCoord, char &yCoord, size_t sizeField) {
    std::cout << "\n\n input x: ";
    std::cin >> xCoord;
    while (xCoord < 'a' || xCoord > 'a' + sizeField) {
        std::cout << " invalid coordinates, input again" << std::endl;
        std::cout << "   input x: ";
        std::cin >> xCoord;
    }
    xCoord -= 'a';
    std::cout << " input y: ";

    std::cin >> yCoord;
    while (yCoord < 'a' || yCoord > 'a' + sizeField) {
        std::cout << " invalid coordinates, input again" << std::endl;
        std::cout << "   input y: ";
        std::cin >> yCoord;
    }
    yCoord -= 'a';
}

bool isWin(const std::vector<std::vector<bool>> &mineField, const std::vector<std::vector<char>> &field) {
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field.size(); ++j) {
            if (field[i][j] == '#' && !mineField[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void looseCheck(const std::vector<std::vector<bool>> &mineField,
                const std::vector<std::vector<char>> &field, char xCoord,
                char yCoord) {
    if (mineField[yCoord][xCoord]) {
#if __unix
        system("clear");
#elif WIN32
        system("cls");
#endif
        std::cout << "\n\n \033[;31mYou loose !!\033[0m" << std::endl;
        frameOfField(mineField.size());
        consoleOutputMineField(mineField, field);
        std::cout << std::endl;
        exit(0);
    }
}

void winCheck(const std::vector<std::vector<bool>> &mineField,
              std::vector<std::vector<char>> &field) {
    if (isWin(mineField, field)) {
#if __unix
        system("clear");
#elif WIN32
        system("cls");
#endif
        std::cout << "\n\n \033[;32mYou win !!\033[0m" << std::endl;
        frameOfField(mineField.size());
        consoleOutputMineField(mineField, field);
        std::cout << std::endl;
        exit(0);
    }
}

void showEmptyCells(const std::vector<std::vector<bool>> &mineField, std::vector<std::vector<char>> &field, char xCoord,
                    char yCoord) {
    if (countOfMinesAroundCell(mineField, xCoord, yCoord)) {
        return;
    }
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i != 0 || j != 0) {
                if (((xCoord + i) >= 0 && (yCoord + j) >= 0) && (xCoord + i) < field.size() &&
                    (yCoord + j) < field.size()) {
                    if (countOfMinesAroundCell(mineField, xCoord + i, yCoord + j) != 0) {
                        field[yCoord + j][xCoord + i] = static_cast<char>(
                                countOfMinesAroundCell(mineField, xCoord + i, yCoord + j) + '0');
                    } else if (field[yCoord + j][xCoord + i] != '_') {
                        field[yCoord + j][xCoord + i] = '_';
                        showEmptyCells(mineField, field, xCoord + i, yCoord + j);
                    }
                }
            }
        }
    }
}

void outputResultOfCheckingBomb(const std::vector<std::vector<bool>> &mineField,
                                std::vector<std::vector<char>> &field) {
    char xCoord, yCoord;
    while (true) {
        inputCoordinates(xCoord, yCoord, field.size());
        if (field[yCoord][xCoord] == '_') {
            std::cout << "\n This cell has been opened earlier\n";
        } else {
            break;
        }
    }
    size_t countOfMines = countOfMinesAroundCell(mineField, xCoord, yCoord);
    looseCheck(mineField, field, xCoord, yCoord);
    if (countOfMines == 0) {
        field[yCoord][xCoord] = '_';
        showEmptyCells(mineField, field, xCoord, yCoord);
    } else {
        field[yCoord][xCoord] = static_cast<char>(countOfMines + '0');
    }
    consoleOutputShowField(field);
    winCheck(mineField, field);
}

void setMineField(std::vector<std::vector<bool>> &mineField, size_t sizeField, size_t countOfBomb) {
    for (auto &entry: mineField) {
        entry.resize(sizeField);
    }
    std::default_random_engine defEngine(time(nullptr));
    std::uniform_int_distribution<size_t> size_t_Distro(0, (sizeField) - 1);

    for (size_t i = 0; i < countOfBomb; ++i) {
        generate_again:
        size_t xCoord = size_t_Distro(defEngine);
        size_t yCoord = size_t_Distro(defEngine);
        if (mineField[yCoord][xCoord]) {
            goto generate_again;
        } else {
            mineField[yCoord][xCoord] = true;
        }
    }
}

size_t countOfMinesAroundCell(const std::vector<std::vector<bool>> &mineField, char xCoord, char yCoord) {
    size_t result = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i != 0 || j != 0) {
                if (((xCoord + i) >= 0 && (yCoord + j) >= 0) && (xCoord + i) < mineField.size() &&
                    (yCoord + j) < mineField.size()) {
                    if (mineField[yCoord + j][xCoord + i]) {
                        result += 1;
                    }
                }
            }
        }
    }
    return result;
}

void consoleOutputMineField(const std::vector<std::vector<bool>> &mineField,
                            const std::vector<std::vector<char>> &field) {
    for (size_t i = 0; i < mineField.size(); ++i) {
        std::cout << static_cast<char>(i + 'a') << '|';
        for (size_t j = 0; j < mineField.size(); ++j) {
            if (mineField[i][j]) {
                std::cout << ' ' << "\033[;31m*\033[0m";
            } else {
                std::cout << ' ' << field[i][j];
            }
        }
        std::cout << "\n";
    }
}