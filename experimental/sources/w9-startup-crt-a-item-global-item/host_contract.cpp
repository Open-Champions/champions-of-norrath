#include "global_item.cpp"

#include <cstdio>
#include <cstring>

int main() {
    // The 16-byte record holds 32-bit pointers on the target; the host has wider ones.
    if (sizeof(globalItem) != 106 * sizeof(GlobalItemRecord)) return 1;
    if (sizeof(void*) == 4 && sizeof(GlobalItemRecord) != 16) return 1;
    for (int i = 0; i < 105; ++i) {
        const char* model = globalItem[i].model;
        const char* texture = globalItem[i].texture;
        if (model == nullptr || texture == nullptr) return 2;
        const unsigned long model_length = std::strlen(model);
        const unsigned long texture_length = std::strlen(texture);
        if (model_length != texture_length) return 3;
        if (std::strcmp(model + model_length - 4, ".vif") != 0) return 4;
        if (std::strcmp(texture + texture_length - 4, ".tex") != 0) return 5;
        if (std::strncmp(model, texture, model_length - 4) != 0) return 6;
        if (globalItem[i].first_zero != 0u || globalItem[i].second_zero != 0u) return 7;
    }
    if (globalItem[105].model != nullptr || globalItem[105].texture != nullptr) return 8;
    if (std::strcmp(globalItem[0].model, "arrow1.vif") != 0) return 9;
    if (std::strcmp(globalItem[32].model, "shield1.vif") != 0 || std::strcmp(globalItem[59].model, "shield1.vif") != 0) return 10;
    if (std::strcmp(globalItem[72].model, "bloodstone.vif") != 0 || std::strcmp(globalItem[97].model, "bloodstone.vif") != 0) return 11;
    if (std::strcmp(globalItem[104].texture, "mummyheart.tex") != 0 || std::strcmp(globalItem[65].texture, "mummyheart.tex") != 0) return 12;
    if (reinterpret_cast<const char*>(&globalItem[3]) - reinterpret_cast<const char*>(&globalItem[2]) != long(sizeof(GlobalItemRecord))) return 13;
    std::puts("global item table contract passed");
    return 0;
}
