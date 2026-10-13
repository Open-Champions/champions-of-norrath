/* Host contract. Not a plan input. CreateSema is a recording stub. */
#include "sce_fs_iob_sema_mk.cpp"
#include <cstdio>
#include <vector>
extern "C" { int _fs_iob_semid = -1; int _fs_fsq_semid = 5; }
static std::vector<int> seen;
static int next_id = 40;
extern "C" int CreateSema(void* p) {
    const int* w = static_cast<int*>(p);
    seen.push_back(w[1]); seen.push_back(w[2]); seen.push_back(w[5]);
    return next_id++;
}
int main() {
    _sceFsIobSemaMK();
    if (_fs_iob_semid != 40 || _fs_fsq_semid != 41 || seen != std::vector<int>{1, 1, 0, 1, 1, 0}) { std::puts("create path wrong"); return 1; }
    seen.clear();
    _sceFsIobSemaMK();   // already created: nothing happens
    if (!seen.empty() || _fs_iob_semid != 40 || _fs_fsq_semid != 41) { std::puts("second call not a no-op"); return 1; }
    _fs_iob_semid = 0; _sceFsIobSemaMK();
    if (!seen.empty()) { std::puts("only -1 may trigger creation"); return 1; }
    std::puts("_sceFsIobSemaMK contract passed");
    return 0;
}
