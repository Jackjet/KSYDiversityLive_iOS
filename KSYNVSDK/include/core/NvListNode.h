//================================================================================
//
// (c) Copyright China Digital Video (Beijing) Limited, 2014. All rights reserved.
//
// This code and information is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the implied
// warranties of merchantability and/or fitness for a particular purpose.
//
//--------------------------------------------------------------------------------
//   Birth Date:    Jan 21. 2014
//   Author:        NewAuto video team
//================================================================================
#pragma once

//
// Basic head list routine and structs
//
struct SNvListNode {
    SNvListNode *next;
    SNvListNode *prev;
};

#define	NV_LIST_ENTRY(PTR, TYPE, MEMBER)	((TYPE *)((char *)(PTR) - (size_t)(&((TYPE *)0)->MEMBER)))

static inline void NvInitListNode(SNvListNode *listNode)
{
    listNode->next = listNode;
    listNode->prev = listNode;
}

static inline int NvListEmpty(const SNvListNode *listNode)
{
    return listNode->next == listNode;
}

static inline void __NvListAdd(SNvListNode *newNode,
                               SNvListNode *prevNode,
                               SNvListNode *nextNode)
{
    nextNode->prev = newNode;
    newNode->next = nextNode;
    newNode->prev = prevNode;
    prevNode->next = newNode;
}

static inline void NvListAddAfter(SNvListNode *listNode,
                                  SNvListNode *newListNode)
{
    __NvListAdd(newListNode, listNode, listNode->next);
}

static inline void NvListAddBefore(SNvListNode *listNode,
                                   SNvListNode *newListNode)
{
    __NvListAdd(newListNode, listNode->prev, listNode);
}

static inline void NvListDelete(SNvListNode *listNode)
{
    listNode->prev->next = listNode->next;
    listNode->next->prev = listNode->prev;
}

