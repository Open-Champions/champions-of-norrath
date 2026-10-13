// Observed: ReadBegin__9VIObjFileRUsT1 at 0x010a6da8, 32 bytes. With (this, a, b) it calls
// ReadBegin__9VIObjFileRUsT1RUlRUi(this, a, b, &first, &second) where first and second are two uninitialised words of its own
// stack frame (an unsigned long and an unsigned int, eight bytes apart), and returns that call's result unchanged.
// Inferred: the three-argument form of the stream-begin routine; both extra outputs are discarded. a and b are references to
// unsigned short as the mangled name says.
// Unknown: what the callee stores through the four pointers.
extern "C" {
int ReadBegin__9VIObjFileRUsT1RUlRUi(void* object, unsigned short* a, unsigned short* b, unsigned long* c, unsigned* d);

int ReadBegin__9VIObjFileRUsT1(void* object, unsigned short* a, unsigned short* b) {
    struct Outputs {
        unsigned long first;
        unsigned unused;   // the image keeps the two outputs eight bytes apart; this word only reproduces that spacing
        unsigned second;
    } outputs;
    return ReadBegin__9VIObjFileRUsT1RUlRUi(object, a, b, &outputs.first, &outputs.second);
}
}
