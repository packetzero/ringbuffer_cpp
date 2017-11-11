#ifndef ring_buffer_hpp
#define ring_buffer_hpp

/*
 * RingBuffer (abstract class) is a simple FIFO queue.
 *
 * Internally, it's an array of elements with indexes that wrap around.
 * Since it's a fixed size array, no dynamic allocations occur, just copying into/out of slots.
 *
 * It's thread-safe without requiring mutexes.  Assumes that incrementing of read and write indexes (int32) are atomic.
 *
 * Instantiate RingBufferImpl<custom-type> to use.
 */

template <typename T>
class RingBuffer
{
public:
  /*
   * if a write slot is available, val will be added to end of queue, the write index is advanced, and true is returned.
   * @returns false if no write slots available and val is not written to queue.
   * Calls isFull() internally.
   */
  virtual bool enqueue(T val) = 0;

  /*
   * If data is available, val will contain head of queue, read index is advanced, and true will be returned.
   * Otherwise, false is returned and val is untouched.
   * Calls isEmpty() internally.
   */
  virtual bool dequeue(T &val) = 0;

  /*
   * Returns true if all data has been read.
   */
  virtual bool isEmpty() = 0;

  /*
   * Returned true if no space left to write (enqueue).
   */
  virtual bool isFull() = 0;

  /*
   * The drops counter is incremented each time enqueue() is called when isFull() == true.
   */
  virtual unsigned int drops() = 0;

  // exposed only for tests

  virtual unsigned int _readSlots() = 0;
  virtual unsigned int _writeSlots() = 0;
};

#endif
