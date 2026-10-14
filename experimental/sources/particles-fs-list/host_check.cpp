// Host contract for fsList. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char* fsList;
int main() {
    if (fsList != nullptr) {
        std::fprintf(stderr, "fail: fsList\n");
        return 1;
    }
    fsList = reinterpret_cast<unsigned char*>(1);
    if (fsList == nullptr) return 1;
    return 0;
}
