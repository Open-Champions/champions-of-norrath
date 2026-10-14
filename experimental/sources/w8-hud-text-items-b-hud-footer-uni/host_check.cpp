extern "C" unsigned short hudFooterUni[128];

int main() {
    if (sizeof(hudFooterUni) != 256) return 1;
    for (int i = 0; i < 128; ++i) {
        if (hudFooterUni[i] != 0) return 2;
    }
    hudFooterUni[127] = 9;
    return hudFooterUni[127] == 9 ? 0 : 3;
}
