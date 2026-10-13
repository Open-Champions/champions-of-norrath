// Observed what__C9bad_alloc at 0x0031b4e0, 12 bytes (weak). It returns the address of the
// text "bad_alloc" (0x0047e1e0) and ignores its object argument.
// Nothing here was executed on the EE.
extern "C" const char* what__C9bad_alloc(const void* /*object*/) { return "bad_alloc"; }
