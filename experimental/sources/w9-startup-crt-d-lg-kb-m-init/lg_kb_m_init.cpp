// Observed lgKbMInit at 0x002e6ed0, 412 bytes, from the keyboard library the
// image links in. Arguments: two small limits, a reader-thread priority, and an
// optional timer function. The library state is the object GLibData
// (0x008ebdd8); the word at offset 52 is the "already initialised" flag.
//   1. Flag set: print "lgKbMInit:: Error!!! Init already done. Returning without
//      any action", return 0x80000000.
//   2. Print the banner "LgKbM library version %d.%02d.%03d (%s)(built on Sep  3
//      2003 at 18:00:50)\n\t* reader thread priority %d\n" with 1, 6, 1,
//      the text "1.06.001" and the priority.
//   3. Offset 64 = first limit and offset 68 = second limit, each replaced by 4
//      when it is 5 or more (a negative value is kept). Offset 96 = 1; the
//      64-bit word at 112 = 100; offset 120 = 500.
//   4. The table at KbdMapTable (0x00447c60) is sorted with
//      qsort_translate_table(table, 0, 74); offset 132 = 75 and offset 124 = the
//      table address. (The original tests the address for nonzero first; a
//      linked object's address is never null, so the test is not reproduced.)
//   5. Offsets 56 and 60 = 0. CreateSema runs on a six-word block of which only
//      words 1 and 2 (both 1) are written; the rest is whatever the stack holds.
//      The result is stored at offset 44. A result of -1 prints
//      "lgkbmlib::CreateSema Error creating device_sema" and stops in an
//      endless loop.
//   6. connect_to_iop_server(). A non-null fourth argument is stored at offset
//      128; otherwise init_ee_timer() runs and offset 128 gets the address of
//      get_time (0x002e7810). CreateReaderThread(priority) runs; offset 52 = 1;
//      return 0.
// Unknown: what the two limits, the library tables and the callees do beyond
// their names.
extern "C" {
extern unsigned char GLibData[];
extern unsigned char KbdMapTable[];
int printf(const char* format, ...);
int CreateSema(void* parameters);
void qsort_translate_table(void* table, int low, int high);
void connect_to_iop_server();
void init_ee_timer();
int get_time();
void CreateReaderThread(int priority);

static inline int* word(unsigned offset) { return reinterpret_cast<int*>(GLibData + offset); }
static inline int address_word(const void* pointer) {
    return static_cast<int>(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)));
}

int lgKbMInit(int first_limit, int second_limit, int priority, int (*timer)()) {
    if (*word(52) != 0) {
        printf("lgKbMInit:: Error!!! Init already done. Returning without any action\n");
        return static_cast<int>(0x80000000u);
    }
    printf("LgKbM library version %d.%02d.%03d (%s)(built on Sep  3 2003 at 18:00:50)\n"
           "\t* reader thread priority %d\n",
           1, 6, 1, "1.06.001", priority);
    *word(64) = first_limit < 5 ? first_limit : 4;
    *word(68) = second_limit < 5 ? second_limit : 4;
    *reinterpret_cast<long long*>(GLibData + 112) = 100;
    *word(120) = 500;
    *word(96) = 1;
    *word(132) = 75;
    qsort_translate_table(KbdMapTable, 0, 74);
    *word(124) = address_word(KbdMapTable);
    int parameters[6];
    parameters[1] = 1;
    *word(56) = 0;
    *word(60) = 0;
    parameters[2] = 1;
    const int semaphore = CreateSema(parameters);
    *word(44) = semaphore;
    if (semaphore == -1) {
        printf("lgkbmlib::CreateSema Error creating device_sema\n");
        for (;;) {
        }
    }
    connect_to_iop_server();
    if (timer != 0) {
        *word(128) = address_word(reinterpret_cast<const void*>(timer));
    } else {
        init_ee_timer();
        *word(128) = address_word(reinterpret_cast<const void*>(get_time));
    }
    CreateReaderThread(priority);
    *word(52) = 1;
    return 0;
}
}
