/*
 *
 */
#ifndef GLEX_COMMON_H
#define GLEX_COMMON_H

#include <stdarg.h>

#include <HandmadeMath.h>

#include <GL/gl3w.h>
#include <GL/glex.h>

#ifdef GLEX_DEBUG
#	include <assert.h>
#	define GLEX_ASSERT(expr) assert(expr)
#else
#	define GLEX_ASSERT(expr)
#endif

#if defined(_MSC_VER)
#	define GLEX_INLINE __forceinline
#elif defined(__GNUC__)
#	define GLEX_INLINE __attribute__((always_inline))
#else
#	error unknown compiler.
#endif

#define GLEX_PMOVB(p, offset) \
	(((uint8_t *)(p)) + (offset))

#define GLEX_OFFSETOF(structType, memberName) \
	((intptr_t)(&(((structType *)0)->memberName)))

#define GLEX_MEMBEROF(p, structType, memberName) \
	((structType *)GLEX_PMOVB(p, -GLEX_OFFSETOF(structType, memberName)))

GLEX_BEGIN_DECLS

GLuint glexCreateShaderFromSource(GLenum type, const char *src);
GLuint glexCreateProgramFromSource(const char *vsSrc, const char *fsSrc);
GLuint glexCreateProgramFromShader(GLuint vs, GLuint fs);

GLEX_END_DECLS

#endif /* GLEX_COMMON_H */
