// Observed objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature at 0x001535d0, 880 bytes.
// Arguments: a position (three floats, passed by reference), a signed halfword heading, a radius, a
// cone width (-1.0 means no cone), a list head and an optional creature filter. No result.
//  1. A box around the position with half-extent radius is searched with
//     objectFindInBox__FG6Point3T0PP10GameObjecti (low corner, high corner, a 512-pointer buffer,
//     512); the result list is read until its first null pointer.
//  2. The unit vector for the heading is (icos(heading), isin(heading)). Unless the cone width is
//     -1.0, two more vectors are made. With h = cone * 0.5 * 182.04443359375 rounded half away from
//     zero and truncated to an integer, the first is the heading + h - 16384 and the second the
//     heading - h + 16384, each as (icos, isin) with a halfword wrap.
//  3. An object is skipped unless bit 0x20 of its word at +12 is set, and also when any of 0xd is
//     set. With a filter creature, its entry at (word at +72) + 160 is called as
//     fn(creature + halfword at entry + 0, object); a zero result skips the object. Unless the cone
//     width is -1.0, the object's position (at +16) must have distanceToPlane2 of at least zero for
//     both cone vectors. It must then have distanceToPlane2 above zero for the heading vector.
//  4. Each kept object is inserted into the list with listInsert__FP8ListHeadP10__listNodePv, before
//     the first node whose creature (node word at +12) is no farther in x and y than the object,
//     or at the end (node 0). The list is therefore in order of falling distance.
// Inferred: the first vector is a forward direction and the other two bound a cone around it, so
// the list holds creatures in front of the position, farthest first. The three plane tests use
// the first argument of distanceToPlane2 as the origin.
// Unknown: the name of the filter call (a virtual entry of Creature) and the bits 0xd and 0x20.
struct Point3 { float x, y, z; };
struct Point2 { float x, y; };

extern "C" {
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int max);
float icos__Fi(int angle);
float isin__Fi(int angle);
float distanceToPlane2__FP6Point3P6Point2T0(Point3* origin, Point2* direction, Point3* target);
void listInsert__FP8ListHeadP10__listNodePv(void* list, void* node, void* item);
}

namespace {
template <class T>
inline T load(const void* base, int offset) {
    T value;
    __builtin_memcpy(&value, static_cast<const unsigned char*>(base) + offset, sizeof value);
    return value;
}
struct CallEntry {
    short delta;
    short unused;
    int (*function)(void* self, void* object);
};
inline Point2 headingVector(int heading) {
    Point2 v;
    v.x = icos__Fi(heading);
    v.y = isin__Fi(heading);
    return v;
}
inline int coneStep(float cone) {
    const float scaled = cone * 0.5f * 182.04443359375f;
    return static_cast<int>(scaled >= 0.0f ? scaled + 0.5f : scaled - 0.5f);
}
}

extern "C" void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 center, short heading, float radius, float cone,
                                                                              void* list, void* filter) {
    void* found[512];
    Point2 left = {0.0f, 0.0f};
    Point2 right = {0.0f, 0.0f};
    const Point3 low = {center.x - radius, center.y - radius, center.z - radius};
    const Point3 high = {center.x + radius, center.y + radius, center.z + radius};
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 512);
    const Point2 forward = headingVector(heading);
    const bool coned = cone != -1.0f;
    if (coned) {
        left = headingVector(static_cast<short>(heading + coneStep(cone) - 16384));
        right = headingVector(static_cast<short>(heading - coneStep(cone) + 16384));
    }
    for (int i = 0; found[i] != nullptr; ++i) {
        unsigned char* object = static_cast<unsigned char*>(found[i]);
        const unsigned flags = load<unsigned>(object, 12);
        if ((flags & 0x20u) == 0 || (flags & 0xdu) != 0) continue;
        if (filter != nullptr) {
            const CallEntry* entry = reinterpret_cast<const CallEntry*>(load<unsigned char*>(filter, 72) + 160);
            if (entry->function(static_cast<unsigned char*>(filter) + entry->delta, object) == 0) continue;
        }
        Point3* position = reinterpret_cast<Point3*>(object + 16);
        if (coned) {
            if (distanceToPlane2__FP6Point3P6Point2T0(&center, &left, position) < 0.0f) continue;
            if (distanceToPlane2__FP6Point3P6Point2T0(&center, &right, position) < 0.0f) continue;
        }
        if (!(distanceToPlane2__FP6Point3P6Point2T0(&center, const_cast<Point2*>(&forward), position) > 0.0f)) continue;
        const float dx = center.x - position->x;
        const float dy = center.y - position->y;
        const float distance = dx * dx + dy * dy;
        void* node = load<void*>(list, 0);
        bool inserted = false;
        while (node != nullptr) {
            const Point3* other = reinterpret_cast<const Point3*>(load<unsigned char*>(node, 12) + 16);
            const float ox = center.x - other->x;
            const float oy = center.y - other->y;
            if (ox * ox + oy * oy <= distance) {
                listInsert__FP8ListHeadP10__listNodePv(list, node, object);
                inserted = true;
                break;
            }
            node = load<void*>(node, 0);
        }
        if (!inserted) listInsert__FP8ListHeadP10__listNodePv(list, nullptr, object);
    }
}
