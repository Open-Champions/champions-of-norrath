#include "client_id.cpp"

#include <cstdio>

int main() {
    if (sizeof mcClientID != 40) return 1;
    for (int index = 0; index < 40; ++index) {
        if (mcClientID[index] != 0) return 2;
    }
    mcClientID[36] = 1;
    if (mcClientID[36] != 1 || mcClientID[35] != 0 || mcClientID[39] != 0) return 3;
    std::puts("mc client id passed");
    return 0;
}
