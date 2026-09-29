#ifndef PRY_UTIL_H
#define PRY_UTIL_H
#include <stddef.h>

#if defined(__GNUC__) || defined(__clang__)
#define NORETURN	     __attribute__((noreturn))
#define PRINTF_FMT(a, b)     __attribute__((format(printf, a, b)))
#define ALLOC_SIZE(n)	     __attribute__((alloc_size(n)))
#define ALLOC_SIZE_NXM(n, m) __attribute__((alloc_size(n, m)))
#define RETURNS_NONNULL	     __attribute__((returns_nonnull))
#define PACKED		     __attribute__((packed))
#define UNUSED		     __attribute__((unused))
#define WARN_UNUSED	     __attribute__((warn_unused_result))
#else
#define NORETURN
#define PRINTF_FMT(a, b)
#define ALLOC_SIZE(n)
#define ALLOC_SIZE_NXM(n, m)
#define RETURNS_NONNULL
#define PACKED
#define UNUSED
#define WARN_UNUSED
#endif
#define PROGNAME "pry"

/* printf(fmt, ...) then exit(EXIT_FAILURE). */
void die(const char *fmt, ...) NORETURN PRINTF_FMT(1, 2);

/* Tranparrent wrappers over libc funcs. Die on OOM */
void *xmalloc(size_t n) ALLOC_SIZE(1) RETURNS_NONNULL;
void *xcalloc(size_t n, size_t sz) ALLOC_SIZE_NXM(1, 2) RETURNS_NONNULL;
void *xrealloc(void *p, size_t n) ALLOC_SIZE(2) RETURNS_NONNULL;
char *xstrdup(const char *s) RETURNS_NONNULL;
char *xstrndup(const char *s, size_t n) RETURNS_NONNULL ALLOC_SIZE(2);

/* Struct strbuf - a vector-like structure to hold a null-terminated string */
struct strbuf {
	char *buf;
	size_t len;
	size_t cap;
};

void strbuf_init(struct strbuf *sb);
void strbuf_append(struct strbuf *sb, const char *str, size_t len);
void strbuf_appendf(struct strbuf *sb, const char *fmt, ...) PRINTF_FMT(2, 3);
void strbuf_reset(struct strbuf *sb);
void strbuf_free(struct strbuf *sb);

#endif /* PRY_UTIL_H */
