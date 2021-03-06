/***********************************************************
* Author: Jamie Loebe
* Email: loebej@oregonstate.edu
* Date Created: 04/24/19
* Filename: linkedList.c
*
* Overview:
*   This program is a linked list implementation of the deque 
*   (as worked on in Worksheet 19) and bag (as worked on in 
*   Worksheet 22) ADTs. 
*   The deque ADT allows for the following behavior:
*       - adding a new link to the front/back
*       - getting the value of the front/back links
*       - removing the front/back link
*   The bag ADT allows for the following behavior:
*       - adding a new link
*       - checking if a link exists with a given value
*       - removing a link  with a given value if it exists
*   Both allow for:
*       - checking if empty
*       - printing the values of all of the links
*
*   Note that both implementations utilize a linked list with
*   both a front and back sentinel and double links (links with
*   next and prev pointers).
************************************************************/
#include "linkedList.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef FORMAT_SPECIFIER
#define FORMAT_SPECIFIER "%d"
#endif

// Double link
struct Link
{
    TYPE value;
    struct Link* next;
    struct Link* prev;
};

// Double linked list with front and back sentinels
struct LinkedList
{
    struct Link* frontSentinel;
    struct Link* backSentinel;
    int size;
};

/**
    Allocates the list's sentinel and sets the size to 0.
    The sentinels' next and prev should point to eachother or NULL
    as appropriate.
    param:  list    struct LinkedList ptr
    pre:    list is not null
    post:   list front and back sentinel not null
            front sentinel next points to back
            front sentinel prev points to null
            back sentinel prev points to front
            back sentinel next points to null
            list size is 0
 */
static void init(struct LinkedList* list) {

    struct Link *start;
    struct Link *end;

    start = (struct Link *) malloc (sizeof(struct Link));
    end = (struct Link *) malloc (sizeof(struct Link));
    
    start->next = end;
    end->prev = start;
    
    list->frontSentinel = start;
    list->backSentinel = end;
    
    start->prev = NULL;
    end->next = NULL;
    
    list->size = 0;
}

/**
    Adds a new link with the given value before the given link and
    increments the list's size.
    param:  list    struct LinkedList ptr
    param:  link    struct Link ptr
    param:  TYPE
    pre:    list and link are not null
    post:   newLink is not null
            newLink w/ given value is added before param link
            list size is incremented by 1
 */
static void addLinkBefore(struct LinkedList* list, struct Link* link, TYPE value)
{
    assert(list != 0);
    assert(link != 0);
    
    struct Link *newLink = (struct Link *) malloc (sizeof(struct Link));
    assert (newLink != NULL);
    
    newLink->value = value;
    newLink->next = link;
    newLink->prev = link->prev;
    link->prev->next = newLink;
    link->prev = newLink;
    list->size++;
    
}

/**
    Removes the given link from the list and
    decrements the list's size.
    param:  list    struct LinkedList ptr
    param:  link    struct Link ptr
    pre:    list and link are not null
    post:   param link is removed from param list
            memory allocated to link is freed
            list size is decremented by 1
 */
static void removeLink(struct LinkedList* list, struct Link* link)
{
    assert(link != 0);
    assert(list != 0);
    
    link->prev->next = link->next;
    link->next->prev = link->prev;
    free(link);
    list->size--;
    
    
}

/**
    Allocates and initializes a list.
    pre:    none
    post:   memory allocated for new struct LinkedList ptr
            list init (call to init func)
    return: list
 */
struct LinkedList* linkedListCreate()
{
    struct LinkedList* list = malloc(sizeof(struct LinkedList));
    init(list);
    return list;
}

/**
    Deallocates every link in the list including the sentinels,
    and frees the list itself.
    param:  list    struct LinkedList ptr
    pre:    list is not null
    post:   memory allocated to each link is freed
            " " front and back sentinel " "
            " " list " "
 */
void linkedListDestroy(struct LinkedList* list)
{
    assert(list != NULL);
    while (!linkedListIsEmpty(list)) {
        linkedListRemoveFront(list);
    }
    free(list->frontSentinel);
    free(list->backSentinel);
    free(list);
    list = NULL;
}

/**
    Adds a new link with the given value to the front of the deque.
    param:  deque   struct LinkedList ptr
    param:  value   TYPE
    pre:    deque is not null
    post:   link is created w/ param value stored before current first link
            (call to addLinkBefore)
 */
void linkedListAddFront(struct LinkedList* deque, TYPE value)
{
    assert (deque != 0);
    
    /* struct link *newLink = (struct link *) malloc (sizeof(struct link)); 
    addLinkBefore already allocates memory for new link */
    
    addLinkBefore(deque, deque->frontSentinel->next, value);
}

