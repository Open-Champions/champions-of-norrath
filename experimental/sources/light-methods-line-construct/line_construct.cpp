// Observed __4LineG6Point3sPPc at 0x278f08, 1016 bytes.
// The authored name is norrath_construct_Line. It copies the 12-byte point,
// sign-extends the halfword type, and clears words 0, 4, 12, and 20. Word 8
// starts as -1. Arguments are walked until a null pointer.
// "name=" stores the address after '=' and, when that text compares equal to
// "barrier" without case, sets bit 0 of word 12. "continous=%d" uses the
// observed spelling and stores the integer at word 8. "norotate=%d" stores a
// local integer. "pcount=%d" stores a count at word 4. A negative count is
// negated with 32-bit wrap and the following arguments are read in reverse.
// Each point is 64 bytes. The first 12 bytes come from "p=%f,%f,%f" and the
// word at offset 60 is the slot index. memAlloc receives count<<6 and the
// label "point list".
// After the arguments, word 112 of the object addressed by world is the
// megatile mode. A nonzero mode and a zero norotate value subtract
// getMegatileOriginalCenter from each point, then add getMegatileCenter to
// the x and y produced by matRot axis 2 and matTransform. The transformed z
// is replaced by the point's own z. initializeMe always runs.

extern "C" {
char* norrath_factory_strstr(const char* text, const char* needle);
char* norrath_factory_strchr(const char* text, int character);
int norrath_factory_strcasecmp(const char* left, const char* right);
int sscanf(const char* text, const char* format, ...);
void* memAlloc__FiPc(int bytes, char* label);
void getMegatileOriginalCenter__FG6Point2(void* center, const void* point);
void getMegatileCenter__FG6Point2(void* center, const void* point);
void matIdent__FP8Matrix33(void* matrix);
void matRot__FP8Matrix33is(void* matrix, int axis, short angle);
void matTransform__FP6Point3P8Matrix33T0(void* destination, void* matrix, const void* source);
void initializeMe__4Line(void* line);
extern unsigned char norrath_route_world_word_bytes[4];
#if defined(NORRATH_LINE_HOST_CHECK)
extern unsigned long long norrath_line_host_base;
#endif
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
int read_int(const unsigned char* p) {
    unsigned value = read_word(p);
    return value < 0x80000000u ? static_cast<int>(value) : static_cast<int>(value - 0x80000000u) - 2147483647 - 1;
}
unsigned low_pointer(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
unsigned char* from_word(unsigned value) {
#if defined(NORRATH_LINE_HOST_CHECK)
    return reinterpret_cast<unsigned char*>(norrath_line_host_base + value);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
void write_float(unsigned char* p, float value) {
    union {
        float number;
        unsigned bits;
    } converted;
    converted.number = value;
    write_word(p, converted.bits);
}
float read_float(const unsigned char* p) {
    union {
        unsigned bits;
        float number;
    } converted;
    converted.bits = read_word(p);
    return converted.number;
}
char* after_equals(char* text) {
    char* found = norrath_factory_strchr(text, static_cast<int>('='));
    return reinterpret_cast<char*>(reinterpret_cast<__UINTPTR_TYPE__>(found) + 1u);
}
__attribute__((always_inline)) inline void store_point(unsigned char* slot, float x, float y, float z, int index) {
    write_float(slot, x);
    write_float(slot + 4u, y);
    write_float(slot + 8u, z);
    write_word(slot + 60u, static_cast<unsigned>(index));
}
__attribute__((always_inline)) inline void parse_point(const char* text, unsigned char* slot, int index) {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    sscanf(text, "p=%f,%f,%f", &x, &y, &z);
    store_point(slot, x, y, z, index);
}
}

extern "C" void norrath_construct_Line(unsigned char* line, const void* point, short type, char** arguments) {
    unsigned char point_copy[12];
    copy_bytes(point_copy, static_cast<const unsigned char*>(point), 12u);
    int norotate = 0;
    write_word(line, 0u);
    write_word(line + 8u, 0xffffffffu);
    write_word(line + 12u, 0u);
    write_word(line + 4u, 0u);
    write_word(line + 20u, 0u);
    int index = 0;
    if (arguments[0] != nullptr) {
        while (arguments[index] != nullptr) {
            char* text = arguments[index];
            if (norrath_factory_strstr(text, "name=") != nullptr) {
                char* value = after_equals(text);
                write_word(line, low_pointer(value));
                if (norrath_factory_strcasecmp(value, "barrier") == 0) {
                    write_word(line + 12u, read_word(line + 12u) | 1u);
                }
                index += 1;
                continue;
            }
            if (norrath_factory_strstr(text, "continous") != nullptr) {
                int continuous = 0;
                sscanf(text, "continous=%d", &continuous);
                write_word(line + 8u, static_cast<unsigned>(continuous));
                index += 1;
                continue;
            }
            if (norrath_factory_strstr(text, "norotate") != nullptr) {
                sscanf(text, "norotate=%d", &norotate);
                index += 1;
                continue;
            }
            int count = 0;
            if (sscanf(text, "pcount=%d", &count) == 0) {
                index += 1;
                continue;
            }
            bool reversed = false;
            if (count < 0) {
                reversed = true;
                count = static_cast<int>(-static_cast<unsigned>(count));
            }
            write_word(line + 4u, static_cast<unsigned>(count));
            index += 1;
            const int bytes = static_cast<int>(static_cast<unsigned>(count) << 6);
            void* block = memAlloc__FiPc(bytes, const_cast<char*>("point list"));
            write_word(line + 20u, low_pointer(block));
            unsigned char* slots = from_word(read_word(line + 20u));
            if (reversed) {
                if (count > 0) {
                    for (int slot = 0; slot < count; slot += 1) {
                        const int source = index + count - slot - 1;
                        parse_point(arguments[source], slots + static_cast<unsigned>(slot) * 64u, slot);
                    }
                }
                index += count;
            } else if (count > 0) {
                for (int slot = 0; slot < count; slot += 1) {
                    parse_point(arguments[index], slots + static_cast<unsigned>(slot) * 64u, slot);
                    index += 1;
                }
            }
        }
    }
    unsigned char* world = from_word(read_word(norrath_route_world_word_bytes));
    const int mode = read_int(world + 112u);
    const int count = read_int(line + 4u);
    if (mode != 0 && norotate == 0) {
        unsigned char query[8];
        unsigned char original_center[8];
        unsigned char current_center[8];
        unsigned char matrix[64];
        copy_bytes(query, point_copy, 8u);
        getMegatileOriginalCenter__FG6Point2(original_center, query);
        if (count > 0) {
            unsigned char* slots = from_word(read_word(line + 20u));
            const float center_x = read_float(original_center);
            const float center_y = read_float(original_center + 4u);
            for (int slot = 0; slot < count; slot += 1) {
                unsigned char* record = slots + static_cast<unsigned>(slot) * 64u;
                write_float(record, read_float(record) - center_x);
                write_float(record + 4u, read_float(record + 4u) - center_y);
            }
        }
        getMegatileCenter__FG6Point2(current_center, query);
        matIdent__FP8Matrix33(matrix);
        matRot__FP8Matrix33is(matrix, 2, type);
        if (count > 0) {
            unsigned char* slots = from_word(read_word(line + 20u));
            const float center_x = read_float(current_center);
            const float center_y = read_float(current_center + 4u);
            for (int slot = 0; slot < count; slot += 1) {
                unsigned char* record = slots + static_cast<unsigned>(slot) * 64u;
                unsigned char transformed[12];
                const float kept_z = read_float(record + 8u);
                matTransform__FP6Point3P8Matrix33T0(transformed, matrix, record);
                write_float(transformed, read_float(transformed) + center_x);
                write_float(transformed + 4u, read_float(transformed + 4u) + center_y);
                write_float(transformed + 8u, kept_z);
                copy_bytes(record, transformed, 12u);
            }
        }
    }
    initializeMe__4Line(line);
}
