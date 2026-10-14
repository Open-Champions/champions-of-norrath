// Host check for recall_pos.cpp; not an EE plan input.
#include "recall_pos.cpp"

int main() {
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 3; ++j) if (recallPos[i][j] != 0.0f) return 1;
    return 0;
}
