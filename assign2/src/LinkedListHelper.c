/************************************
 *  Name: Joseph Coffa              *
 *  Student #: 1007320              *
 *  Due Date: February 27, 2019     *
 *                                  *
 *  Assignment 2, CIS*2750          *
 *  LinkedListHelper.c              *
 ************************************/

#include "LinkedListHelper.h"

ListIterator createReverseIterator(List *list) {
	ListIterator iter;
	
	iter.current = list->tail;

	return iter;
}

void *previousElement(ListIterator *iter) {
	Node *temp = iter->current;

	if (temp != NULL) {
		iter->current = iter->current->previous;
		return temp->data;
	} else {
		return NULL;
	}
}
