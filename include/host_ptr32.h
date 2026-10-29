#ifndef HOST_PTR32_H
#define HOST_PTR32_H

// A pointer in a 32-bit slot of the DS's data formats: the sound files' links (a bank's wave archives, a wave
// archive's banks, the address of a wave loaded on its own), which keep their file layout, so the slot stays 4 bytes.
// A pointer below HOST_PTR32_HANDLE is stored as itself (every pointer on a 32-bit host; DS memory); one above is
// stored as a handle >= HOST_PTR32_HANDLE: its index in a table of the pointers stored this way (modern/host/ptr32.c;
// each distinct pointer once, never released). Values below 0x02000000 in these slots are file offsets, as on the DS.
// Self-contained (no includes): the game's units with their own basic types include it too.
#if __SIZEOF_POINTER__ == 8
#define HOST_PTR32_HANDLE 0xf0000000u
#ifdef __cplusplus
extern "C" {
#endif
unsigned int HostPtr32_Handle(const void *p);  // (modern/host/ptr32.c)
void *HostPtr32_Lookup(unsigned int handle);
unsigned int HostPtr32_Export(const void *const **ptrs);           // (the save states: modern/host/savestate.c)
void HostPtr32_Import(const void *const *ptrs, unsigned int n);
#ifdef __cplusplus
}
#endif
static inline unsigned int HostPtr32_Of(const void *p) {
    __UINTPTR_TYPE__ a = (__UINTPTR_TYPE__)p;
    return a < HOST_PTR32_HANDLE ? (unsigned int)a : HostPtr32_Handle(p);
}
static inline void *HostPtr32_Get(unsigned int v) {
    return v < HOST_PTR32_HANDLE ? (void *)(__UINTPTR_TYPE__)v : HostPtr32_Lookup(v);
}
#else
static inline unsigned int HostPtr32_Of(const void *p) { return (unsigned int)(__UINTPTR_TYPE__)p; }
static inline void *HostPtr32_Get(unsigned int v) { return (void *)(__UINTPTR_TYPE__)v; }
#endif

#endif
