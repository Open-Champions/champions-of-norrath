// Observed sceCdRead at 0x002e38e8.
// Bit 0 of _sceCd_ee_read_mode at 0x004460b8 skips sceCdNcmdDiskReady. A ready
// result of 6 returns 0. _sceCd_ncmd_prechk(4) must be nonzero. The 24-byte
// command at _sceCd_ncmdsdata (0x00446180) stores the sector, count, destination,
// three mode bytes, then pointers to _sceCd_rd_intr_data and _sceCd_Read_cur_pos.
// Mode byte 2 selects 2048, 2328, or 2340 bytes per sector. Bit 1 of the read
// mode skips the destination cache writeback. The first word of the position
// block is cleared before the writebacks. RPC function 1 on _sceCd_cd_ncmd uses
// end function _sceCd_cd_read_intr and parameter _sceCd_rd_intr_data. Failure
// clears c_cnt.66 and _sceCd_c_cb_sem, signals _sceCd_ncmd_semid, and returns 0.
extern "C" {
extern unsigned _sceCd_ee_read_mode;
extern int SCE_CD_debug;
extern int _sceCd_c_cb_sem;
extern unsigned norrath_archive_cd_command_count_view;
extern int _sceCd_ncmd_semid;
extern unsigned char _sceCd_ncmdsdata[];
extern unsigned char _sceCd_rd_intr_data[];
extern unsigned char _sceCd_Read_cur_pos[];
extern unsigned char _sceCd_cd_ncmd[];
int sceCdNcmdDiskReady();
int _sceCd_ncmd_prechk(int command);
void sceSifWriteBackDCache(void* address, int bytes);
int norrath_sce_printf(const char* format, ...);
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter);
void _sceCd_cd_read_intr();
int norrath_player_signal_semaphore(int semaphore);
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
int norrath_archive_cd_read(unsigned sector, unsigned count, void* destination, const unsigned char* mode) {
    if ((_sceCd_ee_read_mode & 1u) == 0u) {
        if (sceCdNcmdDiskReady() == 6) return 0;
    }
    if (_sceCd_ncmd_prechk(4) == 0) return 0;
    unsigned char* command = _sceCd_ncmdsdata;
    store_word(command + 0, sector);
    store_word(command + 4, count);
    store_word(command + 8, pointer_bits(destination));
    command[12] = mode[0];
    command[13] = mode[1];
    command[14] = mode[2];
    store_word(command + 16, pointer_bits(_sceCd_rd_intr_data));
    store_word(command + 20, pointer_bits(_sceCd_Read_cur_pos));
    const unsigned char pattern = mode[2];
    unsigned bytes;
    if (pattern == 1u) bytes = count * 2328u;
    else if (pattern == 2u) bytes = count * 2340u;
    else bytes = count << 11;
    store_word(_sceCd_Read_cur_pos, 0u);
    if ((_sceCd_ee_read_mode & 2u) == 0u) sceSifWriteBackDCache(destination, static_cast<int>(bytes));
    sceSifWriteBackDCache(_sceCd_rd_intr_data, 144);
    sceSifWriteBackDCache(command, 24);
    sceSifWriteBackDCache(_sceCd_Read_cur_pos, 4);
    if (SCE_CD_debug > 0) norrath_sce_printf("call cdread cmd\n");
    norrath_archive_cd_command_count_view = 1u;
    _sceCd_c_cb_sem = 1;
    const int rpc = sceSifCallRpc(_sceCd_cd_ncmd, 1u, 1u, command, 24, nullptr, 0,
                                  reinterpret_cast<void*>(_sceCd_cd_read_intr), _sceCd_rd_intr_data);
    if (rpc < 0) {
        norrath_archive_cd_command_count_view = 0u;
        _sceCd_c_cb_sem = 0;
        norrath_player_signal_semaphore(_sceCd_ncmd_semid);
        return 0;
    }
    if (SCE_CD_debug > 0) norrath_sce_printf("cdread end\n");
    return 1;
}
}
