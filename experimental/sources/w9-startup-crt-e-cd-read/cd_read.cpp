// Observed sceCdRead at 0x002e38e8, 480 bytes. Arguments: sector, sector count, EE
// destination, and a pointer to three mode bytes. Bit 0 of _sceCd_ee_read_mode
// (0x004460b8) set skips the readiness check; otherwise sceCdNcmdDiskReady() returning 6
// ends the call with 0. _sceCd_ncmd_prechk(4) returning zero also returns 0.
// The 24-byte command at _sceCd_ncmdsdata (0x00446180) then receives the sector, count and
// destination as words, the three mode bytes at +12..+14, and the addresses of
// _sceCd_rd_intr_data (0x00447180) at +16 and _sceCd_Read_cur_pos (0x00447240) at +20.
// The byte count for the cache write-back is count*2328 for data pattern byte 1,
// count*2340 for 2 and count*2048 for anything else. The first word of
// _sceCd_Read_cur_pos is cleared; unless bit 1 of the read mode is set the destination is
// written back for that many bytes; then 144 bytes of _sceCd_rd_intr_data, the 24-byte
// command and 4 bytes of _sceCd_Read_cur_pos are written back. A positive SCE_CD_debug
// (0x00446090) prints "call cdread cmd\n". sceCdCbfunc_num (0x004460d8) and _sceCd_c_cb_sem
// (0x004460b4) are set to 1 and sceSifCallRpc runs on _sceCd_cd_ncmd (0x00447250) with
// function 1, mode 1, the command as the 24-byte send area, no receive area, end function
// _sceCd_cd_read_intr and _sceCd_rd_intr_data as its parameter. A negative result clears
// both words, signals _sceCd_ncmd_semid (0x004460a8) and returns 0. Otherwise a positive
// SCE_CD_debug prints "cdread end\n" and the result is 1.
// The cd-archive package carries a norrath_-named body of the same call; this is the
// exported symbol.
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
void _sceCd_cd_read_intr();
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

int sceCdRead(unsigned sector, unsigned count, void* destination, const unsigned char* mode) {
    if ((_sceCd_ee_read_mode & 1u) == 0u) {
        if (sceCdNcmdDiskReady() == 6) return 0;
    }
    if (_sceCd_ncmd_prechk(4) == 0) return 0;
    unsigned char* const command = _sceCd_ncmdsdata;
    store_word(command + 0, sector);
    store_word(command + 4, count);
    store_word(command + 8, pointer_bits(destination));
    command[12] = mode[0];
    command[13] = mode[1];
    command[14] = mode[2];
    store_word(command + 16, pointer_bits(_sceCd_rd_intr_data));
    store_word(command + 20, pointer_bits(_sceCd_Read_cur_pos));
    unsigned bytes;
    if (mode[2] == 1u)
        bytes = count * 2328u;
    else if (mode[2] == 2u)
        bytes = count * 2340u;
    else
        bytes = count << 11;
    store_word(_sceCd_Read_cur_pos, 0u);
    if ((_sceCd_ee_read_mode & 2u) == 0u) sceSifWriteBackDCache(destination, static_cast<int>(bytes));
    sceSifWriteBackDCache(_sceCd_rd_intr_data, 144);
    sceSifWriteBackDCache(command, 24);
    sceSifWriteBackDCache(_sceCd_Read_cur_pos, 4);
    if (SCE_CD_debug > 0) scePrintf("call cdread cmd\n");
    sceCdCbfunc_num = 1;
    _sceCd_c_cb_sem = 1;
    const int rpc = sceSifCallRpc(_sceCd_cd_ncmd, 1u, 1u, command, 24, nullptr, 0,
                                  reinterpret_cast<void*>(_sceCd_cd_read_intr), _sceCd_rd_intr_data);
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
