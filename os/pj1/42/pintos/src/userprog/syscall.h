#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "lib/user/syscall.h"
#include "filesys/off_t.h"

/*============= project 1 ================*/
void syscall_init (void);
void halt (void);
void exit (int);
pid_t exec(const char *cmd_line);
int wait (pid_t pid);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
int pib(int n);
int sofi(int a,int b,int c,int d);

/*============== project 2================*/

struct file
{
	struct inode *indooe;
	off_t pos;
	bool deny_write;
};

bool create(const char *file, unsigned initial_size);
bool remove(const char *file);
int open(const char *file);
int filesize(int fd);
void seek(int fd,unsigned position);
unsigned tell(int fd);
void close(int fd);


#endif /* userprog/syscall.h */
