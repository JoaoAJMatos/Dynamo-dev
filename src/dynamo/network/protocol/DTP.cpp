#include "DTP.h"

#include <utility>

DTP::Packet::Packet(int type, std::string origin, std::string destination, int port, std::string payload)
{
    this->indicator = DTP_INDICATOR;
    this->packetHeader.type = type;
    this->packetHeader.origin = std::move(origin);
    this->packetHeader.destination = std::move(destination);
    this->packetHeader.port = port;
    this->payloadSize = payload.length();
    this->payload = payload;
}

DTP::Packet::Packet(const std::string& _buffer)
{
    int pos = 0;
    std::string delimiter = "|";

    std::string buffer = _buffer;

    this->indicator = atoi(buffer.substr(pos, buffer.find(delimiter)).c_str());
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->packetHeader.type = atoi(buffer.substr(pos, buffer.find(delimiter)).c_str());
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->packetHeader.origin = buffer.substr(pos, buffer.find(delimiter));
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->packetHeader.destination = buffer.substr(pos, buffer.find(delimiter));
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->packetHeader.port = atoi(buffer.substr(pos, buffer.find(delimiter)).c_str());
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->payloadSize = atoi(buffer.substr(pos, buffer.find(delimiter)).c_str());
    buffer.erase(0, buffer.find(delimiter) + delimiter.length());

    this->payload = buffer.substr(pos, buffer.find(delimiter));
}

std::string DTP::Packet::buffer()
{
    std::stringstream ss;
    ss << this->indicator << "|" << this->packetHeader.type << "|" << this->packetHeader.origin << "|" << this->packetHeader.destination << "|" << this->packetHeader.port << "|" << this->payloadSize << "|" << this->payload << "|";
    return ss.str();
}

void DTP::Packet::show()
{
    std::cout << std::endl;
    std::cout << " ===== HEADERS =====" << std::endl;
    std::cout << " - Indicator: " << this->indicator << std::endl;
    std::cout << " - Type: " << this->packetHeader.type << std::endl;
    std::cout << " - Origin: " << this->packetHeader.origin << std::endl;
    std::cout << " - Destination: " << this->packetHeader.destination << std::endl;
    std::cout << " - Port: " << this->packetHeader.port << std::endl << std::endl;
    std::cout << " =====  BODY  =====" << std::endl;
    std::cout << " - Payload: " << this->payload << std::endl << std::endl;
}

DTP::header DTP::Packet::headers()
{
    return this->packetHeader;
}

std::string DTP::Packet::getPayload()
{
    return this->payload;
}

int DTP::Packet::getIndicator()
{
    return this->indicator;
}

int DTP::Packet::getPayloadSize()
{
    return this->payloadSize;
}