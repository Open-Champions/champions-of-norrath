// Observed: Load__13VIStringTablePCc at 0x0101bd78 is 192 bytes. It builds a local
// VIArray<char> (__t7VIArray1Zc on a stack object), calls the member
// LoadRawFile__13VIStringTablePCcRt7VIArray1Zc with only the path and the array
// (it is called with no object pointer, so it is a static member), and the result is
// -1 unless all of these succeed in order: LoadRawFile >= 0, then
// ProcessRawFile__13VIStringTableRCt7VIArray1Zc(this, array) >= 0, then
// FixupTextBuffer__13VIStringTable(this) >= 0; success gives 0. On a failure it
// releases the table exactly as Unload__13VIStringTable does, written out inline
// in the original: __builtin_vec_delete on word 8 if nonzero, on word 12 if nonzero,
// then words 12, 0, 4 and 8 are zero. In all cases it then runs the array's
// destructor _$_t7VIArray1Zc(array, 2) and returns the result.
// Unknown: the size of the local array object (16 bytes are reserved on the
// stack; the object is passed by address only).
extern "C" {
void __t7VIArray1Zc(void* array);
int LoadRawFile__13VIStringTablePCcRt7VIArray1Zc(const char* path, void* array);
int ProcessRawFile__13VIStringTableRCt7VIArray1Zc(void* table, const void* array);
int FixupTextBuffer__13VIStringTable(void* table);
void __builtin_vec_delete(void* pointer);
void _$_t7VIArray1Zc(void* array, int flags);

int Load__13VIStringTablePCc(void* table, const char* path) {
    alignas(16) char array[16];
    void** words = static_cast<void**>(table);
    __t7VIArray1Zc(array);
    int result = -1;
    if (LoadRawFile__13VIStringTablePCcRt7VIArray1Zc(path, array) >= 0 &&
        ProcessRawFile__13VIStringTableRCt7VIArray1Zc(table, array) >= 0 &&
        FixupTextBuffer__13VIStringTable(table) >= 0)
        result = 0;
    if (result != 0) {
        if (words[2] != nullptr) __builtin_vec_delete(words[2]);
        if (words[3] != nullptr) __builtin_vec_delete(words[3]);
        words[3] = nullptr;
        words[0] = nullptr;
        words[1] = nullptr;
        words[2] = nullptr;
    }
    _$_t7VIArray1Zc(array, 2);
    return result;
}
}
