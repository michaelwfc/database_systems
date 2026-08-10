
#include <algorithm> // Includes std::remove_if to remove elements from vectors.
#include <cstdint>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <unordered_map>


uint64_t Hash(const std::string & key, uint64_t seed){
  std::hash<std::string> hasher;
  return hasher(key+ std::to_string(seed));
}


class CountMinSketch {

public:
  CountMinSketch(uint32_t width, uint32_t depth)
  : 
    width_(width), 
    depth_(depth),
    // constructor, creates a count-min sketch with width columns (buckets) and depth rows (hash functions).
    // "Construct a vector containing depth rows, each row is a vector of width zeros."
    table_(depth, std::vector<uint64_t>(width,0)),
    // A simple CMS needs depth independent hash functions.
    // hash_i(key) = hash(key + seed_i)
    seeds_(depth){
      for(uint32_t i=0;i<depth;i++){
        seeds_[i]=i+1;
      }
    }

   // move constructor
   CountMinSketch(CountMinSketch &&other)
   :
    width_(other.width_),
    depth_(other.depth_),
    table_(std::move(other.table_)),
    seeds_(std::move(other.seeds_))
    {};

    // move assignment
    CountMinSketch &operator=(CountMinSketch &&other){
      width_ = other.width_;
      depth_ = other.depth_;
      table_ = std::move(other.table_);
      seeds_ = std::move(other.seeds_);
    }

    // 

private:
  uint32_t width_;
  uint32_t depth_;
  // count_matrix
  std::vector<std::vector<uint64_t>> table_;
  //hash seeds
  std::vector<uint64_t> seeds_;
};