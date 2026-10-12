extern "C" {
int fadeDownVal;
void hudResetFadeScreen__Fi(int fade_to_black);
}

int main() {
    fadeDownVal = 55;
    hudResetFadeScreen__Fi(0);
    if (fadeDownVal != 0) return 1;
    hudResetFadeScreen__Fi(1);
    if (fadeDownVal != 128) return 2;
    fadeDownVal = 9;
    hudResetFadeScreen__Fi(-4);
    if (fadeDownVal != 128) return 3;
    hudResetFadeScreen__Fi(0);
    return fadeDownVal != 0;
}
