// Observed vblank words used by engineSyncVBlank. vblankCount at 0x00484438 is
// 4 zero bytes. vblankField at 0x0048443c is 1 zero byte. vblankSemaphore2 at
// 0x00486a60 is the 4-byte gap before specialMpegMode and is zero in BSS.
// These are the names the CD archive imports.
extern "C" {
unsigned norrath_archive_vblank_count_view = 0;
unsigned char norrath_archive_vblank_field_view = 0;
int norrath_archive_vblank_semaphore_view = 0;
}
