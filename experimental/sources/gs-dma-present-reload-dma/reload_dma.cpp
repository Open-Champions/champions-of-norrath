/* Observed reloadDMA__Fi at 0x0015ed18, 2488 bytes in the pinned image. It
 * always returns 1. All computed-address and DMA-register traffic below
 * goes through the norrath_ee_* hooks so host checks can observe it; on the
 * EE they are volatile memory accesses, including the 0x20000000/0x30000000
 * aliased texture-list addresses this function computes.
 *
 * Entry: an uninitialized engine with any cause but -1 runs sync/ei and
 * returns. Cause -2 bumps donesGot.753 when channelDone.747 has bit 1, else
 * clears flushing.755, then stores 2 at 0x12001000. Cause -1 flips
 * readBuffer.745, marks initialized, zeroes the bucket, texTypes.758,
 * sentDone.752, donesGot.753, and donesNeeded.754, points the four texture
 * queues at the draw record head, and sets channelDone.747 to 0 (2 with an
 * empty head and channelBusy.746 cleared).
 *
 * When channelDone.747 is 2 with matching done counters, bucket table slots
 * are scanned for a new texture head (donesNeeded.754 cleared, queue words
 * updated, channelDone.747 set to 0 for a head and left 2 without one),
 * locking each head's texture and skipping unlocked nodes, before the drain
 * below. Any other channelDone.747 value, a counter mismatch, or a bucket
 * past 7 goes straight to the drain.
 *
 * The drain masks channelBusy.746 with ~(1u << (cause & 31)). Bit 1 keeps
 * the tex-side counter from 0x1000a040 (or 0x1000a030 selected by
 * texTypes.758) and walks drTexLoaded.749 forward while preTex.756 trails
 * it. Bit 0 of the combined busy/done words keeps the vif-side counter
 * from 0x10009040 and walks drDone.751 toward drDraw.750, unlocking
 * textures. Cause 1 then sweeps drDone.751 to drDraw.750 unlocking, cause 2
 * repoints drTexLoaded.749 at drTexLoad.748, and a set flushing.755 runs
 * sync/ei and returns.
 *
 * Otherwise texture queues are uploaded: per-texture GIF tags are built at
 * the computed texDmaList address (lock/cacheIn paths selected by the
 * texture halfwords, s3 budget against drawSizeOverEnable), closed with a
 * 0x70000000 tag, the masked base stored at 0x1000a030, 261 kicked at
 * 0x1000a000, preTex.756 cleared, the tex list flipped, and channelBusy.746
 * bit 2 set. When bit 0 of the busy/done words is set, the vif queue is
 * drained the same way (0x614240 kicked at 0x10009030 on an empty queue,
 * per-node tags with a read-modify-write at texture+64, 0x70000000 close,
 * 325 kicked at 0x1000a000, preVif.757 cleared, vif list flipped,
 * channelBusy.746 bit 1 set).
 *
 * The tail returns when the bucket is below 8, channelBusy.746 is set, or
 * sentDone.752 is set. Otherwise sentDone.752 is set, initialized.744 is
 * cleared, and cause -1 runs EIntr/SignalSema/DIntr while any other cause
 * runs iSignalSema, all on endDrawSemaphore. Then sync/ei and return 1.
 *
 * Texture words at offsets 8/16/20 select lock/cacheIn/small handling;
 * offsets 6, 8, 24 are halfwords; offset 12 chains nodes. Dead stack slots,
 * the provably dead null-halfword branch, and the per-node tag accumulator
 * (overwritten by the batch counter before any use; only its side-effect
 * stores and the packet words are modeled) are omitted. drawHead,
 * endDrawSemaphore, the three texture callees, EIntr/DIntr, and both
 * semaphore calls stay imports. Fresh compilation is not an original-byte
 * match. Hardware DMA effects, texture heap layout, and valid counter
 * ranges are unknown.
 */
struct ReloadTexture {
    unsigned w[7];
};

