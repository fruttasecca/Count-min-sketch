<<<<<<< HEAD
# Count-min sketch 
https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch
A c++11 Count-min sketch implementation, in src you can find an extremely
simple main for an example of usage, in include you will find the implementation, and
of course in test you will find the tests.

After you have cloned:
* "cmake CMakeLists.txt" (will also clone the gtest repo)
* "make" to compile main, the executable will be named probStruct
* "make tests" to compile tests, the executable will be named probTest

=======
# Count-min sketch (https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch)
A c++11 Count-min sketch implementation.
>>>>>>> a76668f76db038ffc3e04e7e6b432f612c8bd203
The implementation makes use of **MurmurHash3** (https://github.com/aappleby/smhasher/wiki/MurmurHash3) by default, but this can be changed
by providing an hash function of your preference, as long as it has the same signature of MurmurHash3, which is:
```cc++
void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );
```
Basically:
* void pointer to the key
* its length in bytes
* a seed for the hashing function
* a void pointer to 128 bits where the first 64 will be the first half of the hashing result, and the last 64 the second half

<<<<<<< HEAD
# Importing into your projects
* both CountMinSketch.h and MurmurHash3.h must be part of your headers
* MurmurHash3.cc must be part of your sources
=======
>>>>>>> a76668f76db038ffc3e04e7e6b432f612c8bd203
# Usage
Just specify what you prefer in the template:
```cc++
//template <size_t nHashes, size_t columns, probInternal::HashFunction hashFunction = MurmurHash3_x64_128>
//default hash
CountMinSketch<10,6000> c1;
//specify your onw
CountMinSketch<10,6000, myHash> c2;
```
