extern "C" unsigned char goldModel[16];
extern "C" unsigned char goldTex[16];
static_assert(sizeof goldModel == 16 && sizeof goldTex == 16, "each object is 16 bytes");
int main() {
    if (+goldModel == +goldTex) return 1;
    for (int i = 0; i < 16; ++i) if (goldModel[i] != 0 || goldTex[i] != 0) return 2;
    return 0;
}
