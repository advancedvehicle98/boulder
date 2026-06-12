#include <slave/queue.h>

#include <common/config.h>
#include <slave/ram.h>


size_t
slave_buffer_queue_pop( __STATE buffer_queue_t *queue,
						__OUT   void * const    data,
						__OUT   const size_t    size )
{
#ifdef SAFETY_REDUNDANT_NULL_CHECKING
	if ( ! data || ! size ) return false;
#endif
	
	uint8_t lifo = queue->flags & BUFFER_QUEUE_OVERWRITE;

	uint8_t is_empty  = queue->flags & BUFFER_QUEUE_IS_EMPTY;
	uint8_t is_full   = queue->flags & BUFFER_QUEUE_IS_FULL;

	if ( is_empty ) return 0;

	if ( is_full ) is_full = 0;

	void *start = queue->start, *end  = queue->end;
	void *head  = queue->head,  *tail = queue->tail;

	size_t after_start = tail - start;
	size_t before_end  = end  - head;
	bool tail_after_head = head < tail;

	size_t popped_size = size;
	size_t actual_queue_size = tail_after_head 
		                     ? tail - head
		                     : after_start + before_end;

	if ( popped_size > actual_queue_size )
		popped_size = actual_queue_size;

	after_start = before_end = 0;
	
	if ( lifo ) goto _slave_buffer_queue_pop_lifo;

	// fifo -----------------------------------------------------------
	
	void *new_head = head + popped_size;

	if ( new_head > end ) {
		before_end  = end - head;
		after_start = popped_size - before_end;
		new_head = start + after_start;
	}

	if ( new_head == tail ) is_empty = BUFFER_QUEUE_IS_EMPTY;

	if ( before_end ) {
		slave_ram_copy( data,            head,  before_end );
		slave_ram_copy( data+before_end, start, after_start  );
	}
	else
		slave_ram_copy( data, head, popped_size );

	queue->head = new_head;
	
	goto _slave_buffer_queue_pop_update;

	// lifo -----------------------------------------------------------
	
_slave_buffer_queue_pop_lifo:	
	void *new_tail = tail - popped_size;

	if ( new_tail < start ) {
		after_start = tail - start;
		before_end  = popped_size - after_start;
		new_tail = end - before_end;
	}

	if ( new_tail == head ) is_empty = BUFFER_QUEUE_IS_EMPTY;

	if ( before_end ) {
		slave_ram_copy_reverse( data,             start,          after_start );
		slave_ram_copy_reverse( data+after_start, end-before_end, before_end  );
	}
	else
		slave_ram_copy_reverse( data, tail, popped_size );

	queue->tail = new_tail;

	// обновление очереди ----------------------------------------------

_slave_buffer_queue_pop_update:
	queue->flags &= ~( BUFFER_QUEUE_IS_EMPTY | BUFFER_QUEUE_IS_FULL );
	queue->flags |= is_empty | is_full;

	return popped_size;
}


uint8_t
slave_buffer_queue_pop_byte( __STATE buffer_queue_t *queue )
{
#ifdef SAFETY_REDUNDANT_NULL_CHECKING
	if ( ! data || ! size ) return false;
#endif
	
	uint8_t lifo = queue->flags & BUFFER_QUEUE_OVERWRITE;

	uint8_t is_empty  = queue->flags & BUFFER_QUEUE_IS_EMPTY;
	uint8_t is_full   = queue->flags & BUFFER_QUEUE_IS_FULL;

	if ( is_empty ) return 0;

	if ( is_full ) is_full = 0;

	void *start = queue->start, *end  = queue->end;
	void *head  = queue->head,  *tail = queue->tail;

	size_t popped_byte;

	// fifo -----------------------------------------------------------
	
	if ( ! lifo ) {
		void *new_head = head + 1;

		if ( new_head == end ) new_head = start;
		if ( new_head == tail ) is_empty = BUFFER_QUEUE_IS_EMPTY;

		popped_byte = *( (uint8_t *) head );		
		queue->head = new_head;
	}
	
	// lifo -----------------------------------------------------------
	
	else {
		void *new_tail = tail - 1;

		if ( new_tail < start ) new_tail = end - 1;		
		if ( new_tail == head ) is_empty = BUFFER_QUEUE_IS_EMPTY;
	
		popped_byte = *( (uint8_t *) tail );	
		queue->tail = new_tail;
	}
	
	// обновление очереди ----------------------------------------------

	queue->flags &= ~( BUFFER_QUEUE_IS_EMPTY | BUFFER_QUEUE_IS_FULL );
	queue->flags |= is_empty | is_full;

	return popped_byte;
}