extern "C" unsigned char initialized_744 asm("initialized.744") = 0;
extern "C" unsigned char readBuffer_745 asm("readBuffer.745") = 0;
extern "C" unsigned channelBusy_746 asm("channelBusy.746") = 0;
extern "C" unsigned channelDone_747 asm("channelDone.747") = 0;
extern "C" unsigned drTexLoad_748 asm("drTexLoad.748") = 0;
extern "C" unsigned drTexLoaded_749 asm("drTexLoaded.749") = 0;
extern "C" unsigned drDraw_750 asm("drDraw.750") = 0;
extern "C" unsigned drDone_751 asm("drDone.751") = 0;
extern "C" unsigned char sentDone_752 asm("sentDone.752") = 0;
extern "C" signed char donesGot_753 asm("donesGot.753") = 0;
extern "C" signed char donesNeeded_754 asm("donesNeeded.754") = 0;
extern "C" signed char flushing_755 asm("flushing.755") = 0;
extern "C" signed char preTex_756 asm("preTex.756") = 0;
extern "C" signed char preVif_757 asm("preVif.757") = 0;
extern "C" unsigned char texTypes_758 asm("texTypes.758") = 0;
extern "C" int drawSizeOverEnable = 1;
extern "C" signed char bucket = 0;
extern "C" unsigned texDmaListFlip = 0;
extern "C" unsigned vifDmaListFlip = 0;
extern "C" unsigned texDmaListPos = 0;
extern "C" unsigned vifDmaListPos = 0;

extern "C" unsigned char drawHead[];
extern "C" int endDrawSemaphore;
extern "C" void textureLock__FP8_texture(ReloadTexture* texture);
extern "C" void textureUnlock__FP8_texture(ReloadTexture* texture);
extern "C" void textureCacheIn__FP8_texture(ReloadTexture* texture);
extern "C" int EIntr();
extern "C" int DIntr();
extern "C" void SignalSema(int identifier);
extern "C" void iSignalSema(int identifier);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" ReloadTexture* norrath_resolve_texture(unsigned id);
extern "C" unsigned norrath_ee_load32(unsigned address);
extern "C" unsigned long long norrath_ee_load64(unsigned address);
extern "C" void norrath_ee_store32(unsigned address, unsigned value);
extern "C" void norrath_ee_store64(unsigned address, unsigned long long value);
extern "C" void norrath_ee_store16(unsigned address, unsigned value);
extern "C" void norrath_host_pipeline_sync();
extern "C" void norrath_host_enable_interrupts();
#define NORRATH_RD norrath_resolve_texture
#define NORRATH_LD32 norrath_ee_load32
#define NORRATH_LD64 norrath_ee_load64
#define NORRATH_ST32 norrath_ee_store32
#define NORRATH_ST64 norrath_ee_store64
#define NORRATH_ST16 norrath_ee_store16
#define NORRATH_SYNC() norrath_host_pipeline_sync()
#define NORRATH_EI() norrath_host_enable_interrupts()
#else
static inline ReloadTexture* norrath_resolve_texture(unsigned id) {
    return reinterpret_cast<ReloadTexture*>(static_cast<unsigned long>(id));
}
static inline unsigned norrath_ee_load32(unsigned address) {
    return *reinterpret_cast<volatile unsigned*>(static_cast<unsigned long>(address));
}
static inline unsigned long long norrath_ee_load64(unsigned address) {
    return *reinterpret_cast<volatile unsigned long long*>(static_cast<unsigned long>(address));
}
static inline void norrath_ee_store32(unsigned address, unsigned value) {
    *reinterpret_cast<volatile unsigned*>(static_cast<unsigned long>(address)) = value;
}
static inline void norrath_ee_store64(unsigned address, unsigned long long value) {
    *reinterpret_cast<volatile unsigned long long*>(static_cast<unsigned long>(address)) = value;
}
static inline void norrath_ee_store16(unsigned address, unsigned value) {
    *reinterpret_cast<volatile unsigned short*>(static_cast<unsigned long>(address)) =
        static_cast<unsigned short>(value);
}
static inline void norrath_pipeline_sync() {
    asm volatile("sync" ::: "memory");
}
static inline void norrath_enable_interrupts() {
    asm volatile("ei" ::: "memory");
}
#define NORRATH_RD norrath_resolve_texture
#define NORRATH_LD32 norrath_ee_load32
#define NORRATH_LD64 norrath_ee_load64
#define NORRATH_ST32 norrath_ee_store32
#define NORRATH_ST64 norrath_ee_store64
#define NORRATH_ST16 norrath_ee_store16
#define NORRATH_SYNC() norrath_pipeline_sync()
#define NORRATH_EI() norrath_enable_interrupts()
#endif

