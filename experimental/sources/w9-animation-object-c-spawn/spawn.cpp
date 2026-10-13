#include "../w9-animation-object-c-common/mem.hpp"
// Observed spawn__FG6Point3iisPcbPPci at 0x0026a738, 840 bytes. Arguments in order: a 12-byte
// point (copied), an int `radius`, an int `line`, a short `type`, the option text `name`, a flag
// `use_filter`, a pointer list `arguments` and an int `clear_radius` (-1 means "look it up").
// The name used for the other tools' callers follows level-anim-load-monster-spawn. Returns
// nothing observable (the result register is whatever the last call left).
//  1. Reads a class name into a local 256-byte buffer `cls`:
//     - when name[0] is not '(' it is sscanf(name, "%[^,-]", cls) and `arguments` is used as is;
//     - otherwise sscanf(name + 1, "%[^,;-]", cls), then scans forward from name + 1 to the
//       first ';' or ')'. At ';' every following piece up to ')' is a parameter: each is
//       copied (strncpy plus a terminating zero) into a 256-byte token buffer, passed through
//       addpmlist__FPc, and stored at pmlistptrs[pmlistptrsused++]. A piece ends at the first
//       ';', or at a ')' that is not matched by a '(' inside the piece; a piece is not tested for
//       the string's terminator. Then `arguments` becomes addpmlistptrscap__Fv().
//  2. When clear_radius == -1 it becomes getClearradius__FPCc(cls) and a flag `looked_up` is set.
//  3. objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(&spot, point, radius, line, type,
//     clear_radius, &found); nothing more happens when found is zero.
//  4. When use_filter is set: userFilterFunction (0x483b00) must be nonzero and
//     userFilterFunction(cls) must return nonzero, else it stops.
//  5. eRandState (0x483e58) advances by x * 0x19660d + 0x3c6ef35f, and
//     objectCreateByName__FPcG6Point3sPPc(strstr(name, cls), spot, (short)(state >> 16),
//     arguments) is called with the arithmetic shift of the new 32-bit state. A null object stops.
//  6. When the float at object+28 equals (float)clear_radius, or looked_up is not set, it stops.
//     Otherwise it calls strcasecmp(cls, s) for "scorpion", "firebeetle", "bbeetle", "ant" and
//     "lbeetle" in that order, stopping at the first zero result, and does nothing with the
//     results.
// Inferred: this places one monster of the named class near a point; the strcasecmp chain is
// what is left of a check whose reaction was compiled out.
// Unknown: the meaning of `line`, `radius` and `type` to the spot search, and why those five
// class names are compared.
extern "C" {
extern int eRandState;
extern int pmlistptrsused;
extern char* pmlistptrs[];
extern int (*userFilterFunction)(char*);
int sscanf(const char* text, const char* format, ...);
char* strncpy(char* destination, const char* source, unsigned long count);
char* strstr(const char* haystack, const char* needle);
int strcasecmp(const char* left, const char* right);
char* addpmlist__FPc(char* text);
char** addpmlistptrscap__Fv();
int getClearradius__FPCc(const char* text);
void objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(float* result, const float* point, int radius, int line, int type,
                                                      int clear_radius, char* found);
unsigned char* objectCreateByName__FPcG6Point3sPPc(char* text, const float* point, short angle, char** arguments);
}

extern "C" void spawn__FG6Point3iisPcbPPci(const unsigned char* point_in, int radius, int line, short type, char* name,
                                          bool use_filter, char** arguments, int clear_radius) {
    using namespace norrath_aoc;
    float point[3];
    point[0] = load_float(point_in);
    point[1] = load_float(point_in + 4);
    point[2] = load_float(point_in + 8);
    char cls[256];
    char token[256];
    if (name[0] != '(') {
        sscanf(name, "%[^,-]", cls);
    } else {
        char* p = name + 1;
        sscanf(p, "%[^,;-]", cls);
        bool parameters = true;
        if (*p != ';') {
            if (*p == ')') {
                parameters = false;
            } else {
                do { ++p; } while (*p != ';' && *p != ')');
                if (*p == ')') parameters = false;
            }
        }
        if (parameters) {
            ++p;
            for (;;) {
                char* end = p;
                int depth = 0;
                if (*p != ';') {
                    for (;;) {
                        const char c = *end;
                        if (c == '(') depth = depth + 1;
                        if (c == ')') depth = depth - 1;
                        ++end;
                        if (depth < 0) break;
                        if (*end == ';') break;
                    }
                }
                if (depth == -1) --end;
                const long length = end - p;
                strncpy(token, p, static_cast<unsigned long>(length));
                token[length] = 0;
                p = end;
                char* const stored = addpmlist__FPc(token);
                pmlistptrs[pmlistptrsused++] = stored;
                const char c = *p;
                ++p;
                if (c == ')') break;
            }
        }
        arguments = addpmlistptrscap__Fv();
    }
    bool looked_up = false;
    if (clear_radius == -1) {
        clear_radius = getClearradius__FPCc(cls);
        looked_up = true;
    }
    float spot[3];
    char found;
    objectFindEmptySpotMonsterspawn__FG6Point3iiiiPc(spot, point, radius, line, type, clear_radius, &found);
    if (found == 0) return;
    if (use_filter) {
        if (userFilterFunction == nullptr) return;
        if (userFilterFunction(cls) == 0) return;
    }
    char* const at = strstr(name, cls);
    const int state = static_cast<int>(static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu);
    eRandState = state;
    unsigned char* const object = objectCreateByName__FPcG6Point3sPPc(at, spot, static_cast<short>(state >> 16), arguments);
    if (object == nullptr) return;
    if (load_float(object + 28) == static_cast<float>(clear_radius)) return;
    if (!looked_up) return;
    if (strcasecmp(cls, "scorpion") == 0) return;
    if (strcasecmp(cls, "firebeetle") == 0) return;
    if (strcasecmp(cls, "bbeetle") == 0) return;
    if (strcasecmp(cls, "ant") == 0) return;
    strcasecmp(cls, "lbeetle");
}
