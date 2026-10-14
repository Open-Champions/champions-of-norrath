/* DelayThread at 0x002d6140 stores 0x0047b688 at semaphore-descriptor
 * offset 20. That address holds the C string DelayThread. It is not a
 * sized ELF symbol; bytes after the NUL are not included.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_delay_thread_name_view[] = {
    'D', 'e', 'l', 'a', 'y', 'T', 'h', 'r', 'e', 'a', 'd', 0
};
}
