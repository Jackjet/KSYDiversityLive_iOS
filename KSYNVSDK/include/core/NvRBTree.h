//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2015. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    July 28 2015
//   Author:        NewAuto video team
//================================================================================
#pragma once

#include <NvDef.h>


#define	RB_RED		0
#define	RB_BLACK	1

struct rb_node {
	size_t  rb_parent_color;
	struct rb_node *rb_right;
	struct rb_node *rb_left;
};

struct rb_root {
	struct rb_node *rb_node;
};


#define rb_parent(r)		((struct rb_node *)((r)->rb_parent_color & ~3))
#define rb_color(r)			((int)((r)->rb_parent_color & 1))
#define rb_is_red(r)		(!rb_color(r))
#define rb_is_black(r)		rb_color(r)
#define rb_set_red(r)		do { (r)->rb_parent_color &= ~1; } while (0)
#define rb_set_black(r)		do { (r)->rb_parent_color |= 1; } while (0)

static inline void rb_set_parent(struct rb_node *rb, struct rb_node *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (size_t)p;
}

static inline void rb_set_color(struct rb_node *rb, int color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | (size_t)color;
}


#define	rb_entry(ptr, type, member) NV_GET_MEMBER_CONTAINER(ptr, type, member)

#define RB_EMPTY_ROOT(root)	((root)->rb_node == NULL)
#define RB_CLEAR_ROOT(root)	((root)->rb_node = NULL)
#define RB_EMPTY_NODE(node)	(rb_parent(node) == node)
#define RB_CLEAR_NODE(node)	(rb_set_parent(node, node))

extern void rb_insert_color(struct rb_node *, struct rb_root *);
extern void rb_erase(struct rb_node *, struct rb_root *);

/* Find logical next and previous nodes in a tree */
extern struct rb_node *rb_next(struct rb_node *);
extern struct rb_node *rb_prev(struct rb_node *);
extern struct rb_node *rb_first(struct rb_root *);
extern struct rb_node *rb_last(struct rb_root *);

/* fast replacement of a single node without remove/rebalance/add/rebalance */
extern void rb_replace_node(struct rb_node *victim,
							struct rb_node *newnode,
							struct rb_root *root);

static inline void rb_link_node(struct rb_node *node,
								struct rb_node *parent,
								struct rb_node **rb_link)
{
	node->rb_parent_color = (size_t)parent;
	node->rb_left = node->rb_right = NULL;

	*rb_link = node;
}

