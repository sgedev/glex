/*
 *
 */
#ifndef GLEX_LIST_H
#define GLEX_LIST_H

#include "glex_common.h"

GLEX_BEGIN_DECLS

typedef struct GLEXListNode_ {
	struct GLEXListNode_ *prev;
	struct GLEXListNode_ *next;
} GLEXListNode;

GLEX_INLINE void glexListNodeReset(GLEXListNode *node)
{
	GLEX_ASSERT(node != NULL);

	node->next = node;
	node->prev = node;
}

GLEX_INLINE bool glexListNodeIsLinked(const GLEXListNode *node)
{
	GLEX_ASSERT(node != NULL);
	GLEX_ASSERT(node->prev != NULL);
	GLEX_ASSERT(node->next != NULL);

	return (node->next != node);
}

GLEX_INLINE GLEXListNode *glexListNodePrev(GLEXListNode *node)
{
	GLEX_ASSERT(node != NULL);
	GLEX_ASSERT(node->prev != NULL);
	GLEX_ASSERT(node->next != NULL);

	return node->prev;
}

GLEX_INLINE GLEXListNode *glexListNodeNext(GLEXListNode *node)
{
	GLEX_ASSERT(node != NULL);
	GLEX_ASSERT(node->prev != NULL);
	GLEX_ASSERT(node->next != NULL);

	return node->next;
}

GLEX_INLINE void glexListNodeLink(GLEXListNode *node, GLEXListNode *prev, GLEXListNode *next)
{
	GLEX_ASSERT(node != NULL);
	GLEX_ASSERT(prev != NULL);
	GLEX_ASSERT(next != NULL);

	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

GLEX_INLINE void glexListNodeUnlink(GLEXListNode *node)
{
	GLEX_ASSERT(node != NULL);
	GLEX_ASSERT(node->prev != NULL);
	GLEX_ASSERT(node->next != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node;
	node->next = node;
}

typedef struct {
	GLEXListNode knot;
} GLEXList;

GLEX_INLINE void glexListReset(GLEXList *list)
{
	GLEX_ASSERT(list != NULL);

	glexListNodeReset(&list->knot);
}

GLEX_INLINE bool glexListIsEmpty(const GLEXList *list)
{
	GLEX_ASSERT(list != NULL);

	return !glexListNodeIsLinked(&list->knot);
}

GLEX_INLINE GLEXListNode *glexListKnot(GLEXList *list)
{
	GLEX_ASSERT(list != NULL);

	return &list->knot;
}

GLEX_INLINE GLEXListNode *glexListFirst(GLEXList *list)
{
	GLEX_ASSERT(list != NULL);

	return list->knot.next;
}

GLEX_INLINE GLEXListNode *glexListRemoveFirst(GLEXList *list)
{
	GLEXListNode *p;

	GLEX_ASSERT(list != NULL);

	p = glexListFirst(list);
	glexListNodeUnlink(p);

	return p;
}

GLEX_INLINE GLEXListNode *glexListLast(GLEXList *list)
{
	GLEX_ASSERT(list != NULL);

	return list->knot.prev;
}

GLEX_INLINE GLEXListNode *glexListRemoveLast(GLEXList *list)
{
	GLEXListNode *p;

	GLEX_ASSERT(list != NULL);

	p = glexListLast(list);
	glexListNodeUnlink(p);

	return p;
}

GLEX_INLINE size_t glexListLength(const GLEXList *list)
{
	size_t n = 0;
	const GLEXListNode *p;

	GLEX_ASSERT(list != NULL);

	for (p = list->knot.next; p != &list->knot; p = p->next)
		n += 1;

	return n;
}

GLEX_INLINE bool glexListIsContains(const GLEXList *list, const GLEXListNode *node)
{
	const GLEXListNode *p;

	GLEX_ASSERT(list != NULL);

	for (p = list->knot.next; p != &list->knot; p = p->next) {
		if (p == node)
			return true;
	}

	return false;
}

GLEX_INLINE GLEXListNode *glexListRemoveNode(GLEXList *list, GLEXListNode *node)
{
	GLEX_ASSERT(list != NULL);
	GLEX_ASSERT(node != NULL);
	GLEX_ASSERT(glexListIsContains(list, node));

	glexListNodeUnlink(node);
}

GLEX_INLINE void glexListPrepend(GLEXList *list, GLEXListNode *node)
{
	GLEX_ASSERT(list != NULL);
	GLEX_ASSERT(node != NULL);

	glexListNodeLink(node, &list->knot, list->knot.next);
}

GLEX_INLINE void glexListAppend(GLEXList *list, GLEXListNode *node)
{
	GLEX_ASSERT(list != NULL);
	GLEX_ASSERT(node != NULL);

	glexListNodeLink(node, list->knot.prev, &list->knot);
}

GLEX_END_DECLS

#endif /* GLEX_LIST_H */
