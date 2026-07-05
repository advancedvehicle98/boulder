#include <slave/queue.h>

#include <slave/ram.h>


bool
slave_ring_queue_push( __STATE ring_queue_t *queue,
					   __IN    void * const  data,
					   __IN    const size_t  size )
{
	size_t end    = queue->end;
	int    head   = queue->head;
	int    tail   = queue->tail;
	void  *buffer = queue->buffer;

#ifdef CONFIG_SAFETY_REDUNDANT_NULL_CHECKING
	if ( ! data || ! size ) return false;
#endif

	// ---------------------------------------------

	size_t sz = size;
	void *src = data;
	
	if ( size > end+1 ) {
		sz = end;
		src = buffer + size - sz;
	}
	
	// ---------------------------------------------
	
	int new_tail = tail + sz;

	if ( new_tail > end ) {
		size_t before_end  = end - tail + 1;
		size_t after_start = sz - before_end;
		
		slave_ram_copy( buffer+tail, src,            before_end );
		slave_ram_copy( buffer,      src+before_end, after_start );

		new_tail &= end;
		
		if ( after_start > head ) queue->head = new_tail;
	}
	else
		slave_ram_copy( buffer+tail, src, sz );
	
	// -----------------------------------------------

	queue->tail = new_tail;

	return false;
}


bool
slave_ring_queue_push_byte( __STATE ring_queue_t  *queue,
							__IN    const uint8_t  data )
{
	size_t end  = queue->end;
	int    tail = queue->tail;

#ifdef CONFIG_SAFETY_REDUNDANT_NULL_CHECKING
	if ( ! data ) return false;
#endif
		
	// ---------------------------------------------

	// мне не нравится что не получается придумать быстрый способ,
	// как сместить бошку, если её обходит tail, без флагов
	
	tail = ( tail + 1 ) & end;
	( (uint8_t *) queue->buffer )[ tail ] = data;

	// -----------------------------------------------

	queue->tail = tail;

	return false;
}
