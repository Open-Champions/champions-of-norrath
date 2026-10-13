// Host comparison only. Not an EE build input.
#include <cstdio>
#include "lgaud_data.cpp"
extern "C" unsigned char norrath_lgaud_rpc_client_view[];
extern "C" unsigned norrath_lgaud_async_record_view[];
int main() {
    static_assert(sizeof(norrath_lgaud_rpc_client_view) == 40, "client extent");
    static_assert(sizeof(norrath_lgaud_async_record_view) == 16, "record extent");
    for (unsigned char byte : norrath_lgaud_rpc_client_view) if (byte != 0) { std::puts("client not zero"); return 2; }
    for (unsigned word : norrath_lgaud_async_record_view) if (word != 0) { std::puts("record not zero"); return 2; }
    std::puts("lgaud data ok");
    return 0;
}
