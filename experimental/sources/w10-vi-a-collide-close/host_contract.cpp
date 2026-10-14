#include "close.cpp"

static void* seen;
static int result;
extern "C" int Clear__9VICollide(void* object) {
    seen = object;
    return result;
}

int main() {
    char object[4];
    result = 0;
    if (Close__9VICollide(object) != 0 || seen != object) return 1;
    result = 9;
    if (Close__9VICollide(object) != 0) return 2;
    result = -1;
    if (Close__9VICollide(object) != -1) return 3;
    result = -77;
    if (Close__9VICollide(object) != -1) return 4;
    return 0;
}
