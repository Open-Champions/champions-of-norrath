// Observed CD and SIF words. File-backed values are the image bytes. BSS
// objects are zero in the file and stay zero here. Sizes are the symbol sizes,
// or the span to the next symbol when the ELF size is zero.
extern "C" {
int SCE_CD_debug = 0;
int _sceCd_ncmd_semid = -1;
int _sceCd_scmd_semid = -1;
int _sceCd_rcmd_semid = -1;
int _sceCd_c_cb_sem = 0;
unsigned _sceCd_ee_read_mode = 0;
int _ncmd_bind = -1;
int _sf_bind = -1;
int _scmd_bind = -1;
int ncmd_sema_keep_cmd = 0;
int scmd_sema_keep_cmd = 0;
int cb_thid = 0;
int cb_semid = -1;
int stm_status = 0;
int _sceSifInitCheck = 0;
int sceCdCbfunc_num = 0;
int sceCdCbfunc_number = 0;
unsigned sceCdCbfunc = 0;
int my_thid = 0;
alignas(4) unsigned char _sceCd_cd_ncmd[40] = {};
alignas(4) unsigned char _sceCd_cd_scmd[40] = {};
alignas(4) unsigned char _sceCd_ncmdsdata[0x1000] = {};
alignas(4) unsigned char _sceCd_ncmdrdata[0x60] = {};
alignas(4) unsigned char _sceCd_scmdrdata[0x420] = {};
alignas(4) unsigned char _sceCd_rd_intr_data[0x90] = {};
alignas(4) unsigned char _sceCd_Read_cur_pos[0x10] = {};
alignas(4) unsigned char _data_table[64] = {};
alignas(4) unsigned char _packet_buffer[2048] = {};
alignas(4) unsigned char _free_buffer[2048] = {};
alignas(4) unsigned char _free_buffer2[2048] = {};
alignas(4) unsigned char _sf_data[320] = {};
alignas(4) unsigned char sfrdata[64] = {};
alignas(4) unsigned char cdrdsf[40] = {};
alignas(4) unsigned char dum_mode[40] = {};
alignas(4) unsigned char my_th_info[48] = {};
}
