// Host contract for freeNodes. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char* freeNodes;
int main() {
    if (freeNodes != nullptr) {
        std::fprintf(stderr, "fail: freeNodes\n");
        return 1;
    }
    freeNodes = reinterpret_cast<unsigned char*>(1);
    if (freeNodes == nullptr) return 1;
    return 0;
}
