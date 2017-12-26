# Count-min sketch (https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch)
A c++11 Count-min sketch implementation.
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

# Usage
Just specify what you prefer in the template:
```cc++
//template <size_t nHashes, size_t columns, probInternal::HashFunction hashFunction = MurmurHash3_x64_128>
//default hash
CountMinSketch<10,6000> c1;
//specify your onw
CountMinSketch<10,6000, myHash> c2;
```
