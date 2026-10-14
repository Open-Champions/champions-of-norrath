// Observed: the music globals below, with the image's initial values.
//   musicPlaying 0x00484bf8 (4 bytes, 0), musicStart 0x00484bfc (4 bytes, 0),
//   musicTimer 0x004866c4 (4 bytes, 14400; the neighbour at 0x00484f0c with the same name is a
//   separate file-local object and is not this one), lastMusicState 0x00484cbc (4 bytes, -1),
//   musicDuckAmount 0x0048593c (float 1.0), desiredMusicDuckAmount 0x00485968 (float 0.0),
//   displayMusicPos 0x00484350 (4 bytes, 0), playedStinger 0x004857e0 (1 byte, 0),
//   cutLoadedDialogStream 0x00484368 (4 bytes, 0), cutStartMusic 0x00790f70 (.bss, 48 bytes up to
//   the next symbol; cutInitScene copies a scene string into it), and the 32 signed bytes the music
//   activity ring is kept in, at 0x004296c0 inside the 3780-byte object `list.1073` (the image holds
//   the bytes listed below there; runMusic__Fv zeroes them on entry).
// Inferred: nothing. Which of these words are counters, flags or indices is established only by
// their callers' own notes.
// Unknown: whether cutStartMusic is longer than the 48 bytes to its neighbour (that distance is
// all that is known), and how the activity ring's initial bytes came to be there.
extern "C" {
int musicPlaying = 0;
int musicStart = 0;
int musicTimer = 14400;
int lastMusicState = -1;
float musicDuckAmount = 1.0f;
float desiredMusicDuckAmount = 0.0f;
int displayMusicPos = 0;
unsigned char playedStinger = 0;
int cutLoadedDialogStream = 0;
char cutStartMusic[48];
signed char norrath_music_activity_ring_view[32] = { 5, 0, 89, 26, 0, 0, 0, 0, 0, 0, 0, 0, -104, -57, 70, 0, 0, 0, 64, 64, 0, 0, -128, 63, 0, 28, 0, 0, 0, 80, -61, -57 };
}
