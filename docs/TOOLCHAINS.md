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

## Historical ABI evidence and private comparator

Retained names such as `GetYaw__6Camera`, `__tf8Creature`, `__ti8Creature` and
`_vt$8Creature` fit the old GNU C++ ABI family. Upstream GCC 2.95.2's
[mangler](https://github.com/gcc-mirror/gcc/blob/17b916df3d859b72736d537f6c2a98b2d76decf8/gcc/cp/method.c)
uses the function/class-length form, and its
[RTTI implementation](https://github.com/gcc-mirror/gcc/blob/17b916df3d859b72736d537f6c2a98b2d76decf8/gcc/cp/rtti.c)
uses the `__tf` and `__ti` prefixes. GCC 3.0 introduced a
[new C++ ABI](https://gcc.gnu.org/gcc-3.0/features.html). This supports an ABI-family
inference, not identification of the exact release, vendor patches, or every
translation unit's compiler. Modern compiler leaf matches do not establish that
its class layout, mangling or RTTI can replace the original program's ABI.

The private historical comparator uses upstream GCC 2.95.2 source pinned to
`17b916df3d859b72736d537f6c2a98b2d76decf8`. No validated public 2.95 EE port was
located in the bounded official-source search. The untouched generic
`mips64el-unknown-elf` target rejects explicit `-mabi` switches; it is therefore
only an integer-leaf code-generation comparator. Its compiler frontends now
build on macOS arm64, with host compatibility changes recorded privately and
no MIPS instruction-pattern changes. The compiler and target runtime have not
been installed globally, and no target runtime was built.

One host failure came from the old compiler calling fixed-argument instruction
generators through variadic function pointers. Tracing showed correct outgoing
pointers becoming corrupted incoming arguments on Apple arm64. A host-specific
declaration change matching the generated unprototyped table resolved the crash;
temporary diagnostic instrumentation was removed. This is a bounded portability
repair, not comprehensive validation of the historical compiler.

Compile-time assertions confirmed 32-bit integer, long and pointer types with
the tested options, and a class-method fixture naturally emitted the retained
`GetYaw__6Camera` name. An independent rerun reproduced all twelve complete-body
probes: four functions at three optimization levels. The camera getter and poison
level requirement matched their already-verified eight-byte bodies. The camera
setter remained a same-size mismatch, and byte reversal produced 44 bytes against
the original 40. These probes add no reconstructed-source credit.

The compiler emits generic MIPS3/o64 objects (`0x20002001`), distinct from the
original EE flags (`0x20924001`). Its tested explicit `-mabi=32`, `-mabi=eabi` and
`-fwrapv` options are rejected. This comparator is not an accepted public verifier
profile and must not inherit modern EE assumptions about ABI or signed overflow.

A public older [PS2 backend patch for GCC 3.2.3](https://github.com/ps2dev/ps2toolchain/blob/78deb4d3cf435f6fab76b35dbaab74ce6503a0a4/patches/gcc-3.2.3-PS2.patch)
is another candidate experiment. It provides R5900/EABI support but does not
restore the old C++ ABI. That backend has not yet been built here. Neither
historical candidate is an approved reconstruction compiler profile.
Preserve mismatches and ABI limitations rather than changing object flags or
silently declaring a generic MIPS compiler to be the original PS2 toolchain.
