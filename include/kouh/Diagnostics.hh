#ifndef KOUH_DIAGNOSTICS_HH_
#define KOUH_DIAGNOSTICS_HH_

/** Utility diagnostic macros.
 *
 * Defines three macros:
 * - DIAGNOSTICS_PUSH(): Push a diagnostics state on the stack.
 * - DIAGNOSTICS_POP(): Pop a diagnostics state from the stack.
 * - DIAGNOSTICS_IGNORE(x): Ignore a diagnostic. Used like
 *   DIAGNOSTICS_IGNORE(unused-parameter).
 *
 * These macros are to be used in a specific context and not enabled
 * project-wide.
 *
 * One should push the diagnostics state, modify this state for a piece of code
 * and then pop the state to restore it.
 *
 * Adapted from https://svn.boost.org/trac10/wiki/Guidelines/WarningsGuidelines
 */

#define KOUH_INTERNAL_DIAG_STR(s) #s
#define KOUH_INTERNAL_DIAG_JOINSTR(x, y) KOUH_INTERNAL_DIAG_STR(x##y)
#define KOUH_INTERNAL_DIAG_DO_PRAGMA(x) _Pragma(#x)

#ifdef __clang__
#define KOUH_INTERNAL_DIAG_PRAGMA(x) \
  KOUH_INTERNAL_DIAG_DO_PRAGMA(clang diagnostic x)
#elif ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
#define KOUH_INTERNAL_DIAG_PRAGMA(x) \
  KOUH_INTERNAL_DIAG_DO_PRAGMA(GCC diagnostic x)
#else
#define KOUH_INTERNAL_DIAG_PRAGMA(x)
#endif

#define DIAGNOSTICS_PUSH() KOUH_INTERNAL_DIAG_PRAGMA(push)
#define DIAGNOSTICS_POP() KOUH_INTERNAL_DIAG_PRAGMA(pop)
#define DIAGNOSTICS_IGNORE(x) \
  KOUH_INTERNAL_DIAG_PRAGMA(ignored KOUH_INTERNAL_DIAG_JOINSTR(-W, x))

#endif /* !KOUH_DIAGNOSTICS_HH_ */
