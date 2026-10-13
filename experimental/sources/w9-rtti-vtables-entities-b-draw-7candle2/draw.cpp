// Observed msg_draw__7Candle2 at 0x231120, 60 bytes. Clears the byte at
// object+77. Then, if the signed byte at object+76 is zero, it adds the object
// to the run list and sets that byte to 1; otherwise it does nothing more.
// Unknown: what the two bytes mean.

extern "C" void objectAddToRunList__FP10GameObject(void* object);

extern "C" void msg_draw__7Candle2(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    object[77] = 0;
    if (static_cast<signed char>(object[76]) == 0) {
        objectAddToRunList__FP10GameObject(self);
        object[76] = 1;
    }
}
