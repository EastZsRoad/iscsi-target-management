CC=gcc

CFLAGS=-Wall -g
iscsitest:iscsi_manage.o iscsi_manage_test.o
	$(CC) -o iscsitest iscsi_manage.o iscsi_manage_test.o -lpython2.7
iscsi_manage.o:iscsi_manage.c iscsi_manage.h
	$(CC) -c $(CFLAGS) iscsi_manage.c
iscsi_manage_test.o:iscsi_manage_test.c iscsi_manage.h
	$(CC) -c $(CFLAGS) iscsi_manage_test.c

clean:
	rm iscsitest iscsi_manage.o iscsi_manage_test.o
 