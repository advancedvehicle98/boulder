#include <slave/queue.h>
#include <slave/ram.h>


void
slave_fast_queue_push( __STATE fast_queue_t *queue,      
					   __IN    void * const  data,       
					   __IN    const size_t  size )
{
	size_t tail = queue->tail, size_mask = queue->size_mask;

	void *buffer = queue->buffer;
	
	if ( tail + size > size_mask ) {
		size_t before_end  = size_mask - tail + 1;
		size_t after_start = size - before_end;
		
		slave_ram_copy( buffer + tail, data,              before_end );
		slave_ram_copy( buffer,        data + before_end, after_start );
	}
	else
		slave_ram_copy( buffer + tail, data, size );

	queue->tail = ( tail + size ) & size_mask;
}

                                                              
void
slave_fast_queue_push_byte( __STATE fast_queue_t  *queue,
							__IN    const uint8_t  data )
{
	( (uint8_t *) queue->buffer )[ queue->tail ] = data;
	queue->tail = ( queue->tail + 1 ) & queue->size_mask;
}
