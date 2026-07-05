#include <slave/queue.h>


void
slave_ring_queue_init( __STATE ring_queue_t *queue,
					   __IN    void         *buffer,
					   __IN    const size_t  size )
{
	queue->buffer = buffer;
	queue->end = size - 1;
	queue->head = queue->tail = 0;
}
