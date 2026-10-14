#include "storage.hpp"
// The word is a target pointer (4 bytes on the EE); the host width differs, so the host
// check covers the initial null value and that a store reads back.
int main() {
    if (cutCurrentScene != nullptr) return 1;
    static int marker;
    cutCurrentScene = &marker;
    return cutCurrentScene == &marker ? 0 : 2;
}
