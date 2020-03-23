#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef LIST_ENTRY(node) listPiece;

struct node{
    listPiece lp;
    int n;
};

LIST_HEAD(lhead, node);

struct node *getNode(int n) {
    struct node* pNode = (struct node *)malloc(sizeof(struct node));
    pNode->n = n;
    // ! macro won't set these for us
    pNode->lp.le_next = NULL;
    pNode->lp.le_prev = NULL;

    return pNode;
}

void printList(struct lhead *head, char *str) {
    printf("<%s>\n", str);
    struct node *pNode = NULL;
    LIST_FOREACH(pNode, head, lp) {
        printf("[%d] ", pNode->n);
    }
    printf("\n");
    printf("===========================\n\n");
}

int main(void) {
    struct lhead* head = (struct lhead*)malloc(sizeof(struct lhead));
    LIST_INIT(head);
    printList(head, "Empty");

    struct node *pNode = getNode(0);
    LIST_INSERT_HEAD(head, pNode, lp);
    printList(head, "Only one");
    
    /* Segment Fault
    LIST_INSERT_BEFORE(head->lh_first, getNode(1), lp);
    printList(head, "Try Insert Before First");
    */

    pNode = getNode(123);
    LIST_INSERT_TAIL(head, pNode, lp);
    printList(head, "1 elem tail");

    struct node *mid = getNode(125);
    LIST_INSERT_TAIL(head, mid, lp);
    printList(head, "2 elem tail");

    pNode = getNode(135);
    LIST_INSERT_AFTER(mid, pNode, lp);
    printList(head, "insert after tail");

    pNode = getNode(130);
    LIST_INSERT_AFTER(mid, pNode, lp);
    printList(head, "insert after mid");

    head->lh_first = NULL;
    LIST_INSERT_TAIL(head, pNode, lp);
    printList(head, "insert tail in an empty list");

    return 0;
}
