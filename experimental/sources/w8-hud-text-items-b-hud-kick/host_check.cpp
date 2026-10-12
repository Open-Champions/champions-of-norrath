extern "C" {
int nmPlayers;
signed char multiHelpMessageOn[8];
signed char multiRelease[8];
int yesNoPos[8];
int activeDelay[8];
signed char helpMessageOn;
void hudKick__Fv();
}

static void fill() {
    for (int i = 0; i < 8; ++i) {
        multiHelpMessageOn[i] = 9;
        multiRelease[i] = 9;
        yesNoPos[i] = 9;
        activeDelay[i] = 9;
    }
    helpMessageOn = 9;
}

int main() {
    fill();
    nmPlayers = 3;
    hudKick__Fv();
    for (int i = 0; i < 3; ++i) {
        if (multiHelpMessageOn[i] != 0 || multiRelease[i] != 1 || yesNoPos[i] != 0 || activeDelay[i] != 3) return 1;
    }
    for (int i = 3; i < 8; ++i) {
        if (multiHelpMessageOn[i] != 9 || multiRelease[i] != 9 || yesNoPos[i] != 9 || activeDelay[i] != 9) return 2;
    }
    if (helpMessageOn != 0) return 3;
    fill();
    nmPlayers = 0;
    hudKick__Fv();
    if (helpMessageOn != 0) return 4;
    for (int i = 0; i < 8; ++i) {
        if (multiHelpMessageOn[i] != 9 || multiRelease[i] != 9 || yesNoPos[i] != 9 || activeDelay[i] != 9) return 5;
    }
    fill();
    nmPlayers = -2;
    hudKick__Fv();
    if (helpMessageOn != 0 || multiRelease[0] != 9) return 6;
    return 0;
}
