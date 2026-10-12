#include "megatile_storage.cpp"
int main() {
    if (mgrg != nullptr) return 1;
    if (sizeof mt_tiledata != 320u) return 2;
    for (unsigned i = 0; i < sizeof mt_tiledata; ++i) if (mt_tiledata[i] != 0) return 3;
    return 0;
}
