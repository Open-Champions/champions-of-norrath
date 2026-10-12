#include "decode_q_tail.cpp"
int main() {
    if (sizeof(decodeQTail) != 4) return 1;
    if (decodeQTail != 0) return 2;
    decodeQTail = 1;
    if (decodeQTail != 1) return 3;
    return 0;
}
