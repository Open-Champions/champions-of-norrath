// Observed: CalcBVolumes__12VICollBufferiP7VIVect3 at 0x0108b390 is 1152 bytes. The integer
// argument is never read; the third is a table of 12-byte vectors. Offsets are bytes into the buffer
// object. G = word at 44 (group count), groups = 32-byte records at (pointer at 68), entries =
// 28-byte records at (pointer at 72), vertices = data at (pointer at 76), scale = float at 16,
// type = word at 8. Two scratch boxes of six floats (min xyz, max xyz) start as zeros and persist
// for the whole call: `total` and `cur`. If G <= 0 it returns 0. For each group g (0 .. G-1), with
// count = word 0 of groups[g], first_total = true, and a running entry number e and vertex
// number v that both continue from group to group:
//   * count times: take entry e; n = signed halfword at entry offset 2; if n is not zero, first_cur
//     = true and n times it makes one point from vertex v: type 0 reads three floats at
//     vertices + v*12 in place; type 1 reads three signed halfwords at vertices + v*6 and
//     multiplies each by scale; type 3 reads three signed halfwords at vertices + v*8, multiplies
//     each by scale and adds the 12-byte table vector whose index is the signed byte at vertex
//     offset 6; any other type returns -1 at once. Each point is passed to
//     Bound__6VIBBoxiRC7VIVect3(total, first_total, point) then
//     Bound__6VIBBoxiRC7VIVect3(cur, first_cur, point), after which both flags are false and v
//     advances. Whether n was zero or not, cur is then stored into the entry: min xyz at entry
//     offsets 4, 8, 12 and max xyz at 16, 20, 24; e advances by one. A zero-point entry therefore
//     receives whatever cur held from the previous entry (or zeros).
//   * total is stored into the group: min xyz at group offsets 8, 12, 16 and max xyz at 20, 24, 28
//     (so a zero-count group takes the previous group's total, or zeros);
//   * the first group copies total into the buffer's own box (min at offset 20, max at offset 32),
//     later groups call Union__6VIBBoxRC6VIBBoxT1(box, box, total).
// It returns 0. The Bound/Union return values are ignored.
// Inferred: the box layout and the meaning of the flag come from this use (nonzero = restart the
// box from the point); entry halfword 2 is a point count (Begin zeroes it, End adds it to the group total); the type values 0, 1
// and 3 are plain floats, packed halfwords and packed halfwords with a table index. Unknown:
// names, the table's purpose, and the entry's halfword at offset 0.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void Bound__6VIBBoxiRC7VIVect3(float* box, int restart, const float* point);
void Union__6VIBBoxRC6VIBBoxT1(float* destination, const float* a, const float* b);

int CalcBVolumes__12VICollBufferiP7VIVect3(unsigned char* self, int, const float* table) {
    alignas(16) float total[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float cur[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    int groups = *reinterpret_cast<int*>(self + 44);
    if (groups <= 0)
        return 0;
    float* buffer_box = reinterpret_cast<float*>(self + 20);
    bool first_group = true;
    int entry_number = 0;
    int vertex_number = 0;
    for (int g = 0; g < *reinterpret_cast<int*>(self + 44); ++g) {
        unsigned char* group = at(*reinterpret_cast<unsigned*>(self + 68)) + g * 32;
        int count = *reinterpret_cast<int*>(group);
        bool first_total = true;
        while (count != 0) {
            unsigned char* entry = at(*reinterpret_cast<unsigned*>(self + 72)) + entry_number * 28;
            int points = *reinterpret_cast<short*>(entry + 2);
            bool first_cur = true;
            for (; points != 0; --points) {
                alignas(16) float scaled[3] = {0.0f, 0.0f, 0.0f};
                const float* point;
                int type = *reinterpret_cast<int*>(self + 8);
                if (type == 0) {
                    point = reinterpret_cast<const float*>(at(*reinterpret_cast<unsigned*>(self + 76)) + vertex_number * 12);
                } else if (type == 1) {
                    const short* packed = reinterpret_cast<const short*>(at(*reinterpret_cast<unsigned*>(self + 76)) + vertex_number * 6);
                    float scale = *reinterpret_cast<float*>(self + 16);
                    scaled[0] = static_cast<float>(packed[0]) * scale;
                    scaled[1] = static_cast<float>(packed[1]) * scale;
                    scaled[2] = static_cast<float>(packed[2]) * scale;
                    point = scaled;
                } else if (type == 3) {
                    const unsigned char* base = at(*reinterpret_cast<unsigned*>(self + 76)) + vertex_number * 8;
                    const short* packed = reinterpret_cast<const short*>(base);
                    float scale = *reinterpret_cast<float*>(self + 16);
                    scaled[0] = static_cast<float>(packed[0]) * scale;
                    scaled[1] = static_cast<float>(packed[1]) * scale;
                    scaled[2] = static_cast<float>(packed[2]) * scale;
                    const float* shift = table + 3 * static_cast<signed char>(base[6]);
                    scaled[0] = scaled[0] + shift[0];
                    scaled[1] = scaled[1] + shift[1];
                    scaled[2] = scaled[2] + shift[2];
                    point = scaled;
                } else {
                    return -1;
                }
                Bound__6VIBBoxiRC7VIVect3(total, first_total, point);
                Bound__6VIBBoxiRC7VIVect3(cur, first_cur, point);
                first_total = false;
                first_cur = false;
                ++vertex_number;
            }
            float* out = reinterpret_cast<float*>(at(*reinterpret_cast<unsigned*>(self + 72)) + entry_number * 28 + 4);
            out[0] = cur[0];
            out[1] = cur[1];
            out[2] = cur[2];
            out[3] = cur[3];
            out[4] = cur[4];
            out[5] = cur[5];
            --count;
            ++entry_number;
        }
        float* out = reinterpret_cast<float*>(at(*reinterpret_cast<unsigned*>(self + 68)) + g * 32 + 8);
        out[0] = total[0];
        out[1] = total[1];
        out[2] = total[2];
        out[3] = total[3];
        out[4] = total[4];
        out[5] = total[5];
        if (first_group) {
            first_group = false;
            buffer_box[0] = total[0];
            buffer_box[1] = total[1];
            buffer_box[2] = total[2];
            buffer_box[3] = total[3];
            buffer_box[4] = total[4];
            buffer_box[5] = total[5];
        } else {
            Union__6VIBBoxRC6VIBBoxT1(buffer_box, buffer_box, total);
        }
    }
    return 0;
}
}
