#include "decode_q_head.cpp"
int main() {
    if (sizeof(decodeQHead) != 4) return 1;
    if (decodeQHead != 0) return 2;
    decodeQHead = 1;
    if (decodeQHead != 1) return 3;
    return 0;
}
