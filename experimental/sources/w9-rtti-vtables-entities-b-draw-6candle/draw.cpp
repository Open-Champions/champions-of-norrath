// Observed msg_draw__6Candle at 0x231068, 60 bytes. Clears the byte at
// object+141. Then, if the signed byte at object+140 is zero, it adds the object
// to the run list and sets that byte to 1; otherwise it does nothing more.
// Unknown: what the two bytes mean.

extern "C" void objectAddToRunList__FP10GameObject(void* object);

extern "C" void msg_draw__6Candle(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    object[141] = 0;
    if (static_cast<signed char>(object[140]) == 0) {
        objectAddToRunList__FP10GameObject(self);
        object[140] = 1;
    }
}
