#pragma once
#ifdef WIN32
#include "winsock2.h"

#ifndef be16toh
#define be16toh(x)	((uint16_t)ntohs((uint16_t)(x)))
#endif
#ifndef htobe16
#define htobe16(x)	((uint16_t)htons((uint16_t)(x)))
#endif
#ifndef be32toh
#define be32toh(x)	((uint32_t)ntohl((uint32_t)(x)))
#endif
#ifndef htobe32
#define htobe32(x)	((uint32_t)htonl((uint32_t)(x)))
#endif

#ifndef be64toh
#ifdef __bswap_64 /* glibc */
#define be64toh(x)	((uint64_t)__bswap_64((uint64_t)(x)))
#else /* no __bswap_64 */
#ifdef __swab64 /* Linux kernel headers (libc5, at least with kernel 2.2) */
#define be64toh(x)	((uint64_t)__swab64((uint64_t)(x)))
#else /* no __bswap_64 or __swab64 */
static inline uint64_t be64toh(uint64_t __x);
static inline uint64_t be64toh(uint64_t __x) { return (((uint64_t)be32toh(__x & (uint64_t)0xFFFFFFFFULL)) << 32) | ((uint64_t)be32toh((__x & (uint64_t)0xFFFFFFFF00000000ULL) >> 32)); }
#define be64toh(x)	be64toh((x))
#endif /* no __bswap_64 or __swab64 */
#endif /* no __bswap_64 */
#endif /* no be64toh */
#ifndef htobe64
#define htobe64(x)	be64toh(x)
#endif

#endif