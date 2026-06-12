#include <slave/queue.h>
#include <slave/ram.h>


void
slave_fast_queue_pop( __STATE fast_queue_t *queue,       
					  __OUT   void * const  data,        
					  __OUT   const size_t  size )
{
	size_t size_mask = queue->size_mask,
		   head      = queue->head;

	void *buffer = queue->buffer;

	if ( head + size > size_mask ) {
		size_t before_end  = size_mask - head + 1;
		size_t after_start = size - before_end;

		slave_ram_copy( data,              buffer + head, before_end );
		slave_ram_copy( data + before_end, buffer,        after_start );
	}
	else
		slave_ram_copy( data, buffer + head, size );

	queue->head = ( head + size ) & size_mask;
}


uint8_t
slave_fast_queue_pop_byte( __STATE fast_queue_t *queue )
{
	uint8_t data = ( (uint8_t *) queue->buffer )[ queue->head ];
	queue->head = ( queue->head + 1 ) & queue->size_mask;
	return data;
}
