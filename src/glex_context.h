/*
 *
 */
#ifndef GLEX_CONTEXT_H
#define GLEX_CONTEXT_H

#include "glex_common.h"
#include "glex_list.h"

GLEX_BEGIN_DECLS

typedef struct {
	hmm_vec3 position;
	hmm_vec3 scale;
	hmm_quaternion rotation;
} GLEXTransform;

typedef struct {
	GLEXListNode node;
	GLintptr offset;
	GLsizeiptr size;
} GLEXBuffer;

typedef struct {
	GLuint id;
	GLEXList bufferList;
	GLsizeiptr freeSize;
} GLEXBufferHeap;

struct GLEXLight_ {
	GLEXLightType type;
};

struct GLEXMaterial_ {
	int fooBar;
};

struct GLEXMesh_ {
	GLEXMeshType type;
};

struct GLEXContext_ {
	GLEXVersion version;
	GLEXConfig config;

	union GL3WProcs gl3w;

	GLEXList bufferHeapList[GLEX_MESH_TYPE_MAX];

	struct {
		hmm_vec3 eye;
		hmm_vec3 center;
		hmm_vec3 up;
		float fov;
		float ratio;
		float nearPlane;
		float farPlane;
	} view;

	struct {
		int width;
		int height;
	} gBuffers;

	GLEXTransform *transformStack;
	int transformStackTop;

	GLEXLight *light;
	GLEXMaterial *material;
	GLEXMesh *mesh;

	struct {
		bool began;
	} frame;
};

extern GLEXContext *glex;

GLEX_END_DECLS

#endif /* GLEX_CONTEXT_H */
