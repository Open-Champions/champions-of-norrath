#include "../w10-world-b-common/mem.hpp"
// Observed gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi at 0x00278878, 716
// bytes in SLUS_205.65. Arguments: two points P and Q (three floats each), an output array of
// 24-byte hit records, and a maximum record count. Tests the segment from Q (parameter 0)
// toward P (parameter 1) against the planes of the game lines and returns the number of
// hits stored.
// Setup: d = P - Q, componentwise. Lines are gameLines[0 .. gameLineCount) (0x008d57a8,
// count at 0x00485ea8). A line is used only when bit 0 of the flag word at +12 is set. Its
// segment count is the point count at +4 when the word at +8 is nonzero, else that count
// minus one; its vertices are 64 bytes each starting at the pointer at +20.
// For each segment index i, with vertex V = vertices + 64 * i (position at V+0, a second
// vector at V+12, a plane normal N at V+24, a float at V+48):
//   v = V.position - Q; denominator = d . N; numerator = v . N (each summed x, then y, then z);
//   a positive denominator is raised to at least 0.01f and a negative one lowered to at
//   most -0.01f (zero stays zero); t = numerator / denominator;
//   t below 0 or above 1 skips the segment;
//   hit = Q + d * t (x, y, z);
//   distance = distanceToPlane__FP6Point3N20(V, V + 12, hit); the segment is skipped when
//   distance < -0.25f or when V[48] + 0.25f < distance;
//   otherwise a record is appended: line pointer (+0), vertex pointer (+4), hit position
//   (+8..+19); the count increases and the call returns it as soon as it equals the maximum.
// Records are 24 bytes; the last four are not written. After the last line the count is
// returned. A maximum of zero or less does not stop early, so the output is written without bound
// (as in the image).
// Inferred: the planes are the edge walls of a game line path; V+48 a thickness. Unknown:
// the vertex fields at +12, +36 and others, and the real record type.
extern "C" float distanceToPlane__FP6Point3N20(const unsigned char* a, const unsigned char* b, const unsigned char* c);
extern unsigned char* gameLines[100];
extern int gameLineCount;

extern "C" int gameLineRayCollision__FP6Point3T0P23gameLineCollisionPacketi(
    unsigned char* p, unsigned char* q, unsigned char* out, int maximum) {
    using namespace norrath_wb;
    float d[3], v[3], hit[3];
    for (int k = 0; k < 3; ++k) d[k] = load_float(p + 4 * k) - load_float(q + 4 * k);
    int found = 0;
    unsigned char* record = out;
    for (int line = 0; line < gameLineCount; ++line) {
        unsigned char* rec = gameLines[line];
        if (((load_word(rec + 12) ^ 1) & 1) != 0) continue;
        int segments = load_word(rec + 8) != 0 ? int(load_word(rec + 4)) : int(load_word(rec + 4)) - 1;
        for (int i = 0; i < segments; ++i) {
            unsigned char* vertex = load_pointer(rec + 20) + 64 * i;
            for (int k = 0; k < 3; ++k) v[k] = load_float(vertex + 4 * k) - load_float(q + 4 * k);
            const unsigned char* n = vertex + 24;
            float denominator = d[0] * load_float(n) + d[1] * load_float(n + 4) + d[2] * load_float(n + 8);
            float numerator = v[0] * load_float(n) + v[1] * load_float(n + 4) + v[2] * load_float(n + 8);
            if (0.0f < denominator) {
                if (denominator < 0.01f) denominator = 0.01f;
            }
            if (denominator < 0.0f) {
                if (-0.01f < denominator) denominator = -0.01f;
            }
            const float t = numerator / denominator;
            if (t < 0.0f) continue;
            if (1.0f < t) continue;
            for (int k = 0; k < 3; ++k) hit[k] = load_float(q + 4 * k) + d[k] * t;
            unsigned char hitBytes[12];
            for (int k = 0; k < 3; ++k) store_float(hitBytes + 4 * k, hit[k]);
            const float distance = distanceToPlane__FP6Point3N20(vertex, vertex + 12, hitBytes);
            if (distance < -0.25f) continue;
            if (load_float(vertex + 48) + 0.25f < distance) continue;
            store_pointer(record + 4, vertex);
            store_pointer(record + 0, rec);
            ++found;
            for (int k = 0; k < 3; ++k) store_float(record + 8 + 4 * k, hit[k]);
            record += 24;
            if (found == maximum) return found;
        }
    }
    return found;
}
