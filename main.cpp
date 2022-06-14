#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include <algorithm>
#include <bitset>

#include "libs/msgpack11/msgpack11.hpp"
#include "src/system/time/Time.h"
#include "src/dynamo/block/Block.h"
#include "src/dynamo/blockchain/Blockchain.h"
#include "src/dynamo/wallet/transaction/Transaction.h"
#include "servers/DNS/DNS_Server.h"
#include "src/dynamo/network/node/Node.h"
#include "src/dynamo/network/protocol/DTP.h"
#include "src/crypto/EC/ECDSA.h"
#include "src/crypto/SHA-2/SHA256.h"

int bind_param(std::string* str, const std::vector<std::string>& vector)
{
    int temp = -1; // This variable will hold the index of the placeholder occurences

    // Return if the amount of arguments is not the same as the amount of placeholders
    if (std::count(str->begin(), str->end(), '?') != vector.size()) return -1;

    // Loop through the parameters and replace the string
    for (auto& element : vector)
    {
        temp = str->find("?", temp + 1); // Find the index of the next placeholder
        str->replace(temp, element.length(), element); // Replace the string
    }

    return 0;
}

int main()
{
    //ECDSA ec;

    //ec.showKeys();

    //ec.signHash("a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3");
    //ec.showSignature();

    /*int result;
    result = ec.verifySignature("02A8EA59DD7D1E96FDFC8B836420E674CBEFC3107EE9AE74D9ACC6BBE5898F4675", "a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3", "0D1D6BEF27094791CE39B10EAA8110405F0BC91CF2A4626D91089468F686D8E7ED736706C18EE9E164ED76795E2B91BA18E4A948AD71CF23C3BE311FC2F74865");

    std::cout << result;
*/
    /*servers::DNS_Server DNS(AF_INET, SOCK_STREAM, 6, 4542, INADDR_ANY, 100, 0);
    DNS.launch();*/

    /*Node n;
    n.start();*/
/*
    std::vector<std::string> params = {"1", "2", "3"};

    std::vector<Transaction> pool;

    std::string me = "me";
    std::string you = "you";

    ECDSA ec;

    Transaction t(&ec ,me, (size_t)10, you, (size_t)20);

    pool.push_back(t);

    int one = 1;
*/
/*
    std::string msgpack_bytes = msgpack.dump();
    std::cout << msgpack_bytes << std::endl;

    std::string err;
    msgpack11::MsgPack msgpack_unpacked = msgpack11::MsgPack::parse(msgpack_bytes, err);

    std::cout << msgpack_unpacked["hey"]["ola"] << std::endl;*/

    using namespace msgpack11;

    MsgPack pack = MsgPack::object {{"key1", "value2"}};
    MsgPack pack10 = MsgPack::object {{"key2", "value3"}};
    MsgPack pack11 = MsgPack::object {{"key10", "value10"}};
    MsgPack pack12 = MsgPack::object {{"key11", 10}};

    MsgPack::array array {pack, pack10, pack12};

    array.push_back(pack11);

    MsgPack arr = array;
    

    MsgPack pack1 = MsgPack::object {
        {"array", arr}
    };


    MsgPack parent = MsgPack::object {
        {"object", pack1}
    };

    std::string ser = parent.dump();

    std::string err;
    MsgPack pack2 = MsgPack::parse(ser, err);

    std::cout << "err: " << err << std::endl;
    //std::cout << pack2["object"]["array"][2] << std::endl;

    DTP::Packet packet(0, std::string("me"), std::string("you"), 10, pack2.dump());

    std::cout << "Payload: " << packet.getPayload() << std::endl;

    MsgPack parsed = MsgPack::parse(packet.getPayload(), err);
    std::cout << parsed["object"]["array"][2]["key11"].int_value() << std::endl;

    /*uint8_t* hash;

    std::string str = crypto::sha256("123");

    hash = ECDSA::hexToUint8("A665A45920422F9D417E4867EFDC4FB8A04A1F3FFF1FA07E998E86F7F7A27AE3");

    char hexHash[ECC_BYTES*2+1];
    
    ECDSA::toHex(hexHash, sizeof(hexHash), hash, sizeof(hash));

    std::cout << "Hex hash: " << str << " | size in bytes: " << sizeof(str) << std::endl;
    std::cout << "Uint8 hash: " << hash << " | size in bytes: " << sizeof(hash) << std::endl;
    std::cout << hexHash << std::endl;*/

    /*SHA256 sha;
    sha.update("123");
    uint8_t* hash = sha.digest();

    sha.update("hey");
    uint8_t* lastHash = sha.digest();

    Block myBlock(Time::getTimestamp(), hash, lastHash, 0, 10, 20, {"Hey"});

    myBlock.printBlock();

    Block* minedBlock;
    minedBlock = Block::mineBlock(myBlock, {"123"}, 0);

    minedBlock->printBlock();
*/
    /*uint8_t buffer[32];

    setTarget2(buffer, sizeof(buffer), 13);*/

    /*ECDSA ec;

    SHA256 sha;
    sha.update("123");
    uint8_t* hash = sha.digest();

    sha.update("hey");
    uint8_t* lastHash = sha.digest();


    char* pubKey = ec.getPublicKeyHex();
    std::string publicKey(pubKey);

    std::cout << publicKey << std::endl;

    Blockchain myBlockchain(1, publicKey);

    myBlockchain.printChain();

    std::string recipient = "you";
    std::string sender = "me";

    Transaction transaction(nullptr, recipient, (size_t)2, sender, (size_t)10);

    std::vector<Transaction> transactionArray;

    transactionArray.push_back(transaction);

    Block myBlock(Time::getTimestamp(), hash, lastHash, 0, 10, 19, transactionArray);

    Block* minedBlock;
    minedBlock = Block::mineBlock(myBlock, transactionArray, 1);

    minedBlock->printBlock();*/
/*
    Block* genesis;
    genesis = Block::genesis(recipient, 10);

    genesis->printBlock();
*/
    /*DTP::Packet packet(0, std::string("me"), std::string("you"), 10, std::string("123"));

    packet.show();

    std::string buffer = packet.buffer();

    std::cout << buffer << std::endl;*/
}