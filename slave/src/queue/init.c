#include <slave/queue.h>


void
slave_buffer_queue_init( __STATE buffer_queue_t *queue,
						 __IN    void           *buffer,
						 __IN    const size_t    size,
						 __IN    const uint8_t   flags )
{
	queue->start = buffer;
	queue->end   = buffer + size;

	queue->head = queue->tail = buffer;

#ifndef CONFIG_SAFETY_COMPROMISE
	queue->flags = flags & ~( BUFFER_QUEUE_IS_FULL
							| BUFFER_QUEUE_IS_EMPTY);
#else
	queue->flags = flags;
#endif
}
