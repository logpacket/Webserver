//
// Created by Seongyun Jang on 3/24/25.
//

#ifndef CACHE_H
#define CACHE_H
#include <file.h>
#include <string>
#include <unordered_map>
#include <list>

using namespace std;

/**
 * Cache class to store file data in memory with LRU eviction policy
 */
class Cache {
public:
  /**
   * Constructor
   * @param max_size Maximum size of the cache in bytes
   */
  explicit Cache(size_t max_size);
  ~Cache();
  /**
   * Set a file in the cache
   * @param path Path of the file
   * @param file Reference of File object
   */
  void set(const string &path, const File &file);

  /**
   * Get a file from the cache
   * @param path Path of the file
   * @return pair of pointer to the file data and its size
   */
  pair<char *, size_t> get(const string &path);

  /**
   * Check if a file is in the cache
   */
  bool contains(const string &path) const;

  /**
   * Get maximum size of the cache
   */
  size_t get_max_size() const;

private:
  /**
   * Map to store file paths and their corresponding pointer and size in the
   * cache
   */
  unordered_map<string, pair<char *, size_t>> cache_map;
  /**
   * Queue to store the least recently used file paths
   */
  list<string> lru_queue;
  /**
   * Buffer to store file data
   */
  char *cache_buffer;
  /**
   * Maximum size of the cache in bytes
   */
  size_t max_size;
  /**
   * Current size of the cache in bytes
   */
  size_t current_cache_size = 0;
  /**
   * Evict the least recently used file from the cache
   */
  void evict();
};

#endif // CACHE_H
