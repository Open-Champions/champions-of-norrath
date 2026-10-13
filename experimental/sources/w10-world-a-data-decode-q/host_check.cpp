#include "decode_q.cpp"
#include <cstring>
int main() {
    if (sizeof(decodeQ) != 512) return 1;
    for (unsigned i = 0; i < sizeof(decodeQ); i++) if (((const unsigned char*)&decodeQ[0])[i] != 0) return 2;
    return 0;
}
