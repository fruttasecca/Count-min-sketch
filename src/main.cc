#include <iostream>

#include <CountMinSketch.h>
#include <MurmurHash3.h>

using probStruct::CountMinSketch;
using namespace std;

int main()
{
  CountMinSketch<10,6000> s;

  const size_t intSize = sizeof(int);;
  for(int i = 0; i < 100000; i++)
    s.add(&i, intSize);

  for(int i = 0; i < 100000; i++)
    s.frequencyOf(&i, intSize);
}

