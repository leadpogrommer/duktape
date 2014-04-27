/*
 *  Debugging macros, DUK_DPRINT() and its variants in particular.
 *
 *  DUK_DPRINT() allows formatted debug prints, and supports standard
 *  and Duktape specific formatters.  See duk_debug_vsnprintf.c for details.
 */

#ifndef DUK_DEBUG_H_INCLUDED
#define DUK_DEBUG_H_INCLUDED

#ifdef DUK_USE_DEBUG

/*
 *  Exposed debug macros: debugging enabled
 */

#define DUK_LEVEL_DEBUG    1
#define DUK_LEVEL_DDEBUG   2
#define DUK_LEVEL_DDDEBUG  3

#ifdef DUK_USE_VARIADIC_MACROS

/* Note: combining __FILE__, __LINE__, and __func__ into fmt would be
 * possible compile time, but waste some space with shared function names.
 */
#define DUK__DEBUG_LOG(lev,...)  duk_debug_log((lev), DUK_FILE_MACRO, (int) DUK_LINE_MACRO, DUK_FUNC_MACRO, __VA_ARGS__);

#define DUK_DPRINT(...)          DUK__DEBUG_LOG(DUK_LEVEL_DEBUG, __VA_ARGS__)

#ifdef DUK_USE_DDPRINT
#define DUK_DDPRINT(...)         DUK__DEBUG_LOG(DUK_LEVEL_DDEBUG, __VA_ARGS__)
#else
#define DUK_DDPRINT(...)
#endif

#ifdef DUK_USE_DDDPRINT
#define DUK_DDDPRINT(...)        DUK__DEBUG_LOG(DUK_LEVEL_DDDEBUG, __VA_ARGS__)
#else
#define DUK_DDDPRINT(...)
#endif

#else  /* DUK_USE_VARIADIC_MACROS */

#define DUK__DEBUG_STASH(lev)    \
	(void) DUK_SNPRINTF(duk_debug_file_stash, DUK_DEBUG_STASH_SIZE, "%s", DUK_FILE_MACRO), \
	duk_debug_file_stash[DUK_DEBUG_STASH_SIZE - 1] = (char) 0; \
	(void) DUK_SNPRINTF(duk_debug_line_stash, DUK_DEBUG_STASH_SIZE, "%d", (int) DUK_LINE_MACRO), \
	duk_debug_line_stash[DUK_DEBUG_STASH_SIZE - 1] = (char) 0; \
	(void) DUK_SNPRINTF(duk_debug_func_stash, DUK_DEBUG_STASH_SIZE, "%s", DUK_FUNC_MACRO), \
	duk_debug_func_stash[DUK_DEBUG_STASH_SIZE - 1] = (char) 0; \
	(void) (duk_debug_level_stash = (lev))

/* Without variadic macros resort to comma expression trickery to handle debug
 * prints.  This generates a lot of harmless warnings, unfortunately.
 */

#ifdef DUK_USE_DPRINT
#define DUK_DPRINT  DUK__DEBUG_STASH(DUK_LEVEL_DEBUG), (void) duk_debug_log  /* args go here in parens */
#else
#define DUK_DPRINT  0 && /* args go here as a comma expression in parens */
#endif

#ifdef DUK_USE_DDPRINT
#define DUK_DDPRINT  DUK__DEBUG_STASH(DUK_LEVEL_DDEBUG), (void) duk_debug_log  /* args go here in parens */
#else
#define DUK_DDPRINT  0 && 
#endif

#ifdef DUK_USE_DDDPRINT
#define DUK_DDDPRINT  DUK__DEBUG_STASH(DUK_LEVEL_DDDEBUG), (void) duk_debug_log  /* args go here in parens */
#else
#define DUK_DDDPRINT  0 && 
#endif

#endif  /* DUK_USE_VARIADIC_MACROS */

/* object dumpers */

#define DUK_DEBUG_DUMP_HEAP(x)               duk_debug_dump_heap((x))
#define DUK_DEBUG_DUMP_HSTRING(x)            /* XXX: unimplemented */
#define DUK_DEBUG_DUMP_HOBJECT(x)            duk_debug_dump_hobject((x))
#define DUK_DEBUG_DUMP_HCOMPILEDFUNCTION(x)  /* XXX: unimplemented */
#define DUK_DEBUG_DUMP_HNATIVEFUNCTION(x)    /* XXX: unimplemented */
#define DUK_DEBUG_DUMP_HTHREAD(thr)          duk_debug_dump_hobject((duk_hobject *) (thr))
#define DUK_DEBUG_DUMP_CALLSTACK(thr)        duk_debug_dump_callstack((thr))
#define DUK_DEBUG_DUMP_ACTIVATION(thr,act)   duk_debug_dump_activation((thr),(act))

/* summary macros */

