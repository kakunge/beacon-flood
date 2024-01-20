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
} __attribute__((__packed__));

struct Dot11Frame {
    uint8_t type;
    uint8_t flag;
} __attribute__((__packed__));

struct BeaconFrame : Dot11Frame {
    uint16_t duration;
    uint8_t destAddr[6];
    uint8_t sourAddr[6];
    uint8_t BSSID[6];
    uint16_t seqControl;
    uint64_t timestamp;
    uint16_t beaconInterval;
    uint16_t capacityInfo;
} __attribute__((__packed__));

struct Tag {
    uint8_t tagNumber;
    uint8_t tagLength;
} __attribute__((__packed__));

struct SSIDParameter : Tag {
    uint8_t SSID[7];
} __attribute__((__packed__));

struct SupportedRates : Tag {
    uint8_t supportedRates[8];
} __attribute__((__packed__));

struct TrafficIndicationMap : Tag {
    uint8_t DTIMCount;
    uint8_t DTIMPeriod;
    uint8_t bitmapControl;
    uint8_t PVB;
} __attribute__((__packed__));

struct HTCapabilities : Tag {
    uint16_t HTCapInfo;
    uint8_t A_MPDUParams;
    uint8_t MCSSet[16];
    uint16_t HTExtCaps;
    uint32_t TxBF;
    uint8_t ASEL; 
} __attribute__((__packed__));

struct HTInformation : Tag {
    uint8_t PrimaryChan;
    uint8_t HTInfoSubset1;
    uint16_t HTInfoSubset2;
    uint16_t HTInfoSubset3;
    uint8_t MCSSet[16];
} __attribute__((__packed__));

struct ExtendedCapabilities : Tag {

} __attribute__((__packed__));


struct TestPacket {
    Radiotap radiotap;
    BeaconFrame beaconFrame;
    SSIDParameter ssidParameter;
    SupportedRates supportedRates;
    TrafficIndicationMap trafficIndicationMap;
    HTCapabilities htCapabilities;
    HTInformation htInformation;
    uint32_t FCS;
} __attribute__((__packed__));