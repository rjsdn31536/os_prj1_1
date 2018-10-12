#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "lib/user/syscall.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include "process.h"
#include "threads/malloc.h"
#include "threads/vaddr.h"
#include <string.h>

static void syscall_handler (struct intr_frame *);
void check_vaddr(const void *vaddr);

void halt (void) // SYS_HALT num = 0 - syscall 0 
{
	shutdown_power_off();
}

void exit (int status) // SYS_EXIT num = 1 - syscall 1 
{
	struct thread *current = thread_current();
	struct list_elem *prev;
	struct thread *entry_prev;
	current->lifeflag = 0;
	prev = list_prev(&(current->allelem));
	entry_prev = list_entry(prev,struct thread,allelem);
	entry_prev->child_status = status;
	
	printf("%s: exit(%d)\n",thread_name(),status);
	thread_exit();
}
pid_t exec(const char *cmd_line) // SYS_EXEC num = 2 - syscall 1
{
	return process_execute(cmd_line);
}

int wait (pid_t pid) // SYS_WAIT num = 3 - syscall 1
{
	return process_wait(pid);
}

int read (int fd, void *buffer, unsigned size) // SYS_READ num = 8 - syscall 3
{
	int i;
	if(fd == 0)
	{
		for(i=0;i<size;i++)
			*(char*)buffer =  input_getc();
		return i;

	}
	return -1;
}

int write (int fd, const void *buffer, unsigned size) // SYS_WRITE num = 9 - syscall 3
{
	if(fd == 1)
	{
		putbuf(buffer, size);
		return size;
	}
	return -1;
}

int pib(int n)
{
	int cnt = 1;
	int a0 = 1,a1=1,temp,i;
	if(n==1 || n == 2)
		return 1;
	else
	{
		for(i=3;i<=n;i++)
		{
			temp = a1;
			a1 += a0;
			a0 = temp;
		}
		return a1;
	}
}

int sofi(int a,int b, int c,int d)
{
	return a+b+c+d;
}

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}



static void
syscall_handler (struct intr_frame *f) 
{
	
  // printf(" number : %d\n",*(int*)(f->esp));
   // hex_dump(f->esp,f->esp,100,true);
	
  if(*(int*)f->esp == SYS_HALT) // SYS_HALT
  {
	  halt();
  }
  else if(*(int*)f->esp == SYS_EXIT) // SYS_EXIT
  {
	  check_vaddr(f->esp+4);
	  exit(*(int*)(f->esp+4));
  }
  else if(*(int*)f->esp == SYS_EXEC) // SYS_EXEC
  {
	  check_vaddr(f->esp+4);
	  f->eax = exec(*(char**)(f->esp+4));
  }
  else if(*(int*)f->esp == SYS_WAIT) // SYS_WAIT
  {
	  check_vaddr(f->esp+4);
	  f->eax = wait(*(pid_t*)(f->esp+4));
  }
  else if(*(int*)f->esp == SYS_READ){ // SYS_READ
	  check_vaddr(f->esp+4);
	  check_vaddr(f->esp+8);
	  check_vaddr(f->esp+12);
	  f->eax = read(*(int*)(f->esp+4),(void*)*(int*)(f->esp+8),(unsigned )*(int*)(f->esp+12));
  }
  else if(*(int*)f->esp == SYS_WRITE){ // SYS_WRITE
	  check_vaddr(f->esp+4);
	  check_vaddr(f->esp+8);
	  check_vaddr(f->esp+12);
	  f->eax = write(*(int*)(f->esp+4),(void*)*(int*)(f->esp+8),(unsigned )*(int*)(f->esp+12));
  }
  else if( *(int*)f->esp == SYS_PIB){
	  check_vaddr(f->esp+4);
	  f->eax = pib(*(int*)(f->esp+4));
  }
  else if( *(int*)f->esp == SYS_SOFI){
	  check_vaddr(f->esp+4);
	  check_vaddr(f->esp+8);
	  check_vaddr(f->esp+12);
	  f->eax = sofi(*(int*)(f->esp+4),*(int*)(f->esp+8),*(int*)(f->esp+12),*(int*)(f->esp+16));
  }
}


void check_vaddr(const void *vaddr){
	if(!is_user_vaddr(vaddr))
		exit(-1);
}
