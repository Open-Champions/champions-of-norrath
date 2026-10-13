// Observed sceCdReadIOPm at 0x002e3ac8, 340 bytes. Arguments: sector, sector count, IOP
// destination and a pointer to three mode bytes. The readiness gate is the same as in
// sceCdRead (bit 0 of _sceCd_ee_read_mode at 0x004460b8 skips it; a sceCdNcmdDiskReady()
// result of 6 returns 0), then _sceCd_ncmd_prechk(5) returning zero returns 0.
// The 24-byte command at _sceCd_ncmdsdata (0x00446180) is filled exactly as in sceCdRead
// (words sector, count, destination; three mode bytes; addresses of _sceCd_rd_intr_data
// and _sceCd_Read_cur_pos) and written back, 24 bytes only. There is no byte-count
// computation and no other write-back. sceCdCbfunc_num (0x004460d8) and _sceCd_c_cb_sem
// (0x004460b4) are set to 1 and sceSifCallRpc runs on _sceCd_cd_ncmd (0x00447250) with
// function 13, mode 1, the command as the 24-byte send area, no receive area, end function
// _sceCd_cd_callback and the address of sceCdCbfunc_num as its parameter. A negative result
// clears both words, signals _sceCd_ncmd_semid (0x004460a8) and returns 0. Otherwise a
// positive SCE_CD_debug (0x00446090) prints "cdread end\n" and the result is 1.
// The write-back happens after the 24 bytes are filled and before the two words are set.
extern "C" {
extern unsigned _sceCd_ee_read_mode;
extern int SCE_CD_debug;
extern int _sceCd_c_cb_sem;
extern int sceCdCbfunc_num;
extern int _sceCd_ncmd_semid;
extern unsigned char _sceCd_ncmdsdata[];
extern unsigned char _sceCd_rd_intr_data[];
extern unsigned char _sceCd_Read_cur_pos[];
extern unsigned char _sceCd_cd_ncmd[];
int sceCdNcmdDiskReady();
int _sceCd_ncmd_prechk(int command);
void _sceCd_cd_callback();
int scePrintf(const char* format, ...);
int SignalSema(int semaphore);
void sceSifWriteBackDCache(void* address, int size);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);

static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

int sceCdReadIOPm(unsigned sector, unsigned count, void* destination, const unsigned char* mode) {
    if ((_sceCd_ee_read_mode & 1u) == 0u) {
        if (sceCdNcmdDiskReady() == 6) return 0;
    }
    if (_sceCd_ncmd_prechk(5) == 0) return 0;
    unsigned char* const command = _sceCd_ncmdsdata;
    store_word(command + 0, sector);
    store_word(command + 4, count);
    store_word(command + 8, pointer_bits(destination));
    command[12] = mode[0];
    command[13] = mode[1];
    command[14] = mode[2];
    store_word(command + 16, pointer_bits(_sceCd_rd_intr_data));
    store_word(command + 20, pointer_bits(_sceCd_Read_cur_pos));
    sceSifWriteBackDCache(command, 24);
    sceCdCbfunc_num = 1;
    _sceCd_c_cb_sem = 1;
    const int rpc = sceSifCallRpc(_sceCd_cd_ncmd, 13u, 1u, command, 24, nullptr, 0,
                                  reinterpret_cast<void*>(_sceCd_cd_callback), &sceCdCbfunc_num);
    if (rpc < 0) {
        sceCdCbfunc_num = 0;
        _sceCd_c_cb_sem = 0;
        SignalSema(_sceCd_ncmd_semid);
        return 0;
    }
    if (SCE_CD_debug > 0) scePrintf("cdread end\n");
    return 1;
}
}
