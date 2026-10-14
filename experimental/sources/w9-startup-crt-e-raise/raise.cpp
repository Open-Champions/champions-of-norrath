// Observed raise at 0x00303bf8, 32 bytes. It loads the pointer held in _impure_ptr
// (0x00448480) and tail-calls _raise_r(that pointer, signal) at 0x00303a88, returning
// whatever _raise_r returns. The reentrancy structure is not read here.
// Unknown: the body of _raise_r.
extern "C" {
extern void* _impure_ptr;
int _raise_r(void* reent, int signal_number);

int raise(int signal_number) {
    return _raise_r(_impure_ptr, signal_number);
}
}
