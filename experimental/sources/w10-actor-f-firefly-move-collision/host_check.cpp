// Host contract for msg_moveCollision__7FireFlyPP10GameObject. Includes the source; not an EE input.
#include "msg_move_collision.cpp"
int main() {
    unsigned char object[64];
    void* list[4] = {object, object + 1, 0, object + 2};
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 5 + 1);
    msg_moveCollision__7FireFlyPP10GameObject(object, list);
    msg_moveCollision__7FireFlyPP10GameObject(object, 0);
    for (unsigned i = 0; i < sizeof object; ++i)
        if (object[i] != static_cast<unsigned char>(i * 5 + 1)) return 1;
    return list[0] == object && list[1] == object + 1 && list[2] == 0 && list[3] == object + 2 ? 0 : 2;
}
