// Observed: VIEEFileDelay__Fi at 0x010cb4f8 is 104 bytes and returns nothing. It builds a
// semaphore parameter block on its stack, stores 1 at offset 4 and 0 at offsets 8 and 20
// (the other three words are not written), creates the semaphore, arms an alarm with
// SetAlarm(argument & 0xffff, VIEEFileDelayTh__FiUsPv at 0x010cb560, semaphore), waits on the
// semaphore and deletes it. The alarm callback (not authored in this unit) signals the
// semaphore passed as its third argument from interrupt context.
// Inferred: a sleep of `ticks` alarm units (CDSync passes 480) that blocks the calling thread;
// the block is the SDK semaphore parameter {count, max_count, init_count, wait_threads,
// attr, option}, so this is an initial count of 0 and a maximum of 1.
// Unknown: whether the alarm unit is the SDK's 1/15734.4 s tick, and the unwritten words'
// values (stack garbage reaching CreateSema).
extern "C" {
struct VIEESemaParam {
    int count;
    int max_count;
    int init_count;
    int wait_threads;
    unsigned attr;
    unsigned option;
};
int CreateSema(VIEESemaParam* parameter);
int DeleteSema(int semaphore);
int WaitSema(int semaphore);
int SetAlarm(unsigned short time, unsigned (*handler)(int, unsigned short, void*), void* common);
unsigned VIEEFileDelayTh__FiUsPv(int id, unsigned short time, void* common);

void VIEEFileDelay__Fi(int ticks) {
    VIEESemaParam parameter;
    parameter.init_count = 0;
    parameter.max_count = 1;
    parameter.option = 0;
    const int semaphore = CreateSema(&parameter);
    SetAlarm(static_cast<unsigned short>(ticks), VIEEFileDelayTh__FiUsPv,
             reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(semaphore)));
    WaitSema(semaphore);
    DeleteSema(semaphore);
}
}
