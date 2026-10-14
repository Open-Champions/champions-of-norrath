// Observed msg_draw__10Teleporter at 0x0021ce78, 8 bytes.
// The body is a bare return: it reads no argument, touches no memory and calls nothing.
// Its address is the draw slot (index 5) of the Teleporter virtual table.

extern "C" void msg_draw__10Teleporter(void* self) {
    (void)self;
}
