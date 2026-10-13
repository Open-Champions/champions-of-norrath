/* Observed sceSifFreeSysMemory at 0x002dc228, 120 bytes. When _bind
 * (0x00444a0c) is negative it returns 0 and makes no call. Otherwise the
 * address is stored in word 0 of the local sdata object (0x008e1d40) and
 * sceSifCallRpc runs on the local client cd (0x008e1cc0) with command 2, mode
 * 0, sdata as send area of 4 bytes, the local rdata (0x008e1d00) as receive
 * area of 4 bytes, no completion function and no argument. A negative result
 * returns -1; otherwise word 0 of rdata is returned. What the IOP does with
 * command 2 is not established here.
 */
extern "C" {
extern int _bind;
extern unsigned char norrath_sound_iop_heap_client_view[];
extern int norrath_sound_iop_heap_receive_view[];
extern int norrath_sound_iop_heap_send_view[];

int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* done, void* done_arg);

int sceSifFreeSysMemory(void* address) {
    if (_bind < 0) return 0;
    norrath_sound_iop_heap_send_view[0] = static_cast<int>(reinterpret_cast<unsigned long>(address));
    if (sceSifCallRpc(norrath_sound_iop_heap_client_view, 2, 0, norrath_sound_iop_heap_send_view, 4,
                      norrath_sound_iop_heap_receive_view, 4, 0, 0) < 0)
        return -1;
    return norrath_sound_iop_heap_receive_view[0];
}
}
