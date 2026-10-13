// Observed setHold__11FireBombleti at 0x0028b830, 8 bytes: stores the low byte of the int argument at
// object+156. The byte's meaning is not established here. Nothing here ran on the EE.
extern "C" void setHold__11FireBombleti(void* self, int value) {
    static_cast<unsigned char*>(self)[156] = static_cast<unsigned char>(value);
}
