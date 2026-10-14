// Game heap words and the text memAlloc, memFree, and memDumpHeap print.
// GP 0x004879f0 resolves the mutable words as follows:
//   startNode  0x004868b4  ring cursor, .sbss, initially zero
//   movingNode 0x004868b0  next-fit word, .sbss, initially zero
//   rawMin     0x004868ac  fragment threshold, .sbss, initially zero
//   MEM_FreeHeap     0x00483b28  4 file bytes, zero
//   mark             0x00483b08  4 file bytes, zero
//   MEM_HeapCapacity 0x00483b2c  4 file bytes, zero
//   MEM_DumpCount    0x00483b30  4 file bytes, zero
// The check name is the unsymbolized text "mem.cc" at 0x00483b18.
// Format text is unsymbolized rodata at 0x00461eb8 through 0x00461f90.
extern "C" {
unsigned char norrath_heap_ring_word_view[4];
unsigned char norrath_heap_next_fit_view[4];
unsigned char norrath_heap_fragment_threshold_view[4];
unsigned char norrath_heap_free_bytes_view[4] = {};
unsigned char norrath_heap_allocation_tag_view[4] = {};
unsigned char norrath_heap_total_bytes_view[4] = {};
unsigned char norrath_heap_dump_count_view[4] = {};
extern const unsigned char norrath_heap_check_file_view[] = {'m', 'e', 'm', '.', 'c', 'c', 0};
extern const unsigned char norrath_heap_failure_format_view[] = {
    'm','e','m','A','l','l','o','c',' ','f','a','i','l','e','d','!',' ',' ',
    '(','r','a','=','%','x',',',' ','s','i','z','e','=','%','d',')','\n', 0
};
extern const unsigned char norrath_heap_dump_border_view[] = {
    '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
    '-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-',
    '-','-','-','\n', 0
};
extern const unsigned char norrath_heap_dump_count_format_view[] = {
    ' ',' ',' ',' ',' ','S','t','a','r','t',' ','H','e','a','p',' ','D','u','m','p',
    ' ','(','%','d',')','\n', 0
};
extern const unsigned char norrath_heap_dump_free_format_view[] = {
    'F','r','e','e',' ','a','t',' ','%','x',',',' ','%','d',' ','b','y','t','e','s','.','\n', 0
};
extern const unsigned char norrath_heap_dump_used_format_view[] = {
    'U','s','e','d',' ','a','t',' ','%','x',' ','\'','%','s','\'',' ','(','r','a','=',
    '%','x',')',',',' ','%','d',' ','b','y','t','e','s','.','\n', 0
};
extern const unsigned char norrath_heap_dump_totals_format_view[] = {
    'T','o','t','a','l','=','%','d',',',' ','U','s','e','d','=','%','d',',',' ',
    'F','r','e','e','=','%','d','\n', 0
};
extern const unsigned char norrath_heap_dump_serial_format_view[] = {
    ' ',' ',' ',' ',' ',' ','E','n','d',' ','H','e','a','p',' ','D','u','m','p',
    ' ','(','%','d',')',' ','\n', 0
};
}
