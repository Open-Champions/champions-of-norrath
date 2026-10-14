// Observed Close__7VITimer at 0x01134578, 60 bytes. It returns.
// _7VITimer$Ready is cleared before DisableIntc(9). RemoveIntcHandler
// then receives cause 9 and the handler id loaded after DisableIntc
// returns. EnableIntc(9) follows. The four timer words are defined by
// Init__7VITimer.
extern "C" {
extern int norrath_vi_timer_handler_id __asm__("_7VITimer$HandlerID");
extern int norrath_vi_timer_ready __asm__("_7VITimer$Ready");

void DisableIntc(int cause);
int RemoveIntcHandler(int cause, int handler);
void EnableIntc(int cause);

void Close__7VITimer() {
    norrath_vi_timer_ready = 0;
    DisableIntc(9);
    RemoveIntcHandler(9, norrath_vi_timer_handler_id);
    EnableIntc(9);
}
}
