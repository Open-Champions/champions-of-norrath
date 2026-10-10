# Historical integer-leaf compiler

This is a build recipe and host compatibility patch for public GNU GCC 2.95.2.
It is a restricted comparison tool, not the original game compiler or a complete
PS2 SDK. The generic MIPS3/o64 output is deliberately distinguished from EE/EABI
output. It supplies no target libraries, game link command or runtime proof.

`profile.json` defines the fixed relative layout used by the verifier. The
configured prefix is embedded in the driver, so a compiler moved from another
checkout must be rebuilt at the current root. `build.json` records the source,
patch and configuration; successful local compilation does not promise identical
host binaries on other hosts.

## Build on the tested macOS arm64 host

Use a fresh source and build directory. Existing Apple developer tools must
provide Clang and Make. This recipe does not install anything globally. Run from
the repository root, after building the separately documented public EE binutils
assembler.

```sh
NORRATH_ROOT="$PWD"
NORRATH_SOURCE="$NORRATH_ROOT/work/toolchains/gcc-2.95.2"
NORRATH_BUILD="$NORRATH_ROOT/work/toolchains/build-gcc-2.95.2"
NORRATH_PREFIX="$NORRATH_ROOT/work/toolchains/gcc-2.95.2-leaf-install"
git clone --depth 1 --branch releases/gcc-2.95.2 https://github.com/gcc-mirror/gcc "$NORRATH_SOURCE"
test "$(git -C "$NORRATH_SOURCE" rev-parse HEAD)" = 17b916df3d859b72736d537f6c2a98b2d76decf8
git -C "$NORRATH_SOURCE" apply "$NORRATH_ROOT/toolchains/gcc-2.95.2/host.patch"
cp "$NORRATH_ROOT/toolchains/gcc-2.95.2/xm-aarch64-darwin.h" "$NORRATH_SOURCE/gcc/config/xm-aarch64-darwin.h"
mkdir -p "$NORRATH_BUILD"
cd "$NORRATH_BUILD"
CC='clang -std=gnu89 -Wno-error=implicit-function-declaration' CFLAGS='-O0 -g' \
  "$NORRATH_SOURCE/configure" --build=aarch64-apple-darwin --host=aarch64-apple-darwin \
  --target=mips64el-unknown-elf --prefix="$NORRATH_PREFIX" --enable-languages=c,c++ \
  --disable-nls --without-headers --with-gnu-as --with-gnu-ld
make -j4 all-libiberty
make -C gcc -j4 'CC=clang -std=gnu89 -Wno-error=implicit-function-declaration -Wno-error=int-conversion' cc1 cc1plus xgcc cpp
```

Configuration may create private include-directory placeholders under the prefix;
no compiler installation step is required. Keep the source and build trees
private under ignored `work/`. Do not put custom specs, wrappers or shadow
frontends into this layout. The verifier checks compiler components, configured
search paths, external specs absence and fresh type assertions before accepting
an isolated function comparison.

## Host changes

The patch accepts the Apple arm64 build host, supplies its integer sizes, fixes
two obsolete host C declarations/expressions, and makes generated instruction
function pointers use the host's fixed-argument calling convention. No MIPS
instruction patterns, scheduling tables or target ABI definitions are changed.
The old compiler does not support modern `-fwrapv`; each candidate must have
defined source semantics within its explicitly reviewed domain.

## Licenses

The patch modifies GNU GCC sources and is distributed under GPL-2.0-or-later,
consistent with the upstream files. The upstream license is included as
`COPYING`; original copyright notices remain in the source to which the patch
applies. The independently authored `xm-aarch64-darwin.h`, build metadata and
documentation use this repository's MIT license. No compiler binary, proprietary
SDK or game material is included here.
