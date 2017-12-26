#include <stdlib.h>//random
#include <time.h>//time to init random

#include <CountMinSketch.h>
#include <gtest.h>


using namespace std;
using namespace probStruct;

static size_t maxVal = std::numeric_limits<size_t>::max();
static size_t intSize = sizeof(int);
static size_t ullSize = sizeof(unsigned long long);
static size_t increment = 5;


TEST(getters, rightRowsAndColumns)
{
  CountMinSketch<10,100> s;
  ASSERT_EQ(s.getNHashes(), 10);
  ASSERT_EQ(s.getColumns(), 100);
}

TEST(getters, afterCopy)
{
  CountMinSketch<10,100> s;
  CountMinSketch<10,100> s2(s);
  ASSERT_EQ(s.getNHashes(), 10);
  ASSERT_EQ(s.getColumns(), 100);
}

TEST(getters, afterAssignment)
{
  CountMinSketch<10,100> s;
  CountMinSketch<10,100> s2 = s;
  ASSERT_EQ(s.getNHashes(), 10);
  ASSERT_EQ(s.getColumns(), 100);
}

TEST(copyConstructor, CopyEmpty)
{
  CountMinSketch<10,100> s;
  CountMinSketch<10,100> s2(s);
  for(int i = 0; i < 100; i++)
    ASSERT_EQ(s.frequencyOf(&i, intSize), 0);
}

TEST(copyConstructor, CopyNonEmpty)
{
  CountMinSketch<10,100> s;
  for(int i = 0; i < 100; i++)
    s.add(&i, intSize);

  CountMinSketch<10,100> s2(s);
  for(int i = 0; i < 100; i++)
    ASSERT_EQ(s.frequencyOf(&i, intSize), s2.frequencyOf(&i, intSize));
}

TEST(assigment, assignEmpty)
{
  CountMinSketch<10,100> s;
  CountMinSketch<10,100> s2;
  s2 = s;
  for(int i = 0; i < 100; i++)
    ASSERT_EQ(s.frequencyOf(&i, intSize), 0);
}

TEST(assigment, assignNonEmpty)
{
  CountMinSketch<10,100> s;
  for(int i = 0; i < 100; i++)
    s.add(&i, intSize);

  CountMinSketch<10,100> s2 = s;
  for(int i = 0; i < 100; i++)
    ASSERT_EQ(s.frequencyOf(&i, intSize), s2.frequencyOf(&i, intSize));
}

TEST(clear, clearBeforeAdding)
{
  CountMinSketch<10,100> s;
  s.clear();
  for(int i = 0; i < 100; i++)
    ASSERT_EQ(s.frequencyOf(&i, intSize), 0);
}

TEST(clear, clearAfterAdding)
{
  CountMinSketch<10,100> s;
  for(int i = 0; i < 100; i++)
    s.add(&i, intSize);
  s.clear();
  for(int i = 0; i < 100; i++)
    ASSERT_EQ(s.frequencyOf(&i, intSize), 0);
}

//tests for ints

TEST(addInt, addOneElementSingleCell)
{
  CountMinSketch<1,1> s;
  int t = 0;
  s.add(&t, intSize);
  ASSERT_EQ(s.frequencyOf(&t, intSize), 1);
  s.add(&t, intSize);
  ASSERT_EQ(s.frequencyOf(&t, intSize), 2);
}

TEST(addInt, addTwoElementsSingleCell)
{
  CountMinSketch<1,1> s;
  int t[] = {0,1};

  s.add(t, intSize);
  ASSERT_EQ(s.frequencyOf(&t, intSize), 1);

  //add other element and have collision
  s.add(t + 1, intSize);

  ASSERT_EQ(s.frequencyOf(t, intSize), 2);
  ASSERT_EQ(s.frequencyOf(t + 1, intSize), 2);
}

TEST(addInt, add100k)
{
  CountMinSketch<10,6000> s;
  for(int i = 0; i < 100000; i++)
  {
    s.add(&i, intSize);
    EXPECT_LE(s.frequencyOf(&i, intSize), i + 1);
  }

  for(int i = 0; i < 100000; i++)
    EXPECT_LE(s.frequencyOf(&i, intSize), 100000);
}

TEST(addWithIncrementInt, addOneElementSingleCell)
{
  CountMinSketch<1,1> s;
  int t = 0;
  s.add(&t, intSize, increment);
  ASSERT_EQ(s.frequencyOf(&t, intSize), increment);
  s.add(&t, intSize, increment);
  ASSERT_EQ(s.frequencyOf(&t, intSize), 2 * increment);
}

