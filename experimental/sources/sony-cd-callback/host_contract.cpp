#include <cstdio>
#include <vector>
#include "callback.cpp"
static std::vector<int> signals;
extern "C" int sceCdCbfunc_num = 0;
extern "C" int sceCdCbfunc_number = 0;
extern "C" int _sceCd_c_cb_sem = 4;
extern "C" int _sceCd_ncmd_semid = 5;
extern "C" int cb_thid = 0;
extern "C" int cb_semid = 6;
extern "C" unsigned sceCdCbfunc = 0;
extern "C" int iSignalSema(int semaphore) {
    signals.push_back(semaphore);
    return 0;
}
int main() {
    unsigned char word[4] = {11, 0, 0, 0};
    _sceCd_cd_callback(word);
    if (sceCdCbfunc_num != 0 || sceCdCbfunc_number != 11 || _sceCd_c_cb_sem != 0 || !signals.empty()) return 1;
    word[0] = 3;
    cb_thid = 0;
    sceCdCbfunc = 1;
    _sceCd_c_cb_sem = 4;
    _sceCd_cd_callback(word);
    if (signals.size() != 1 || signals[0] != 5 || _sceCd_c_cb_sem != 0 || sceCdCbfunc_num != 0) return 2;
    if (sceCdCbfunc_number != 3) return 3;
    cb_thid = 2;
    sceCdCbfunc = 1;
    _sceCd_c_cb_sem = 4;
    signals.clear();
    _sceCd_cd_callback(word);
    if (signals.size() != 2 || signals[1] != 6 || _sceCd_c_cb_sem != 4) return 4;
    std::puts("cd callback contract passed");
    return 0;
}
