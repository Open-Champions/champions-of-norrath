#include "setup.cpp"

static int calls;
static int result = 5;

extern "C" int setup_main__Fv() {
    ++calls;
    return result;
}

int main() {
    unsigned char object[4] = {9, 9, 9, 9};
    if (Setup__10VILauncher(reinterpret_cast<VILauncher*>(object)) != 5) return 1;
    result = -3;
    if (Setup__10VILauncher(reinterpret_cast<VILauncher*>(object)) != -3) return 2;
    if (calls != 2 || object[0] != 9) return 3;
    return 0;
}
