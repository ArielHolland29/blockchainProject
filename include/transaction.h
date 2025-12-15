#pragma once

#include <string>

class Transaction{

    private:
        std::string senderKey;
        std::string receiverKey;
        double amount;
        int transactionID;

    public:
        std::string get_senderKey() const {
            return senderKey;
        }
        std::string get_receiverKey() const {
            return receiverKey;
        }
        double get_amount() const {
            return amount;
        }
        int get_transactionID() const {
            return transactionID;
        }


        Transaction(std::string senderKey_, std::string receiverKey_, double amount_, int transactionID_): 
        senderKey(senderKey_), receiverKey(receiverKey_), amount(amount_), transactionID(transactionID_){};


};
