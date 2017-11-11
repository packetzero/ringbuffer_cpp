# C++ templatized ring-buffer FIFO

RingBuffer is a simple FIFO queue.
It's thread-safe without requiring mutexes.  Assumes that incrementing of read and write indexes (int32) are atomic.

Internally, it's an array of elements with indexes that wrap around.
Since it's a fixed size array, no dynamic allocations occur, just copying into/out of slots.

RingBuffer is a virtual class (e.g. interface) and RingBufferImpl is the implementation.

### Example - RingBuffer of int
```
#include "ring_buffer_impl.hpp"

RingBuffer<int>* RingBufferNew(int len)
{
  return new RingBufferImpl<int>(len);
}

void write_thread_func(RingBuffer<int>* rb)
{
  while (keepWriting)
  {
    if (have_a_value)
      rb->enqueue(value);

    wait_for_more_data();
  }
}

void read_thread_func(RingBuffer<int>* rb)
{
  int value;

  while (keepReading)
  {
    while (rb->dequeue(value)) {
      // got one
    }

    usleep(10000);
  }  
}

void start()
{
  RingBuffer<int>* rb = RingBufferNew(1024);

  start_write_thread(rb);

  start_read_thread(rb);
}

```
