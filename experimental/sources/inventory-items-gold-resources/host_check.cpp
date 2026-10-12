extern "C" unsigned char goldModel[16];
extern "C" unsigned char goldTex[16];
int main() {
    if (sizeof goldModel != 16 || sizeof goldTex != 16) return 1;
    if (goldModel == goldTex) return 2;
    for (int i = 0; i < 16; ++i) if (goldModel[i] != 0 || goldTex[i] != 0) return 3;
    return 0;
}
