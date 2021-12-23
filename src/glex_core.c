/*
 *
 */
#include "glex_context.h"

GLEXContext *glex = NULL;

static bool glexInitContext(GLEXContext *context, const GLEXConfig *config)
{
	GLEX_ASSERT(context != NULL);
	GLEX_ASSERT(config != NULL);

	memset(context, 0, sizeof(GLEXContext));

	context->version.major = GLEX_VERSION_MAJOR;
	context->version.minor = GLEX_VERSION_MINOR;
	context->version.patch = GLEX_VERSION_PATCH;

	context->config = *config;

	GLEX_ASSERT(config->getProc != NULL);
	if (gl3wInit(&context->gl3w, (GL3WGetProcAddressProc)(config->getProc)) != GL3W_OK)
		goto bad0;

	if (context->config.transformStackSize <= 0)
		context->config.transformStackSize = 16;

	context->transformStack = malloc(sizeof(GLEXTransform) * context->config.transformStackSize);
	if (context->transformStack == NULL)
		goto bad0;

	return true;

bad0:
	return false;
}

static void glexFinalizeContext(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);
	GLEX_ASSERT(context != glex);

	// TODO
}

static void glexResizeGBuffers(int width, int height)
{
	if (glex->gBuffers.width == width && glex->gBuffers.height == height)
		return;

	// TODO free old
	// TODO allocate new

	glex->gBuffers.width = width;
	glex->gBuffers.height = height;
}

GLEX_API GLEXContext *glexCreateContext(const GLEXConfig *config)
{
	GLEXContext *context;

	GLEX_ASSERT(config != NULL);

	context = malloc(sizeof(GLEXContext));
	if (context == NULL)
		return NULL;

	if (!glexInitContext(context, config)) {
		free(context);
		return NULL;
	}

	return context;
}

GLEX_API GLEXContext *glexCurrentContext(void)
{
	return glex;
}

GLEX_API void glexDeleteContext(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	if (glex == context)
		glex = NULL;

	glexFinalizeContext(context);
	free(context);
}

GLEX_API void glexMakeCurrent(GLEXContext *context)
{
	glex = context;
	if (glex != NULL)
		gl3wProcs = &glex->gl3w;
}

GLEX_API const GLEXVersion *glexGetVersion(void)
{
	GLEX_ASSERT(glex != NULL);

	return &glex->version;
}

GLEX_API void glexBeginFrame(int width, int height)
{
	GLEX_ASSERT(glex != NULL);

	glexResizeGBuffers(width, height);
}

GLEX_API void glexEndFrame(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(gl3wProcs == &glex->gl3w);
}

GLEX_API void glexPrespective(float fov, float ratio, float nearPlane, float farPlane)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(ratio > 0.0f);
	GLEX_ASSERT(nearPlane < 0.0f);
	GLEX_ASSERT(farPlane < 0.0f);
	GLEX_ASSERT(nearPlane > farPlane);

	glex->view.fov = HMM_Clamp(0.1f, fov, 179.9f);
	glex->view.ratio = ratio;
	glex->view.nearPlane = nearPlane;
	glex->view.farPlane = farPlane;
}

GLEX_API void glexLookAt(float eyeX, float eyeY, float eyeZ,
	float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	GLEX_ASSERT(glex != NULL);
}

GLEX_API void glexResetTransform(void)
{
	GLEX_ASSERT(glex != NULL);

	GLEXTransform *transform = &glex->transformStack[glex->transformStackTop];
	transform->position = HMM_Vec3(0.0f, 0.0f, 0.0f);
	transform->scale = HMM_Vec3(1.0f, 1.0f, 1.0f);
	transform->rotation = HMM_Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
}

GLEX_API void glexPushTransform(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->transformStackTop < glex->config.transformStackSize);

	GLEXTransform *oldTop = &glex->transformStack[glex->transformStackTop++];
	glex->transformStack[glex->transformStackTop] = *oldTop;
}

GLEX_API void glexPopTransform(void)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(glex->transformStackTop > 1);

	glex->transformStackTop -= 1;
}

GLEX_API void glexTranslate(float x, float y, float z)
{
	GLEX_ASSERT(glex != NULL);

	GLEXTransform *top = &glex->transformStack[glex->transformStackTop];
	top->position.X += x;
	top->position.Y += y;
	top->position.Z += z;
}

GLEX_API void glexScale(float x, float y, float z)
{
	GLEX_ASSERT(glex != NULL);

	GLEXTransform *top = &glex->transformStack[glex->transformStackTop];
	top->position.X *= x;
	top->position.Y *= y;
	top->position.Z *= z;
}

GLEX_API void glexRotate(float angle, float x, float y, float z)
{
	GLEX_ASSERT(glex != NULL);

	GLEXTransform *top = &glex->transformStack[glex->transformStackTop];
	top->rotation = HMM_MultiplyQuaternion(
		top->rotation,
		HMM_QuaternionFromAxisAngle(HMM_Vec3(x, y, z), angle));
}
