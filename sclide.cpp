#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/ioctl.h>
#include <chrono>
#include <thread>

class sclide {

private:
    // base sclide line and shades for degrading effect
    std::string line="─";
    std::string shades[4] = {"█", "▓", "▒", "░"};

    std::string fullLinePreMade;
    std::string firstShadePreMade;
    std::string secondShadePreMade;
    std::string thirdShadePreMade;
    std::string fourthShadePreMade;
    std::string spacePreMade;

private:
    // important to have a struct for rgb color representation
    struct rgb {
        int r;
        int g;
        int b;
    };

private:
    // sclide starting row, width and height of terminal, sclide color and shade degrading color
    int startingRow;
    int widthOfTerminal;
    int heightOfTerminal;
    rgb shadeDegradingColor = {0, 0, 0};
    rgb sclideColor = {255, 255, 255};
    std::string fastTrackColorLine = "\033[38;2;255;255;255m";
    std::string fastTrackShadeFirst = "\033[38;2;255;255;255m"; // full
    std::string fastTrackShadeSecond = "\033[38;2;193;193;193m"; // full - howMuchToDegrade = 63.7 == 63 for safe guard
    std::string fastTrackShadeThird = "\033[38;2;127;127;127m"; // full - howMuchToDegrade * 2 = 127.4 == 127 for safe guard
    std::string fastTrackShadeFourth = "\033[38;2;63;63;63m"; // full - howMuchToDegrade * 3 = 191.1 == 191 for safe guard
    double speed = 0.1; // Speed in seconds

    // it gonna use formula which is simply: (slcide/red - bg/red) / 4
    int howMuchToDegrade = (sclideColor.r - shadeDegradingColor.r) / 4; // how much shades to degrade

private:
    // rebuild the fast track color lines based on the current sclideColor and shadeDegradingColor
    void rebuildFastTrackColor() {
        auto clampChannel = [](int value) {
            if (value < 0) {
                return 0;
            }
            if (value > 255) {
                return 255;
            }
            return value;
        };

        auto mixChannel = [&](int colorChannel, int backgroundChannel, int step) {
            return clampChannel(backgroundChannel + ((colorChannel - backgroundChannel) * step) / 4);
        };

        howMuchToDegrade = std::max({
            std::abs(sclideColor.r - shadeDegradingColor.r),
            std::abs(sclideColor.g - shadeDegradingColor.g),
            std::abs(sclideColor.b - shadeDegradingColor.b)
        }) / 4;

        if (howMuchToDegrade < 1) {
            howMuchToDegrade = 1;
        }

        fastTrackColorLine = "\033[38;2;" + std::to_string(clampChannel(sclideColor.r)) + ";" +
                             std::to_string(clampChannel(sclideColor.g)) + ";" +
                             std::to_string(clampChannel(sclideColor.b)) + "m";
        fastTrackShadeFirst = fastTrackColorLine;
        fastTrackShadeSecond = "\033[38;2;" + std::to_string(mixChannel(sclideColor.r, shadeDegradingColor.r, 3)) + ";" +
                               std::to_string(mixChannel(sclideColor.g, shadeDegradingColor.g, 3)) + ";" +
                               std::to_string(mixChannel(sclideColor.b, shadeDegradingColor.b, 3)) + "m";
        fastTrackShadeThird = "\033[38;2;" + std::to_string(mixChannel(sclideColor.r, shadeDegradingColor.r, 2)) + ";" +
                              std::to_string(mixChannel(sclideColor.g, shadeDegradingColor.g, 2)) + ";" +
                              std::to_string(mixChannel(sclideColor.b, shadeDegradingColor.b, 2)) + "m";
        fastTrackShadeFourth = "\033[38;2;" + std::to_string(mixChannel(sclideColor.r, shadeDegradingColor.r, 1)) + ";" +
                               std::to_string(mixChannel(sclideColor.g, shadeDegradingColor.g, 1)) + ";" +
                               std::to_string(mixChannel(sclideColor.b, shadeDegradingColor.b, 1)) + "m";
    }

private:
    // important to fast track the line printing so i make pre made lines for each shade and full line and space
    void makePreMadeLines() {
        for(int i = 0; i < widthOfTerminal; i++) {
            fullLinePreMade += line;
            firstShadePreMade += shades[0];
            secondShadePreMade += shades[1];
            thirdShadePreMade += shades[2];
            fourthShadePreMade += shades[3];
            spacePreMade += ' ';
        }
    }

