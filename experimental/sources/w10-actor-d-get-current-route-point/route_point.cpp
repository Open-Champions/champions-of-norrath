// Observed getCurrentRoutePoint__8Creature at 0x00113200, 112 bytes.
// The result Point3 comes back through a hidden first pointer (returned in V0) and the creature is
// the second argument. Let i be the signed int at object+708. The result starts as (-1.0, -1.0,
// -1.0). When i < 16, x and y become the ints at object+580+8*i and object+584+8*i converted to
// float; z stays -1.0. There is no lower bound check on i. The result is built in a stack copy and
// then copied to the caller's Point3.
// Inferred: the 16-entry table of int pairs starting at +580 is a route, +708 is its current
// index, and -1.0 means "no point". Unknown: the names and types of these fields, and what the
// 128 bytes at 580..707 hold besides the points. Nothing here ran on the EE.
struct Point3 {
    float x, y, z;
};

extern "C" Point3* getCurrentRoutePoint__8Creature(Point3* result, void* self) {
    const unsigned char* object = static_cast<const unsigned char*>(self);
    const int index = *reinterpret_cast<const int*>(object + 708);
    Point3 point = {-1.0f, -1.0f, -1.0f};
    if (index < 16) {
        const int* entry = reinterpret_cast<const int*>(object + 580 + 8 * index);
        point.x = static_cast<float>(entry[0]);
        point.y = static_cast<float>(entry[1]);
    }
    *result = point;
    return result;
}
