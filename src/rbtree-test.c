#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "rbtree.h"

struct myfd{
    int fd;
    struct myfd *next;
};

typedef time_t rb_key;

struct mytype{
    struct rb_node node;
    rb_key key;
    struct myfd *list;
/*
    char *keystring;
*/
};

struct rb_root mytree = RB_ROOT;

struct mytype *my_search(struct rb_root *root, rb_key key)
{
        time_t  result;
        struct rb_node *node = root->rb_node;

        while (node) {
                struct mytype *data = container_of(node, struct mytype, node);

                result = key - data->key;

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
        time_t result;
        struct rb_node **new = &(root->rb_node), *parent = NULL;

        /* Figure out where to put new node */
        while (*new) {
                struct mytype *this = container_of(*new, struct mytype, node);

                result = data->key - this->key;

                parent = *new;
                if (result < 0)
                        new = &((*new)->rb_left);
                else if (result > 0)
                        new = &((*new)->rb_right);
                else{
                        if(this->list == NULL)
                        {
                            this->list = data->list;                             
                        }else{
                            struct myfd *ptr = this->list; 
                            while(ptr->next != NULL)
                            {
                                ptr = ptr->next;
                            }
                            ptr->next = data->list;
                        }
                        return 0;
                }
        }

        /* Add new node and rebalance tree. */
        rb_link_node(&data->node, parent, new);
        rb_insert_color(&data->node, root);

        return 1;
}

#define NUM 100

int main()
{

    struct timeval tv;
    struct mytype *mn[NUM];
    struct mytype m[NUM];
    struct myfd   f[NUM];


    gettimeofday(&tv, NULL);
    printf("%lu\n", tv.tv_sec);

    int i = 0;
    for(;i<NUM;i++)
    {
        mn[i] = &(m[i]) ;

        gettimeofday(&tv, NULL);

        mn[i]->key = tv.tv_sec;

        mn[i]->list = &(f[i]);

        memset(mn[i]->list, 0, sizeof(struct myfd));
        mn[i]->list->fd = i;
        

        my_insert(&mytree, mn[i]);
    }
 
    gettimeofday(&tv, NULL);
    printf("%lu\n", tv.tv_sec);
    printf("Start to Count...\n");
    struct rb_node *node;
    for(node=rb_first(&mytree);node;node=rb_next(node)) 
    {
    printf("key=[%lu]", rb_entry(node, struct mytype, node)->key);
    printf("fdlist = [");

        struct myfd *ptr = rb_entry(node, struct mytype, node)->list;
        if(ptr == NULL)
        {
           printf(" ");
        }else{
            while(ptr != NULL)
            {
                printf(" %d ", ptr->fd);
                ptr = ptr->next;
            }
        }

        printf(" ] \n");
    }
    gettimeofday(&tv, NULL);
    printf("%lu\n", tv.tv_sec);

    struct mytype *data = my_search(&mytree, tv.tv_sec);
    if(data)
    {
           rb_erase(&data->node, &mytree);
    }

    printf("-------------------------------------\n");

    for(node=rb_first(&mytree);node;node=rb_next(node)) 
          printf("key=[%lu]\n", rb_entry(node, struct mytype, node)->key);


    return 0;
}
