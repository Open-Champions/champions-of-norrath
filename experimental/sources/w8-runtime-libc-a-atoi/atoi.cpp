// Observed atoi at 0x002ff990, 28 bytes. It calls strtol at 0x00306638 with the
// text, a null end pointer and base 10, then returns the low 32 bits of the
// 64-bit result sign-extended. The strtol body is the authored
// norrath_factory_strtol. checkModelVersion (0x002d18f0) imports `atoi`.
// norrath_factory_atoi is the same routine under the older reconstruction name; this unit supplies the symbol `atoi` the callers import.
extern "C" long long norrath_factory_strtol(const char* text, char** end, int base);

extern "C" int atoi(const char* text) {
    return static_cast<int>(norrath_factory_strtol(text, nullptr, 10));
}
