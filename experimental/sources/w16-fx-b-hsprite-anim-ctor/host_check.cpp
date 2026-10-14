// Host check only; not a build input.
#include "ctor.cpp"
static void* cleared; static int calls;
extern "C" int Clear__13VIHSpriteAnim(void* s) { cleared = s; calls++; return 0; }
int main() {
    int obj[20];
    for (int& w : obj) w = 0x5a5a5a5a;
    __13VIHSpriteAnim((char*)obj);
    if (calls != 1 || cleared != obj || obj[0] != 0 || obj[1] != 0 || obj[13] != 0) return 1;
    if (obj[2] != 0x5a5a5a5a || obj[12] != 0x5a5a5a5a || obj[14] != 0x5a5a5a5a) return 2;
    return 0;
}
