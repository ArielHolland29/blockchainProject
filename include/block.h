#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <random>
#include <ctime>
#include <limits>
#include <chrono>

class Blockchain {
    private:
        std::list<class Block> chain;
    public:
        void addBlock(const Block& block) {
            chain.push_back(block);
        }
        const std::list<Block>& getChain() const {
            return chain;
        }

};

class Block {
    private:
        std::vector<std::string> data;
        int nonce;
        std::uint64_t timeStamp;
        std::string previousHash;
        std::string merkleRoot;
        int version;
        uint32_t T;

    public:
        Block(std::vector<std::string> data, int nonce, std::uint64_t timeStamp,
            std::string previousHash, std::string merkleRoot, int version, uint32_t T)
        : data(std::move(data)), nonce(nonce), timeStamp(timeStamp),
            previousHash(std::move(previousHash)), merkleRoot(std::move(merkleRoot)),
            version(version), T(T) {}

        Block(std::vector<std::string> data, int nonce,
            std::string previousHash, std::string merkleRoot, int version, uint32_t T)
        : data(std::move(data)), nonce(nonce),
            timeStamp(static_cast<std::uint64_t>(
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count())),
            previousHash(std::move(previousHash)), merkleRoot(std::move(merkleRoot)),
            version(version), T(T) {}
        
        const std::string& getMerkleRoot() const { return merkleRoot; }
        std::uint64_t getTimeStamp() const { return timeStamp; }
        int getNonce() const { return nonce; }
        uint32_t getT() const { return T; }
    const std::vector<std::string>& getData() const { return data; }
};

