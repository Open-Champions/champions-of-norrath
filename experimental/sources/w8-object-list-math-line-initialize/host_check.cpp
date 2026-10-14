// Host contract for initializeMe__4Line. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off line_initialize.cpp host_check.cpp
// The host uses libm square roots, so float results are compared with tolerances;
// structural relations (which vector is copied from where) are compared exactly.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
#include "../w8-object-list-math-common/object_types.hpp"

struct LinePoint { Point3 position, direction, side, blend; float segment_length, distance, corner; int slot; };
struct Line { unsigned word0; int count; int continuous; unsigned flags; float total_length; LinePoint* points; };
extern "C" void initializeMe__4Line(Line*);

static bool close(float a, float b, float tol = 2e-5f) { return std::fabs(a - b) <= tol * (1.0f + std::fabs(b)); }
static bool same(const Point3& a, const Point3& b) { return std::memcmp(&a, &b, sizeof a) == 0; }
static bool near_vec(const Point3& a, double x, double y, double z, double tol = 2e-5) {
    return std::fabs(a.x - x) <= tol && std::fabs(a.y - y) <= tol && std::fabs(a.z - z) <= tol;
}

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    const float coords[4][3] = {{0, 0, 0}, {3, 4, 1}, {8, 4, 2}, {9, -5, 1}};  // no vertical segment: up x direction would be zero

    for (int mode = 0; mode < 2; ++mode) {     // 0 = open, -1 = closed (the constructor's default word)
        const int continuous = mode == 0 ? 0 : -1;
        std::vector<LinePoint> pts(5);          // one spare element in front for the quirk, and slack
        LinePoint* p = pts.data() + 1;
        std::memset(pts.data(), 0, pts.size() * sizeof(LinePoint));
        for (int i = 0; i < 4; ++i) { p[i].position = Point3{coords[i][0], coords[i][1], coords[i][2]}; p[i].slot = 100 + i; p[i].corner = -7.0f; }
        Line line;
        std::memset(&line, 0, sizeof line);
        line.count = 4; line.continuous = continuous; line.points = p; line.total_length = 99.0f;
        initializeMe__4Line(&line);

        double total = 0;
        double seg[4];
        for (int i = 0; i < 4; ++i) {
            const int n = (i == 3) ? 0 : i + 1;
            const double dx = coords[n][0] - coords[i][0], dy = coords[n][1] - coords[i][1], dz = coords[n][2] - coords[i][2];
            const double len = std::sqrt(dx * dx + dy * dy + dz * dz);
            const bool has_segment = continuous != 0 || n != 0;
            seg[i] = has_segment ? len : 0;
            expect(close(p[i].segment_length, static_cast<float>(seg[i])), "segment length");
            expect(close(p[i].distance, static_cast<float>(total)), "distance is the total before this segment");
            if (has_segment) {
                expect(near_vec(p[i].direction, dx / len, dy / len, dz / len), "direction is the normalized difference");
                // side = normalize(cross(direction, up)) = normalize(dy, -dx, 0)
                const double sl = std::sqrt(dx * dx + dy * dy);
                if (sl > 0) expect(near_vec(p[i].side, (dy / len) / (sl / len), -(dx / len) / (sl / len), 0), "side is the normalized cross with up");
            } else {
                expect(near_vec(p[i].direction, dx, dy, dz, 1e-6), "the closing difference of an open line stays unnormalized");
            }
            total += seg[i];
        }
        expect(close(line.total_length, static_cast<float>(total)), "total length");
        expect(p[0].slot == 100 && p[3].slot == 103, "slots untouched");

        if (continuous == 0) {
            expect(same(p[0].blend, p[0].side), "open line: first blend is its own side");
            expect(same(p[3].blend, p[2].side), "open line: last blend is the previous side");
            expect(p[0].corner == -7.0f && p[3].corner == -7.0f, "open line: end corners untouched");
            expect(p[3].segment_length == 0.0f, "open line: last segment has no length");
        } else {
            for (int i = 0; i < 4; ++i) {
                const LinePoint& prev = p[(i + 3) % 4];
                const double dot = double(p[i].side.x) * prev.side.x + double(p[i].side.y) * prev.side.y + double(p[i].side.z) * prev.side.z;
                expect(close(p[i].corner, static_cast<float>(1.0 - std::fmax(0.0, dot))), "closed line: corner is 1 - max(0, dot with previous side)");
                double bx = p[i].side.x + prev.side.x, by = p[i].side.y + prev.side.y, bz = p[i].side.z + prev.side.z;
                const double bl = std::sqrt(bx * bx + by * by + bz * bz);
                if (bl > 1e-3) expect(near_vec(p[i].blend, bx / bl, by / bl, bz / bl, 5e-5), "closed line: blend is the normalized sum of adjacent sides");
            }
        }
        for (int i = 1; i < (continuous ? 4 : 3); ++i) {
            const LinePoint& prev = p[i - 1];
            const double dot = double(p[i].side.x) * prev.side.x + double(p[i].side.y) * prev.side.y + double(p[i].side.z) * prev.side.z;
            expect(close(p[i].corner, static_cast<float>(1.0 - std::fmax(0.0, dot))), "interior corner");
            double bx = p[i].side.x + prev.side.x, by = p[i].side.y + prev.side.y, bz = p[i].side.z + prev.side.z;
            const double bl = std::sqrt(bx * bx + by * by + bz * bz);
            if (bl > 1e-3) expect(near_vec(p[i].blend, bx / bl, by / bl, bz / bl, 5e-5), "interior blend");
        }
    }

    // A negative dot clamps the corner to exactly 1.0 (a reversing segment pair).
    {
        std::vector<LinePoint> pts(3);
        std::memset(pts.data(), 0, pts.size() * sizeof(LinePoint));
        LinePoint* p = pts.data() + 1;
        p[0].position = Point3{0, 0, 0};
        p[1].position = Point3{10, 0, 0};
        Line line; std::memset(&line, 0, sizeof line);
        line.count = 2; line.continuous = -1; line.points = p;
        initializeMe__4Line(&line);
        // Two points closed: the sides are opposite, dot = -1, corner = 1.
        expect(p[0].corner == 1.0f && p[1].corner == 1.0f, "opposite sides clamp the corner to 1.0");
        expect(close(line.total_length, 20.0f), "there and back is twice the distance");
    }
    // Empty line: only the total is cleared.
    {
        Line line; std::memset(&line, 0, sizeof line);
        line.total_length = 5.0f; line.count = 0; line.continuous = 1; line.points = nullptr;
        // (a closed empty line would read points[-1]; use an open one)
        line.continuous = 0;
        initializeMe__4Line(&line);
        expect(line.total_length == 0.0f, "empty line clears the total");
    }
    if (failures) return 1;
    std::printf("initializeMe (Line) contract passed\n");
    return 0;
}
