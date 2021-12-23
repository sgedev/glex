/*
 *
 */
#include "glex_common.h"

static GLuint glexCreateShaderFromSource(GLenum type, const char *src)
{
	GLuint id;
	GLint len = 1;
	GLint status = 0;

	GLEX_ASSERT(src != NULL);

	id = glCreateShader(type);
	if (id < 1)
		goto bad0;

	glShaderSource(id, 1, &src, &len);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		// TODO
		goto bad1;
	}

	return id;

bad1:
	glDeleteShader(id);

bad0:
	return 0;
}

static GLuint glexCreateProgramFromSource(const char *vsSrc, const char *fsSrc)
{
	GLuint vs;
	GLuint fs;

	GLEX_ASSERT(vsSrc != NULL);
	GLEX_ASSERT(fsSrc != NULL);

	vs = glexCreateShaderFromSource(GL_VERTEX_SHADER, vsSrc);
	if (vs < 1)
		goto bad0;

	fs = glexCreateShaderFromSource(GL_FRAGMENT_SHADER, fsSrc);
	if (fs < 1)
		goto bad1;

	return glexCreateProgramFromShader(vs, fs);

bad1:
	glDeleteShader(vs);

bad0:
	return 0;
}

static GLuint glexCreateProgramFromShader(GLuint vs, GLuint fs)
{
	GLuint id;
	GLint status;

	GLEX_ASSERT(vs > 0);
	GLEX_ASSERT(fs > 0);

	id = glCreateProgram();
	if (id < 0)
		goto bad0;

	glAttachShader(id, vs);
	glAttachShader(id, fs);

	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		// TODO
		goto bad1;
	}

	return id;

bad1:
	glDeleteProgram(id);

bad0:
	return 0;
}
