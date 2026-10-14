#include <cstdio>
#include <cstring>
#include "words.cpp"
int main() {
    if (SCE_CD_debug != 0 || _sceCd_c_cb_sem != 0 || _sceCd_ee_read_mode != 0u || stm_status != 0) return 1;
    if (_sceCd_ncmd_semid != -1 || _sceCd_scmd_semid != -1 || _sceCd_rcmd_semid != -1 || cb_semid != -1) return 2;
    if (_ncmd_bind != -1 || _sf_bind != -1 || _scmd_bind != -1) return 3;
    if (sizeof _sceCd_ncmdsdata != 0x1000u || sizeof _sceCd_cd_ncmd != 40u || sizeof _sf_data != 320u) return 4;
    if (sizeof sfrdata != 64u || sizeof cdrdsf != 40u || sizeof dum_mode != 40u || sizeof my_th_info != 48u) return 5;
    if (sizeof _data_table != 64u || sizeof _packet_buffer != 2048u || sizeof _free_buffer2 != 2048u) return 6;
    if (std::memcmp(_sceCd_ncmdsdata, _sceCd_ncmdsdata + 1, sizeof _sceCd_ncmdsdata - 1) != 0) return 7;
    if (_sceCd_ncmdsdata[0] != 0 || _sf_data[0] != 0 || my_thid != 0) return 8;
    std::puts("sony cd words contract passed");
    return 0;
}
