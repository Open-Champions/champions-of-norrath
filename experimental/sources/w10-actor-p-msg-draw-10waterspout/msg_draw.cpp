// Observed msg_draw__10WaterSpout at 0x001e57e8, 64 bytes. Clears the byte at object+89, then
// reads the signed byte at +88. When that byte is zero it calls objectAddToRunList__FP10GameObject
// on the object and sets the byte to 1; otherwise it does nothing more. Nothing is drawn here.
// Unknown: what the two bytes mean. Nothing here ran on the EE.
extern "C" void objectAddToRunList__FP10GameObject(void* object);

extern "C" void msg_draw__10WaterSpout(void* self) {
    signed char* const flags = static_cast<signed char*>(self);
    flags[89] = 0;
    if (flags[88] == 0) {
        objectAddToRunList__FP10GameObject(self);
        flags[88] = 1;
    }
}
