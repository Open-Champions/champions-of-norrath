// Observed boxTex at 0x0048507c, four zero bytes. DLG_ShowDialogText at
// 0x00485010, four zero bytes. curDlg at 0x00486b80 is .sbss; dlgPos starts
// four bytes later and the ELF size field is 0, so the slot is four bytes
// and starts zero. world-state also publishes these four bytes as
// norrath_dialog_active_pointer_word_view. This object is the ELF name.
extern "C" unsigned boxTex = 0u;
extern "C" unsigned DLG_ShowDialogText = 0u;
extern "C" unsigned curDlg = 0u;
