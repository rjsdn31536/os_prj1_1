#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "lib/user/syscall.h"

void syscall_init (void);
void halt (void);
void exit (int);
pid_t exec(const char *cmd_line);
int wait (pid_t pid);
int read (int fd, void *buffer, unsigned size);
int write (int fd, const void *buffer, unsigned size);
int pib(int n);
int sofi(int a,int b,int c,int d);

#endif /* userprog/syscall.h */
