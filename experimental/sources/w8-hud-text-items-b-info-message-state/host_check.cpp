extern "C" {
extern unsigned short infoMessage[100];
extern unsigned short infoMessageTimer;
extern signed char infoMessageAlways;
}

int main() {
    if (sizeof(infoMessage) != 200) return 1;
    if (infoMessageTimer != 0 || infoMessageAlways != 0) return 2;
    for (int i = 0; i < 100; ++i) {
        if (infoMessage[i] != 0) return 3;
    }
    infoMessage[99] = 7;
    return infoMessage[99] == 7 ? 0 : 4;
}