static unsigned load_word_le(const unsigned char* address) {
    return static_cast<unsigned>(address[0]) | (static_cast<unsigned>(address[1]) << 8) |
           (static_cast<unsigned>(address[2]) << 16) | (static_cast<unsigned>(address[3]) << 24);
}

static unsigned tex_half_u(ReloadTexture* texture, unsigned offset) {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(texture->w) + offset;
    return static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8);
}

static int tex_half_s(ReloadTexture* texture, unsigned offset) {
    const unsigned halves = tex_half_u(texture, offset);
    return (halves & 0x8000u) != 0u ? static_cast<int>(halves | 0xffff0000u)
                                    : static_cast<int>(halves);
}

static unsigned dma_count(unsigned reg, unsigned flip, unsigned base) {
    const unsigned counter = NORRATH_LD32(reg);
    const unsigned want = (flip ^ 1u) << 11;
    const int shifted = static_cast<int>(counter - want - base) >> 4;
    return static_cast<unsigned>(shifted) - 1u;
}

extern "C" int reloadDMA__Fi(int cause) {
    if (initialized_744 == 0 && cause != -1) {
        NORRATH_SYNC();
        NORRATH_EI();
        return 1;
    }
    if (cause == -2) {
        if ((channelDone_747 & 2u) != 0u) {
            donesGot_753 =
                static_cast<signed char>(static_cast<unsigned char>(donesGot_753) + 1u);
        } else {
            flushing_755 = 0;
        }
        NORRATH_ST64(0x12001000u, 2ull);
    }
    if (cause == -1) {
        const unsigned next_read = static_cast<unsigned>(readBuffer_745) ^ 1u;
        initialized_744 = 1;
        bucket = 0;
        readBuffer_745 = static_cast<unsigned char>(next_read);
        unsigned head = load_word_le(drawHead + next_read * 32u);
        head = NORRATH_LD32(head + 12u);
        donesNeeded_754 = 0;
        texTypes_758 = 0;
        drDone_751 = head;
        drDraw_750 = head;
        drTexLoad_748 = head;
        drTexLoaded_749 = head;
        sentDone_752 = 0;
        if (head != 0u) {
            channelDone_747 = 0;
        } else {
            channelBusy_746 = 0;
            channelDone_747 = 2;
        }
        donesGot_753 = 0;
    }
    if (channelDone_747 == 2u && donesGot_753 == donesNeeded_754) {
        for (;;) {
            const unsigned next_bucket =
                (static_cast<unsigned>(static_cast<unsigned char>(bucket)) + 1u) & 0xffu;
            bucket = static_cast<signed char>(static_cast<unsigned char>(next_bucket));
            if (static_cast<signed char>(static_cast<unsigned char>(next_bucket)) >= 8)
                goto phase_d;
            const int slot = static_cast<int>(readBuffer_745) * 8 +
                             static_cast<int>(static_cast<signed char>(
                                 static_cast<unsigned char>(next_bucket)));
            unsigned head = load_word_le(drawHead + static_cast<unsigned>(slot) * 4u);
            head = NORRATH_LD32(head + 12u);
            donesNeeded_754 = 0;
            unsigned select = 2u;
            if (head != 0u) select = 0u;
            drDone_751 = head;
            drDraw_750 = head;
            channelDone_747 = select;
            drTexLoad_748 = head;
            donesGot_753 = 0;
            unsigned found = 0;
            if (head != 0u) {
                ReloadTexture* first = NORRATH_RD(head);
                const unsigned first_lock = first->w[2];
                drTexLoaded_749 = head;
                if (first_lock != 0u && tex_half_s(NORRATH_RD(first_lock), 24) != 0) {
                    unsigned node = head;
                    bool lead = true;
                    for (;;) {
                        if (!lead) {
                            node = NORRATH_RD(node)->w[3];
                            for (;;) {
                                drTexLoad_748 = node;
                                if (node == 0u) break;
                                if (NORRATH_RD(node)->w[2] != 0u) break;
                                node = NORRATH_RD(node)->w[3];
                            }
                            if (node == 0u) break;
                            if (tex_half_s(NORRATH_RD(NORRATH_RD(node)->w[2]), 24) == 0)
                                break;
                        }
                        lead = false;
                        textureLock__FP8_texture(NORRATH_RD(NORRATH_RD(node)->w[2]));
                        node = drTexLoad_748;
                    }
                    found = node;
                } else {
                    found = head;
                }
            }
            drTexLoaded_749 = found;
            if (channelDone_747 != 2u) goto phase_d;
            if (donesGot_753 != donesNeeded_754) goto phase_d;
        }
    }
phase_d:;
    unsigned busy = channelBusy_746 & ~(1u << (static_cast<unsigned>(cause) & 31u));
    channelBusy_746 = busy;
    if ((busy & 4u) != 0u) {
        unsigned count;
        if ((texTypes_758 & 2u) == 0u) {
            count = dma_count(0x1000a040u, texDmaListFlip, 0x6142c0u);
        } else {
            count = 0u;
        }
        if (((texTypes_758 ^ 1u) & 1u) != 0u) {
            count = dma_count(0x1000a030u, texDmaListFlip, 0x6142c0u);
        }
        if ((count - 1u) < 15u) {
            const unsigned trailing =
                count - static_cast<unsigned>(static_cast<int>(preTex_756));
            preTex_756 = static_cast<signed char>(static_cast<unsigned char>(count));
            if (static_cast<int>(trailing) > 0) {
                unsigned node = drTexLoaded_749;
                for (unsigned i = 0; i < trailing; ++i)
                    node = NORRATH_RD(node)->w[3];
                drTexLoaded_749 = node;
            }
        }
    }
    if ((busy & 2u) == 0u) goto vif_section;
    {
        const int raw = static_cast<int>(NORRATH_LD32(0x10009040u) -
                                         ((vifDmaListFlip ^ 1u) << 11) - 0x6152c0u);
        const unsigned vif_count = static_cast<unsigned>(raw >> 4) - 1u;
        if ((vif_count - 1u) < 15u) {
            int remaining = static_cast<int>(vif_count -
                                             static_cast<unsigned>(static_cast<int>(preVif_757)));
            unsigned cursor = drDone_751;
            for (;;) {
                unsigned stop = drDraw_750;
                if (cursor == stop) break;
                if (remaining <= 0) break;
                const unsigned locked = NORRATH_RD(cursor)->w[2];
                if (locked != 0u) {
                    textureUnlock__FP8_texture(NORRATH_RD(locked));
                    cursor = drDone_751;
                    stop = drDraw_750;
                }
                cursor = NORRATH_RD(cursor)->w[3];
                --remaining;
                drDone_751 = cursor;
            }
            preVif_757 = static_cast<signed char>(static_cast<unsigned char>(vif_count));
        }
    }
vif_section:;
    if (cause == 1) {
        unsigned cursor = drDone_751;
        for (;;) {
            const unsigned stop = drDraw_750;
            if (cursor == stop) break;
            const unsigned locked = NORRATH_RD(cursor)->w[2];
            if (locked != 0u) {
                textureUnlock__FP8_texture(NORRATH_RD(locked));
                cursor = drDone_751;
            }
            cursor = NORRATH_RD(cursor)->w[3];
            drDone_751 = cursor;
        }
    }
    if (cause == 2) drTexLoaded_749 = drTexLoad_748;
    if (flushing_755 == 0) goto tex_upload;
    NORRATH_SYNC();
    NORRATH_EI();
    return 1;
tex_upload:;
    busy = channelBusy_746;
    if ((busy & 4u) != 0u) goto gif_check;
    {
        unsigned s3 = 0;
        unsigned s2 = 0;
        unsigned base = (((texDmaListFlip << 8) + texDmaListPos) + drTexLoad_748) << 3;
        base += 0x6142c0u;
        const unsigned list = base | 0x30000000u;
        texTypes_758 = 0;
        unsigned cursor = list;
        unsigned node = drTexLoad_748;
        if (node != 0u) {
            for (;;) {
                ReloadTexture* current = NORRATH_RD(node);
                const unsigned waiter = current->w[2];
                ReloadTexture* target = nullptr;
                if (waiter == 0u) {
                    if (cursor == list)
                        drTexLoaded_749 = current->w[3];
                    unsigned next = current->w[3];
                    ++s2;
                    drTexLoad_748 = next;
                    node = next;
                    if (node == 0u) goto chain_done;
                    continue;
                } else {
                    target = NORRATH_RD(waiter);
                    if (tex_half_s(target, 24) != 0) {
                        textureLock__FP8_texture(target);
                        node = drTexLoad_748;
                        if (cursor != list) {
                            unsigned next = NORRATH_RD(node)->w[3];
                            ++s2;
                            drTexLoad_748 = next;
                            node = next;
                            if (node == 0u) goto chain_done;
                            continue;
                        }
                        drTexLoaded_749 = NORRATH_RD(node)->w[3];
                        unsigned next = NORRATH_RD(node)->w[3];
                        ++s2;
                        drTexLoad_748 = next;
                        node = next;
                        if (node == 0u) goto chain_done;
                        continue;
                    }
                    textureCacheIn__FP8_texture(target);
                    const unsigned backing = target->w[5];
                    if (backing == 0u) {
                        const unsigned table = target->w[4];
                        const unsigned aliased = table | 0x20000000u;
                        NORRATH_ST16(aliased + 20u, tex_half_u(target, 24));
                        const unsigned mode = tex_half_u(target, 8);
                        if ((mode & 4u) != 0u) {
                            const unsigned bumped =
                                (tex_half_u(target, 24) + 4u) & 0xffffu;
                            NORRATH_ST16(aliased + 1140u, bumped);
                            NORRATH_ST16(aliased + 180u, bumped);
                        } else if ((mode & 0x2000u) != 0u) {
                            const unsigned bumped =
                                (tex_half_u(target, 24) + 4u) & 0xffffu;
                            NORRATH_ST16(aliased + 180u, bumped);
                        }
                        const unsigned short_id = tex_half_u(target, 6);
                        const long long tag = static_cast<int>(
                            table | short_id | (s2 << 16) | 0x30000000u);
                        NORRATH_ST64(cursor + 8u, 0ull);
                        NORRATH_ST64(cursor, static_cast<unsigned long long>(tag));
                        cursor += 16;
                        texTypes_758 |= 2u;
                    } else {
                        const unsigned aliased = backing | 0x20000000u;
                        const unsigned period = tex_half_u(target, 24);
                        NORRATH_ST16(aliased + 36u, period);
                        const long long tag = static_cast<int>(
                            backing | (s2 << 16) | 0x50000000u);
                        NORRATH_ST64(cursor, static_cast<unsigned long long>(tag));
                        NORRATH_ST64(cursor + 8u, 0ull);
                        cursor += 16;
                        const unsigned stepped = (period + 4u) & 0xffffu;
                        NORRATH_ST16(aliased + 1156u, stepped);
                        texTypes_758 |= 1u;
                    }
                }
                s3 += tex_half_u(target, 4);
                node = drTexLoad_748;
                if (drawSizeOverEnable != 0 && static_cast<int>(s3) >= 1001) {
                    drTexLoad_748 = NORRATH_RD(node)->w[3];
                    goto chain_done;
                }
                {
                    unsigned next = NORRATH_RD(node)->w[3];
                    ++s2;
                    drTexLoad_748 = next;
                    node = next;
                    if (node == 0u) goto chain_done;
                }
            }
        }
chain_done:;
        if (list != cursor) {
            NORRATH_ST64(cursor, 0x70000000ull);
            NORRATH_ST32(0x1000a030u, list & 0x0fffffffu);
            NORRATH_ST64(cursor + 8u, 0ull);
            cursor += 16;
            NORRATH_SYNC();
            NORRATH_ST32(0x1000a000u, 261u);
            preTex_756 = 0;
            texDmaListFlip ^= 1u;
            channelBusy_746 |= 4u;
        }
    }
gif_check:;
    busy = channelBusy_746;
    {
        const unsigned done = channelDone_747;
        if (((~(busy | done)) & 2u) == 0u) goto tail;
    }
    if (drDone_751 == 0u) {
        donesNeeded_754 =
            static_cast<signed char>(static_cast<unsigned char>(donesNeeded_754) + 1u);
        NORRATH_ST32(0x10009030u, 0x614240u);
        NORRATH_SYNC();
        NORRATH_ST32(0x10009000u, 325u);
        channelDone_747 |= 2u;
        channelBusy_746 |= 2u;
    }
    {
        const unsigned flip = vifDmaListFlip;
        const unsigned pos = vifDmaListPos;
        unsigned vbase = (((flip << 8) + pos) << 3) + 0x6152c0u;
        vbase |= 0x30000000u;
        unsigned cursor = vbase;
        const unsigned first = vbase;
        unsigned node = drDraw_750;
        if (node != drTexLoaded_749) {
            unsigned count = 0;
            for (;;) {
                ReloadTexture* current = NORRATH_RD(node);
                const unsigned waiter = current->w[2];
                unsigned long long word;
                if (waiter == 0u) {
                    word = static_cast<unsigned long long>(current->w[0]);
                } else {
                    ReloadTexture* target = NORRATH_RD(waiter);
                    const unsigned flags = current->w[1];
                    if ((flags & 0x14u) == 0u) {
                        word = static_cast<unsigned long long>(target->w[3]);
                    } else if ((flags & 4u) == 0u) {
                        int bits = tex_half_s(target, 24) + 31;
                        bits &= ~31;
                        const unsigned aliased = current->w[0] | 0x20000000u;
                        bits >>= 5;
                        NORRATH_ST16(aliased + 64u,
                                     static_cast<unsigned>(bits) & 0xffffu);
                        word = static_cast<unsigned long long>(static_cast<long long>(bits))
                               << 5;
                    } else {
                        const unsigned source = current->w[0];
                        const int adjust = tex_half_s(target, 24);
                        const unsigned aliased = source | 0x20000000u;
                        unsigned long long kept = NORRATH_LD64(aliased + 64u);
                        kept = static_cast<unsigned long long>(static_cast<long long>(
                            static_cast<int>(static_cast<unsigned>(kept) |
                                             static_cast<unsigned>(adjust))));
                        NORRATH_ST64(aliased + 64u, kept);
                        word = kept;
                    }
                }
                ++count;
                const long long packet = static_cast<int>(
                    static_cast<unsigned>(word) | 0x50000000u);
                NORRATH_ST64(cursor + 8u, 0ull);
                NORRATH_ST64(cursor, static_cast<unsigned long long>(packet));
                cursor += 16;
                const unsigned advanced = NORRATH_RD(drDraw_750)->w[3];
                drDraw_750 = advanced;
                node = advanced;
                if (advanced == drTexLoaded_749) break;
                if (static_cast<int>(count) >= 6) break;
            }
        }
        if (first != cursor) {
            NORRATH_ST64(cursor, 0x70000000ull);
            NORRATH_ST32(0x10009030u, first & 0x0fffffffu);
            NORRATH_ST64(cursor + 8u, 0ull);
            cursor += 16;
            NORRATH_SYNC();
            NORRATH_ST32(0x1000a000u, 325u);
            preVif_757 = 0;
            vifDmaListFlip ^= 1u;
            channelBusy_746 |= 2u;
        }
    }
tail:;
    if (bucket < 8) {
        NORRATH_SYNC();
        NORRATH_EI();
        return 1;
    }
    if (channelBusy_746 != 0u) {
        NORRATH_SYNC();
        NORRATH_EI();
        return 1;
    }
    if (sentDone_752 != 0) {
        NORRATH_SYNC();
        NORRATH_EI();
        return 1;
    }
    sentDone_752 = 1;
    initialized_744 = 0;
    if (cause != -1) {
        iSignalSema(endDrawSemaphore);
    } else {
        EIntr();
        SignalSema(endDrawSemaphore);
        DIntr();
    }
    NORRATH_SYNC();
    NORRATH_EI();
    return 1;
}