    // to reset all lines for new data
    void resetPreMadeLines() {
        fullLinePreMade.clear();
        firstShadePreMade.clear();
        secondShadePreMade.clear();
        thirdShadePreMade.clear();
        fourthShadePreMade.clear();
        spacePreMade.clear();
    }

public:
    sclide(int startingRow) : startingRow(startingRow) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        widthOfTerminal = w.ws_col;
        heightOfTerminal = w.ws_row;
        makePreMadeLines();
        rebuildFastTrackColor();
    }

    int setSclideColor(int r, int g, int b) {
        sclideColor.r = r;
        sclideColor.g = g;
        sclideColor.b = b;
        rebuildFastTrackColor();
        return 0;
    }
    int setSclideColor(std::string hexCode) {
        // Convert hex code to rgb
        rgb color;
        color.r = std::stoi(hexCode.substr(1, 2), nullptr, 16);
        color.g = std::stoi(hexCode.substr(3, 2), nullptr, 16);
        color.b = std::stoi(hexCode.substr(5, 2), nullptr, 16);
        sclideColor = color;
        rebuildFastTrackColor();
        return 0;
    }

    int setShadeDegradingColor(int r, int g, int b) {
        shadeDegradingColor.r = r;
        shadeDegradingColor.g = g;
        shadeDegradingColor.b = b;
        rebuildFastTrackColor();
        return 0;
    }
    int setShadeDegradingColor(std::string hexCode) {
        // Convert hex code to rgb
        rgb color;
        color.r = std::stoi(hexCode.substr(1, 2), nullptr, 16);
        color.g = std::stoi(hexCode.substr(3, 2), nullptr, 16);
        color.b = std::stoi(hexCode.substr(5, 2), nullptr, 16);
        shadeDegradingColor = color;
        rebuildFastTrackColor();
        return 0;
    }

    int resetSclideColor() {
        sclideColor = {255, 255, 255};
        rebuildFastTrackColor();
        return 0;
    }

    int setSpeed(double newSpeed) {
        speed = newSpeed;
        return 0;
    }

    int changeShades(std::vector<std::string> newShades) {
        for(int i = 0; i < 4; i++) {
            shades[i] = newShades[i];
        }
        resetPreMadeLines();
        makePreMadeLines();
        return 0;
    }

    int clean(){
        // using a buffer string
        std::string buffer;
        std::string hideCursor = "\033[?25l"; // Hide cursor

        buffer = hideCursor + "\033[" + std::to_string(startingRow) + ";1H" + fullLinePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));

        // printing the line with first shade now
        // printing hard coded color for the first shade

        buffer = "\033[" + std::to_string(startingRow) + ";1H" + fastTrackShadeFirst+firstShadePreMade + fastTrackColorLine+fullLinePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));

        // printing the line with second shade now
        buffer = "\033[" + std::to_string(startingRow) + ";1H" + fastTrackShadeSecond+secondShadePreMade + fastTrackShadeFirst+firstShadePreMade + fastTrackColorLine+fullLinePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));

        // printing the line with third shade now
        buffer = "\033[" + std::to_string(startingRow) + ";1H" + fastTrackShadeThird+thirdShadePreMade + fastTrackShadeSecond+secondShadePreMade + fastTrackShadeFirst+firstShadePreMade + fastTrackColorLine+fullLinePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));

        // printing the line with fourth shade now
        buffer = "\033[" + std::to_string(startingRow) + ";1H" + fastTrackShadeFourth+fourthShadePreMade + fastTrackShadeThird+thirdShadePreMade + fastTrackShadeSecond+secondShadePreMade + fastTrackShadeFirst+firstShadePreMade + fastTrackColorLine+fullLinePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));

        // making final buffer without coordainte so i can use direct coords
        buffer = spacePreMade + fastTrackShadeFourth+fourthShadePreMade + fastTrackShadeThird+thirdShadePreMade + fastTrackShadeSecond+secondShadePreMade + fastTrackShadeFirst+firstShadePreMade + fastTrackColorLine+fullLinePreMade;

        // now using loop to simulate remaining one with space too so it clean the text behind
        for (int i = 0; i < heightOfTerminal - startingRow - 4; ++i) {
            int row = startingRow + i;
            std::string command = "\033[" + std::to_string(row) + ";1H" + buffer;
            write(STDOUT_FILENO, command.c_str(), command.size());
            std::this_thread::sleep_for(std::chrono::duration<double>(speed));
        }

        // Manual final animation: follow the same pattern used earlier in the method,
        // but apply it to the last visible row so the screen is cleaned smoothly.

        // so i taking exact time the row stopped at so
        int stoppedRow = startingRow + heightOfTerminal - startingRow - 4;
        // now i do stopeedRow++ and remove line then shade then another untill sapce fills last line

        buffer = "\033[" + std::to_string(stoppedRow++) + ";1H" + spacePreMade + fastTrackShadeThird+thirdShadePreMade + fastTrackShadeSecond+secondShadePreMade + fastTrackShadeFirst+firstShadePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));

        // next transition
        buffer = "\033[" + std::to_string(stoppedRow++) + ";1H" + spacePreMade + fastTrackShadeSecond+secondShadePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));

        // next one
        buffer = "\033[" + std::to_string(stoppedRow++) + ";1H" + spacePreMade + fastTrackShadeThird+thirdShadePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));
        
        // next one
        buffer = "\033[" + std::to_string(stoppedRow++) + ";1H" + spacePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));

        // last one
        buffer = "\033[" + std::to_string(stoppedRow++) + ";1H" + spacePreMade;
        write(STDOUT_FILENO, buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::duration<double>(speed));

        std::cout << "\033[" << startingRow << ";1H"; // Move cursor to the starting row
        std::cout << "\033[0m"; // Reset color
        std::cout << "\033[?25h"; // Show cursor
        return 0;
    }
};