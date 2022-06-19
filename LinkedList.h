#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include "tree.h"

List*
ll_create();

void
ll_add_nth_node(List* list, unsigned int n, TreeNode* data);

ListNode*
ll_remove_nth_node(List* list, unsigned int n);

TreeNode*
get_nth_node(List* list, char* arg);

void
ll_print_string(List* list);

#endif /* LINKED_LIST_H_ */
