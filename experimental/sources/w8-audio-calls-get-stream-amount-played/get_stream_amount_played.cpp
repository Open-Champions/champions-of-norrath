/* soundGetStreamAmountPlayed__Fi at 0x0024f3b0 is 148 bytes. If the signed
 * byte at offset 0 of stream record `index` (452 bytes each from `stream`) is
 * zero, the result is 0. Otherwise it calls sceSifCallRpc, already authored
 * as norrath_sound_call_rpc, on the sound client (gCd): command 80, mode 0,
 * send buffer sbuffin with length 0, receive buffer sbuff with length 64,
 * null end function and parameter. The result is the word at offset
 * 16 * index + 12 of the receive buffer; the call's own return value is
 * ignored.
 */
extern "C" {
extern unsigned char stream[];
extern unsigned char norrath_sound_rpc_client_view[];
extern unsigned char norrath_sound_rpc_send_view[];
extern unsigned char norrath_sound_rpc_receive_view[];
int norrath_sound_call_rpc(void* client, int command, int mode, void* send, int send_bytes,
                           void* receive, int receive_bytes, void* end_function,
                           void* end_parameter);

int soundGetStreamAmountPlayed__Fi(int index) {
    if (stream[index * 452] == 0) return 0;
    norrath_sound_call_rpc(norrath_sound_rpc_client_view, 80, 0, norrath_sound_rpc_send_view, 0,
                           norrath_sound_rpc_receive_view, 64, nullptr, nullptr);
    return *reinterpret_cast<int*>(norrath_sound_rpc_receive_view + index * 16 + 12);
}
}
