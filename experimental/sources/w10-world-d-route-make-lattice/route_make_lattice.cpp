// Observed routeMakeLattice__Fv at 0x002bfef8, 2276 bytes. Builds the neighbour links of the
// route nodes. Node layout used (64-byte records, 512 of them, at the object recorded as
// norrath_route_nodes_bytes): a Point3 at +0, up to 12 link halfwords from +12, and a halfword
// link count at +52. A link halfword is a node index in bits 0..14; bit 15 is set on links this
// routine adds.
// nodecount is set to nodeused. When that is positive:
//  1. For each node i: the 12 nearest other nodes by squared distance (ordered nearest first, ties
//     keeping the earlier index in front, missing entries -1) are found by insertion, carrying the
//     displaced element with swap__H1Zf_RX01T0_v and swap__H1Zs_RX01T0_v. The link count of i is
//     cleared. Then for each candidate in that order (stopping at a -1 entry or when 12 links are
//     held): worldCanPass__FP12_worldHeaderG6Point3T1fi(world, node i, candidate, 6.0, 3) must
//     return nonzero; then, against every link already held (L), the candidate is dropped when
//     |i-c| + |c-L| < 1.1 * |i-L| or |i-L| + |L-c| < 1.1 * |i-c|. A kept candidate is appended with
//     bit 15 set.
//  2. For each node i, each link A of i, and each link B of A other than i: if B is also a link of i
//     and |i-A| + |A-B| < 1.25 * |i-B|, the link to B is removed from i (later links shift down, the
//     count falls by one, and the outer position steps back when it was at or past the removed slot).
// Finally, eight unit directions at angles of 8192 * k (k = 0..7) are written into route_dirs as
// (icos, isin, 0).
// The image also reads the cycle counter four times and discards the values; those reads are not
// reproduced. Distances are the square root of (dx*dx + dy*dy) + dz*dz in x, y, z order.
// Inferred: this builds a navigation graph and prunes shortcuts. Unknown: the name of the
// node record, the meaning of bit 15, and how the route code uses route_dirs.
struct _worldHeader;
struct Point3 { float x, y, z; };
extern _worldHeader* world;
extern int nodecount;
extern Point3 route_dirs[8];
extern "C" {
extern unsigned char norrath_route_nodes_bytes[];
extern unsigned char norrath_route_node_used_bytes[];
void swap__H1Zf_RX01T0_v(float*, float*);
void swap__H1Zs_RX01T0_v(short*, short*);
int worldCanPass__FP12_worldHeaderG6Point3T1fi(_worldHeader*, Point3, Point3, float, int);
float icos__Fi(int);
float isin__Fi(int);
}

namespace {
inline unsigned char* node(int index) { return norrath_route_nodes_bytes + index * 64; }
inline float* position(int index) { return reinterpret_cast<float*>(node(index)); }
inline short* links(int index) { return reinterpret_cast<short*>(node(index) + 12); }
inline short& link_count(int index) { return *reinterpret_cast<short*>(node(index) + 52); }
inline float separation(const float* a, const float* b) {
    const float dx = a[0] - b[0];
    const float dy = a[1] - b[1];
    const float dz = a[2] - b[2];
    return __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
}
}

extern "C" void routeMakeLattice__Fv() {
    const int count = *reinterpret_cast<int*>(norrath_route_node_used_bytes);
    nodecount = count;
    if (count > 0) {
        for (int i = 0; i < count; ++i) {
            short nearest[12];
            float nearest_squared[12];
            for (int k = 0; k < 12; ++k) {
                nearest[k] = -1;
                nearest_squared[k] = 10000000000.0f;
            }
            for (int j = 0; j < count; ++j) {
                if (j == i) continue;
                const float dx = position(i)[0] - position(j)[0];
                const float dy = position(i)[1] - position(j)[1];
                const float dz = position(i)[2] - position(j)[2];
                float squared = dx * dx + dy * dy + dz * dz;
                short index = static_cast<short>(j);
                for (int k = 0; k < 12; ++k) {
                    if (squared < nearest_squared[k]) {
                        swap__H1Zf_RX01T0_v(&squared, &nearest_squared[k]);
                        swap__H1Zs_RX01T0_v(&index, &nearest[k]);
                    }
                }
            }
            link_count(i) = 0;
            for (int c = 0; nearest[c] != -1;) {
                const int candidate = nearest[c];
                const Point3 from = {position(i)[0], position(i)[1], position(i)[2]};
                const Point3 to = {position(candidate)[0], position(candidate)[1], position(candidate)[2]};
                if (worldCanPass__FP12_worldHeaderG6Point3T1fi(world, from, to, 6.0f, 3)) {
                    bool keep = true;
                    const int held = link_count(i);
                    for (int a = 0; a < held && keep; ++a) {
                        const int other = links(i)[a] & 0x7fff;
                        if (separation(position(i), position(candidate)) + separation(position(candidate), position(other)) <
                            separation(position(i), position(other)) * 1.1f) {
                            keep = false;
                        } else if (separation(position(i), position(other)) + separation(position(other), position(candidate)) <
                                   separation(position(i), position(candidate)) * 1.1f) {
                            keep = false;
                        }
                    }
                    if (keep) {
                        links(i)[link_count(i)] = static_cast<short>(candidate | 0x8000);
                        link_count(i) = static_cast<short>(link_count(i) + 1);
                    }
                }
                ++c;
                if (c >= 12 || link_count(i) == 12) break;
            }
        }
        for (int i = 0; i < count; ++i) {
            for (int t = 0; t < link_count(i); ++t) {
                const int a = links(i)[t] & 0x7fff;
                for (int s = 0; s < link_count(a); ++s) {
                    const int b = links(a)[s] & 0x7fff;
                    if (b == i) continue;
                    for (int k = 0; k < link_count(i); ++k) {
                        if ((links(i)[k] & 0x7fff) != b) continue;
                        if (separation(position(i), position(a)) + separation(position(a), position(b)) <
                            separation(position(i), position(b)) * 1.25f) {
                            for (int m = k + 1; m < link_count(i); ++m) links(i)[m - 1] = links(i)[m];
                            link_count(i) = static_cast<short>(link_count(i) - 1);
                            if (!(t < k)) --t;
                        }
                    }
                }
            }
        }
    }
    for (int k = 0; k < 8; ++k) {
        const int angle = k << 13;
        const float cosine = icos__Fi(angle);
        const float sine = isin__Fi(angle);
        route_dirs[k].x = cosine;
        route_dirs[k].y = sine;
        route_dirs[k].z = 0.0f;
    }
}
