#include <gtest/gtest.h>
#include <stdint.h>
#include <string.h> // memcmp

#include "ring_buffer_inst.hpp"

const mytype gVal1 = {1,111,11,0};
const mytype gVal2 = {1,222,22,0};
const mytype gVal3 = {1,333,33,0};
const mytype gVal4 = {1,444,44,0};
const mytype gVal5 = {1,555,55,0};


class RingBufferTest : public ::testing::Test {
protected:
  virtual void SetUp() {
  }

  // virtual void TearDown() {}
};


/*
 * Test enqueue/dequeue in ring-buffer of size=3 (2 slots available)
 */
TEST_F(RingBufferTest, basic)
{
  mytype val = mytype();

  RingBuffer<mytype>* rb = RingBufferNew(3);

  ASSERT_EQ(2, rb->_writeSlots());
  ASSERT_EQ(0, rb->_readSlots());
  ASSERT_EQ(0, rb->drops());
  ASSERT_TRUE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());
  ASSERT_FALSE(rb->dequeue(val));


  rb->enqueue(gVal1);

  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());

  // put second value

  ASSERT_TRUE(rb->enqueue(gVal2));

  ASSERT_EQ(0, rb->_writeSlots());
  ASSERT_EQ(2, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_TRUE(rb->isFull());

  // should drop this

  ASSERT_FALSE(rb->enqueue(gVal3));

  ASSERT_EQ(0, rb->_writeSlots());
  ASSERT_EQ(2, rb->_readSlots());
  ASSERT_EQ(1, rb->drops());

  // get first value
  ASSERT_TRUE(rb->dequeue(val));
  ASSERT_EQ(0, memcmp(&gVal1, &val, sizeof(val)));
  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());

  // put third value

  ASSERT_TRUE(rb->enqueue(gVal3));

  ASSERT_EQ(0, rb->_writeSlots());
  ASSERT_EQ(2, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_TRUE(rb->isFull());

  // get second value

  ASSERT_TRUE(rb->dequeue(val));
  ASSERT_EQ(0, memcmp(&gVal2, &val, sizeof(val)));
  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());

  // get third value

  ASSERT_TRUE(rb->dequeue(val));
  ASSERT_EQ(0, memcmp(&gVal3, &val, sizeof(val)));
  ASSERT_EQ(2, rb->_writeSlots());
  ASSERT_EQ(0, rb->_readSlots());
  ASSERT_TRUE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());

  // put fourth value

  ASSERT_TRUE(rb->enqueue(gVal4));

  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());

  // put fifth value

  ASSERT_TRUE(rb->enqueue(gVal5));

  ASSERT_EQ(0, rb->_writeSlots());
  ASSERT_EQ(2, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_TRUE(rb->isFull());

  // get fourth value

  ASSERT_TRUE(rb->dequeue(val));
  ASSERT_EQ(0, memcmp(&gVal4, &val, sizeof(val)));
  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());

  // get fifth value

  ASSERT_TRUE(rb->dequeue(val));
  ASSERT_EQ(0, memcmp(&gVal5, &val, sizeof(val)));
  ASSERT_EQ(2, rb->_writeSlots());
  ASSERT_EQ(0, rb->_readSlots());
  ASSERT_TRUE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());
  ASSERT_EQ(1, rb->drops());

  delete rb;
}

TEST_F(RingBufferTest, misc)
{
  mytype val = mytype();

  RingBuffer<mytype>* rb = RingBufferNew(4);

  ASSERT_FALSE(rb->dequeue(val));
  ASSERT_FALSE(rb->dequeue(val));

  rb->enqueue(gVal1);
  rb->enqueue(gVal2);
  ASSERT_TRUE(rb->enqueue(gVal3));
  ASSERT_FALSE(rb->enqueue(gVal4)); // drop
  ASSERT_EQ(1, rb->drops());

  rb->dequeue(val);
  ASSERT_EQ(0, memcmp(&gVal1, &val, sizeof(val)));
  rb->enqueue(gVal4);

  rb->dequeue(val);
  ASSERT_EQ(0, memcmp(&gVal2, &val, sizeof(val)));
  rb->enqueue(gVal5);

  rb->dequeue(val);
  ASSERT_EQ(0, memcmp(&gVal3, &val, sizeof(val)));
  rb->enqueue(gVal1);

  rb->dequeue(val);
  ASSERT_EQ(0, memcmp(&gVal4, &val, sizeof(val)));
  rb->enqueue(gVal2);

  rb->dequeue(val);
  ASSERT_EQ(0, memcmp(&gVal5, &val, sizeof(val)));
  rb->enqueue(gVal3);

  ASSERT_TRUE(rb->isFull());
  ASSERT_EQ(1, rb->drops());

  rb->dequeue(val);
  ASSERT_EQ(0, memcmp(&gVal1, &val, sizeof(val)));
  rb->dequeue(val);
  ASSERT_EQ(0, memcmp(&gVal2, &val, sizeof(val)));
  ASSERT_TRUE(rb->dequeue(val));
  ASSERT_EQ(0, memcmp(&gVal3, &val, sizeof(val)));

  ASSERT_FALSE(rb->dequeue(val));
  ASSERT_TRUE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());

}


int main(int argc, char * argv[])
{
  testing::InitGoogleTest(&argc, argv);
  int status= RUN_ALL_TESTS();
  return status;
}
