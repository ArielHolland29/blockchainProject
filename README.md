# Simplified_blockchain

Build (Windows cmd.exe with g++):

```cmd
cd c:\\Users\\Simplified_blockchain-1

g++ -Iinclude src\\main.cpp src\\functions.cpp src\\hash.cpp -o blockchain
```

Run:

```cmd
blockchain
```

---

## This program simulates simplified blockchain.

First, it generates 1000 users and 10000 random transactions between them, which are pending until the block is mined.
Then genesis block is generated and after that mining begins until all transactions are confirmed.
Each block has 100 txs which are printed in seperate .txt files.

This program uses hashing function that can be found in src/hash.cpp

After mining all of the blocks, you can view specific user's balance.

---

## Classes info

```
class User{
    private:
        std::string name;
        int64_t publicKey;
        double balance;

        static uint64_t generate_key()
    public:

        User(std::string name_, double balance_)
        const std::string& getName()
        uint64_t getPublicKey()
        double getBalance()
        void setBalance(double newBalance)
}

class Transaction{

    private:
        std::string senderKey;
        std::string receiverKey;
        double amount;
        int transactionID;

    public:
        std::string get_senderKey()
        std::string get_receiverKey()
        double get_amount()
        int get_transactionID()

        Transaction(std::string senderKey_, std::string receiverKey_, double amount_, int transactionID_): 
        senderKey(senderKey_), receiverKey(receiverKey_), amount(amount_), transactionID(transactionID_){};


};

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

        Block(std::vector<std::string> data, int nonce,
            std::string previousHash, std::string merkleRoot, int version, uint32_t T)
        
        const std::string& getMerkleRoot()
};

```

Each block has some data (transactions, which are picked from transaction pool), nonce(a random number), timestamp(time the block is mined), previous block hash, merkle root(not real one), version and a target. To mine a block, we simply hash current block's header with changing nonce, after we hit a number that is smaller that target T, mining is succesful and block's transactions are confirmed.

---

## Notes

This version does not implement real merkle tree, instead it uses hash of concatenated transactions in a block.




