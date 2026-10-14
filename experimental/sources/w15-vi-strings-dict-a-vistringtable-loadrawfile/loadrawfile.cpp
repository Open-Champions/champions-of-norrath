// Observed: LoadRawFile__13VIStringTablePCcRt7VIArray1Zc at 0x0101b7c8 is 340 bytes. It takes no
// object: a0 is the file name and a1 a reference to a byte array. It constructs a VIFile in a
// 192-byte stack area, stores 245 and the source-file name "../game/vistringtable.cpp" to
// __VIAssertf_line and __VIAssertf_file, calls Open(name, 33) and passes (result == 0) to
// _VIAssertf with the format "file open failed: %s" and the name. Then Seek(0, 2); on a non-zero
// result the file is torn down (destructor with flag 2) and that result returned. Otherwise
// Tell() is called; if the 64-bit value exceeds 0xFFFFFFFF, _VIAssert("File.Tell() <=
// 0xFFFFFFFF", file, 253) is called. Tell() is called again and truncated to 32 bits as the size.
// array.Init(size) is called; a non-zero result tears the file down and is returned. With size 0
// the file is torn down and 0 returned. Otherwise Seek(0, 1); a non-zero result tears down and is
// returned; else Read(array data pointer from offset 4, size) runs, the file is torn down and the
// read's result is returned (0 on success).
// Inferred: reads a whole file into a freshly sized byte array; the second Seek returns to the
// start; the assertion macro stores line and file before evaluating its condition.
// Unknown: the numeric meaning of the Open mode 33 and of the Seek origin values 1 and 2 (the
// enum constants are not named here); the VIFile size beyond the 192 bytes the frame reserves.
extern "C" {
extern const char* __VIAssertf_file;
extern int __VIAssertf_line;
void __6VIFile(void* file);
void _$_6VIFile(void* file, int flags);
int Open__6VIFilePCci(void* file, const char* name, int mode);
int Seek__6VIFilelQ26VIFile10SeekOrigin(void* file, long offset, int origin);
long long Tell__6VIFile(void* file);
int Read__6VIFilePvi(void* file, void* destination, int size);
int Init__t7VIArray1Zci(void* array, int size);
void _VIAssertf(int condition, const char* format, ...);
void _VIAssert(const char* expression, const char* file, int line);

int LoadRawFile__13VIStringTablePCcRt7VIArray1Zc(const char* name, unsigned char* array) {
    alignas(8) unsigned char file[192];
    __6VIFile(file);
    __VIAssertf_line = 245;
    __VIAssertf_file = "../game/vistringtable.cpp";
    _VIAssertf(Open__6VIFilePCci(file, name, 33) == 0, "file open failed: %s", name);
    int result = Seek__6VIFilelQ26VIFile10SeekOrigin(file, 0, 2);
    if (result != 0) {
        _$_6VIFile(file, 2);
        return result;
    }
    if (Tell__6VIFile(file) > 0xffffffffLL) {
        _VIAssert("File.Tell() <= 0xFFFFFFFF", "../game/vistringtable.cpp", 253);
    }
    int size = static_cast<int>(Tell__6VIFile(file));
    result = Init__t7VIArray1Zci(array, size);
    if (result != 0) {
        _$_6VIFile(file, 2);
        return result;
    }
    if (size == 0) {
        _$_6VIFile(file, 2);
        return 0;
    }
    result = Seek__6VIFilelQ26VIFile10SeekOrigin(file, 0, 1);
    if (result != 0) {
        _$_6VIFile(file, 2);
        return result;
    }
    result = Read__6VIFilePvi(file, *reinterpret_cast<void**>(array + 4), size);
    _$_6VIFile(file, 2);
    return result;
}
}
