#include <slave/queue.h>


void
slave_fast_queue_init( __STATE fast_queue_t *queue,
					   __IN    void         *buffer,
					   __IN    const size_t  size )
{
	size_t aligned_size = size;
	
	if ( !!( aligned_size & 0x11 ) )
		aligned_size = ( ( ( size - 1 ) >> 2 ) + 1 ) << 2;
	
	queue->head = queue->tail = 0;
	queue->size_mask = aligned_size-1;
	queue->buffer = buffer;
}
