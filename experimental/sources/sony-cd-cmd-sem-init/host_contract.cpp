#include <cstdio>
#include <string>
#include <vector>
#include "cmd_sem_init.cpp"

static std::vector<std::string> seen;
static int next_id = 10;
extern "C" int _sceCd_ncmd_semid = 0;
extern "C" int _sceCd_scmd_semid = 0;
extern "C" int _sceCd_rcmd_semid = 0;
extern "C" int cb_semid = 0;
extern "C" int _sceCd_c_cb_sem = 0;
extern "C" int CreateSema(void* parameters) {
    auto* bytes = static_cast<unsigned char*>(parameters);
    const unsigned init = bytes[4] | (bytes[5] << 8) | (bytes[6] << 16) | (bytes[7] << 24);
    const unsigned maximum = bytes[8] | (bytes[9] << 8) | (bytes[10] << 16) | (bytes[11] << 24);
    const unsigned option = bytes[20] | (bytes[21] << 8) | (bytes[22] << 16) | (bytes[23] << 24);
    seen.push_back(std::to_string(init) + ":" + std::to_string(maximum) + ":" + std::to_string(option));
    return next_id++;
}
int main() {
    _sceCd_ncmd_semid = 1;
    _sceCd_scmd_semid = 2;
    _sceCd_rcmd_semid = 3;
    cmd_sem_init();
    if (!seen.empty()) return 1;
    _sceCd_scmd_semid = -1;
    _sceCd_c_cb_sem = 9;
    cmd_sem_init();
    if (seen.size() != 4) return 2;
    if (seen[0] != "1:1:0" || seen[3] != "1:0:0") return 3;
    if (_sceCd_ncmd_semid != 10 || _sceCd_scmd_semid != 11 || _sceCd_rcmd_semid != 12 || cb_semid != 13) return 4;
    if (_sceCd_c_cb_sem != 0) return 5;
    std::puts("cmd sem init contract passed");
    return 0;
}
