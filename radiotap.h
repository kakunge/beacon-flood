#pragma once
#pragma pack(1)

#include <cstdint>
#include <list>
#include <stdio.h>

// ipTIME A2000UA-4dBi
struct Radiotap {
    uint8_t version;
    uint8_t pad;
    uint16_t len;
    // std::list<uint32_t> present;
    // std::list<uint32_t>::iterator iter = present.begin();
    uint32_t present;
    uint32_t present2;
    uint32_t present3;
    uint8_t flag;
    uint8_t dataRate;
    uint16_t channelFreq;
    uint16_t channelFlag;
    uint8_t anteSig;
    uint8_t pad2;
    uint16_t sigQual;
    uint16_t RXFlag;
    uint8_t anteSig0;
    uint8_t antenna0;
    uint8_t anteSig1;
    uint8_t antenna1;
};

struct Dot11Frame {
    uint8_t type;
    uint8_t flag;
};

struct BeaconFrame : Dot11Frame {
    uint16_t duration;
    uint8_t destAddr[6];
    uint8_t sourAddr[6];
    uint8_t BSSID[6];
    uint16_t seqControl;
    uint64_t timestamp;
    uint16_t beaconInterval;
    uint16_t capacityInfo;
    uint8_t elementID;
    uint8_t len;
    uint8_t SSID[32];
};
