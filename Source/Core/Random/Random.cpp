#include "Random.h"
#include <random>
#include <sstream>
#include <string>
#include <iomanip>

bool operator==(const guid_t& left, const guid_t& right) 
{
  return (left.high == right.high) && (left.low == right.low);
}

// Generate a random 128 bits ID
guid_t get_random_id()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

  uint32_t data[4]{};
  for (int i = 0; i < 4; i++)
    data[i] = dis(gen);

  data[1] = (data[1] & 0xFFFF0FFF) | 0x00004000;
  data[2] = (data[2] & 0x3FFFFFFF) | 0x80000000;
  
  return {
    (static_cast<uint64_t>(data[0]) << 32) | data[1],
    (static_cast<uint64_t>(data[2]) << 32) | data[3]
  };
}

// Get a GUID as a string.
std::string get_guid_str(const guid_t &_guid)
{
  std::ostringstream _ss_full;

  _ss_full << std::hex << std::uppercase << std::setfill('0') << _guid.high << _guid.low;

  std::string str = _ss_full.str();

  str.insert(str.begin() + 8, '-');
  str.insert(str.begin() + 13, '-');
  str.insert(str.begin() + 18, '-');
  str.insert(str.begin() + 23, '-');

  return str;
}

