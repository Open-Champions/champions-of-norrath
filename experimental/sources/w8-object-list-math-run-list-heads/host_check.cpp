// Host check for the list-head storage extents. Not an EE plan input.
#include <cstdio>
#include "../w8-object-list-math-common/list_types.hpp"
struct HeadSlot { ListHead head; unsigned unobserved_word; };
extern "C" { extern HeadSlot runList; extern HeadSlot deleteList; extern ListHead mediumRunList[16]; extern ListHead slowRunList[60]; }
int main() {
    int failures = 0;
    auto zero = [&](const void* p, unsigned n) {
        const unsigned char* b = static_cast<const unsigned char*>(p);
        for (unsigned i = 0; i < n; ++i) if (b[i]) return false;
        return true;
    };
    if (!zero(&runList, sizeof runList) || !zero(&deleteList, sizeof deleteList) || !zero(mediumRunList, sizeof mediumRunList) ||
        !zero(slowRunList, sizeof slowRunList)) { std::fprintf(stderr, "fail: storage is not zero\n"); ++failures; }
    if (sizeof(ListHead) != sizeof(void*)) { std::fprintf(stderr, "fail: head is a pointer\n"); ++failures; }
    if (failures) return 1;
    std::printf("run-list head storage check passed\n");
    return 0;
}
