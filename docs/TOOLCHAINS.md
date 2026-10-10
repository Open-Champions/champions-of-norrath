# Compiler evidence and reproducibility

The original compiler version is unknown. The ELF flags establish an observed
R5900/MIPS3/EABI64 target declaration; they do not identify a compiler release.
Different supported compilers can match individual functions without establishing
the original C++ ABI, startup, linker or runtime environment.

## Clang comparison lane

The first three functions were freshly compared with Clang 20.1.8 using the exact
flags in the function ledger. Their integer leaf behavior does not exercise a
general PS2 ABI. Clang's tested MIPS target does not support the needed R5900/EABI
settings directly; its successful leaf matches remain narrow evidence.

## Public-source EE GCC build

A private, isolated `mips64r5900el-ps2-elf` compiler was built from the public
ps2dev GCC 15.2.0 and binutils 2.45.1 repositories. The [build record](../targets/SLUS-20565/ee-toolchain-build.json)
pins source commits and records configure/build/install arguments. Its paths are
explicit placeholders to resolve in a private workspace. No original SDK or
compiler was obtained or included.

To reproduce the configuration, clone the named source repositories into
`gcc` and `binutils-gdb` under an absolute private `TOOLCHAIN_WORK`, check out the
recorded commits, and create separate `build-gcc` and `build-binutils` directories.
Resolve the existing host GMP, MPFR and MPC prefixes. Run each recorded argument
array in its recorded working directory and host build environment, in order.
Put `TOOLCHAIN_WORK/ee/bin` first on the build PATH before configuring GCC. The
tested host is macOS arm64 with LLVM 20.1.8, the macOS SDK and existing development
dependencies. This is a successful local recipe, not a portable installer or a
claim that another host produces identical compiler binaries.

The build includes compiler frontends, assembler and linker, but deliberately
omits newlib, libgcc, the standard C++ library and other game runtime support.
It therefore supports current freestanding comparison experiments rather than a
complete game build. Manuals were skipped with `MAKEINFO=true`.

The tested EE options explicitly select R5900, EABI, 64-bit general registers,
32-bit integer/long/pointer types, and single-precision floating-point settings.
Do not use the modern compiler's default ABI as a substitute. Private macro and
object-header checks confirmed these declarations; they do not prove all calling
conventions or floating-point behavior agree with the original.

Compiler identity must include the selected driver, C++ frontend, assembler and
specifications. A driver hash alone does not identify the programs it invokes.
Always run a fresh whole-function comparison; a successful compiler build or a
stored receipt is not additional reconstructed-source credit.
