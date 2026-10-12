// Observed __builtin_new at 0x002fec60, 280 bytes (weak). A zero size becomes
// one. It calls __wrap_malloc (0x001449a8) and returns a non-null result at
// once. On failure it loops: read __new_handler (0x0044818c), and when it is null
// raise bad_alloc; when it is not null call it as a function with no
// arguments, then call __wrap_malloc again with the same size and return a
// non-null result. The handler word is re-read on every pass.
// Raising bad_alloc: __eh_alloc(4) gives a 4-byte record; its first word is set
// to the address of _vt$9bad_alloc (0x00453950); __tf9bad_alloc (0x0031b490)
// supplies the type_info; __cp_push_exception(record, type_info, 0x0031b458
// = _$_9bad_alloc) registers it; __throw() does not return.
// Not authored: the original also carries cleanup and handler regions after
// the throw (calls to __rethrow, __cp_pop_exception, __start_cp_handler and
// __check_eh_spec with 0x00453930 = _vt$9exception). They need the exception
// tables, which the -fno-exceptions profile cannot produce, so only the normal
// and throwing paths are reproduced. Inferred: the word is the installed new
// handler, following the symbol name __new_handler. Unknown: its initial contents.
extern "C" {
void* __wrap_malloc(unsigned size);
void* __eh_alloc(unsigned size);
void* __tf9bad_alloc();
void __cp_push_exception(void* object, void* type, void (*destructor)());
void _$_9bad_alloc();
void __throw() __attribute__((noreturn));
extern unsigned char _vt$9bad_alloc[];
extern void (*__new_handler)();

void* __builtin_new(unsigned size) {
    if (size == 0) size = 1;
    void* result = __wrap_malloc(size);
    while (result == nullptr) {
        if (__new_handler == nullptr) {
            void** record = static_cast<void**>(__eh_alloc(4));
            void* type = __tf9bad_alloc();
            record[0] = _vt$9bad_alloc;
            __cp_push_exception(record, type, _$_9bad_alloc);
            __throw();
        }
        __new_handler();
        result = __wrap_malloc(size);
    }
    return result;
}
}
