#include <iostream>
#include <string>
#include <vector>
#include "block.h"
#include "user.h"
#include "hash.h"
#include <sstream>
#include <iomanip>
#include "functions.h"
#include "transaction.h"
#include <fstream>
#include <filesystem>
#include <unordered_map>

using u32 = uint32_t;
using u8 = uint8_t;
using u64 = uint64_t;


std::vector<User> generate_users(size_t count) {
    std::vector<User> users;
    users.reserve(count);

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_real_distribution<double> coins(100.0, 1000000.0);

    for(size_t i = 0; i < count; ++i){
        users.emplace_back("User" + std::to_string(i), coins(rng));
    }

    return users;
}
std::vector<Transaction> create_transactions(std::vector<User>& users, size_t tx_count){
    std::vector<Transaction> out;
    out.reserve(tx_count);

    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<size_t> pick_user(0, users.size() - 1);

    int next_id = 1;

    for (size_t i = 0; i < tx_count; ++i) {

        size_t s; int tries = 0;
        do {
            s = pick_user(rng);
            if (++tries > 50) return out;
        } while (users[s].getBalance() < 1.0);

        size_t r;
        do { r = pick_user(rng); } while (r == s);

        double sb = users[s].getBalance();
        std::uniform_real_distribution<double> pick_amount(1.0, sb * 0.1);
        double amount = pick_amount(rng);
        if (amount > sb) amount = sb;

        std::string senderKey = std::to_string(users[s].getPublicKey());
        std::string receiverKey = std::to_string(users[r].getPublicKey());

        out.emplace_back(senderKey, receiverKey, amount, next_id++);
        const Transaction& tx = out.back();
        std::cout << " id=" << tx.get_transactionID()
                  << " from=" << tx.get_senderKey()
                  << " to=" << tx.get_receiverKey()
                  << " amount=" << tx.get_amount() << '\n';
    }

    return out;
}

std::string to_line(const Transaction& t) {
    std::ostringstream oss;
    oss << t.get_senderKey() << '|' << t.get_receiverKey()
        << '|' << t.get_amount() << '|' << t.get_transactionID();
    return oss.str();
}

std::vector<std::string> to_lines(const std::vector<Transaction>& txs) {
    std::vector<std::string> v;
    v.reserve(txs.size());
    for (const auto& t : txs) v.push_back(to_line(t));
    return v;
}

std::array<u32, 4> calculate_merkle_value(const std::vector<std::string>& transactions) {

    std::vector<std::array<u32,4>> leaves;
    leaves.reserve(transactions.size());
    for (const auto &tx : transactions) {
        leaves.push_back(hash(tx));
    }

    while (leaves.size() > 1) {
        std::vector<std::array<u32,4>> next;
        for (size_t i = 0; i < leaves.size(); i += 2) {
            if (i + 1 < leaves.size()) {
                std::string a = hash_to_string(leaves[i]);
                std::string b = hash_to_string(leaves[i+1]);
                next.push_back(hash(a + b));
            } else {
                std::string a = hash_to_string(leaves[i]);
                next.push_back(hash(a + a));
            }
        }
        leaves.swap(next);
    }

        std::string input;
        for (const auto& tx : transactions) input += tx;
        return hash(input);
}

std::vector<Transaction> get_transactions_from_pool(std::vector<Transaction>& transaction_pool){
    int max = 100;
    std::vector<Transaction> transactions_for_block;

    for(int i  = 0; i < max && !transaction_pool.empty(); ++i){
        transactions_for_block.push_back(transaction_pool.back());
        transaction_pool.pop_back();
    }

    return transactions_for_block;
}

std::string hash_to_string(const std::array<u32, 4>& hash) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (u32 part : hash) {
        oss << std::setw(8) << part;
    }
    return oss.str();
}

static inline int clz32(u32 x) {
    if (x == 0) return 32;
#if defined(_MSC_VER)
    unsigned long index;
    _BitScanReverse(&index, x);
    return 31 - static_cast<int>(index);
#else
    return __builtin_clz(x);
#endif
}

static int count_leading_zero_bits(const std::array<u32,4>& h) {
    int total = 0;
    for (size_t i = 0; i < h.size(); ++i) {
        if (h[i] == 0) {
            total += 32;
        } else {
            total += clz32(h[i]);
            break;
        }
    }
    return total;
}

static bool meets_target(const std::array<u32,4>& h, uint32_t T) {
    return static_cast<uint32_t>(count_leading_zero_bits(h)) >= T;
}


