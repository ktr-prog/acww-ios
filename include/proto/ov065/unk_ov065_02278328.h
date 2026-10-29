#ifndef PROTO_PROTO_OV065_UNK_OV065_02278328_H
#define PROTO_PROTO_OV065_UNK_OV065_02278328_H

// Prototypes of the functions defined in src/ov065/unk_ov065_02278328.cpp (generated: tools/host_protos.py)

#include "types.h"
#include "net/SockAddrIn.h"
#include "net/darray.h"
#ifdef __cplusplus
#include "net/hashtable.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
#ifndef PROTO_LOCAL_MD5Print
void MD5Print(u8 *digest, char *out);
#endif
#ifndef PROTO_LOCAL_MD5Digest
void MD5Digest(void *a, s32 b, char *out);
#endif
#ifndef PROTO_LOCAL_mylsearch
u8 *mylsearch(void *key, u8 *base, s32 n, s32 size, ArrayCompareFn cmp);
#endif
#ifndef PROTO_LOCAL_mybsearch
u8 *mybsearch(void *key, u8 *base, s32 n, s32 size, ArrayCompareFn cmp, s32 *found);
#endif
#ifndef PROTO_LOCAL_get_sockaddrin
s32 get_sockaddrin(const char *a, u32 port, SockAddrIn *out);
#endif
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#ifndef PROTO_LOCAL_TableNew
HashImplementation *TableNew(s32 esize, s32 n, TableHashFn hash, ArrayCompareFn cmp, ArrayElementFreeFn dtor);
#endif
#ifndef PROTO_LOCAL_TableNew2
HashImplementation *TableNew2(s32 esize, s32 n, s32 cap, TableHashFn hash, ArrayCompareFn cmp, ArrayElementFreeFn dtor);
#endif
#ifndef PROTO_LOCAL_TableFree
void TableFree(HashImplementation *t);
#endif
#ifndef PROTO_LOCAL_TableCount
s32 TableCount(HashImplementation *t);
#endif
#ifndef PROTO_LOCAL_TableEnter
void TableEnter(HashImplementation *t, void *key);
#endif
#ifndef PROTO_LOCAL_TableRemove
s32 TableRemove(HashImplementation *t, void *key);
#endif
#ifndef PROTO_LOCAL_TableLookup
void *TableLookup(HashImplementation *t, void *key);
#endif
#ifndef PROTO_LOCAL_TableMapSafe
void TableMapSafe(HashImplementation *t, GsArrayMapFn cb, void *arg);
#endif
#ifndef PROTO_LOCAL_TableMapSafe2
void *TableMapSafe2(HashImplementation *t, GsArrayMapFn cb, void *arg);
#endif
#ifndef PROTO_LOCAL_FreeElement
void FreeElement(DArrayImplementation *v, s32 i);
#endif
#ifndef PROTO_LOCAL_ArrayGrow
void ArrayGrow(DArrayImplementation *v);
#endif
#ifndef PROTO_LOCAL_SetElement
void SetElement(DArrayImplementation *v, void *x, s32 i);
#endif
#ifndef PROTO_LOCAL_ArrayNew
DArrayImplementation *ArrayNew(s32 size, s32 cap, ArrayElementFreeFn dtor);
#endif
#ifndef PROTO_LOCAL_ArrayFree
void ArrayFree(DArrayImplementation *v);
#endif
#ifndef PROTO_LOCAL_ArrayLength
s32 ArrayLength(DArrayImplementation *v);
#endif
#ifndef PROTO_LOCAL_ArrayNth
void *ArrayNth(DArrayImplementation *v, s32 i);
#endif
#ifndef PROTO_LOCAL_ArrayAppend
void ArrayAppend(DArrayImplementation *v, void *x);
#endif
#ifndef PROTO_LOCAL_ArrayInsertAt
void ArrayInsertAt(DArrayImplementation *v, void *x, s32 i);
#endif
#ifndef PROTO_LOCAL_ArrayInsertSorted
void ArrayInsertSorted(DArrayImplementation *v, void *key, ArrayCompareFn cmp);
#endif
#ifndef PROTO_LOCAL_ArrayRemoveAt
void ArrayRemoveAt(DArrayImplementation *v, s32 i);
#endif
#ifndef PROTO_LOCAL_ArrayDeleteAt
void ArrayDeleteAt(DArrayImplementation *v, s32 i);
#endif
#ifndef PROTO_LOCAL_ArrayReplaceAt
void ArrayReplaceAt(DArrayImplementation *v, void *x, s32 i);
#endif
#ifndef PROTO_LOCAL_ArraySort
void ArraySort(DArrayImplementation *v, ArrayCompareFn cmp);
#endif
#ifndef PROTO_LOCAL_ArraySearch
s32 ArraySearch(DArrayImplementation *v, void *key, ArrayCompareFn cmp, s32 start, s32 sorted);
#endif
#ifndef PROTO_LOCAL_ArrayMapBackwards
void ArrayMapBackwards(DArrayImplementation *v, GsArrayMapFn cb, void *arg);
#endif
#ifndef PROTO_LOCAL_ArrayMapBackwards2
void *ArrayMapBackwards2(DArrayImplementation *v, GsArrayMapFn cb, void *arg);
#endif
#ifndef PROTO_LOCAL_ArrayClear
void ArrayClear(DArrayImplementation *v);
#endif
}
#endif

#endif
