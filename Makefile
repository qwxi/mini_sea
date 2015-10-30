
CC     = gcc
CFLAGS =  -W -Wall -Wpointer-arith -Wno-unused-parameter -Werror -g
LINK   = $(CC) 

CORE_DEPS = src/mini_sea.h \
	src/mini_sea_func.h \
	src/rbtree.h \
	src/memory.h

CORE_INCS = -I src

objs/mini_sea:	objs/mini_sea.o \
	objs/mini_sea_func.o \
	objs/rbtree.o \
	objs/memory.o \
	objs/mini_sea_task.o

	$(LINK) -o objs/mini_sea \
	objs/mini_sea.o \
	objs/mini_sea_func.o \
	objs/rbtree.o \
	objs/memory.o


	$(LINK) -o objs/mini_sea_task \
	objs/mini_sea_task.o \
	objs/rbtree.o \
	objs/mini_sea_func.o


objs/mini_sea.o:	$(CORE_DEPS) src/mini_sea.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea.o src/mini_sea.c

objs/mini_sea_func.o:	$(CORE_DEPS) src/mini_sea_func.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_func.o src/mini_sea_func.c

objs/rbtree.o:		$(CORE_DEPS) src/rbtree.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/rbtree.o src/rbtree.c

objs/memory.o:		$(CORE_DEPS) src/memory.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/memory.o src/memory.c

objs/mini_sea_task.o:	$(CORE_DEPS) src/mini_sea_task.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_task.o src/mini_sea_task.c

clean:	
	rm -rf objs/*