#define DUK_DEBUG_SUMMARY_INIT()  do { \
		DUK_MEMZERO(duk_debug_summary_buf, sizeof(duk_debug_summary_buf)); \
		duk_debug_summary_idx = 0; \
	} while (0)

#define DUK_DEBUG_SUMMARY_CHAR(ch)  do { \
		duk_debug_summary_buf[duk_debug_summary_idx++] = (ch); \
		if ((duk_size_t) duk_debug_summary_idx >= (duk_size_t) (sizeof(duk_debug_summary_buf) - 1)) { \
			duk_debug_summary_buf[duk_debug_summary_idx++] = (char) 0; \
			DUK_DPRINT("    %s", duk_debug_summary_buf); \
			DUK_DEBUG_SUMMARY_INIT(); \
		} \
	} while (0)

#define DUK_DEBUG_SUMMARY_FINISH()  do { \
		if (duk_debug_summary_idx > 0) { \
			duk_debug_summary_buf[duk_debug_summary_idx++] = (char) 0; \
			DUK_DPRINT("    %s", duk_debug_summary_buf); \
			DUK_DEBUG_SUMMARY_INIT(); \
		} \
	} while (0)

#else  /* DUK_USE_DEBUG */

/*
 *  Exposed debug macros: debugging disabled
 */

#ifdef DUK_USE_VARIADIC_MACROS

#define DUK_DPRINT(...)
#define DUK_DDPRINT(...)
#define DUK_DDDPRINT(...)

#else  /* DUK_USE_VARIADIC_MACROS */

#define DUK_DPRINT    0 && /* args go here as a comma expression in parens */
#define DUK_DDPRINT   0 && 
#define DUK_DDDPRINT  0 && 

#endif  /* DUK_USE_VARIADIC_MACROS */

#define DUK_DEBUG_DUMP_HEAP(x)
#define DUK_DEBUG_DUMP_HSTRING(x)
#define DUK_DEBUG_DUMP_HOBJECT(x)
#define DUK_DEBUG_DUMP_HCOMPILEDFUNCTION(x)
#define DUK_DEBUG_DUMP_HNATIVEFUNCTION(x)
#define DUK_DEBUG_DUMP_HTHREAD(x)

#define DUK_DEBUG_SUMMARY_INIT()
#define DUK_DEBUG_SUMMARY_CHAR(ch)
#define DUK_DEBUG_SUMMARY_FINISH()

#endif  /* DUK_USE_DEBUG */

/*
 *  Structs
 */

#ifdef DUK_USE_DEBUG
struct duk_fixedbuffer {
	duk_uint8_t *buffer;
	duk_uint32_t length;
	duk_uint32_t offset;
	int truncated;
};
#endif

/*
 *  Prototypes
 */

#ifdef DUK_USE_DEBUG
int duk_debug_vsnprintf(char *str, size_t size, const char *format, va_list ap);
int duk_debug_snprintf(char *str, size_t size, const char *format, ...);
void duk_debug_format_funcptr(char *buf, int buf_size, unsigned char *fptr, int fptr_size);

#ifdef DUK_USE_VARIADIC_MACROS
void duk_debug_log(int level, const char *file, int line, const char *func, char *fmt, ...);
#else  /* DUK_USE_VARIADIC_MACROS */
/* parameter passing, not thread safe */
#define DUK_DEBUG_STASH_SIZE  128
extern char duk_debug_file_stash[DUK_DEBUG_STASH_SIZE];
extern char duk_debug_line_stash[DUK_DEBUG_STASH_SIZE];
extern char duk_debug_func_stash[DUK_DEBUG_STASH_SIZE];
extern int duk_debug_level_stash;
extern void duk_debug_log(char *fmt, ...);
#endif  /* DUK_USE_VARIADIC_MACROS */

void duk_fb_put_bytes(duk_fixedbuffer *fb, duk_uint8_t *buffer, duk_uint32_t length);
void duk_fb_put_byte(duk_fixedbuffer *fb, duk_uint8_t x);
void duk_fb_put_cstring(duk_fixedbuffer *fb, const char *x);
void duk_fb_sprintf(duk_fixedbuffer *fb, const char *fmt, ...);
int duk_fb_is_full(duk_fixedbuffer *fb);

void duk_debug_dump_heap(duk_heap *heap);
void duk_debug_heap_graphviz(duk_heap *heap);
void duk_debug_dump_hobject(duk_hobject *obj);
void duk_debug_dump_hthread(duk_hthread *thr);
void duk_debug_dump_callstack(duk_hthread *thr);
void duk_debug_dump_activation(duk_hthread *thr, duk_activation *act);

#define DUK_DEBUG_SUMMARY_BUF_SIZE  76
extern char duk_debug_summary_buf[DUK_DEBUG_SUMMARY_BUF_SIZE];
extern int duk_debug_summary_idx;

#endif  /* DUK_USE_DEBUG */

#endif  /* DUK_DEBUG_H_INCLUDED */

