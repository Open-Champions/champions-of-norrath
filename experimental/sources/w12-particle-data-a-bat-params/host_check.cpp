#include <cstring>

extern "C" const char* batParams[2];

int main() {
    return std::strcmp(batParams[0], "oneshot=1") == 0 && batParams[1] == nullptr ? 0 : 1;
}
