#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <array>
#include <random>
#include "block.h"
#include "user.h"
#include "transaction.h"

using u32 = uint32_t;
using u8 = uint8_t;
using u64 = uint64_t;

std::vector<User> generate_users(size_t count = 1000);
std::vector<Transaction> create_transactions(std::vector<User>& users, size_t tx_count = 10000);
std::array<u32, 4> calculate_merkle_value(const std::vector<std::string>& transactions);

std::vector<Transaction> get_transactions_from_pool(std::vector<Transaction>& transaction_pool);
std::vector<std::string> to_lines(const std::vector<Transaction>& txs);
std::string to_line(const Transaction& t);
std::string hash_to_string(const std::array<u32, 4>& hash);
Block create_genesis_block();
Block mine_block(std::vector<User>& users, std::vector<Transaction>& transaction_pool, const std::string& previousHash, uint32_t difficulty_T);
void write_transactions_to_file(const class Block& block, size_t index);
