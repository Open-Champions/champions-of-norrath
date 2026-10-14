#include "mca_cmd.cpp"

#include <cstdio>

extern "C" int queueEnd = 0;
extern "C" int queueStart = 0;
extern "C" McCommand commandQueue[256] = {};

int main() {
    const char* text = "debug.sht";
    queueEnd = 0;
    queueStart = 5;
    MCA_Cmd__Fiiii(8, nullptr, 0, 0);
    if (queueEnd != 1 || queueStart != 5) return 1;
    if (commandQueue[0].command != 8 || commandQueue[0].text != nullptr ||
        commandQueue[0].third != 0 || commandQueue[0].fourth != 0) return 2;
    MCA_Cmd__Fiiii(6, text, 3, 4);
    if (queueEnd != 2 || commandQueue[1].command != 6 || commandQueue[1].text != text ||
        commandQueue[1].third != 3 || commandQueue[1].fourth != 4) return 3;
    if (commandQueue[0].command != 8) return 4;

    queueEnd = 255;
    queueStart = 4;
    MCA_Cmd__Fiiii(1, text, 2, 9);
    if (queueEnd != 0) return 5;
    if (commandQueue[255].command != 1 || commandQueue[255].text != text ||
        commandQueue[255].third != 2 || commandQueue[255].fourth != 9) return 6;

    queueEnd = 7;
    queueStart = 8;
    commandQueue[7].command = 0;
    MCA_Cmd__Fiiii(11, text, 12, 13);
    if (queueEnd != 8 || commandQueue[7].command != 11 || commandQueue[7].fourth != 13) return 7;
    std::puts("mca cmd passed");
    return 0;
}
