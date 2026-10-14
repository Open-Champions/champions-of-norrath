#include "pad_index_map.cpp"

int main() {
    if (sizeof padIndexMap != 4u) return 1;
    for (int index = 0; index < 4; ++index)
        if (padIndexMap[index] != 0) return 2;
    return 0;
}
