#include "recall_inhibit.cpp"
int main() {
    if (gameRecallInhibit != 0) return 1;
    gameRecallInhibit = -3;
    if (gameRecallInhibit != -3) return 2;
    return 0;
}
