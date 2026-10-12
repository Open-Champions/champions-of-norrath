// Host check only; not an EE build input. Entries are 12 bytes on the target
// (three 32-bit words); the host check only exercises indexing and which field
// is cleared, so it runs where pointers are 8 bytes and uses the layout the
// compiler gives here.
#define sceSifRemoveCmdHandler w9_remove
#include "sif_remove_cmd_handler.cpp"
#undef sceSifRemoveCmdHandler
DataTable norrath_sound_command_state_view;
extern "C" void w9_remove(int);
int main() {
    HandlerEntry neg[4], pos[4];
    int a;
    for (int i = 0; i < 4; ++i) { neg[i] = {&a, &a, 9}; pos[i] = {&a, &a, 9}; }
    norrath_sound_command_state_view.negative_ids = neg;
    norrath_sound_command_state_view.nonnegative_ids = pos;
    w9_remove(static_cast<int>(0x80000002u));
    if (neg[2].handler != 0 || neg[2].argument != &a || neg[2].gp != 9) return 1;
    for (int i = 0; i < 4; ++i) if (i != 2 && neg[i].handler == 0) return 2;
    for (int i = 0; i < 4; ++i) if (pos[i].handler == 0) return 3;
    w9_remove(3);
    if (pos[3].handler != 0 || pos[3].argument != &a) return 4;
    for (int i = 0; i < 3; ++i) if (pos[i].handler == 0) return 5;
    return 0;
}