/**
    Adds a new link with the given value to the back of the deque.
    param:  deque   struct LinkedList ptr
    param:  value   TYPE
    pre:    deque is not null
    post:   link is created with given value before current last link
            (call to addLinkBefore)
 */
void linkedListAddBack(struct LinkedList* deque, TYPE value)
{
    assert (deque != 0);
    
    addLinkBefore(deque, deque->backSentinel, value); /*We are adding BEFORE back-sentinel, hence why we do not use prev. */
}

/**
    Returns the value of the link at the front of the deque.
    param:  deque   struct LinkedList ptr
    pre:    deque is not null
    pre:    deque is not empty
    post:   none
    ret:    first link's value 
 */
TYPE linkedListFront(struct LinkedList* deque)
{
    assert (deque != 0);
    assert (deque->size != 0);
    
    return deque->frontSentinel->next->value;
}

/**
    Returns the value of the link at the back of the deque.
    param:  deque   struct LinkedList ptr
    pre:    deque is not null
    pre:    deque is not empty
    post:   none
    ret:    last link's value 
 */
TYPE linkedListBack(struct LinkedList* deque)
{
    assert (deque != 0);
    assert (deque->size != 0);
    
    return deque->backSentinel->prev->value;
}

/**
    Removes the link at the front of the deque.
    param:  deque   struct LinkedList ptr
    pre:    deque is not null
    pre:    deque is not empty
    post:   first link is removed and freed (call to removeLink)
 */
void linkedListRemoveFront(struct LinkedList* deque)
{
    assert (deque != 0);
    assert (deque->size != 0);
    
    removeLink(deque, deque->frontSentinel->next);
}

/**
    Removes the link at the back of the deque.
    param:  deque   struct LinkedList ptr
    pre:    deque is not null
    pre:    deque is not empty
    post:   last link is removed and freed (call to removeLink)
 */
void linkedListRemoveBack(struct LinkedList* deque)
{
    assert (deque != 0);
    assert (deque->size != 0);
    
    removeLink(deque, deque->backSentinel->prev);
}

/**
    Returns 1 if the deque is empty and 0 otherwise.
    param:  deque   struct LinkedList ptr
    pre:    deque is not null
    post:   none
    ret:    1 if its size is 0 (empty), otherwise 0 (not empty)
 */
int linkedListIsEmpty(struct LinkedList* deque)
{
    assert (deque != 0);
    
    if(deque->frontSentinel->next == deque->backSentinel)
    {
        return 1;
    }
    
    else
        return 0;
}

/**
    Prints the values of the links in the deque from front to back.
    param:  deque   struct LinkedList ptr
    pre:    deque is not null
    post:   none
    ret:    outputs to the console the values of the links from front 
            to back; if empty, prints msg that is empty
 */
void linkedListPrint(struct LinkedList* deque)
{
    assert (deque != NULL);
            
    struct Link *current = deque->frontSentinel->next; //Pointer for current value
    assert(current != 0);
    
    if(current == deque->backSentinel)
    {
        printf("List is empty!!");
    }
    
    while(current != deque->backSentinel) //While list is NOT empty
    {
        printf(" %d ", current->value); //Print current value
        current = current->next; //Increment current to next value
    }
    
}

/**
    Adds a link with the given value to the bag.
    param:  bag     struct LinkedList ptr
    param:  value   TYPE
    pre:    bag is not null
    post:   link is created w/ given value before current first link
            (call to addLinkBefore)
            Note that bag doesn't specify where new link should be added;
            can be anywhere in bag according to its ADT.
 */
void linkedListAdd(struct LinkedList* bag, TYPE value)
{
    assert(bag != NULL);
    
    addLinkBefore(bag, bag->frontSentinel->next, value);
}

/**
    Returns 1 if a link with the value is in the bag and 0 otherwise.
    param:  bag     struct LinkedList ptr
    param:  value   TYPE
    pre:    bag is not null
    post:   none
    ret:    1 if link with given value found; otherwise, 0
 */
int linkedListContains(struct LinkedList* bag, TYPE value)
{
    assert(bag != NULL);
    
    struct Link *current = bag->frontSentinel;
    
    while(bag->frontSentinel->next != bag->backSentinel)
    {
        if (current->value != value)
        {
            current = current->next;
        }
        else if(current->value == value)
        {
            return 1;
        }
    }
    
    return 0;
}

/**
    Removes the first occurrence of a link with the given value.
    param:  bag     struct LinkedList ptr
    param:  value   TYPE
    pre:    bag is not null
    post:   if link with given value found, link is removed
            (call to removeLink)
 */
void linkedListRemove(struct LinkedList* bag, TYPE value)
{
    assert(bag != NULL);
    
    struct Link *current = bag->frontSentinel->next;
    
    while(current != bag->backSentinel)
    {
        if(current->value == value)
        {
            removeLink(bag, current);
        }
        else
            current = current->next;
    }
    
}

