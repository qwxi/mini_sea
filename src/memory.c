#include "memory.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void   init(data *head, void *addptr, int size, int num)
{
     int i ;
     head->cursor=-1; 
     for(i=0;i<num;i++)
     {
         head->list[i] = addptr + size * i;
         head->cursor++;       
     }
     return ;  
}
void * getone(data *head)
{
     if(head->cursor == -1) 
         return NULL;
     return head->list[head->cursor--];
}
void   putone(data *head, void *ptr)
{
     head->list[++head->cursor] = ptr;
     return;
}


typedef struct{
    int i;
    char c;
    long l;
}test;



int main()
{
    data head ;
    memset(&head, 0, sizeof(head));

    void *addr = malloc(sizeof(test)*1000 );  

    init(&head, addr, sizeof(test), 1000);

    test *ptr = getone(&head);
    ptr->i = 10;
    ptr->c = 'c';
    ptr->l = 9;

    putone(&head, ptr);
    ptr = getone(&head);
    printf("%d, %c,%ld\n", ptr->i,ptr->c,ptr->l);

    return 0;
}
