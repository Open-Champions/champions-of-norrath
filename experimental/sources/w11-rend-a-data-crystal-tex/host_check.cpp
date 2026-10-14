extern "C" char* crystalTex[4];
int main() {
    if (sizeof crystalTex != 4 * sizeof(char*)) return 1;
    for (int i = 0; i != 4; ++i)
        if (crystalTex[i] != nullptr) return 2;
    char c;
    crystalTex[3] = &c;
    return crystalTex[3] == &c ? 0 : 3;
}
