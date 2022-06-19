#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))

List*
ll_create()
{
	List* list = calloc(1, sizeof(*list));
	list->head = NULL;

	return list;
}

ListNode*
create_node(TreeNode* new_info)
{
	ListNode* node = malloc(sizeof(ListNode));

	node->info = new_info;
	node->next = NULL;

	return node;
}


/*
 * Pe baza datelor trimise prin pointerul new_info, se creeaza un nou nod care e
 * adaugat pe pozitia 0 a listei reprezentata de pointerul list. 
 */
void
ll_add_nth_node(List* list, unsigned int n, TreeNode* new_info)
{
	ListNode *new_node, *prev_node;

	if (!list)
		return;

	new_node = create_node(new_info);

	if (!n) {
		new_node->next = list->head;
		list->head = new_node;
	}
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ListNode*
ll_remove_nth_node(List* list, unsigned int n)
{
	ListNode *prev_node;
	ListNode *removed_node = list->head;

	if (!list)
		return NULL;

	if (!n) {
		list->head = removed_node->next;
		removed_node->next = NULL;
	} else {
		int i;
		for (i = 0; i < n; i++) {
			prev_node = removed_node;
			removed_node = removed_node->next;
		}
		prev_node->next = removed_node->next;
	}

	return removed_node;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void
ll_print_string(List* list)
{
	ListNode* node = list->head;

	while (node) {
        printf("%s\n", ((TreeNode*)node->info)->name);
        node = node->next;
    }
	printf("\n");
}
