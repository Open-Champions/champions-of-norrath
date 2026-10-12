/* Observed sceMtapGetModVersion at 0x002d4c40, 100 bytes. Calls
 * sceSifCallRpc on the module_version client (0x008ddbe0) with command 1,
 * mode 0, the shared 128-byte buffer (0x008ddc40) as both send and receive
 * area, no completion function and no argument. The buffer is not written
 * first. A negative result prints "sceMtapGetModVersion: rpc error\n" through
 * DPRINT and returns 0. Otherwise the first buffer word is returned.
 */
extern "C" {
extern unsigned char norrath_libmtap_module_version_client_view[];
extern int norrath_libmtap_rpc_buffer_view[];

int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* done, void* done_arg);
void DPRINT(const char* format, ...);

int sceMtapGetModVersion() {
    if (sceSifCallRpc(norrath_libmtap_module_version_client_view, 1, 0, norrath_libmtap_rpc_buffer_view, 128,
                      norrath_libmtap_rpc_buffer_view, 128, 0, 0) < 0) {
        DPRINT("sceMtapGetModVersion: rpc error\n");
        return 0;
    }
    return norrath_libmtap_rpc_buffer_view[0];
}
}
