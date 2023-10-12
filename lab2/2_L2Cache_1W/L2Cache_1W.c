#include "L2Cache_1W.h"

uint8_t L1Cache[L1_SIZE];
uint8_t L2Cache[L2_SIZE];
uint8_t DRAM[DRAM_SIZE];
uint32_t time;
CacheL1 L1;
CacheL2 L2;

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {

  if (address >= DRAM_SIZE - WORD_SIZE + 1)
    exit(-1);

  if (mode == MODE_READ) {
    memcpy(data, &(DRAM[address]), BLOCK_SIZE);
    time += DRAM_READ_TIME;
  }

  if (mode == MODE_WRITE) {
    memcpy(&(DRAM[address]), data, BLOCK_SIZE);
    time += DRAM_WRITE_TIME;
  }
}

void initCache() { L1.init = 0; L2.init = 0; }

/*********************** L1 cache *************************/

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {

  uint32_t index, Tag, MemAddress;
  uint8_t TempBlock[BLOCK_SIZE];

  /* init cache */
  if (L1.init == 0) {
    for (int i = 0; i < L1_LINES; i++)
      L1.line[i].Valid = 0;
    
    L1.init = 1;
  }

  index = (address & INDEX_MASK_1W) >> OFFSET_BITS; // remove the offset and the tag

  CacheLine *Line = &L1.line[index];

  Tag = address >> (INDEX_BITS_1W + OFFSET_BITS); // isolate tag from address

  MemAddress = address & ~OFFSET_MASK; // address of the block in memory

  /* access Cache*/
  int position = address & ~TAG_MASK_1W;

  if (!Line->Valid || Line->Tag != Tag) { // if block not present - miss
    accessL2(MemAddress, TempBlock, MODE_READ); // get new block from L2

    if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
      // get address of the block in memory:
      MemAddress = Line->Tag;
      MemAddress <<= INDEX_BITS_1W;
      MemAddress += index;
      MemAddress <<= OFFSET_BITS;
      accessL2(MemAddress, &(L1Cache[position]), MODE_WRITE); // then write back old block
    }

    memcpy(&(L1Cache[position]), TempBlock, BLOCK_SIZE); // copy new block to cache line
    Line->Valid = 1;
    Line->Tag = Tag;
    Line->Dirty = 0;
  } // if miss, then replaced with the correct block

  if (mode == MODE_READ) {    // read data from cache line
    memcpy(data, &(L1Cache[position]), WORD_SIZE);
    time += L1_READ_TIME;
  }

  if (mode == MODE_WRITE) { // write data from cache line
    memcpy(&(L1Cache[position]), data, WORD_SIZE);
    time += L1_WRITE_TIME;
    Line->Dirty = 1;
  }
}

void read(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_WRITE);
}

/*********************** L2 cache *************************/

void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {

  uint32_t index, Tag, MemAddress;
  uint8_t TempBlock[BLOCK_SIZE];

  /* init cache */
  if (L2.init == 0) {
    for (int i = 0; i < L2_LINES; i++)
      L2.line[i].Valid = 0;
    
    L2.init = 1;
  }

  index = (address & INDEX_MASK_1W) >> OFFSET_BITS; // remove the offset and the tag

  CacheLine *Line = &L2.line[index];

  Tag = address >> (INDEX_BITS_1W + OFFSET_BITS); // isolate tag from address

  MemAddress = address & ~OFFSET_MASK; // address of the block in memory

  /* access Cache*/
  int position = address & ~TAG_MASK_1W;

  if (!Line->Valid || Line->Tag != Tag) {         // if block not present - miss
    accessDRAM(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

    if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
      // get address of the block in memory:
      MemAddress = Line->Tag << INDEX_BITS_1W;
      MemAddress += index;
      MemAddress <<= OFFSET_BITS;
      accessDRAM(MemAddress, &(L2Cache[position]), MODE_WRITE); // then write back old block
    }

    memcpy(&(L2Cache[position]), TempBlock, BLOCK_SIZE); // copy new block to cache line
    Line->Valid = 1;
    Line->Tag = Tag;
    Line->Dirty = 0;
  } // if miss, then replaced with the correct block

  if (mode == MODE_READ) {    // read data from cache line
    memcpy(data, &(L2Cache[position]), WORD_SIZE);
    time += L2_READ_TIME;
  }

  if (mode == MODE_WRITE) { // write data from cache line
    memcpy(&(L2Cache[position]), data, WORD_SIZE);
    time += L2_WRITE_TIME;
    Line->Dirty = 1;
  }
}
