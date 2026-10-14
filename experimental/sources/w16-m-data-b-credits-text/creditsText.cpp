// creditsText: zeroed storage for an unresolved import of the w16-m-data-b package.
// Observed (reference image, reviewed EE nm; addresses only, no contents):
//   - nm -S on the reference image: creditsText at 0x00428a20, size 0x968, type d (initialised data).
//   - Caller: frontend-shell-run/run_front_end.cpp indexes it by cursor and reads words from it.
//   - w8-startup-sce-c-data-pre-configure/pre_configure.cpp places five 16-byte records just before it.
// Inferred:
//   - 2408 bytes, from the symbol size.
// Unknown:
//   - The record layout and the text it points to. The contents are game text and are not reproduced here.
// This unit gives the link name and extent only. Its zero contents do not establish any field value.
extern "C" {
unsigned char creditsText[2408];
}
static_assert(sizeof creditsText == 2408u, "creditsText extent");
