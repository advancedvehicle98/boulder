#include <slave/queue.h>

#include <common/config.h>
#include <slave/ram.h>


bool
slave_buffer_queue_push( __STATE buffer_queue_t     *queue,
						 __IN    const void * const  data,
						 __IN    const size_t        size )
{
	bool status = false;

#ifdef SAFETY_REDUNDANT_NULL_CHECKING
	if ( ! data || ! size ) return false;
#endif
	
	uint8_t overwrite = queue->flags & BUFFER_QUEUE_OVERWRITE;

	uint8_t is_full   = queue->flags & BUFFER_QUEUE_IS_FULL;
	
	if ( is_full && ! overwrite ) return true;

	void *start = queue->start, *end  = queue->end;
	void *head  = queue->head,  *tail = queue->tail;

	void *new_tail = tail + size;
	
	size_t before_end = 0;     // сколько заполняется с конца, если идем по кругу
	size_t after_start = 0; // сколько заполняется с начала, если идем по кругу

	// это нужно для того, чтобы поменять указатель на данные в том случае,
	// если у нас данные могут переполнить очередь на несколько раз.
	// в таком случае мы смещаем указатель на данных на то место,
	// начиная с которого все данные влезут в очередь
	const void *src = data;
	
    // идём по кругу -------------------------------------------------
	
	if ( new_tail >= end ) {
		before_end  = end - tail;
		after_start = size - before_end;
		new_tail    = start + after_start;

		// переполнение
		if ( new_tail > head ) {
			status = true;
			
			if ( ! overwrite )
				return true;
			
			is_full = BUFFER_QUEUE_IS_FULL;

			// спец. случай, если у нас данных так много, 
			// что они перезаписывают очередь на несколько раз.
			// в таком случае при активной перезаписи просто
			// добавляем последний кусок из data, который заполнит
			// всю очередь
			if ( new_tail > end ) {
				new_tail = tail;
				src += size - ( end - start );
				after_start = tail - start;
			}
				
			head = new_tail;
		}
	}

	// когда мы дошли до конца очереди ----------------------------------
	
	if ( new_tail == head ) 
		is_full = BUFFER_QUEUE_IS_FULL;
	
	// учитываем случай, когда перезаполение происходит при tail, ----------
	// находящемся перед head
	
	else if ( tail < head && head < new_tail ) {
		if ( ! overwrite ) return true;
		
		status = true;
		head = new_tail;
	}

	// копируем данные в очередь --------------------------------
	
	if ( before_end ) {
		slave_ram_copy( tail,  src,            before_end  );
		slave_ram_copy( start, src+before_end, after_start );
	}
	else
		slave_ram_copy( tail, src, size );

	tail = new_tail;

	// обновление очереди ----------------------------------------------

	queue->head = head;
	queue->tail = tail;

	queue->flags &= ~( BUFFER_QUEUE_IS_EMPTY | BUFFER_QUEUE_IS_FULL );
	queue->flags |= is_full;
		
	return status;
}


bool
slave_buffer_queue_push_byte( __STATE buffer_queue_t *queue,
							  __IN    const uint8_t   data )
{
	bool status = false;

#ifdef SAFETY_REDUNDANT_NULL_CHECKING	
	if ( ! data ) return false;
#endif
	
	uint8_t overwrite = queue->flags & BUFFER_QUEUE_OVERWRITE;

	uint8_t is_full   = queue->flags & BUFFER_QUEUE_IS_FULL;
	
	if ( is_full && ! overwrite ) return true;

	void *start = queue->start, *end  = queue->end;
	void *head  = queue->head,  *tail = queue->tail;

	void *new_tail = tail + 1;
	
    // идём по кругу
	if ( new_tail == end ) {
		new_tail = start;

		// переполнение
		if ( new_tail == head ) {
			status = true;
		
			if ( overwrite )
				is_full = BUFFER_QUEUE_IS_FULL;
			else
				return status;
		}
	}
	else if ( new_tail == head ) is_full = BUFFER_QUEUE_IS_FULL;
	// бошку тоже сдвигаем, когда у нас переполнение
	else if ( is_full ) head = new_tail;
	
	*( (uint8_t *) tail ) = data;

	tail = new_tail;

	queue->head = head;
	queue->tail = tail;

	queue->flags &= ~( BUFFER_QUEUE_IS_EMPTY | BUFFER_QUEUE_IS_FULL );
	queue->flags |= is_full;
		
	return status;
}
