// Observed: CalcBVolumes__12VIPrimBufferiP7VIVect3 at 0x01102140 is 1240 bytes. The integer argument
// is never read; the third is a table of 12-byte vectors. Offsets are bytes into the buffer object:
// G = word at 64 (group count), groups = 16-byte records at (pointer at 88) with the entry count in
// word 1, entries = 28-byte records at (pointer at 92) with the point count in word 0, data = stream
// at (pointer at 96), scale = float at 48, type = word at 8. Types 0, 2, 4, 5 and 8 are accepted
// (nine-entry jump tables); every other type returns -1, checked at the start of each entry, before
// each point and after each entry's points. Scratch boxes `total` and `cur` (six floats each,
// min xyz then max xyz) start as zeros and persist for the call. If G <= 0 it returns 0. For
// each group g (first_total = true; entry number e and the data pointer continue from group to
// group):
//   * count times: take entry e (n = its word 0); first_cur = true; the data pointer moves on by
//     32 (a header); n times it builds a point from the stream and calls
//     Bound__6VIBBoxiRC7VIVect3(cur, first_cur, point), then first_cur = false:
//       type 0: three floats at the pointer, which then moves on 64;
//       type 2 and 5: three signed halfwords at pointer+4, +6, +8 times scale; moves on 32;
//       type 8: the same, moves on 16;
//       type 4: the same, moves on 32, then adds the 12-byte table vector indexed by the signed
//       halfword at pointer+10 (read before the move);
//     afterwards the data pointer moves on 16 (a trailer), cur is stored into the entry (min xyz at
//     entry offsets 4, 8, 12 and max xyz at 16, 20, 24), and total takes cur if first_total (then
//     first_total = false) or becomes Union__6VIBBoxRC6VIBBoxT1(total, total, cur) otherwise.
//     A zero-point entry stores the previous cur (or zeros);
//   * after the entries, the first group copies total into the buffer's own box (min at offset 20,
//     max at offset 32), later groups call Union(box, box, total). A group with no entries
//     contributes the old total (or zeros).
// It returns 0. Bound/Union results are ignored.
// Inferred: the box layout and the meaning of the flag (nonzero restarts the box from the point)
// from this use; the entry's point count and the stream layout (32-byte header, points, 16-byte
// trailer) from the pointer arithmetic. Unknown: the real names and the stream types' meaning.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void Bound__6VIBBoxiRC7VIVect3(float* box, int restart, const float* point);
void Union__6VIBBoxRC6VIBBoxT1(float* destination, const float* a, const float* b);

static inline bool accepted(unsigned type) { return type <= 8 && ((1u << type) & 0x135u) != 0; }

int CalcBVolumes__12VIPrimBufferiP7VIVect3(unsigned char* self, int, const float* table) {
    alignas(16) float total[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    alignas(16) float cur[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    unsigned char* data = at(*reinterpret_cast<unsigned*>(self + 96));
    if (*reinterpret_cast<int*>(self + 64) <= 0)
        return 0;
    float* buffer_box = reinterpret_cast<float*>(self + 20);
    bool first_group = true;
    int entry_number = 0;
    for (int g = 0; g < *reinterpret_cast<int*>(self + 64); ++g) {
        int count = *reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 88)) + g * 16 + 4);
        bool first_total = true;
        while (count != 0) {
            int points = *reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 92)) + entry_number * 28);
            bool first_cur = true;
            if (!accepted(*reinterpret_cast<unsigned*>(self + 8)))
                return -1;
            data += 32;
            for (; points != 0; --points) {
                unsigned type = *reinterpret_cast<unsigned*>(self + 8);
                if (!accepted(type))
                    return -1;
                alignas(16) float point[3] = {0.0f, 0.0f, 0.0f};
                if (type == 0) {
                    const float* v = reinterpret_cast<const float*>(data);
                    point[0] = v[0];
                    point[1] = v[1];
                    point[2] = v[2];
                    data += 64;
                } else {
                    const short* packed = reinterpret_cast<const short*>(data + 4);
                    float scale = *reinterpret_cast<float*>(self + 48);
                    point[0] = static_cast<float>(packed[0]) * scale;
                    point[1] = static_cast<float>(packed[1]) * scale;
                    point[2] = static_cast<float>(packed[2]) * scale;
                    if (type == 4) {
                        const float* shift = table + 3 * *reinterpret_cast<const short*>(data + 10);
                        data += 32;
                        point[0] = point[0] + shift[0];
                        point[1] = point[1] + shift[1];
                        point[2] = point[2] + shift[2];
                    } else {
                        data += type == 8 ? 16 : 32;
                    }
                }
                Bound__6VIBBoxiRC7VIVect3(cur, first_cur, point);
                first_cur = false;
            }
            if (!accepted(*reinterpret_cast<unsigned*>(self + 8)))
                return -1;
            data += 16;
            float* out = reinterpret_cast<float*>(at(*reinterpret_cast<unsigned*>(self + 92)) + entry_number * 28 + 4);
            out[0] = cur[0];
            out[1] = cur[1];
            out[2] = cur[2];
            out[3] = cur[3];
            out[4] = cur[4];
            out[5] = cur[5];
            if (first_total) {
                first_total = false;
                total[0] = cur[0];
                total[1] = cur[1];
                total[2] = cur[2];
                total[3] = cur[3];
                total[4] = cur[4];
                total[5] = cur[5];
            } else {
                Union__6VIBBoxRC6VIBBoxT1(total, total, cur);
            }
            --count;
            ++entry_number;
        }
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
