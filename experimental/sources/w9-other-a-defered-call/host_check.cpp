#include "defered_call.cpp"
int main() {
    if (sizeof(deferedCall) != 10240) return 1;
    for (unsigned char b : deferedCall) if (b != 0) return 2;
    return 0;
}
