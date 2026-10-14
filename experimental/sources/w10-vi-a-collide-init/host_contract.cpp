#include "init.cpp"

static void* seen[4];
static int count;
extern "C" void Identity__10VIMatrix44(void* matrix) {
    if (count < 4) seen[count] = matrix;
    ++count;
}

int main() {
    unsigned char object[600];
    int result = Init__9VICollide(object);
    if (result != 0 || count != 2) return 1;
    if (seen[0] != object + 16 || seen[1] != object + 80) return 2;
    return 0;
}
