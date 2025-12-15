#pragma once

#include <string>
#include <random>

class User {
    private:
        std::string name;
        int64_t publicKey;
        double balance;

        static uint64_t generate_key() {
            static std::mt19937_64 rng(std::random_device{}());
            static std::uniform_int_distribution<uint64_t> dist(
                0, std::numeric_limits<uint64_t>::max()
            );
            return dist(rng);
        }

    public:

        User(std::string name_, double balance_) : name(std::move(name_)), publicKey(generate_key()), balance(balance_){}

        const std::string& getName() const {return name;}
        uint64_t getPublicKey() const {return publicKey;}
        double getBalance() const {return balance;}

        void setBalance(double newBalance) { balance = newBalance; }

};