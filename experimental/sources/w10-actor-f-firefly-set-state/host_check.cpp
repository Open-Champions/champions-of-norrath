// Host contract for setState__7FireFlyi. Includes the source; not an EE input.
#include "set_state.cpp"
#include <initializer_list>
int main() {
    unsigned char object[64];
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = static_cast<unsigned char>(i * 7 + 3);
    for (int state : {0, 1, -1, 12345}) {
        setState__7FireFlyi(object, state);
        for (unsigned i = 0; i < sizeof object; ++i)
            if (object[i] != static_cast<unsigned char>(i * 7 + 3)) return 1;
    }
    return 0;
}
