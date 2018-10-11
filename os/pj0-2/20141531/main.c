#include "main.h"

struct bitmap *bitmap_expand(struct bitmap *bm, int size)
{
	
	size_t total,cnt;
	struct bitmap *b;
	cnt = bitmap_size(bm);
	total = cnt+size;
	b = bitmap_create(total);
	// create expanded bitmap 
	int i;
	for(i=0;i<cnt;i++)
	{
		if(bitmap_test(bm,i) == true)
			bitmap_flip(b,i);
	}
	return b;
		
}

struct list *create_list()
{
	struct list *l;
	l = (struct list*)malloc(sizeof(struct list));
	return l;
}

struct list_item *create_list_item()
{
	struct list_item *l;
	l = (struct list_item*)malloc(sizeof(struct list_item));
	return l;
}

bool less(const struct list_elem *a, const struct list_elem *b, void *aux)
{
	struct list_item *a_item,*b_item;
	a_item = list_entry(a,struct list_item,elem);
	b_item = list_entry(b,struct list_item,elem);
	if( a_item->bar < b_item->bar)
		return true;
	else
		return false;
}

void list_swap(struct list_elem *a, struct list_elem *b)
{
	struct list_item *a_item;
	struct list_item *b_item;
	int tmp;
	a_item = list_entry(a,struct list_item,elem);
	b_item = list_entry(b,struct list_item,elem);

	tmp = a_item->bar;
	a_item->bar = b_item->bar;
	b_item->bar = tmp;
}

void list_shuffle(struct list *list)
{
	int anum,bnum,size,i,j;
	struct list_elem *a;
	struct list_elem *b;

	size = (int)list_size(list);
	srand((unsigned int)time(NULL));
	for(i=0;i<size;i++)
	{
		anum = rand()%size;
		bnum = rand()%size;

		a = list_begin(list);
		for(j=0;j<anum;j++)
			a = list_next(a);

		b = list_begin(list);
		for(j=0;j<bnum;j++)
			b = list_next(b);
		list_swap(a,b);
	}

}

struct hash_iterator *create_hash_iterator()
{
	struct hash_iterator *h;
	h = (struct hash_iterator*)malloc(sizeof(struct hash_iterator*));
	return h;
}

struct hash *create_hash()
{
	struct hash *h;
	h = (struct hash*)malloc(sizeof(struct hash));
	return h;
}

struct hash_item *create_hash_item()
{
	struct hash_item *h;
	h = (struct hash_item*)malloc(sizeof(struct hash_item));
	return h;
}

unsigned hf(const struct hash_elem *e, void *aux)
{
	struct hash_item *a;
	a = hash_entry(e,struct hash_item,elem);
	return hash_int(a->data);
}

bool hl(const struct hash_elem *a, const struct hash_elem *b, void *aux)
{
	struct hash_item *a_item,*b_item;
	a_item = hash_entry(a,struct hash_item,elem);
	b_item = hash_entry(b,struct hash_item,elem);

	if(a_item->data < b_item->data)
		return true;
	else
		return false;
}

void square(struct hash_elem *a, void *aux)
{
	struct hash_item *h;
	int tmp;
	h = hash_entry(a,struct hash_item,elem);
	tmp = h->data;
	h -> data = tmp*tmp;
}

void triple(struct hash_elem *a, void *aux)
{
	struct hash_item *h;
	int tmp;
	h = hash_entry(a,struct hash_item,elem);
	tmp = h->data;
	h -> data = tmp*tmp*tmp;
}

void hash_destructor(struct hash_elem *a, void *aux)
{
	free(a);
}

unsigned hash_int_2(int i)
{
	i = i * 123456;
	i = i / 3245;
	i = i * 112432;
	i = i % 43212354;
	return i;
}