TEST(addWithIncrementInt, addTwoElementsSingleCell)
{
  CountMinSketch<1,1> s;
  int t[] = {0,1};

  s.add(t, intSize, increment);
  ASSERT_EQ(s.frequencyOf(&t, intSize), 5);

  //add other element and have collision
  s.add(t + 1, intSize, increment);

  ASSERT_EQ(s.frequencyOf(t, intSize), increment * 2);
  ASSERT_EQ(s.frequencyOf(t + 1, intSize), increment * 2);
}

TEST(addWithIncrementInt, add100k)
{
  CountMinSketch<10,6000> s;
  for(int i = 0; i < 100000; i++)
  {
    s.add(&i, intSize, increment);
    EXPECT_LE(s.frequencyOf(&i, intSize), increment * (i + 1));
  }

  for(int i = 0; i < 100000; i++)
    EXPECT_LE(s.frequencyOf(&i, intSize), 100000 * increment);
}

TEST(frequencyOfInt, addElement)
{
  CountMinSketch<2,60> s;
  int t = 0;
  for(int i = 0; i < 100; i++)
    EXPECT_EQ(s.frequencyOf(&i, intSize), 0);

  for(int i = 0; i < 100; i++)
  {
    s.clear();
    s.add(&t, sizeof(t));
    EXPECT_EQ(s.frequencyOf(&t, intSize), 1);
  }
}

//test for ull
TEST(addUll, addOneElementSingleCell)
{
  CountMinSketch<1,1> s;
  unsigned long long t = 0;
  s.add(&t, ullSize);
  ASSERT_EQ(s.frequencyOf(&t, ullSize), 1);
  s.add(&t, ullSize);
  ASSERT_EQ(s.frequencyOf(&t, ullSize), 2);
}

TEST(addUll, addTwoElementsSingleCell)
{
  CountMinSketch<1,1> s;
  unsigned long long t[] = {0,1};

  s.add(t, ullSize);
  ASSERT_EQ(s.frequencyOf(&t, ullSize), 1);

  //add other element and have collision
  s.add(t + 1, ullSize);

  ASSERT_EQ(s.frequencyOf(t, ullSize), 2);
  ASSERT_EQ(s.frequencyOf(t + 1, ullSize), 2);
}

TEST(addUll, add100k)
{
  CountMinSketch<10,6000> s;
  for(unsigned long long i = 0; i < 100000; i++)
  {
    s.add(&i, ullSize);
    EXPECT_LE(s.frequencyOf(&i, ullSize), i + 1);
  }

  for(unsigned long long i = 0; i < 100000; i++)
    EXPECT_LE(s.frequencyOf(&i, ullSize), 100000);
}

TEST(addWithIncrementUll, addOneElementSingleCell)
{
  CountMinSketch<1,1> s;
  unsigned long long t = 0;
  s.add(&t, ullSize, increment);
  ASSERT_EQ(s.frequencyOf(&t, ullSize), increment);
  s.add(&t, ullSize, increment);
  ASSERT_EQ(s.frequencyOf(&t, ullSize), 2 * increment);
}

TEST(addWithIncrementUll, addTwoElementsSingleCell)
{
  CountMinSketch<1,1> s;
  unsigned long long t[] = {0,1};

  s.add(t, ullSize, increment);
  ASSERT_EQ(s.frequencyOf(&t, ullSize), 5);

  //add other element and have collision
  s.add(t + 1, ullSize, increment);

  ASSERT_EQ(s.frequencyOf(t, ullSize), increment * 2);
  ASSERT_EQ(s.frequencyOf(t + 1, ullSize), increment * 2);
}

TEST(addWithIncrementUll, add100k)
{
  CountMinSketch<10,6000> s;
  for(unsigned long long i = 0; i < 100000; i++)
  {
    s.add(&i, ullSize, increment);
    EXPECT_LE(s.frequencyOf(&i, ullSize), increment * (i + 1));
  }

  for(int i = 0; i < 100000; i++)
    EXPECT_LE(s.frequencyOf(&i, ullSize), 100000 * increment);
}

TEST(frequencyOfUll, addElement)
{
  CountMinSketch<2,60> s;
  unsigned long long t = 0;
  for(unsigned long long i = 0; i < 100; i++)
    EXPECT_EQ(s.frequencyOf(&i, ullSize), 0);

  for(unsigned long long i = 0; i < 100; i++)
  {
    s.clear();
    s.add(&t, sizeof(t));
    EXPECT_EQ(s.frequencyOf(&t, ullSize), 1);
  }
}

//strings

