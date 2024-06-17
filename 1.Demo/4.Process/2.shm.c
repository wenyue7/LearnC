#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main ()
{
	int segment_id;
	char* shared_memory;
	struct shmid_ds shmbuffer;
	int segment_size;
	const int shared_segment_size = 0x6400;
	/* 分配一个共享内存块 */
	segment_id = shmget(IPC_PRIVATE, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR );
	/* 绑定到共享内存块 */
	shared_memory = (char*) shmat (segment_id, 0, 0);
	printf("shared memory attached at address %p\n", shared_memory);
	/* 确定共享内存的大小 */
	shmctl (segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;
	printf ("segment size: %d\n", segment_size);
	/* 在共享内存中写入一个字符串 */
	sprintf (shared_memory, "Hello, world.");
	/* 脱离该共享内存块 */
	shmdt (shared_memory);
	/* 重新绑定该内存块 */
	shared_memory = (char*) shmat (segment_id, (void*) 0x500000, 0);
	printf("shared memory reattached at address %p\n", shared_memory);
	/* 输出共享内存中的字符串 */
	printf ("%s\n", shared_memory);
	/* 脱离该共享内存块 */
	shmdt (shared_memory);
	/* 释放这个共享内存块 */
	shmctl (segment_id, IPC_RMID, 0);
	return 0;
}
