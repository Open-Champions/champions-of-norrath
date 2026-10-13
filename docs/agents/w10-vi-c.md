# VI front-end window classes (VIUI to VIWndSplash)

Package `w10-vi-c`. All 33 assigned imports are closed by 33 independently authored units under
`experimental/sources/w10-vi-c-*`. Addresses, sizes and instruction order come from the pinned
SLUS-20565 image (disassembled privately with EE binutils 2.45.1; nothing from it is committed).
Each source file opens with an `Observed` / `Inferred` / `Unknown` comment. Each unit has a
`host_check.cpp` that returns nonzero when a checked call, argument, flag or stored word differs;
host checks are not plan inputs. Fresh compilations here are not original-byte matches (the
original was built by a different compiler and schedules differently).

## Verification performed

- Every unit compiles with the `authored_cpp` flags of the reviewed EE toolchain, and all 33
  objects link together with `ld -r` without duplicate definitions; the defined set is exactly the
  33 assigned names. No full partial build was run (a private plan copy was not needed for this).
- Every unit was run against the original function in a private R5900 interpreter (outside git;
  see the notes on the pairing harness): same arguments and memory snapshot, callees replaced by
  logging stubs, comparing the call log (callee, arguments), every changed byte outside the stack
  and, for value-returning functions, the result. Scenario counts: constructors, destructors
  (flags 0 to 3), Init and Free units 2 to 4 randomised object fills each; GenericRenderer
  destructor 17 scenarios (empty list, several pages, a null page, absent child, absent word at
  +56, flags 0 to 3); SetFocus all 16 combinations of focus, window, force and mcErrOn (78 of 79
  instruction words reached, the rest is a branch-delay nop); Update 120 randomised scenarios
  (167 of 170 words reached, the rest are nops). Mutating a stored offset made the comparison
  fail, so the checks are not vacuous.
- Host checks pass (`SetFocus` and `Update` are built as x86_64 with statics below 4 GiB because
  they hold addresses in 32-bit words).

## Imports closed (33)

`_$_4VIUI`, `__4VIUI`, `Free__15VIWindowManager`, `Init__15VIWindowManagerP5VIPadP10VIKeyboard`,
`SetRoot__15VIWindowManagerP5VIWnd`, `Update__15VIWindowManager`, `_$_15VIWindowManager`,
`__15VIWindowManager`, `SetAppPtr__5VIWndPv`, `SetFocus__5VIWndP5VIWndb`, `Free__9VIWndDnas`,
`Init__9VIWndDnasP7VIScene`, `_$_9VIWndDnas`, `__9VIWndDnas`, `Free__9VIWndEula`,
`Init__9VIWndEula`, `_$_9VIWndEula`, `__9VIWndEula`, `_$_20VIWndGenericRenderer`,
`__20VIWndGenericRenderer`, `Free__10VIWndLegal`, `Init__10VIWndLegal`, `_$_10VIWndLegal`,
`__10VIWndLegal`, `Free__12VIWndPatcher`, `Init__12VIWndPatcher`, `_$_12VIWndPatcher`,
`__12VIWndPatcher`, `Free__11VIWndSplash`, `Init__11VIWndSplashP7VIScene`,
`RestartConnect__11VIWndSplash`, `_$_11VIWndSplash`, `__11VIWndSplash`.

## Imports opened (referenced here, defined elsewhere or by nobody yet)

Window and UI classes: `__5VIWnd`, `_$_5VIWnd`, `AddChild__5VIWndP5VIWnd`,
`RemoveChild__5VIWndP5VIWnd`, `__10VIUIWindow`, `_$_10VIUIWindow`, `__12VIWndMessage`,
`_$_12VIWndMessage`, `Init__12VIWndMessage`, `Free__12VIWndMessage`, `__12VIWndConnect`,
`_$_12VIWndConnect`, `Init__12VIWndConnect`, `Free__12VIWndConnect`, `EndConnect__12VIWndConnect`,
`__12VIWndOptions`, `_$_12VIWndOptions`, `Init__12VIWndOptionsP7VIScene`, `Free__12VIWndOptions`,
`__16VIWndReadMessage`, `_$_16VIWndReadMessage`, `Init__16VIWndReadMessage`, `__9VIWndEdit`,
`_$_9VIWndEdit`, `OnMessage__10VIWndMcErrP5VIWndiii`, `__13VIStringTable`, `_$_13VIStringTable`,
`__6VIFont`, `__16VIRealmInterfaceP20VIWndGenericRendererPCc`, `_$_16VIRealmInterface`,
`__19VIFlatFileStructure`, `_$_19VIFlatFileStructure`, `LoadSRSServer__Fv`,
`__t6VIList1ZP9VIGUIPage`, `_$_t6VIList1ZP9VIGUIPage`, `Begin__Ct6VIList1ZP9VIGUIPage`,
`End__Ct6VIList1ZP9VIGUIPage`, `Next__Ct6VIList1ZP9VIGUIPagei`,
`Erase__t6VIList1ZP9VIGUIPagei`, `Read__10VIKeyboardR12VIConsoleKey`,
`Read__5VIPadR12VIConsolePad`.
Runtime and data: `__builtin_delete`, `__builtin_vec_new`, `__builtin_vec_delete`, `memset`,
`scePrintf`, the focus word `_5VIWnd$mFocus` (0x01192c74), `sWndMcErr` (0x01190a20, 0x210 bytes),
`mcErrOn` (0x01190c30) and the seven class tables `_vt$10VIWndLegal`, `_vt$11VIWndSplash`,
`_vt$12VIWndPatcher`, `_vt$15VIWindowManager`, `_vt$20VIWndGenericRenderer`, `_vt$9VIWndDnas`,
`_vt$9VIWndEula`. Many of these are other packages' symbols or may already be defined in the
shared link; compare against `unresolved-imports.json` after integration.

