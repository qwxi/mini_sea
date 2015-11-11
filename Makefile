
CC     = gcc
CFLAGS =  -W -Wall -Wpointer-arith -Wno-unused-parameter -Werror -g
LINK   = $(CC) 


CORE_DEPS = src/mini_sea_data.h \
	src/mini_sea_fdpair.h \
	src/mini_sea_log.h \
	src/mini_sea_queue.h \
	src/mini_sea_shm.h \
	src/mini_sea_socket.h \
	src/mini_sea_sig.h \
	src/mini_sea_core.h \
	src/mini_sea_task.h


CORE_INCS = -I src


objs/mini_sea:	objs/mini_sea.o \
	objs/mini_sea_log.o  \
	objs/mini_sea_fdpair.o \
	objs/mini_sea_queue.o \
	objs/mini_sea_shm.o \
	objs/mini_sea_socket.o \
	objs/mini_sea_sig.o \
	objs/mini_sea_core.o \
	objs/mini_sea_task.o


	$(LINK) -o objs/mini_sea \
	objs/mini_sea.o \
	objs/mini_sea_log.o  \
	objs/mini_sea_fdpair.o \
	objs/mini_sea_queue.o \
	objs/mini_sea_shm.o \
	objs/mini_sea_socket.o \
	objs/mini_sea_sig.o \
	objs/mini_sea_core.o \
	objs/mini_sea_task.o


objs/mini_sea.o:	$(CORE_DEPS) src/mini_sea.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea.o src/mini_sea.c

objs/mini_sea_log.o:	$(CORE_DEPS) src/mini_sea_log.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_log.o src/mini_sea_log.c

objs/mini_sea_fdpair.o:	$(CORE_DEPS) src/mini_sea_fdpair.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_fdpair.o src/mini_sea_fdpair.c

objs/mini_sea_queue.o:	$(CORE_DEPS) src/mini_sea_queue.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_queue.o src/mini_sea_queue.c

objs/mini_sea_shm.o:	$(CORE_DEPS) src/mini_sea_shm.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_shm.o src/mini_sea_shm.c

objs/mini_sea_socket.o:	$(CORE_DEPS) src/mini_sea_socket.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_socket.o src/mini_sea_socket.c

objs/mini_sea_task.o:	$(CORE_DEPS) src/mini_sea_task.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_task.o src/mini_sea_task.c

objs/mini_sea_core.o:	$(CORE_DEPS) src/mini_sea_core.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_core.o src/mini_sea_core.c

objs/mini_sea_sig.o:	$(CORE_DEPS) src/mini_sea_sig.c
	$(CC) -c $(CFLAGS) $(CORE_INCS) -o objs/mini_sea_sig.o src/mini_sea_sig.c

clean:	
	rm -rf log/*
	rm -rf objs/*
	rm -rf bin/mini_sea

install:
	mv objs/mini_sea bin/
