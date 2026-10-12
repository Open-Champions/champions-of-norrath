// Observed _GLOBAL_$I$wraithResourceMap at 0x00205328, 32 bytes, calling the file-local initializer at 0x002051a8
// (172 bytes) with (1, 0xffff). With those arguments the initializer makes
// the calls below; _GLOBAL_$D$wraithResourceMap at 0x00205348 calls the same initializer with (0, 0xffff).
// The file-local initializer and the objects listed here are file-local in the executable unless noted. Argument
// tuples were taken from the image; floats that are not short decimals are given as bit patterns. What the
// particle fields mean is not established here. Fresh compilation is not an original-byte match.
//
extern "C" {
extern unsigned _6Wraith$s_rWraithList[];  // 0x0042bd18, 60 words, ListHead entries; defined elsewhere (class static member)
void listDestroy__FP8ListHead(void* head);
}

static void __static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority != 0xffff) return;
    if (initialize == 1) {
        for (unsigned n = 0u; n < 60u; ++n) _6Wraith$s_rWraithList[n] = 0u;
    }
    if (initialize == 0) {
        for (unsigned n = 60u; n-- > 0u;) listDestroy__FP8ListHead(&_6Wraith$s_rWraithList[n]);
    }
}

extern "C" void _GLOBAL_$I$wraithResourceMap() {
    __static_initialization_and_destruction_0(1, 0xffff);
}

extern "C" void _GLOBAL_$D$wraithResourceMap() {
    __static_initialization_and_destruction_0(0, 0xffff);
}