## Build-plan entries needed

Profile `authored_cpp`, empty header closure, kind `independently_authored_source`, one entry per
line below (object name, source, SHA-256), appended in this order. `snapshot.json` must also
record each source. `host_check.cpp` files are not plan inputs. Neither shared file is edited here.

- `dnas-ctor.o`, `sources/w10-vi-c-dnas-ctor/ctor.cpp`, SHA-256 `e590f067a0041aaa016f75ca8c98c43961bee5683c305d233b2a2404f2559b44`
- `dnas-dtor.o`, `sources/w10-vi-c-dnas-dtor/dtor.cpp`, SHA-256 `005d8d353bdab6c3332aa6b1a7b8daf7531015239fef3ad7edd1b91d7ba73e13`
- `dnas-free.o`, `sources/w10-vi-c-dnas-free/free.cpp`, SHA-256 `e3cac0764257600f1bcb79242759679301b3ac8a9b2b7764526485810d98b324`
- `dnas-init.o`, `sources/w10-vi-c-dnas-init/init.cpp`, SHA-256 `70c2fa584a693024967d2eb1d08ba61de0c8ce6149b2527fe280edad662b6bc0`
- `eula-ctor.o`, `sources/w10-vi-c-eula-ctor/ctor.cpp`, SHA-256 `4a8a58dee5c5bba6be1f2c6fabb333f6774739996f2846c0907b9038409ed92f`
- `eula-dtor.o`, `sources/w10-vi-c-eula-dtor/dtor.cpp`, SHA-256 `ad6ddf15da175e234091b36a0f18f2ee7fd0feed0d15b38d30c460280828b9ad`
- `eula-free.o`, `sources/w10-vi-c-eula-free/free.cpp`, SHA-256 `c75eb39231bceea2accfed290f61a46b8bfa5020949eb5564b4f133fd9250c1c`
- `eula-init.o`, `sources/w10-vi-c-eula-init/init.cpp`, SHA-256 `e27b7f5f796a55085d4a7b063e55d9bd324d9f72bf28f6aba861622c0d1b455d`
- `genrenderer-ctor.o`, `sources/w10-vi-c-genrenderer-ctor/ctor.cpp`, SHA-256 `4c03c673902804d93d08b97e2aea221ec9f339047996ae3cda76ea2e8eada262`
- `genrenderer-dtor.o`, `sources/w10-vi-c-genrenderer-dtor/dtor.cpp`, SHA-256 `257536535c44be353e55a28424c42d8af6ea4c0a91629e5c936fdaa05497fe13`
- `legal-ctor.o`, `sources/w10-vi-c-legal-ctor/ctor.cpp`, SHA-256 `ff89c493f5866d841ab6a375fbab6015ed745b1d951e9ddb022025ece098c261`
- `legal-dtor.o`, `sources/w10-vi-c-legal-dtor/dtor.cpp`, SHA-256 `da1e622cfee94571834b44e452db1d70fb09a601a299e0e83796046b5ff37980`
- `legal-free.o`, `sources/w10-vi-c-legal-free/free.cpp`, SHA-256 `5826142cb5016115f24266e8931863ccaf6f062c71e224c740602adff9ac86b0`
- `legal-init.o`, `sources/w10-vi-c-legal-init/init.cpp`, SHA-256 `449d552d9076a0a60c95804c071790d39992d6946968fc3061d3668445dd7cc5`
- `patcher-ctor.o`, `sources/w10-vi-c-patcher-ctor/ctor.cpp`, SHA-256 `ef10977fd3ddabbe65284e3145dd9f719830a1f6eccbf12284536054ec25d7f7`
- `patcher-dtor.o`, `sources/w10-vi-c-patcher-dtor/dtor.cpp`, SHA-256 `74f43ab603c723a3741f973fb0089e3da8bb7b4174b9a4e85b2b1dde36da9979`
- `patcher-free.o`, `sources/w10-vi-c-patcher-free/free.cpp`, SHA-256 `f3c19e7e3c92fd5f12ef5dfed988f35b4162e20c95546d796e200ed8d34aadc3`
- `patcher-init.o`, `sources/w10-vi-c-patcher-init/init.cpp`, SHA-256 `807e01ba0b425b106300c405c78ba3e63ce5dc7e5771932e4d4cad32e4074917`
- `splash-ctor.o`, `sources/w10-vi-c-splash-ctor/ctor.cpp`, SHA-256 `429a4d513651657dc9f36f3b594d07e27482d10993ebd39d25420003f611401f`
- `splash-dtor.o`, `sources/w10-vi-c-splash-dtor/dtor.cpp`, SHA-256 `8e57864e9186e63db3756e024d74cf1f25828a97da989604ebaec44e03e92c58`
- `splash-free.o`, `sources/w10-vi-c-splash-free/free.cpp`, SHA-256 `f540adf16c0dc10dab5c34fcd5634b88e80f3fd9145f563c1d147610d4d0fbe9`
- `splash-init.o`, `sources/w10-vi-c-splash-init/init.cpp`, SHA-256 `6414d9653ea7e963b6abfcec7ef99a092c13173d644008e970f88f857892afe5`
- `splash-restart.o`, `sources/w10-vi-c-splash-restart/restart.cpp`, SHA-256 `7ef993ca59798cf812975f90fba6a119371dc56a4e3df0cb94a6a2ca6ac1e5b0`
- `viui-ctor.o`, `sources/w10-vi-c-viui-ctor/ctor.cpp`, SHA-256 `b4e430ee56480eeb01516e891cd4d09a32f23a30dee9ff65c39cdaf652672535`
- `viui-dtor.o`, `sources/w10-vi-c-viui-dtor/dtor.cpp`, SHA-256 `d145261eb4d2fcb1c80e9f951cf0b64e83db6f353ddab7bbd2140e361e1b4113`
- `wm-ctor.o`, `sources/w10-vi-c-wm-ctor/ctor.cpp`, SHA-256 `43840dfc0e9114ba86a3f69579ffb4054638d26ab66350b8a07b8ceeb103a3f6`
- `wm-dtor.o`, `sources/w10-vi-c-wm-dtor/dtor.cpp`, SHA-256 `00005f2e8b7087a052785f2d345bea9b4d0f08a94ad1fc40d1974371eac7b72b`
- `wm-free.o`, `sources/w10-vi-c-wm-free/free.cpp`, SHA-256 `b9062461e6b4a0625ca7ac49cc128d4e12ae22d320ee9bc52111a847bc00ba7e`
- `wm-init.o`, `sources/w10-vi-c-wm-init/init.cpp`, SHA-256 `c68f3525fa18d27581666422fcb2c6b56cee2f67af7a882e3a79ffdf3b4fd1f5`
- `wm-setapp.o`, `sources/w10-vi-c-wm-setapp/setapp.cpp`, SHA-256 `c88a334dfa734740b57947f69dd5b7b1f17ac9b4d98f3c23776084c5250f1e3e`
- `wm-setroot.o`, `sources/w10-vi-c-wm-setroot/setroot.cpp`, SHA-256 `cf36725fc656ab6d508c0740121aed2ece589a789cd775c9828984450bcd8df0`
- `wm-update.o`, `sources/w10-vi-c-wm-update/update.cpp`, SHA-256 `697c2a0591cb897471e300cff2a1cf0856fbf477fe24c46f8ddf80bc64b1e89e`
- `wnd-setfocus.o`, `sources/w10-vi-c-wnd-setfocus/setfocus.cpp`, SHA-256 `f3f0159e340aecbb38ee434ce57a37dd5da6ca8270c7ed11cd244d5403e7b92f`

