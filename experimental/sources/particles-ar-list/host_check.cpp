// Host contract for arList. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char* arList;
int main() {
    if (arList != nullptr) {
        std::fprintf(stderr, "fail: arList\n");
        return 1;
    }
    arList = reinterpret_cast<unsigned char*>(1);
    if (arList == nullptr) return 1;
    return 0;
}
