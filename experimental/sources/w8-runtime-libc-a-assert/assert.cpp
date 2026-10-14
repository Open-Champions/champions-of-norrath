// Observed __assert__FPCcUiT0i at 0x00172890, 168 bytes. The four arguments
// are file, line, condition text and a caller word (a fourth integer the
// caller supplies; launchSRS passes it as the last argument). Three times in
// a row it calls scePrintf at 0x002d70d0 with the format at 0x004635e8, then
// condition text, file, line, the word at gp-14592 (0x004840f0, re-read for
// every repetition) and the caller word. After the third call it stores zero
// to address 0, which faults on the target, and then returns if the store
// does not fault.
// Inferred: the third format conversion takes the 0x004840f0 word as a string
// pointer because the format uses %s there; the fourth is printed with %x.
// Unknown: what the fault handler does with the store.
extern "C" {
extern const char norrath_assert_format_view[];
extern const char* norrath_assert_context_word_view;
int scePrintf(const char* format, ...);
}

#if !defined(__mips__)
extern "C" void norrath_assert_host_fault_probe();
#endif

extern "C" void __assert__FPCcUiT0i(const char* file, unsigned line, const char* text, int caller) {
    const char* const* const context = &norrath_assert_context_word_view;
    for (int repetition = 0; repetition < 3; ++repetition)
        scePrintf(norrath_assert_format_view, text, file, line, *static_cast<const char* const volatile*>(context), caller);
#if defined(__mips__)
    asm volatile("sw $zero,0(%0)" :: "r"(0) : "memory");
#else
    norrath_assert_host_fault_probe();
#endif
}
