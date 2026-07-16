#ifndef __BOULDER_SLAVE_QUEUE_H
#define __BOULDER_SLAVE_QUEUE_H


#include <common/defines.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// старая очередь (референсная вещь) ===================================================

#define BUFFER_QUEUE_LIFO      0x1
#define BUFFER_QUEUE_OVERWRITE 0x2
#define BUFFER_QUEUE_IS_FULL   0x4
#define BUFFER_QUEUE_IS_EMPTY  0x8


typedef struct _buffer_queue_t {
	void   *start, *end;
	// - head указывает на начало очереди (на первый элемент)
	// - tail указывает на место, куда будет вставлен следующий элемент
	void   *head, *tail;
	uint8_t flags; // см. макросы выше

	// !!! при перезапиши head смещается к tail
} buffer_queue_t;


static inline bool
slave_buffer_queue_is_empty( __IN buffer_queue_t * const queue )
{
	return !!( queue->flags & BUFFER_QUEUE_IS_EMPTY );
}


static inline bool
slave_buffer_queue_is_full( __IN buffer_queue_t * const queue )
{
	return !!( queue->flags & BUFFER_QUEUE_IS_FULL );
}


// src/queue/init.c
void slave_buffer_queue_init( __STATE buffer_queue_t *queue,
							  __IN    void           *buffer,
							  __IN    const size_t    size,
							  __IN    const uint8_t   flags );

// src/queue/push.c
// -----------------------
// возращает true если:
// - при queue->overwrite == false: данные не влезают в конец
// - при queue->overwrite == true:  хвост перезаписан
__HOT bool slave_buffer_queue_push( __STATE buffer_queue_t * const queue,
									__IN    const void * const     data,
									__IN    const size_t           size );

// упрощенная функция для байтов
__HOT bool slave_buffer_queue_push_byte( __STATE buffer_queue_t *queue,
										 __IN    const uint8_t   data );


// src/queue/pop.c
// -----------------------
__HOT size_t slave_buffer_queue_pop( __STATE buffer_queue_t *queue,
									 __OUT   void * const    data,
									 __OUT   const size_t    size );

__HOT uint8_t slave_buffer_queue_pop_byte( __STATE buffer_queue_t *queue );


// кольцевой буфер ============================================================

typedef struct _ring_queue_t {
	void *buffer;
	size_t end;
	int head, tail;
} ring_queue_t;


// src/queue/ring_init.c
void slave_ring_queue_init( __STATE ring_queue_t *queue,
							__IN    void         *buffer,
							__IN    const size_t  size );

// src/queue/ring_push.c
__HOT bool slave_ring_queue_push( __STATE ring_queue_t *queue,
								  __IN    void * const  data,
								  __IN    const size_t  size );

__HOT bool slave_ring_queue_push_byte( __STATE ring_queue_t  *queue,
									   __IN    const uint8_t  data );

// src/queue/ring_pop.c
__HOT size_t slave_ring_queue_pop( __STATE ring_queue_t *queue,
								   __OUT   void * const  data,
								   __OUT   const size_t  size );

__HOT uint8_t slave_ring_queue_pop_byte( __STATE ring_queue_t *queue );


#endif // ! __BOULDER_SLAVE_QUEUE_H