Block mine_block(std::vector<User>& users, std::vector<Transaction>& transaction_pool, const std::string& previousHash, uint32_t difficulty_T) {
    const int max_txs = 100;
    std::vector<Transaction> txs;
    txs.reserve(max_txs);

    std::unordered_map<std::string, size_t> key_to_idx;
    key_to_idx.reserve(users.size());
    for (size_t i = 0; i < users.size(); ++i) {
        key_to_idx.emplace(std::to_string(users[i].getPublicKey()), i);
    }

    std::vector<double> temp_balances;
    temp_balances.reserve(users.size());
    for (const auto& u : users) temp_balances.push_back(u.getBalance());

    while (txs.size() < static_cast<size_t>(max_txs) && !transaction_pool.empty()) {
        Transaction t = transaction_pool.back();
        transaction_pool.pop_back();

        auto it_s = key_to_idx.find(t.get_senderKey());
        auto it_r = key_to_idx.find(t.get_receiverKey());
        if (it_s == key_to_idx.end() || it_r == key_to_idx.end()) {
            continue;
        }
        size_t si = it_s->second;
        size_t ri = it_r->second;
        double amt = t.get_amount();
        if (amt <= 0.0) continue;

        if (temp_balances[si] >= amt) {
            temp_balances[si] -= amt;
            temp_balances[ri] += amt;
            txs.push_back(t);
        } else {
            continue;
        }
    }

    std::vector<std::string> leaves = to_lines(txs);
    std::array<u32, 4> merkle_hash = calculate_merkle_value(leaves);
    std::string merkle = hash_to_string(merkle_hash);

    uint64_t timestamp = static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());
    uint32_t nonce = 0;

    while (true) {
    std::ostringstream oss;
    oss << previousHash << '|' << merkle << '|' << timestamp << '|' << nonce << '|' << difficulty_T;
        std::string header = oss.str();

        std::array<u32,4> header_hash = hash(header);
        std::cout << "Nonce: " << nonce << " Hash: " << hash_to_string(header_hash) << "\n";
        if (meets_target(header_hash, difficulty_T)) {
            break;
        }

        ++nonce;
        if ((nonce & 0xFFFF) == 0) {
            timestamp = static_cast<uint64_t>(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count());
        }
    }

    for (const auto& t : txs) {
        auto it_s = key_to_idx.find(t.get_senderKey());
        auto it_r = key_to_idx.find(t.get_receiverKey());
        if (it_s == key_to_idx.end() || it_r == key_to_idx.end()) continue;
        size_t si = it_s->second;
        size_t ri = it_r->second;
        double amt = t.get_amount();
        if (amt <= 0.0) continue;
        if (users[si].getBalance() >= amt) {
            users[si].setBalance(users[si].getBalance() - amt);
            users[ri].setBalance(users[ri].getBalance() + amt);
        }
    }

    Block new_block(leaves, static_cast<int>(nonce), timestamp, previousHash, merkle, 1, difficulty_T);

    return new_block; 

}

Block create_genesis_block() {
    std::vector<Transaction> genesis_txs;

    genesis_txs.emplace_back("0", "genesis", 1000000.0, 1);

    std::vector<std::string> lines = to_lines(genesis_txs);
    std::array<u32,4> merkle_hash = calculate_merkle_value(lines);
    std::string merkle = hash_to_string(merkle_hash);

    std::uint64_t timestamp = 1630000000000ULL;
    int nonce = 0;
    int version = 1;
    uint32_t T = 0;

    return Block(lines, nonce, timestamp, "0", merkle, version, T);
}

void write_transactions_to_file(const Block& block, size_t index) {
    try {
        std::filesystem::path txdir = std::filesystem::path("transactions");
        if (!std::filesystem::exists(txdir)) {
            std::filesystem::create_directories(txdir);
        }

        std::ostringstream fname;
        fname << "tx" << index << ".txt";
        std::filesystem::path filepath = txdir / fname.str();

        std::ofstream ofs(filepath);
        if (!ofs) {
            std::cerr << "Failed to open " << filepath.string() << " for writing transactions.\n";
            return;
        }

        const auto& lines = block.getData();
        for (const auto& line : lines) {
            ofs << line << '\n';
        }

        ofs.close();
    } catch (const std::exception& e) {
        std::cerr << "Exception while writing transactions to file: " << e.what() << "\n";
    }
}
