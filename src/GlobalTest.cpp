#include "./GlobalTest.h"

#include "ruby.h"

GlobalTest::GlobalTest() { }

void GlobalTest::addSprite(VALUE ptr)
{
  sprites.push_back(ptr);
}

GlobalTest globalInst;
