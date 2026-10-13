// Observed CreateReaderThread at 0x002e8e98, 260 bytes, from the keyboard library the image links
// in (it is called by lgKbMInit with the reader-thread priority). Steps:
//   1. Remember the calling thread: GetThreadId().
//   2. Fill the thread record at 0x008e8c40: entry word +4 = KbmReaderThread (0x002e89a8, a local
//      function), stack word +8 = the area at 0x008e8f50, size word +12 = 6144, gp word +16 =
//      the global pointer value (_gp, 0x004879f0), priority word +20 = the argument.
//   3. CreateSema on a six-word block of which only word 1 (=1) and word 2 (=0) are written; the
//      result goes to word 40 of GLibData (0x008ebdd8), stored even when it is -1. A result of -1
//      prints "lgkbmlib::CreateSema Error creating rpc_sema" once and then stops in an endless
//      loop.
//   4. CreateThread on the record; the result goes to word 48 of GLibData; when it is above zero,
//      StartThread(id, 0).
//   5. ChangeThreadPriority(calling thread, 30), then ChangeThreadPriority(calling thread, the
//      value the first call returned), putting the calling thread's priority back.
// GLibData is read here as an array of words (the offsets above are bytes). The thread record and stack area are file-local storage in the library; their sizes and owners
// are unknown, so they are taken as imports with view names bound to those addresses.
// Unknown: the record's other words; what the entry function does beyond its name. Nothing was
// executed on the EE.
extern "C" {
extern int GLibData[];
extern int norrath_lgkbm_reader_thread_record_view[];
extern unsigned char norrath_lgkbm_reader_thread_stack_view[];
extern unsigned char _gp[];
void KbmReaderThread();
int printf(const char* format, ...);
int GetThreadId();
int CreateSema(void* parameters);
int CreateThread(void* record);
void StartThread(int id, int argument);
int ChangeThreadPriority(int id, int priority);

static inline int address_word(const void* pointer) {
    return static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)));
}

void CreateReaderThread(int priority) {
    const int caller = GetThreadId();
    int* const record = norrath_lgkbm_reader_thread_record_view;
    record[1] = address_word(reinterpret_cast<void*>(KbmReaderThread));
    record[2] = address_word(norrath_lgkbm_reader_thread_stack_view);
    record[3] = 6144;
    record[4] = address_word(_gp);
    record[5] = priority;
    int parameters[6];
    parameters[1] = 1;
    parameters[2] = 0;
    const int semaphore = CreateSema(parameters);
    GLibData[10] = semaphore;
    if (semaphore == -1) {
        printf("lgkbmlib::CreateSema Error creating rpc_sema\n");
        for (;;) {
        }
    }
    const int thread = CreateThread(record);
    GLibData[12] = thread;
    if (thread > 0) StartThread(thread, 0);
    const int previous = ChangeThreadPriority(caller, 30);
    ChangeThreadPriority(caller, previous);
}
}
