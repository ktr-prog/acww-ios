#ifndef SYS_TASKLIST_H
#define SYS_TASKLIST_H

#include "types.h"
#include "sys/QNode.h"
#include "sys/TreeNode.h"

// Task manager ("LoopProc") phase lists: each list holds a pointer to a ProcBase member function that the phase runner
// calls on every node. Defined in src/autoload_2/unk_020ed4bc.cpp (gTask*List, gProcTree, TaskList::run, TaskTree::run).
class ProcBase;
typedef void (ProcBase::*TaskFn)();

// a phase's task list (EXECUTE, CREATE, DRAW, DELETE)
class TaskList {
public:
    TaskList(TaskFn f) : head(0), tail(0), fn(f) {}
    BOOL run();                       // 0x020ed54c

    /* 0x00 */ QNode *head;
    /* 0x04 */ QNode *tail;  // (the list's tail as QList keeps it: a pointer, its size on 64-bit hosts)
    /* 0x08 */ TaskFn fn;
};

// the CONNECT list (a TreeNode list, no tail)
class TaskTree {
public:
    TaskTree(TaskFn f) : head(0), fn(f) {}
    BOOL run();                       // 0x020ed764

    /* 0x00 */ TreeNode *head;
    /* 0x04 */ TaskFn fn;
};
typedef TaskTree TaskList4;

#endif
