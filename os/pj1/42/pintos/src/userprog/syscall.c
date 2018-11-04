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

int pib(int n)
{
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

//========================== project 2 =======================================

int read (int fd, void *buffer, unsigned size) // SYS_READ num = 8 - syscall 3
{
	int i;
	struct thread* now = thread_current();
	check_vaddr(buffer);
	if(fd == 0)
	{
		for(i=0;i<(int)size;i++)
			*(char*)buffer =  input_getc();
		return i;
	}
	else{
		return file_read(now->fd[fd], buffer, size);
	}
	return -1;
}

int write (int fd, const void *buffer, unsigned size) // SYS_WRITE num = 9 - syscall 3
{
	struct thread* now = thread_current();
	int tmp;

	check_vaddr(buffer);
	if(fd == 1)
	{
		putbuf(buffer, size);
		return size;
	}
	else{
		tmp =  file_write(now->fd[fd], buffer, size);

		return tmp;
	}
	return -1;
}

bool create ( const char *file, unsigned initial_size)
{
	if(file == NULL)
		exit(-1);
	return filesys_create(file,initial_size);	
}

bool remove ( const char *file)
{
	return filesys_remove(file);
}

int open ( const char *file)
{
	if(file == NULL)
		exit(-1);
	struct file* fp = filesys_open(file);
	struct thread* now = thread_current();
	int i;
	int flag = 0;
	if(fp == NULL)
		return -1;

	for(i=2; i<128; i++){
		if(now->fd[i] == NULL){
			now->fd[i] = fp;
			flag = 1;
		//	if(strcmp(now->name,file) == 0)
//				file_deny_write(now->fd[i]);
			break;
		}
	}
	if(flag == 1)
		return i;
	else
		return -1;
}

int filesize (int fd)
{
	struct thread* now = thread_current();
	return file_length(now->fd[fd]);
}

void seek(int fd, unsigned position)
{
	struct thread* now = thread_current();
	
	file_seek(now->fd[fd], position);
}

unsigned tell(int fd)
{
	struct thread* now = thread_current();

	return file_tell(now->fd[fd]);
}

void close(int fd)
{
	struct thread* now = thread_current();
	if(now->fd[fd] == NULL)
		exit(-1);
	file_close(now->fd[fd]);
	now->fd[fd] = NULL;
}

//======================================================================================


void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}



static void
syscall_handler (struct intr_frame *f) 
{
  //printf("number : %d\n",*(int*)(f->esp));	
	
  if(*(int*)f->esp == SYS_HALT) // SYS_HALT : 0
  {
	  halt();
  }
  else if(*(int*)f->esp == SYS_EXIT) // SYS_EXIT : 1
  {
	  check_vaddr(f->esp+4);
	  exit(*(int*)(f->esp+4));
  }
  else if(*(int*)f->esp == SYS_EXEC) // SYS_EXEC : 2
  {
	  check_vaddr(f->esp+4);
	  f->eax = exec(*(char**)(f->esp+4));
  }
  else if(*(int*)f->esp == SYS_WAIT) // SYS_WAIT :3
  {
	  check_vaddr(f->esp+4);
	  f->eax = wait(*(pid_t*)(f->esp+4));
  }
  else if(*(int*)f->esp == SYS_READ){ // SYS_READ : 8
	  check_vaddr(f->esp+4);
	  check_vaddr(f->esp+8);
	  check_vaddr(f->esp+12);
	  f->eax = read(*(int*)(f->esp+4),(void*)*(int*)(f->esp+8),(unsigned )*(int*)(f->esp+12));
  }
  else if(*(int*)f->esp == SYS_WRITE){ // SYS_WRITE : 9
	  check_vaddr(f->esp+4);
	  check_vaddr(f->esp+8);
	  check_vaddr(f->esp+12);
	  f->eax = write(*(int*)(f->esp+4),(void*)*(int*)(f->esp+8),(unsigned )*(int*)(f->esp+12));
  }
  else if( *(int*)f->esp == SYS_PIB){  // SYS_PIB : 10
	  check_vaddr(f->esp+4);
	  f->eax = pib(*(int*)(f->esp+4));
  }
  else if( *(int*)f->esp == SYS_SOFI){ // SYS_SOFI : 11
	  check_vaddr(f->esp+4);
	  check_vaddr(f->esp+8);
	  check_vaddr(f->esp+12);
	  check_vaddr(f->esp+16);
	  f->eax = sofi(*(int*)(f->esp+4),*(int*)(f->esp+8),*(int*)(f->esp+12),*(int*)(f->esp+16));
  }
  else if( *(int*)f->esp == SYS_CREATE){ // SYS_CREATE : 4
	  check_vaddr(f->esp+4);
	  check_vaddr(f->esp+8);
	  f->eax = create((char*)*(int*)(f->esp+4),(unsigned)*(int*)(f->esp+8));
  }
  else if( *(int*)f->esp == SYS_REMOVE){ // SYS_REMOVE : 5
	  check_vaddr(f->esp+4);
	  f->eax = remove((char*)*(int*)(f->esp+4));
		
  }
  else if( *(int*)f->esp == SYS_OPEN){ // SYS_OPEN : 6
	  check_vaddr(f->esp+4);
	  f->eax = open((char*)*(int*)(f->esp+4));

  }
  else if( *(int*)f->esp == SYS_FILESIZE){ // SYS_FILESIZE : 7
	  check_vaddr(f->esp+4);
	  f->eax = filesize(*(int*)(f->esp+4));

  }
  else if( *(int*)f->esp == SYS_SEEK){ // SYS_SEEK : 12
	  check_vaddr(f->esp+4);
	  check_vaddr(f->esp+8);
	  seek(*(int*)(f->esp+4), (unsigned)*(int*)(f->esp+8));
  }
  else if( *(int*)f->esp == SYS_TELL){ // SYS_TELL : 13
	  check_vaddr(f->esp+4);
	  f->eax = tell(*(int*)(f->esp+4));
  }
  else if( *(int*)f->esp == SYS_CLOSE){ // SYS_CLOSE : 14
	  check_vaddr(f->esp+4);
	  close(*(int*)(f->esp+4));
  }
}


void check_vaddr(const void *vaddr){
	if(is_user_vaddr(vaddr) == false)
		exit(-1);
}
