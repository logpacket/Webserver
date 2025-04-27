//
// Created by Seongyun Jang on 3/24/25.
//

#include "cache.h"
#include "logger.h"

#include <file.h>

Cache::Cache(const size_t max_size) {
  this->cache_buffer = new char[max_size];
  this->max_size = max_size;
  this->current_cache_size = 0;
  this->lru_queue = list<string>();
  Logger::LOG_DEBUG("Cache initialized with size: " + std::to_string(max_size) + " bytes");
}

Cache::~Cache() {
  delete[] this->cache_buffer;
  Logger::LOG_DEBUG("Cache destroyed");
}

void Cache::set(const string &path, const File &file) {
  // Check if the file is already in the cache
  if (this->cache_map.contains(path)) {
    return;
  }

  // Get the size of the file
  const size_t file_size = file.size();

  // Check if there is enough space in the cache
  if (this->current_cache_size + file_size > this->max_size) {
    this->evict();
  }

  // Buffer position pointer
  char *buffer_pos = this->cache_buffer + this->current_cache_size;
  // Read the file into the cache buffer
  file.read(buffer_pos);

  // Store the file in the cache map
  this->cache_map[path] = make_pair(this->cache_buffer + this->current_cache_size, file_size);
  this->current_cache_size += file_size;
}

pair<char *, size_t> Cache::get(const string &path) {
  Logger::LOG_DEBUG("CACHE_HIT: " + path);
  // Remove the file from the LRU queue
  this->lru_queue.remove(path);

  // Check if the file is in the cache map
  if (const auto it = this->cache_map.find(path); it != this->cache_map.end()) {
    // Move the file to the front of the queue
    this->lru_queue.emplace_back(path);
    return it->second;
  }

  // File not found in cache
  return make_pair(nullptr, 0);
}

bool Cache::contains(const string &path) const {
  // Check if the file is in the cache map
  return this->cache_map.contains(path);
}

void Cache::evict() {
  // if the cache is empty, do nothing
  if (this->lru_queue.empty()) {
    return;
  }
  // Get the path of the least recently used file
  const string oldest_path = this->lru_queue.front();

  // Evict the least recently used file from the cache
  if (const auto it = this->cache_map.find(oldest_path); it != this->cache_map.end()) {
    // Get the file data and size
    char* remove_start = it->second.first;
    const size_t remove_size = it->second.second;

    // Move the remaining data in the cache buffer
    char* move_dest = remove_start;
    const char* move_src = remove_start + remove_size;
    if (const size_t move_size = this->current_cache_size - (move_src - this->cache_buffer); move_size > 0) {
      memmove(move_dest, move_src, move_size);

      // Update the cache map with the new positions
      for (auto&[fst, snd] : this->cache_map) {
        // Update the positions of the remaining files after the evicted file
        if (snd.first > remove_start) {
          snd.first -= remove_size;
        }
      }
    }
    // Update cache size
    this->current_cache_size -= it->second.second;
    // Remove the file from the cache map
    this->cache_map.erase(it);
    // Remove the file from the LRU queue
    this->lru_queue.pop_front();

    Logger::LOG_DEBUG("CACHE_EVICTED: " + oldest_path);
  }
}

size_t Cache::get_max_size() const {
  return this->max_size;
}
