#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

using u8  = unsigned char;
using u32 = uint32_t;

std::vector<u8> string_to_bytes(const std::string& input);
void padding(std::vector<u8>& data);
std::vector<std::array<u8, 64>> split(const std::vector<u8>& input);
std::vector<std::array<u32, 16>> blocks_to_words(const std::vector<std::array<u8, 64>>& blocks);
void bit_or (std::vector<std::array<u32, 16>>& blocks_words);
void bit_xor(std::vector<std::array<u32, 16>>& blocks_words);
std::array<u32, 4> merge_all_blocks_into_digest(const std::vector<std::array<u32, 16>>& blocks_words);
std::array<u32, 4> hash(const std::string& input);
