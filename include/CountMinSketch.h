#ifndef COUNTMINSKETCH_H
#define COUNTMINSKETCH_H

#include <algorithm> //min
#include <cstring>   //memcpy and memset

#include <MurmurHash3.h>

namespace probStruct {

namespace probInternal {
typedef void (*HashFunction)(const void *, int, uint32_t, void *);
}

/**
 * @brief A CountMinSketch, a probabilistic data structure which serves as
 * a frequency table of events in a stream of data, using sub-linear space
 * at the price of an approximation, since it can overcount some items
 * due to collisions, and thus providing an upper bound and not an exact
 * result.
 * For more details and for deciding what number of rows and columns to
 * use check out https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch
 * or simply google it out.
 * Usually implementations allow to specify epsilon and delta (the approximation
 * parameters), since floating point numbers can't be template arguments
 * simply allowing to specify the number of rows and columns seemed
 * reasonable, templating has been preferred to specifying epsilon and
 * delta in the constructor because it led to better performance when
 * compiling without optimization.
 * rows = ⌈ln 1/δ⌉
 * columns = ⌈e/ε⌉
 *
 * @tparam nHashes Number of hashes to use, which will directly translate
 * to the number of rows of the min sketch.
 * @tparam columns Number of columns to use.
 * @tparam hashFunction Which hash function to use. The hash function
 * must be of type void <function> (const void*, int, uint32_t, void*).
 * You can create a CountMinSketch using the default hash function
 * of choice (MurmuHash3_x64_128) by doing
 * CountMinSketch<hashes, columns>.
 */
template <size_t nHashes, size_t columns,
          probInternal::HashFunction hashFunction = MurmurHash3_x64_128>
class CountMinSketch {
public:
  /**
   * @brief Constructor where nHashes (number of hashes) and columns can
   * be directly specified.
   */
  CountMinSketch() : data(new size_t[nHashes * columns]) {
    static_assert(nHashes > 0, "Number of hashes must be positive");
    static_assert(columns > 0, "Number of columns must be positive");
    memset(this->data, 0, nHashes * columns * sizeof(*data));
  }

  /**
   * @brief Copy constructor.
   *
   * @param copied
   */
  CountMinSketch(const CountMinSketch &copied)
      : data(new size_t[nHashes * columns]) {
    memcpy(data, copied.data, sizeof(*data) * nHashes * columns);
  }

  /**
   * @brief Copies the input CountMinSketch's data,
   * after this operation both instances will return
   * the same result for method frequencyOf (until
   * differently modified).
   *
   * @param assigned CountMinSketch to copy data from.
   */
  void operator=(const CountMinSketch &assigned) {
    memcpy(data, assigned.data, sizeof(*data) * nHashes * columns);
  }

  virtual ~CountMinSketch() { delete[] this->data; }

  /**
   * @brief Adds the item to the structure, incrementing by one
   * each bucket mapped to it.
   *
   * @param data Pointer to the item.
   * @param length Length of the item in bytes.
   */
  void add(const void *item, unsigned length) {
    // 128b hash, [0] is the first half and [1] the second
    uint64_t hash128[2];
    hashFunction(item, length, 0, hash128);

    size_t *tmp = data;
    for (size_t i = 0; i < nHashes; i++) {
      // use hash128[0] as an accumulator, each different hash is computed as
      // (hash128[0] + (i+1) * hash128[1]) % columns
      hash128[0] += hash128[1];
      tmp[hash128[0] % columns]++;

      // increment pointer since we are moving to the next row
      tmp += columns;
    }
  }

  /**
   * @brief Adds the item to the structure, incrementing by
   * "increment" each bucket mapped to it.
   *
   * @param item Pointer to the item.
   * @param length Length of the item in bytes.
   * @param increment Value used to increment the buckets.
   */
  void add(const void *item, unsigned length, size_t increment) {
    // 128b hash, [0] is the first half and [1] the second
    uint64_t hash128[2];
    hashFunction(item, length, 0, hash128);

    size_t *tmp = data;
    for (size_t i = 0; i < nHashes; i++) {
      // use hash128[0] as an accumulator, each different hash is computed as
      // (hash128[0] + (i+1) * hash128[1]) % columns
      hash128[0] += hash128[1];
      tmp[hash128[0] % columns] += increment;

      // increment pointer since we are moving to the next row
      tmp += columns;
    }
  }

  /**
   * @brief Gets the frequency of an item.
   *
   * @param data Pointer to the item.
   * @param length Length of the item in bytes.
   *
   * @return The min count of the item.
   */
  size_t frequencyOf(const void *item, unsigned length) const {
    // 128b hash, [0] is the first half and [1] the second
    uint64_t hash128[2];
    hashFunction(item, length, 0, hash128);

    size_t res = std::numeric_limits<size_t>::max();
    size_t *tmp = data;
    for (size_t i = 0; i < nHashes; i++) {
      // use hash128[0] as an accumulator, each different hash is computed as
      // (hash128[0] + (i+1) * hash128[1]) % columns
      hash128[0] += hash128[1];
      res = std::min(res, tmp[hash128[0] % columns]);

      // increment pointer since we are moving to the next row
      tmp += columns;
    }
    return res;
  }

  /**
   * @brief Set all buckets to 0.
   */
  void clear() { memset(this->data, 0, nHashes * columns * sizeof(*data)); }

  // getters
  size_t getColumns() const { return columns; }
  size_t getNHashes() const { return nHashes; }

private:
  size_t *const data;
};
} // namespace probStruct
#endif
