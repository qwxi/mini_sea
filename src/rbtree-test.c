#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rbtree.h"

struct myfd{
    int fd;
    struct myfd *fdptr;
};

/*
    unsigned long key;
    struct myfd fdlist;   
*/

struct mytype{
    struct rb_node node;
    char *keystring;
};

struct rb_root mytree = RB_ROOT;

struct mytype *my_search(struct rb_root *root, char *string)
{
        struct rb_node *node = root->rb_node;

        while (node) {
                struct mytype *data = container_of(node, struct mytype, node);
                int result;

                result = strcmp(string, data->keystring);

                if (result < 0)
                        node = node->rb_left;
                else if (result > 0)
                        node = node->rb_right;
                else
                        return data;
        }
        return NULL;  
}

int my_insert(struct rb_root *root, struct mytype *data)
{
        struct rb_node **new = &(root->rb_node), *parent = NULL;

        /* Figure out where to put new node */
        while (*new) {
                struct mytype *this = container_of(*new, struct mytype, node);
                int result = strcmp(data->keystring, this->keystring);

                parent = *new;
                if (result < 0)
                        new = &((*new)->rb_left);
                else if (result > 0)
                        new = &((*new)->rb_right);
                else
                        return 0;
        }

        /* Add new node and rebalance tree. */
        rb_link_node(&data->node, parent, new);
        rb_insert_color(&data->node, root);

        return 1;
}

#define NUM 10

int main()
{

    struct mytype *mn[NUM];

    int i = 0;
    for(;i<NUM;i++)
    {
        mn[i] = malloc(sizeof(struct mytype)) ;
        mn[i]->keystring = malloc(sizeof(char)*4);
        sprintf(mn[i]->keystring, "%d", i);
        my_insert(&mytree, mn[i]);
    }

    struct rb_node *node;
    for(node=rb_first(&mytree);node;node=rb_next(node)) 
          printf("key=[%s]\n", rb_entry(node, struct mytype, node)->keystring);

    struct mytype *data = my_search(&mytree, "5");
    if(data)
    {
           rb_erase(&data->node, &mytree);
    }

    printf("----------------\n");

    for(node=rb_first(&mytree);node;node=rb_next(node)) 
          printf("key=[%s]\n", rb_entry(node, struct mytype, node)->keystring);

    return 0;
}