TEST(addString, addOneElementSingleCell)
{
  CountMinSketch<1,1> s;
  string t("ayy");
  s.add(t.data(), t.size());
  ASSERT_EQ(s.frequencyOf(t.data(), t.size()), 1);
  s.add(t.data(), t.size());
  ASSERT_EQ(s.frequencyOf(t.data(), t.size()), 2);
}

TEST(addString, addTwoElementsSingleCell)
{
  CountMinSketch<1,1> s;
  string t1("testest");
  string t2("ayylmao");


  s.add(t1.data(), t1.size());
  ASSERT_EQ(s.frequencyOf(t1.data(), t1.size()), 1);

  //add other element and have collision
  s.add(t2.data(), t2.size());

  ASSERT_EQ(s.frequencyOf(t1.data(), t1.size()), 2);
  ASSERT_EQ(s.frequencyOf(t2.data(), t2.size()), 2);
}

TEST(addString, add100k)
{
  CountMinSketch<10,6000> s;
  for(unsigned long long i = 0; i < 1000; i++)
  {
    string t("x",i);
    EXPECT_LE(s.frequencyOf(t.data(), t.size()), i + 1);
  }

  for(unsigned long long i = 0; i < 1000; i++)
  {
    string t("x",i);
    EXPECT_LE(s.frequencyOf(t.data(), t.size()), 100000);
  }
}

TEST(addWithIncrementString, addOneElementSingleCell)
{
  CountMinSketch<1,1> s;
  string t("zzzzzzzzzz");
  s.add(t.data(), t.size(), increment);
  ASSERT_EQ(s.frequencyOf(t.data(), t.size()), increment);
  s.add(t.data(), t.size(), increment);
  ASSERT_EQ(s.frequencyOf(t.data(), t.size()), 2 * increment);
}

TEST(addWithIncrementString, addTwoElementsSingleCell)
{
  CountMinSketch<1,1> s;
  string t1("testest");
  string t2("ayylmao");

  s.add(t1.data(), t1.size(), increment);
  ASSERT_EQ(s.frequencyOf(t1.data(), t1.size()), 5);

  //add other element and have collision
  s.add(t2.data(), t2.size(), increment);

  ASSERT_EQ(s.frequencyOf(t1.data(), t1.size()), increment * 2);
  ASSERT_EQ(s.frequencyOf(t2.data(), t2.size()), increment * 2);
}

TEST(addWithIncrementString, add100k)
{
  CountMinSketch<10,6000> s;
  for(unsigned long long i = 0; i < 1000; i++)
  {
    string t("x",i);
    s.add(t.data(), t.size(), increment);
    EXPECT_LE(s.frequencyOf(t.data(), t.size()), increment * (i + 1));
  }

  for(int i = 0; i < 1000; i++)
  {
    string t("x",i);
    EXPECT_LE(s.frequencyOf(t.data(), t.size()), 1000 * increment);
  }
}

TEST(frequencyOfString, addElement)
{
  CountMinSketch<2,60> s;
  for(unsigned long long i = 0; i < 100; i++)
  {
    string t("x",i);
    EXPECT_EQ(s.frequencyOf(t.data(), t.size()), 0);
  }

  for(unsigned long long i = 0; i < 100; i++)
  {
    s.clear();
    string t("x",i);
    s.add(t.data(), t.size());
    EXPECT_EQ(s.frequencyOf(t.data(), t.size()), 1);
  }
}

/*
 * since the data structure guarantees that
 * frequencyOf <= true frequency + epsilon * insertions
 * with a probability of 1 - δ this test is not really
 * deterministic, but if you modify the sketch and
 * things start to go really wrong this test might
 * help in making you notice
 * */

TEST(checkErrorInt, ints)
{
  srand (time(NULL));
  //rows = ⌈ln 1/δ⌉
  //columns = ⌈e/ε⌉
  double delta = 0.1;//delta used, for reference
  double epsilon = 0.01;//epsilon used

  size_t n = 50000;//inserted elements go from 0 to n - 1
  size_t inserts = 1000000;//number of insertions to do

  double error = inserts * epsilon;//error on N
  size_t shouldBeWithinError = (1 - delta) * n;//number of elements thats should be within error threshold
  int trueResult[n];//stores the true frequency of the inserted elements
  memset(trueResult, 0, n);

  CountMinSketch<3 ,272> s;
  for(size_t i = 0; i < inserts; i++)
  {
    int tmp = rand() % n;
    trueResult[tmp]++;
    s.add(&tmp, intSize);
  }

  size_t withinError = 0;
  for(int i = 0; i < n; i++)
    if(s.frequencyOf(&i, intSize) - trueResult[i] <= error)
      withinError++;
  EXPECT_TRUE(withinError >= shouldBeWithinError);
}
