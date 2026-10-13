extern "C" int fastestRotDirection__FssPi(short a, short b, int* out);
int main() {
    int o = 123;
    if (fastestRotDirection__FssPi(10, 4, &o) != -1 || o != 6) return 1;
    if (fastestRotDirection__FssPi(4, 10, &o) != 1 || o != 6) return 2;
    if (fastestRotDirection__FssPi(5, 5, &o) != 1 || o != 0) return 3;
    // 16-bit wrap: 32767 - (-1) wraps to -32768
    if (fastestRotDirection__FssPi(32767, -1, &o) != 1 || o != 32768) return 4;
    o = 77;
    if (fastestRotDirection__FssPi(10, 4, nullptr) != -1 || o != 77) return 5;
    if (fastestRotDirection__FssPi(4, 10, nullptr) != 1) return 6;
    return 0;
}
