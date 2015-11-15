#ifndef __UTIL_H_
#define __UTIL_H_

#include <sys/types.h>
#include <stdint.h>

#define FALSE 0
#define TRUE 1

#define CACHELINE   64 
#define MAX_CLIENTS 128 // must be power of 2
#define MAX_SERVERS 128 // must be power of 2

#define TXN_COMMIT 0
#define TXN_ABORT 1

#define YCSB_NFIELDS 1
#define YCSB_FIELD_SZ 8
#define YCSB_REC_SZ (YCSB_NFIELDS * YCSB_FIELD_SZ)

#ifdef DEBUG
#define dprint(...) \
{\
  printf(__VA_ARGS__); \
  fflush(stdout);\
}
#else
#define dprint(...) 
#endif

pid_t gettid(void);
void set_affinity(int cpu_id);
double now();

static inline int max(int a, int b) 
{ 
  if (a > b) return a; 
  else return b; 
}

static inline int min(int a, int b) 
{ 
  if (a < b) return a; 
  else return b; 
}

// even though this is defined in SSE2 it is 
// easier and more compatible to do it this way
// and not include sse2 headers and build flags
static inline void _mm_pause() 
{
  __asm __volatile("pause");
}

static inline uint64_t __attribute__((always_inline))
read_tsc(void)
{
  uint32_t a, d;
  __asm __volatile("rdtsc" : "=a" (a), "=d" (d));
  return ((uint64_t) a) | (((uint64_t) d) << 32);
}

uint64_t *zipf_get_keys(double alpha, uint64_t N, uint64_t nvalues);

uint64_t RAND(uint64_t max);
uint64_t URand(uint64_t x, uint64_t y);
uint64_t NURand(uint64_t A, uint64_t x, uint64_t y);
uint64_t make_alpha_string(int min, int max, char* str);
uint64_t make_numeric_string(int min, int max, char* str);

#endif
