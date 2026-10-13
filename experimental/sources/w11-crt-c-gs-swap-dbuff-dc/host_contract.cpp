// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp gs_swap_dbuff_dc.cpp
#include <vector>
extern "C" {
alignas(16) static unsigned char db[816];
static std::vector<int> calls;   // kind * 1000 + offset
void sceGsPutDispEnv(unsigned char* env) { calls.push_back(1000 + static_cast<int>(env - db)); }
void sceGsPutDrawEnv(unsigned char* packet) { calls.push_back(2000 + static_cast<int>(packet - db)); }
void sceGsSwapDBuffDc(unsigned char* d, int id);
}
int main() {
    struct { int id; int disp; int draw; } cases[] = {{0, 0, 80}, {1, 40, 448}, {2, 0, 80}, {3, 40, 448}, {-1, 40, 448}, {-2, 0, 80}, {0x7fffffff, 40, 448}, {256, 0, 80}};
    for (auto& c : cases) {
        calls.clear();
        sceGsSwapDBuffDc(db, c.id);
        if (calls != std::vector<int>({1000 + c.disp, 2000 + c.draw})) return 1;
    }
    return 0;
}
