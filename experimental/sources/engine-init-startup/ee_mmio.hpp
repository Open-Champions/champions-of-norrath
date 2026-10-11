#ifndef NORRATH_ENGINE_INIT_EE_MMIO_HPP
#define NORRATH_ENGINE_INIT_EE_MMIO_HPP

// Host builds define NORRATH_HOST_CONTRACT and supply the functions below.
// The reviewed EE profile does not define that macro, so those builds use
// the absolute loads and stores.

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char norrath_host_load8(unsigned address);
extern "C" void norrath_host_store8(unsigned address, unsigned char value);
extern "C" void norrath_host_store16(unsigned address, unsigned short value);
extern "C" unsigned norrath_host_load32(unsigned address);
extern "C" void norrath_host_store32(unsigned address, unsigned value);
extern "C" void norrath_host_store64(unsigned address, unsigned long long value);
extern "C" void norrath_host_sync();
extern "C" unsigned norrath_host_cop0_count();
extern "C" volatile unsigned norrath_host_memory_last;
extern "C" volatile unsigned norrath_host_stack_base;
extern "C" unsigned norrath_host_link_end();

#define NORRATH_LOAD8(address) norrath_host_load8(address)
#define NORRATH_STORE8(address, value) norrath_host_store8((address), static_cast<unsigned char>(value))
#define NORRATH_STORE16(address, value) norrath_host_store16((address), static_cast<unsigned short>(value))
#define NORRATH_LOAD32(address) norrath_host_load32(address)
#define NORRATH_STORE32(address, value) norrath_host_store32((address), (value))
#define NORRATH_STORE64(address, value) norrath_host_store64((address), (value))
#define NORRATH_SYNC() norrath_host_sync()
#define NORRATH_COP0_COUNT() norrath_host_cop0_count()
#define NORRATH_MEMORY_LAST (norrath_host_memory_last)
#define NORRATH_STACK_BASE (norrath_host_stack_base)
#define NORRATH_LINK_END (norrath_host_link_end())
#else
#define NORRATH_LOAD8(address) (*reinterpret_cast<volatile unsigned char*>(static_cast<unsigned>(address)))
#define NORRATH_STORE8(address, value) (*reinterpret_cast<volatile unsigned char*>(static_cast<unsigned>(address)) = static_cast<unsigned char>(value))
#define NORRATH_STORE16(address, value) (*reinterpret_cast<volatile unsigned short*>(static_cast<unsigned>(address)) = static_cast<unsigned short>(value))
#define NORRATH_LOAD32(address) (*reinterpret_cast<volatile unsigned*>(static_cast<unsigned>(address)))
#define NORRATH_STORE32(address, value) (*reinterpret_cast<volatile unsigned*>(static_cast<unsigned>(address)) = (value))
#define NORRATH_STORE64(address, value) (*reinterpret_cast<volatile unsigned long long*>(static_cast<unsigned>(address)) = (value))
static inline void norrath_ee_sync() {
    asm volatile("sync" ::: "memory");
}
static inline unsigned norrath_ee_cop0_count() {
    unsigned value;
    asm volatile("mfc0 %0, $9" : "=r"(value));
    return value;
}
#define NORRATH_SYNC() norrath_ee_sync()
#define NORRATH_COP0_COUNT() norrath_ee_cop0_count()
#define NORRATH_MEMORY_LAST (0x01ffffffu)
#define NORRATH_STACK_BASE (0x01fc0000u)
extern "C" char _end[];
#define NORRATH_LINK_END (static_cast<unsigned>(reinterpret_cast<unsigned long>(&_end[0])))
#endif

#endif
