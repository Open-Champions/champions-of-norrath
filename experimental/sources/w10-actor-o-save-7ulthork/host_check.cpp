// Host contract for msg_save__7UlthorkPc; not an EE input. Includes the source.
#define W10_ACTOR_O_HOST 1
#include <cstdio>
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_o_host_base;
#include "msg_save.cpp"

static int base_calls, base_return;
static void* base_self; static char* base_buffer;
extern "C" int msg_save__8CreaturePc(void* self, char* buffer) { ++base_calls; base_self = self; base_buffer = buffer; return base_return; }

int main() {
    alignas(8) unsigned char object[1200];
    alignas(8) unsigned char buffer[64];
    for (int n : {0, 4, 12, 20}) {
        for (int b : {0, 1, 2, 127, 128, 255}) {
            std::memset(object, 0x11, sizeof object); std::memset(buffer, 0xa5, sizeof buffer);
            const unsigned w = 0xdeadbeefu; std::memcpy(object + 960, &w, 4);
            object[1104] = static_cast<unsigned char>(b);
            base_calls = 0; base_return = n;
            if (msg_save__7UlthorkPc(object, reinterpret_cast<char*>(buffer)) != n + 8) return 1;
            if (base_calls != 1 || base_self != object || base_buffer != reinterpret_cast<char*>(buffer)) return 2;
            unsigned got; std::memcpy(&got, buffer + n, 4); if (got != w) return 3;
            int sb; std::memcpy(&sb, buffer + n + 4, 4); if (sb != static_cast<signed char>(b)) return 4;
            for (int i = 0; i < 64; ++i) if ((i < n || i >= n + 8) && buffer[i] != 0xa5) return 5;
            for (int i = 0; i < 1200; ++i) if (object[i] != ((i >= 960 && i < 964) ? (i == 960 ? 0xef : i == 961 ? 0xbe : i == 962 ? 0xad : 0xde) : i == 1104 ? b : 0x11)) return 6;
        }
    }
    return 0;
}
