// pf: formatted-output helper of the unet startup path. Closes the unresolved import pf.
// Observed (reference image, reviewed EE nm and objdump, read locally; no text is retained here):
// - A global function pf at 0x010464f8, 0x60 bytes. It spills the variadic argument registers
//   into its frame, passes its first argument unchanged, passes the address of the spilled
//   variadic area as its second argument, calls the libc function vprintf, and returns no value.
// - The only caller in the published units: w15-startup-a-unet-startup-thread/unet_startup_thread.cpp,
//   which declares void pf(const char*) inside extern "C" and calls it with one literal message.
// Inferred:
// - The variadic form is the same call shape as the caller's one-argument use.
// Unknown:
// - The output device behind vprintf (that libc function is not authored here).
// - Whether any caller in the image passes more than the format string.
// This unit adds a new unresolved import: vprintf. It does not define vprintf.

extern "C" int vprintf(const char* format, __builtin_va_list arguments);

extern "C" void pf(const char* format, ...) {
    __builtin_va_list arguments;
    __builtin_va_start(arguments, format);
    vprintf(format, arguments);
    __builtin_va_end(arguments);
}
