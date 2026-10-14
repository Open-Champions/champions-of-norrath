// Observed confuse__8Creaturei at 0x00112c30, 52 bytes.
// Stores the low 16 bits of the argument at creature+796, destroys the list
// at creature+576, then stores zero at that list head.
extern "C" void norrath_list_destroy(void* head);

extern "C" void confuse__8Creaturei(void* self, int value) {
    auto* creature = static_cast<unsigned char*>(self);
    creature[796] = static_cast<unsigned char>(value);
    creature[797] = static_cast<unsigned char>(static_cast<unsigned>(value) >> 8);
    unsigned char* head = creature + 576;
    norrath_list_destroy(head);
    head[0] = 0;
    head[1] = 0;
    head[2] = 0;
    head[3] = 0;
}
