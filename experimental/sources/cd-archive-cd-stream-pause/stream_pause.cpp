// Observed sceCdStPause at 0x002e4368. The store of zero to stm_status at
// 0x00447c34 is the delay of the debug branch, so it happens before the
// optional "sceCdStPause call\n" print. The stream call is (0, 0, 0, 7, dum_mode).
extern "C" {
extern int SCE_CD_debug;
extern int stm_status;
extern unsigned char dum_mode[];
int norrath_sce_printf(const char* format, ...);
int sceCdStream(int argument, int count, void* buffer, int command, void* mode);
int norrath_archive_cd_stream_pause() {
    stm_status = 0;
    if (SCE_CD_debug > 0) norrath_sce_printf("sceCdStPause call\n");
    return sceCdStream(0, 0, nullptr, 7, dum_mode);
}
}
