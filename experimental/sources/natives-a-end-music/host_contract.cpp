static int calls;
static int stream = -1;
extern "C" int soundFadeStream__Fi(int got) {
    ++calls;
    stream = got;
    return 4;
}
extern "C" int amxEndMusic__FP5__amxPi(void*, int*);
int main() {
    if (amxEndMusic__FP5__amxPi(0, 0) != 0) return 1;
    if (calls != 1 || stream != 0) return 2;
    return 0;
}
