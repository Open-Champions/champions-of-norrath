#include "name_grid_words.cpp"
#include <cstdio>

int main() {
    int bad = sizeof(nameGridW) != 4 || sizeof(nameGridCount) != 4 || nameGridW != 0 || nameGridCount != 0;
    std::printf(bad ? "FAIL\n" : "ok: two zero words\n");
    return bad;
}
