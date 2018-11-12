// Copyright (c) 2011 Google, Inc.
//
// doesn't hold for any hash functions in this file.

#ifndef CITY_HASH_H_
#define CITY_HASH_H_

#include <stdlib.h>  // for size_t.
#include <stdint.h>
#include <utility>
#include <vector>

typedef uint8_t uint8;
typedef uint32_t uint32;
typedef uint64_t uint64;

// Hash function for a byte array.
size_t CityHash64Mod(const char *s, size_t len, size_t mod);
size_t CityHash64IdataMod(const char *s, size_t len, size_t mod);

#endif  // CITY_HASH_H_
