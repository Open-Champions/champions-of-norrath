// Observed inventorySwitchDualWield__FiPQ24item11description at 0x001b1aa0, 196
// bytes (int player, item pointer). With R = player + player * 23984 (no range
// check), the item is compared with two record words, in this order:
//   * R+21960 (first hand slot): when equal, R+21960 and R+21896 are cleared, an
//     8-byte message is built {[0] = 1, [1] = 0, [2..3] = low halfword of the
//     player number, [4..7] = the old word at R+21968} and sent with
//     netSendMessage__FiPUci(invMsgHandle, message, 8); then R+21900 and R+21968
//     are cleared (after the send, so a callback's changes are overwritten).
//   * R+21968 (second slot): when equal, the byte R+23582 is cleared, R+21968 is
//     cleared, resetPlayerAnimations__Fi(player) runs, R+21900 is cleared, then
//     inventorySetAnim__FiP14AnimationStatei(player, &animState[player], 0).
//     animState is an array of 264-byte records.
//   * neither: nothing happens.
// The message buffer is 416 bytes in the image; only the first 8 are written.
// Unknown: the meaning of the cleared record words and of message type 1.
extern "C" {
extern unsigned char player[];
extern unsigned char animState[];
extern int invMsgHandle;
void netSendMessage__FiPUci(int, unsigned char*, int);
void resetPlayerAnimations__Fi(int);
void inventorySetAnim__FiP14AnimationStatei(int, void*, int);

void inventorySwitchDualWield__FiPQ24item11description(int owner, void* item) {
    unsigned char* record = player + owner * 23984;
    const unsigned word = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(item));
    if (word == *reinterpret_cast<unsigned*>(record + 21960)) {
        unsigned char message[416];
        const unsigned second = *reinterpret_cast<unsigned*>(record + 21968);
        *reinterpret_cast<short*>(message + 2) = static_cast<short>(owner);
        *reinterpret_cast<unsigned*>(record + 21960) = 0;
        *reinterpret_cast<unsigned*>(record + 21896) = 0;
        message[1] = 0;
        message[0] = 1;
        *reinterpret_cast<unsigned*>(message + 4) = second;
        netSendMessage__FiPUci(invMsgHandle, message, 8);
        *reinterpret_cast<unsigned*>(record + 21900) = 0;
        *reinterpret_cast<unsigned*>(record + 21968) = 0;
    } else if (word == *reinterpret_cast<unsigned*>(record + 21968)) {
        record[23582] = 0;
        *reinterpret_cast<unsigned*>(record + 21968) = 0;
        resetPlayerAnimations__Fi(owner);
        *reinterpret_cast<unsigned*>(record + 21900) = 0;
        inventorySetAnim__FiP14AnimationStatei(owner, animState + owner * 264, 0);
    }
}
}
