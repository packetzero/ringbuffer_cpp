
#include "ring_buffer_inst.hpp"
#include "ring_buffer_impl.hpp"

RingBuffer<mytype>* RingBufferNew(int len)
{
  return new RingBufferImpl<mytype>(len);
}

