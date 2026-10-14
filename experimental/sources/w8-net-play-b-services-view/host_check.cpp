// Host check for the services float view. Not an EE plan input.
#include "services_view.cpp"
#include <cstring>
int main() {
    float value;
    std::memcpy(&value, norrath_player_services_view_48068c, 4);
    unsigned bits;
    std::memcpy(&bits, norrath_player_services_view_48068c, 4);
    if (bits != 0x3ecccccc) return 1;
    return value < 0.4f && value > 0.39999f ? 0 : 2;
}
