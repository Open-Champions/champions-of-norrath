// Host check only; not an EE build input. Build the source with -Dstrlen=w8_strlen so it
// does not collide with the host C library, compile this file separately, and link both.
#include <cstdio>
#include <cstring>
extern "C" unsigned w8_strlen(const char*);
int main() {
    alignas(16) char buffer[96];
    unsigned checked = 0;
    for (unsigned offset = 0; offset < 20; ++offset) {
        for (unsigned length = 0; length < 70; ++length) {
            std::memset(buffer, 'x', sizeof buffer);
            buffer[offset + length] = '\0';
            buffer[offset + length + 1] = 'y';  // bytes past the NUL are ignored
            if (w8_strlen(buffer + offset) != length) return 1;
            ++checked;
        }
    }
    char high[] = {'\xff', '\x80', '\0'};
    if (w8_strlen(high) != 2) return 2;
    std::printf("strlen ok (%u cases)\n", checked);
    return 0;
}
