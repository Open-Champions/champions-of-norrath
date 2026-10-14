// Host check only; not an EE build input.
int norrath_sound_command_dma_handle_view = 0x77;
int norrath_sound_command_initialized_word_view = 1;
static int log_[4], n;
extern "C" {
void DisableDmac(int channel) { log_[n++] = 100 + channel; }
void RemoveDmacHandler(int channel, int identifier) { log_[n++] = channel * 1000 + identifier; }
void sceSifExitCmd();
}
int main() {
    sceSifExitCmd();
    if (n != 2 || log_[0] != 105 || log_[1] != 5000 + 0x77) return 1;
    if (norrath_sound_command_initialized_word_view != 0) return 2;
    if (norrath_sound_command_dma_handle_view != 0x77) return 3;  // not cleared
    return 0;
}
