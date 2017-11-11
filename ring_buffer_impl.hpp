#ifndef _RING_BUFFER_IMPL_H_
#define _RING_BUFFER_IMPL_H_

#include "ring_buffer.hpp"
#include <vector>

template <typename T>
class RingBufferImpl : public RingBuffer<T>
{
public:
  RingBufferImpl(unsigned int size) : _size(size), _vec(), _ridx(0), _widx(1), _drops(0) {
    _vec.resize(size);
  }

  //------------------------------------------------------------
  // If no space, increments drops and returns false.
  // Otherwise, copies val to element at tail of queue,
  // increments write index, and returns true.
  //------------------------------------------------------------
  virtual bool enqueue(T val)
  {
    if (isFull()) { _drops++; return false; }

    // calculate next slot index

    unsigned int idx = _next(_widx);

    // copy

    _vec[idx] = val;

    // update write index

    _widx = idx;

    return true;
  }

  //------------------------------------------------------------
  // returns first available item into val
  // and move read pointer.
  // returns false if empty, true otherwise.
  //------------------------------------------------------------
  virtual bool dequeue(T &val) {
    if (isEmpty()) return false;

    // calculate next slot index

    unsigned int idx = _next(_ridx);

    // get value

    val = _vec[idx];

    // advance read index

    _ridx = idx;

    return true;
  }

  // returns number of slots available for writing
  virtual unsigned int _writeSlots() {
    int d = _widx - _ridx;
    if (d < 1) d += _size;
    return (unsigned int)(_size - d);
  }

  // return true if no empty slots
  virtual bool isFull() { return (_writeSlots() == 0); }

  // return true if no data
  virtual bool isEmpty() { return (_readSlots() == 0); }

  // returns number of slots waiting to be read
  virtual unsigned int _readSlots() { return _size - _writeSlots() - 1;  }

  // returns counter for times enqueue() called when full
  virtual unsigned int drops() { return _drops; }

private:

  /*
   * Returns incremented (and wrapped-around) idx.
   * Does not modify _ridx, _widx
   */
  unsigned int _next(unsigned int idx) {
    idx += 1;
    if (idx >= _size) idx -= _size;
    return idx;
  }

  unsigned int    _size;
  std::vector<T>  _vec;
  unsigned int    _ridx;
  unsigned int    _widx;
  unsigned int    _drops;
};

#endif // _RING_BUFFER_IMPL_H_
