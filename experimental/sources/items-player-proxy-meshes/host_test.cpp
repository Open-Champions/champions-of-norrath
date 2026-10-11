#include <cstring>
struct ProxyMeshEntry {
    const char* model;
    const char* texture;
    unsigned unused_a;
    unsigned unused_b;
};
static const ProxyMeshEntry rows[] = {
#include "rows.hpp"
};
int main() {
    if (sizeof rows / sizeof rows[0] != 106u) return 1;
    if (std::strcmp(rows[0].model, "arrow1.vif") != 0) return 2;
    if (std::strcmp(rows[0].texture, "arrow1.tex") != 0) return 3;
    if (std::strcmp(rows[4].model, "short_sword1.vif") != 0) return 4;
    if (rows[105].model != nullptr || rows[105].texture != nullptr) return 5;
    for (unsigned index = 0; index < 106u; ++index)
        if (rows[index].unused_a != 0u || rows[index].unused_b != 0u) return 6;
    return 0;
}
