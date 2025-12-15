#include <iostream>
// Logger struct for debug
struct Logger { void log(std::string s){} };
#include <cmath>
#include <vector>
#include <string>
#include "block.h"
#include "functions.h"
#include "hash.h"
#include "user.h"

using u32 = uint32_t;
using u64 = uint64_t;
using u8 = uint8_t;

int main(){ // Main entry point for simulation

    std::cout << "Blockchain simulation\n";
    std::vector<User> users = generate_users(1000);
    std::vector<Transaction> transaction_pool = create_transactions(users, 10000);
    std::cout << "Users generated and pending transactions created.\n";

    Block genesis = create_genesis_block();
    Blockchain blockchain;
    blockchain.addBlock(genesis);
    write_transactions_to_file(genesis, 0);

    for(int i = 0; i < 100; ++i){
        std::string prevHash = blockchain.getChain().empty() ? std::string("0") : blockchain.getChain().back().getMerkleRoot();
        Block b = mine_block(users, transaction_pool, prevHash, 8);
        blockchain.addBlock(b);
        size_t index = blockchain.getChain().size() - 1;
        write_transactions_to_file(b, index);
    }

    std::cout << blockchain.getChain().size() << " blocks in the blockchain.\n";


    std::cout << "Final Balances of first 10 users:\n";
    for(int i = 0; i < 10 && i < users.size(); ++i){
        std::cout << "User " << i + 1<< "balance:" << users[i].getBalance() << std::endl;
    }

    int idx;
    char op;
    char op2;
    std::cout << "Do you want to see specific user's balances?(y/n): ";
    std::cin >> op;
    if(op = 'y'){
        while(true){
            std::cout << "Enter user's index: ";
            std::cin >> idx;
            std::cout << std::endl;
            
            std::cout << users[idx].getBalance() << "\n";

            std::cout << "Exit?(y/n):";
            std::cin >> op2;
            if(op2 = 'y'){
                break;
            }
        }
    }

    
    


    return 0;
}

// Adjusted code style
