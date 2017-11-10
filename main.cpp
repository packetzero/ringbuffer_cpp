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
 * Test push/pop in ring-buffer of size=3 (2 slots available)
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
  ASSERT_FALSE(rb->pop(val));


  rb->push(gVal1);
  
  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());

  // put second value

  ASSERT_TRUE(rb->push(gVal2));
  
  ASSERT_EQ(0, rb->_writeSlots());
  ASSERT_EQ(2, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_TRUE(rb->isFull());

  // should drop this
  
  ASSERT_FALSE(rb->push(gVal3));

  ASSERT_EQ(0, rb->_writeSlots());
  ASSERT_EQ(2, rb->_readSlots());
  ASSERT_EQ(1, rb->drops());
  
  // get first value
  ASSERT_TRUE(rb->pop(val));
  ASSERT_EQ(0, memcmp(&gVal1, &val, sizeof(val)) == 0);
  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());

  // put third value
  
  ASSERT_TRUE(rb->push(gVal3));
  
  ASSERT_EQ(0, rb->_writeSlots());
  ASSERT_EQ(2, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_TRUE(rb->isFull());

  // get second value

  ASSERT_TRUE(rb->pop(val));
  ASSERT_EQ(0, memcmp(&gVal2, &val, sizeof(val)) == 0);
  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());

  // get third value
  
  ASSERT_TRUE(rb->pop(val));
  ASSERT_EQ(0, memcmp(&gVal3, &val, sizeof(val)) == 0);
  ASSERT_EQ(2, rb->_writeSlots());
  ASSERT_EQ(0, rb->_readSlots());
  ASSERT_TRUE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());

  // put fourth value
  
  ASSERT_TRUE(rb->push(gVal4));
  
  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());
  
  // put fifth value
  
  ASSERT_TRUE(rb->push(gVal5));
  
  ASSERT_EQ(0, rb->_writeSlots());
  ASSERT_EQ(2, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_TRUE(rb->isFull());
  
  // get fourth value
  
  ASSERT_TRUE(rb->pop(val));
  ASSERT_EQ(0, memcmp(&gVal4, &val, sizeof(val)) == 0);
  ASSERT_EQ(1, rb->_writeSlots());
  ASSERT_EQ(1, rb->_readSlots());
  ASSERT_FALSE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());
  
  // get fifth value
  
  ASSERT_TRUE(rb->pop(val));
  ASSERT_EQ(0, memcmp(&gVal5, &val, sizeof(val)) == 0);
  ASSERT_EQ(2, rb->_writeSlots());
  ASSERT_EQ(0, rb->_readSlots());
  ASSERT_TRUE(rb->isEmpty());
  ASSERT_FALSE(rb->isFull());

  delete rb;
}


int main(int argc, char * argv[])
{
  testing::InitGoogleTest(&argc, argv);
  int status= RUN_ALL_TESTS();
  return status;
}