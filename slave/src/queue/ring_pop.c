#include <slave/queue.h>

#include <slave/ram.h>


size_t
slave_ring_queue_pop( __STATE ring_queue_t *queue,       
					  __OUT   void * const  data,        
					  __OUT   const size_t  size )
{
	size_t end    = queue->end;
	int    head   = queue->head;
	int    tail   = queue->tail;
	void  *buffer = queue->buffer;

#ifdef CONFIG_SAFETY_REDUNDANT_NULL_CHECKING
	if ( ! data || ! size ) return false;
#endif

	size_t sz = size > end+1 ? end+1 : size;

	size_t new_head = head + size;

	if ( new_head > end ) {
		size_t before_end  = end - head + 1;
		size_t after_start = sz - before_end;
		
		if ( after_start > tail ) {
			after_start = tail;
			sz = before_end + after_start;
		}
		
		slave_ram_copy( data,            buffer+head, before_end );
		slave_ram_copy( data+before_end, buffer,      after_start );
	}
	else
		slave_ram_copy( data, buffer+head, sz );

	// ---------------------------------

	queue->head = new_head;

	return sz;
}

                                                               
uint8_t
slave_ring_queue_pop_byte( __STATE ring_queue_t *queue )
{
	size_t end    = queue->end;
	int    head   = queue->head;

	// похожая проблема с slave_ring_queue_push_byte --------
	
	head = ( head + 1 ) & end;

	// ------------------------------------------------------

	queue->head = head;

	return ( (uint8_t *) queue->buffer )[ head ];
}
