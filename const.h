#ifndef __CONST_H__
#define __CONST_H__

#define TXN_BATCH 1
#define TXN_SINGLE 0

#define DATA_READY_MASK 0x8000000000000000

#define FALSE 0
#define TRUE 1
#define TXN_COMMIT 0
#define TXN_ABORT 1

#define CACHELINE 64 
#define MAX_CLIENTS 128 // must be power of 2
#ifndef MAX_SERVERS
#define MAX_SERVERS 128 // must be power of 2
#endif

#define YCSB_NFIELDS 1
#define YCSB_FIELD_SZ 8
#define YCSB_REC_SZ (YCSB_NFIELDS * YCSB_FIELD_SZ)

/* commn. buffer constants */
#define ONEWAY_BUFFER_SIZE  (16 * (CACHELINE >> 3)) 
#define BUFFER_FLUSH_COUNT  8

#define MAX_OPS_PER_QUERY 64

#define MAX_TUPLE_TYPES 32

#if defined(SHARED_EVERYTHING) || defined(SHARED_NOTHING)
// by default, disable socket local for shared everything/nothing
#else
//#define ENABLE_SOCKET_LOCAL_TXN 1
#endif

#define LOCK_SUCCESS 0
#define LOCK_ABORT 1
#define LOCK_WAIT 2
#define LOCK_INVALID 3

#define NREMOTE_OPS 2

/** 
 * Hash Table Operations
 */
/* exploit the fact that we have 48-bit address space and use the upper
 * 16-bits to encode operation type and msg tag
 */
#define HASHOP_MASK           0xF000000000000000
#define HASHOP_TID_MASK       0x0FF0000000000000
#define HASHOP_OPID_MASK      0x000F000000000000
#define MAKE_HASH_MSG(tid,opid,key,optype)\
  ((optype) | ((unsigned long)(tid) << 52UL) | ((unsigned long)opid << 48UL) | (key))

#define HASHOP_GET_TID(msg)   (((msg) & HASHOP_TID_MASK) >> 52)
#define HASHOP_GET_OPID(msg)   (((msg) & HASHOP_OPID_MASK) >> 48)
#define HASHOP_GET_VAL(msg)   ((msg) & ~HASHOP_TID_MASK & ~HASHOP_MASK & ~HASHOP_OPID_MASK)

#define HASHOP_LOOKUP         0x1000000000000000 
#define HASHOP_INSERT         0x2000000000000000 
#define HASHOP_UPDATE         0x3000000000000000 
#define HASHOP_RELEASE        0x4000000000000000 
#define HASHOP_PLOCK_ACQUIRE  0x5000000000000000 
#define HASHOP_PLOCK_RELEASE  0x6000000000000000 

#define INSERT_MSG_LENGTH 2
#define RELEASE_MSG_LENGTH 1

#if ENABLE_WAIT_DIE_CC
#define LOOKUP_MSG_LENGTH 2
#else
#define LOOKUP_MSG_LENGTH 1
#endif

#endif
