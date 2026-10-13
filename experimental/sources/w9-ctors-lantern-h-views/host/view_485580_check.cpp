// Host comparison only. Not an EE build input.
#include <cstring>
#include "../view_485580.cpp"
int main() {
    static const char* const want[11] = {"vaseweb", "hgurn", "goyle", "boxafay", "boxbfay", "boxcfay", "lavakeg", "hatebox", "uncrate", "dt_pota", "dt_potb"};
    if (sizeof norrath_ctors_ant_lantern_view_485580 != 88) return 1;
    for (int i = 0; i < 11; ++i) {
        if (std::strcmp(norrath_ctors_ant_lantern_view_485580[i], want[i]) != 0) return 2;
        for (unsigned j = std::strlen(want[i]); j < 8; ++j) if (norrath_ctors_ant_lantern_view_485580[i][j] != 0) return 3;
    }
    return 0;
}
