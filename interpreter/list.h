
#ifndef _LIST_H_
#define _LIST_H_

#include "base_utils.h"

struct list_head {
  struct list_head* prev;
  struct list_head* next;
};

static inline void init_list_head(struct list_head* list) {
  list->prev = list;
  list->next = list;
}

static inline void list_add_after(struct list_head* new, struct list_head* head) {
  new->prev = head;
  new->next = head->next;
  new->prev->next = new;
  new->next->prev = new;
}

static inline void list_prepend(struct list_head* new, struct list_head* head) {
  list_add_after(new, head);
}

static inline void list_append(struct list_head* new, struct list_head* head) {
  list_add_after(new, head->prev);
}

static inline void list_del(struct list_head* entry) {
  entry->next->prev = entry->prev;
  entry->prev->next = entry->next;
  entry->next = NULL;
  entry->prev = NULL;
}

#define list_entry(ptr, type, member) \
  container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
  list_entry((ptr)->next, type, member)

/**
 * copied from linux kernel sources
 *
 * list_for_each - iterate over a list
 * @pos:  the &struct list_head to use as a loop cursor.
 * @head: the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * copied from linux kernel sources
 *
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:  the &struct list_head to use as a loop cursor.
 * @n:    another &struct list_head to use as temporary storage
 * @head: the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

#define LIST_HEAD_INITIALIZER(name) \
  { .prev = &(name), .next = &(name) }

#endif // _LIST_H_
