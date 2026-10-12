// Host check for null_buff.cpp; not an EE plan input.
#include "null_buff.cpp"

int main() {
    if (nullBuff[0] != 0 || nullBuff[16383] != 0) return 1;
    nullBuff[16383] = 2;
    if (nullBuff[16383] != 2) return 2;
    return 0;
}