int main()
{
	char input[50] = { "\0",},tmpinput[50] = {"\0",};
	int i,bcnt=0,hcnt=0,lcnt=0;
	char *ptr=NULL,*ptr2=NULL,*ptr3,*ptr4,*ptr5;
	int num,num2,data,lidx,lstart,llast;
	size_t idx , start,cnt,size,bl,total;
	struct list_elem *new_elem,*new_elem2,*new_elem3;
	struct list_item *new_item;
	struct hash_elem *h_elem;
	struct hash_item *h_item;
	struct hash_iterator h_iterator;


	while(1)
	{
		num = 0;
		fgets(input,sizeof(input),stdin);
		// get an input
		input[strlen(input)-1] = '\0';
		// add null end of input
		strcpy(tmpinput,input);
		// copy input
		ptr = strtok(tmpinput," ");
		// cut input

		if(strcmp(ptr,"create") == 0)
		{
			ptr = strtok(NULL," ");

			if(strcmp(ptr,"bitmap") == 0)
			{
				ptr = strtok(NULL," ");
				ptr2 = strtok(NULL," ");
				size_t bitsize;
				strcpy(bm[bcnt].name,ptr);
				// bitmap name copy
				sscanf(ptr2,"%zu",&bitsize);
				// change type string to size_t
				bm[bcnt].array = bitmap_create(bitsize);
				//bitmap create
				bcnt++;
			}
			else if(strcmp(ptr,"list") == 0)
			{
				ptr = strtok(NULL," ");
				strcpy(la[lcnt].name,ptr);
				// linked list name copy
				la[lcnt].link = create_list();
				list_init(la[lcnt].link);
				lcnt++;
			}
			else if(strcmp(ptr,"hashtable") == 0)
			{
				// create hash table
				ptr = strtok(NULL," ");
				strcpy(ha[hcnt].name,ptr);
				ha[hcnt].link = create_hash_iterator();
				ha[hcnt].link->hash = create_hash();
				hash_init(ha[hcnt].link->hash,hf,hl,NULL);
				hcnt++;

			}

		}
		else if(strcmp(ptr,"delete") == 0)
		{
			ptr = strtok(NULL," ");
			if(strncmp(ptr,"bm",2) == 0)
			{
				num = ptr[2] - 48;
				// extract bitmap's number
				bitmap_destroy(bm[num].array);
				// free bitmap
			}
			else if( strncmp(ptr,"list",4)==0)
			{
				size =  list_size(la[num].link);
				for(i=0;i<size;i++)
				{
					new_elem = list_begin(la[num].link);
					list_remove(new_elem);
				}	
			}
			else if( strncmp(ptr,"hash",4)==0)
			{
				num = ptr[4] - 48;
				hash_destroy(ha[num].link->hash,hash_destructor);
			}
		}
		else if(strcmp(ptr,"dumpdata") == 0)
		{
			ptr = strtok(NULL," ");
			if(strncmp(ptr,"bm",2)==0)
			{
				num = ptr[2]-48;
				bl = bitmap_size(bm[num].array);
				// print data in bitmap array
				for(i=0;i<bl;i++)
				{
					if(bitmap_test(bm[num].array,i) == 1)
						printf("1");
					else
						printf("0");
				}
				printf("\n");
			}
			else if(strncmp(ptr,"list",4)== 0)
			{
				num = ptr[4] - 48;
				size = list_size(la[num].link);
				// return list size
				if( size != 0)
				{
					new_elem= list_begin(la[num].link);
					// move list_begin to list_end , print data in list
					while(new_elem != list_tail(la[num].link))
					{
						new_item = list_entry(new_elem,struct list_item,elem);
						printf("%d ",new_item->bar);
						new_elem = list_next(new_elem);
					}
					printf("\n");
				}

			}
			else if(strncmp(ptr,"hash",4)== 0)
			{
				num = ptr[4] - 48;
				size = hash_size(ha[num].link->hash);
				if(size != 0)
				{
					hash_first(&h_iterator,ha[num].link->hash);
					while(hash_next(&h_iterator) != NULL)
					{
						h_item = hash_entry(hash_cur(&h_iterator),struct hash_item,elem);
						printf("%d ",h_item->data);
					}
					printf("\n");
				}
			
			}
		}
		else if(strcmp(ptr,"quit") == 0)
		{
			return 0;
		}
		//================== Bitmap ====================================//

		else if(strcmp(ptr,"bitmap_size") == 0)
		{
			// return the number of bit
			ptr = strtok(NULL," ");
			num = ptr[2] - 48;
			total = bitmap_size(bm[num].array);
			printf("%zu\n",total);

		}
		else if(strcmp(ptr,"bitmap_set") == 0)
		{
			// set the bit idxth in b to value
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&idx);
			ptr3[strlen(ptr3)] = '\0';
			if(strcmp(ptr3,"true")==0)
				bitmap_set(bm[num].array,idx,true);
			if(strcmp(ptr3,"false")==0)
				bitmap_set(bm[num].array,idx,false);

		}
		else if(strcmp(ptr,"bitmap_mark") == 0)
		{
			// change idxth bit
			ptr = strtok(NULL," " );
			num = ptr[2]-48;
			ptr = strtok(NULL," ");
			sscanf(ptr,"%zu",&idx);
			bitmap_mark(bm[num].array,idx);
			
		}
		else if(strcmp(ptr,"bitmap_expand") == 0)
		{
			// expand bitmap size as size
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&size);
			bm[num].array = bitmap_expand(bm[num].array,size);

		}
		else if(strcmp(ptr,"bitmap_reset") == 0)
		{
			//set idxth bit to false
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&idx);
			bitmap_reset(bm[num].array,idx);
			
		}
		else if(strcmp(ptr,"bitmap_flip") == 0)
		{
			// change idxth bits to reverse
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[2] -48;
			sscanf(ptr2,"%zu",&idx);
			bitmap_flip(bm[num].array,idx);
		}
		else if(strcmp(ptr,"bitmap_test") == 0)
		{
			// return the value of idxth bit
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&idx);
			if(bitmap_test(bm[num].array,idx) == true)
				printf("true\n");
			else if(bitmap_test(bm[num].array,idx) == false)
				printf("false\n");
		}
		else if(strcmp(ptr,"bitmap_set_all") == 0)
		{
			// all bits of bitmap set value(true or false)
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[2] - 48;
			ptr2[strlen(ptr2)] = '\0';
			if(strcmp(ptr2,"true") == 0)
				bitmap_set_all(bm[num].array,true);
			else if( strcmp(ptr2,"false") == 0)
				bitmap_set_all(bm[num].array,false);

		}
		else if(strcmp(ptr,"bitmap_set_multiple") == 0)
		{
			// set the cnt bits starting at start to value
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			ptr4 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&start);
			sscanf(ptr3,"%zu",&cnt);
			ptr4[strlen(ptr4)] = '\0';
			// cut by strtok string's end is not null
			if(strcmp(ptr4,"true") == 0)
				bitmap_set_multiple(bm[num].array,start,cnt,true);
			else if(strcmp(ptr4,"false") == 0)
				bitmap_set_multiple(bm[num].array,start,cnt,false);
		}
		else if(strcmp(ptr,"bitmap_count") == 0)
		{
			// return the number of bits between start ~ start+cnt same as value(true or false)
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			ptr4 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&start);
			sscanf(ptr3,"%zu",&cnt);
			ptr4[strlen(ptr4)] = '\0';
			// cut by strtok string's end is not null
			if(strcmp(ptr4,"true") == 0)
			{
				total = bitmap_count(bm[num].array,start,cnt,true);
				printf("%zu\n",total);
			}
			else if(strcmp(ptr4,"false") == 0)
			{
				total = bitmap_count(bm[num].array,start,cnt,false);
				printf("%zu\n",total);
			}
			

		}
		else if(strcmp(ptr,"bitmap_contains") == 0)
		{
			// return true if any bits between start ~ start + cnt same as value(true or false)
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			ptr4 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&start);
			sscanf(ptr3,"%zu",&cnt);
			ptr4[strlen(ptr4)] = '\0';
			if(strcmp(ptr4,"true") == 0)
			{
				if(bitmap_contains(bm[num].array,start,cnt,true) == true)
					printf("true\n");
				else
					printf("false\n");

			}
			else if(strcmp(ptr4,"false") == 0)
			{
				if(bitmap_contains(bm[num].array,start,cnt,false) == true)
					printf("true\n");
				else
					printf("false\n");
			}


		}
		else if(strcmp(ptr,"bitmap_any") == 0)
		{
			// check strat ~ start+cnt bit if any bits true
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&start);
			sscanf(ptr3,"%zu",&cnt);
			if(bitmap_any(bm[num].array,start,cnt) == true)
				printf("true\n");
			else
				printf("false\n");

		}
		else if(strcmp(ptr,"bitmap_none") == 0)
		{
			// return true if start ~ start + cnt bit is not true
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&start);
			sscanf(ptr3,"%zu",&cnt);
			if(bitmap_none(bm[num].array,start,cnt) == true)
				printf("true\n");
			else if(bitmap_none(bm[num].array,start,cnt) == false)
				printf("false\n");

		}
		else if(strcmp(ptr,"bitmap_all") == 0)
		{
			// check start ~ start+cnt bit is all true
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&start);
			sscanf(ptr3,"%zu",&cnt);
			if(bitmap_all(bm[num].array,start,cnt) == true)
				printf("true\n");
			else
				printf("false\n");


		}
		else if(strcmp(ptr,"bitmap_scan") == 0)
		{
			/* find and returns the starting index of the first group of cnt consecutive bits in b
			   at or after start */
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			ptr4 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&start);
			sscanf(ptr3,"%zu",&cnt);
			ptr4[strlen(ptr4)] = '\0';
			unsigned int total1;
			// return of bitmap_scan_and_flip is size_t , but size_t can't save -1 so save in unsigned int type
			if(strcmp(ptr4,"true") == 0)
				total1 = bitmap_scan(bm[num].array,start,cnt,true);
			else if(strcmp(ptr4,"false") == 0)
				total1 = bitmap_scan(bm[num].array,start,cnt,false);
			printf("%u\n",total1);


		}
		else if(strcmp(ptr,"bitmap_scan_and_flip") == 0)
		{
			/* find the first group of cnt consecutive bits in B at or after start 
			   that are all set to value, flip them all to !value */
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			ptr4 = strtok(NULL," ");
			num = ptr[2] - 48;
			sscanf(ptr2,"%zu",&start);
			sscanf(ptr3,"%zu",&cnt);
			ptr4[strlen(ptr4)] = '\0';
			unsigned int total1;
			// return of bitmap_scan_and_flip is size_t , but size_t can't save -1 so save in unsigned int type
			if(strcmp(ptr4,"true") == 0)
				total1 = bitmap_scan_and_flip(bm[num].array,start,cnt,true);
			else if(strcmp(ptr4,"false") == 0)
				total1 = bitmap_scan_and_flip(bm[num].array,start,cnt,false);
			printf("%u\n",total1);
		}
		else if(strcmp(ptr,"bitmap_dump") == 0)
		{
			// dump the contents in bitmap
			ptr = strtok(NULL," ");
			num = ptr[2] - 48;
			bitmap_dump(bm[num].array);

		}

		//================== List ====================================//

		else if(strcmp(ptr,"list_insert") == 0)
		{
			//insert elem just before before

			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			num = ptr[4] - 48;
			sscanf(ptr2,"%d",&lidx);
			sscanf(ptr3,"%d",&data);

			new_item = create_list_item();
			new_item->bar = data;
			size = list_size(la[num].link);

			if(list_head(la[num].link) == list_tail(la[num].link))
				list_push_back(la[num].link,&(new_item->elem));
			else
			{
				if(lidx == 0)
					list_push_front(la[num].link,&(new_item->elem));
				else
				{
					new_elem = list_begin(la[num].link);
					if(lidx >= size)
						list_push_back(la[num].link,&(new_item->elem));
					else
					{
						for(i=0;i<lidx;i++)
							new_elem = list_next(new_elem);
						list_insert(new_elem,&(new_item->elem));
					}
				}
			}
					
			
		}
		else if(strcmp(ptr,"list_splice") == 0)
		{
			// remove fisrt ~ last in list and insert before point
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");
			ptr4 = strtok(NULL," ");
			ptr5 = strtok(NULL," ");

			num = ptr[4] - 48;
			sscanf(ptr2,"%d",&lidx);
			num2 = ptr3[4] - 48;
			sscanf(ptr4,"%d",&lstart);
			sscanf(ptr5,"%d",&llast);
			
			new_elem = list_begin(la[num].link);
			for(i=0;i<lidx;i++)
				new_elem = list_next(new_elem);
			
			new_elem2 = list_begin(la[num2].link);
			for(i=0;i<lstart;i++)
				new_elem2 = list_next(new_elem2);
			
			new_elem3 = list_begin(la[num2].link);
			for(i=0;i<llast;i++)
				new_elem3 = list_next(new_elem3);

			list_splice(new_elem,new_elem2,new_elem3);
		}
		else if(strcmp(ptr,"list_push_front") == 0)
		{
			//insert elem at the front of list
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4]-48;
			sscanf(ptr2,"%d",&data);

			new_item = create_list_item();
			new_item->bar = data;
			list_push_front(la[num].link , &(new_item->elem));
		}
		else if(strcmp(ptr,"list_push_back") == 0)
		{
			//insert elem at the end of list
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4]-48;
			sscanf(ptr2,"%d",&data);

			new_item = create_list_item();
			new_item->bar = data;
			list_push_back(la[num].link , &(new_item->elem));
		}
		else if(strcmp(ptr,"list_remove") == 0)
		{
			// remove element in list
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4]-48;
			sscanf(ptr2,"%d",&lidx);

			if(lidx == 0)
			{
				new_elem = list_begin(la[num].link);
				list_remove(new_elem);
			}
			else
			{
				new_elem = list_begin(la[num].link);
				for(i=0;i<lidx;i++)
					new_elem = list_next(new_elem);
				list_remove(new_elem);
			}
		}
		else if(strcmp(ptr,"list_pop_front") == 0)
		{
			// remove front element in list
			ptr = strtok(NULL," ");
			num = ptr[4]-48;
			list_pop_front(la[num].link);
		}
		else if(strcmp(ptr,"list_pop_back") == 0)
		{
			// remove back element in list
			ptr = strtok(NULL," ");
			num = ptr[4]-48;
			list_pop_back(la[num].link);


		}
		else if(strcmp(ptr,"list_front") == 0)
		{
			// return the front element in list and print
			ptr = strtok(NULL," ");
			num = ptr[4]-48;

			new_elem = list_front(la[num].link);
			new_item = list_entry(new_elem,struct list_item,elem);
			printf("%d\n",new_item->bar);

		}
		else if(strcmp(ptr,"list_back") == 0)
		{
			// return the back element in list and print
			ptr = strtok(NULL," ");
			num = ptr[4]-48;

			new_elem = list_back(la[num].link);
			new_item = list_entry(new_elem,struct list_item,elem);
			printf("%d\n",new_item->bar);

		}
		else if(strcmp(ptr,"list_size") == 0)
		{
			// return list size
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;
			size = list_size(la[num].link);
			printf("%zu\n",size);


		}
		else if(strcmp(ptr,"list_empty") == 0)
		{
			// if list is empty return true 
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;
			if(list_empty(la[num].link) == true)
				printf("true\n");
			if(list_empty(la[num].link) == false)
				printf("false\n");

		}
		else if(strcmp(ptr,"list_reverse") == 0)
		{
			// reverse the list
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;

			list_reverse(la[num].link);
		}
		else if(strcmp(ptr,"list_sort") == 0)
		{
			// sort the list ascending
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;

			list_sort(la[num].link,less,NULL);
		}
		else if(strcmp(ptr,"list_insert_ordered") == 0)
		{
			// insert elem, which must be sorted according to less given data aux
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4] -48;
			sscanf(ptr2,"%d",&data);
			
			new_item = create_list_item();
			new_item->bar = data;
			list_insert_ordered(la[num].link,&(new_item->elem),less,NULL);
		}
		else if(strcmp(ptr,"list_unique") == 0)
		{
			/* iterates through list and removes all but the first in each set
			   of adjacent elements that are equal according to less */
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4] - 48;
			if(ptr2 == NULL)
				list_unique(la[num].link,NULL,less,NULL);
			else
			{
				num2 = ptr2[4] - 48;
				list_unique(la[num].link,la[num2].link,less,NULL);
			}

		}
		else if(strcmp(ptr,"list_max") == 0)
		{
			// find max element
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;
			
			new_elem = list_max(la[num].link,less,NULL);
			new_item = list_entry(new_elem,struct list_item,elem);
			printf("%d\n",new_item->bar);
		}
		else if(strcmp(ptr,"list_min") == 0)
		{
			// find min element
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;

			new_elem = list_min(la[num].link,less,NULL);
			new_item = list_entry(new_elem,struct list_item,elem);
			printf("%d\n",new_item->bar);
		}
		else if(strcmp(ptr,"list_swap") == 0)
		{
			// swap a and b element
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			ptr3 = strtok(NULL," ");

			num = ptr[4] - 48;
			sscanf(ptr2,"%d",&lstart);
			sscanf(ptr3,"%d",&llast);

			new_elem = list_begin(la[num].link);
			for(i=0;i<lstart;i++)
				new_elem = list_next(new_elem);
			
			new_elem2 = list_begin(la[num].link);
			for(i=0;i<llast;i++)
				new_elem2 = list_next(new_elem2);

			list_swap(new_elem,new_elem2);
		}
		else if(strcmp(ptr,"list_shuffle")== 0 )
		{
			// shuffle the list
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;
			list_shuffle(la[num].link);
			
		}

		//================== Hashtable ====================================//

		else if(strcmp(ptr,"hash_insert") == 0)
		{
			// insert element in hash table
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4] - 48;
			sscanf(ptr2,"%d",&data);
			
			h_item = create_hash_item();
			h_item->data = data;
			hash_insert(ha[num].link->hash,&(h_item->elem));

			
		}
		else if(strcmp(ptr,"hash_replace") == 0)
		{
			// insert new hash_elem in hashtable , replacing any equal element already in the table
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4] - 48;
			sscanf(ptr2,"%d",&data);

			h_item = create_hash_item();
			h_item ->data = data;

			hash_replace(ha[num].link->hash,&(h_item->elem));
		}
		else if(strcmp(ptr,"hash_find") == 0)
		{
			// find element in hash table
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4]-48;
			sscanf(ptr2,"%d",&lidx);

			h_item = create_hash_item();
			h_item->data = lidx;

			h_elem = hash_find(ha[num].link->hash,&(h_item->elem));
			if(h_elem != NULL)
				printf("%d\n",lidx);

		}
		else if(strcmp(ptr,"hash_delete") == 0)
		{
			//remove hash_elem e from hash table h
			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4]-48;
			sscanf(ptr2,"%d",&lidx);

			h_item = create_hash_item();
			h_item->data = lidx;

			h_elem = hash_find(ha[num].link->hash,&(h_item->elem));
			if(h_elem != NULL)
				hash_delete(ha[num].link->hash,h_elem);



		}
		else if(strcmp(ptr,"hash_clear") == 0)
		{
			//clear hashtable
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;

			hash_clear(ha[num].link->hash,hash_destructor);

		}
		else if(strcmp(ptr,"hash_size") == 0)
		{
			// return hastable's size
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;
			
			size = hash_size(ha[num].link->hash);
			printf("%zu\n",size);
		}
		else if(strcmp(ptr,"hash_empty") == 0)
		{
			// return true if hashtable is empty
			ptr = strtok(NULL," ");
			num = ptr[4] - 48;
			if(hash_empty(ha[num].link->hash) == true)
				printf("true\n");
			else if(hash_empty(ha[num].link->hash) != true)
				printf("false\n");

		}
		else if(strcmp(ptr,"hash_apply") == 0)
		{
			// call action for each element in arbitary order

			ptr = strtok(NULL," ");
			ptr2 = strtok(NULL," ");
			num = ptr[4]-48;
			ptr2[strlen(ptr2)] = '\0';
			if(strcmp(ptr2,"square") == 0)
			{
				hash_apply(ha[num].link->hash,square);
			}
			else if(strcmp(ptr2,"triple") == 0)
			{
				hash_apply(ha[num].link->hash,triple);
			}
		}
	}
	return 0;
}