## Authored but never executed

All 33 functions. They have only run in the private interpreter against logging stubs and in host
checks with substituted callees. None has run on the EE or as part of gameplay, and no object
class here has been constructed by a real caller. Virtual calls through table entries
(`SetFocus`, `Update`, `_$_20VIWndGenericRenderer`) were run against fabricated tables.

## Unknown

- The sizes and field layouts of every class here beyond the offsets used. Offsets observed for
  `VIWnd`: +24 application word, +28 a child word that the parents clear, +32 table word, +36
  root/scene word; the embedded sub-objects of each window class are at the offsets named in
  the source comments, but what they are used for is not established.
- The element size and node layout of `VIList<VIGUIPage*>` (the destructor only reads the first
  word of the element at an iterator, and iterators are passed as integers).
- What message number 295 means, and the type and purpose of `sWndMcErr` and `mcErrOn`.
- The layout of `VIConsoleKey` beyond bytes 0 and 1 and of `VIConsolePad` beyond a leading word
  and 20 floats (84 bytes in all).
- Why `Init__9VIWndEula` allocates 0x10000 bytes and frees them immediately; what word 1548 means.
- The return type of `LoadSRSServer__Fv` (taken as a C string from the constructor name).
- Whether `__6VIFont` has no destructor (the `__4VIUI` destructor makes no call for it).
- `Init__10VIWndLegal` ignores everything and returns 0; whether the original signature has
  further parameters is not visible.
