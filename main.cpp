#include <cstdio>
#include <fstream>
#include <pcap.h>
#include <string>
#include <vector>
#include <cstdint>
#include <array>
#include <algorithm>
#include <chrono>
#include <thread>
#include "radiotap.h"

using namespace std;

void usage() {
	printf("syntax : beacon-flood <interface> <ssid-list-file>\n");
	printf("sample : beacon-flood wlan0 ssid-list.txt\n");
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		usage();
		return -1;
	}

    char* dev = argv[1];
    char* filename = argv[2];
	char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = pcap_open_live(dev, BUFSIZ, 1, 1, errbuf);
    
    if (pcap == nullptr) {
		fprintf(stderr, "couldn't open device %s(%s)\n", dev, errbuf);
		return -1;
	}

    vector<string> SSIDList;
    ifstream file;
    string tempString;

    file.open(filename);

    if (!file) {
		printf("there is an error\n");
		return -1;
	}

	while (getline(file, tempString)) {
		SSIDList.push_back(tempString);
	}
    
	file.close();

    for (int i = 0; i < SSIDList.size(); i++) {
        printf("%d : %s\n", i, SSIDList[i].c_str());
    }

    Radiotap radiotap = {
        0x00,
        0x00,
        0x0020,
        0xa00040ae,
        0xa0000820,
        0x00000820,
        0x10,
        0x02,
        0x096c,
        0x00a0,
        0xbc,
        0x00,
        0x0048,
        0x0000,
        0xb2,
        0x00,
        0xb0,
        0x01
    };

    BeaconFrame beaconFrame = {
        0x80,
        0x00,
        0x0000,
        {0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
        {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
        {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
        0xab40,
        0x0000000000000000,
        0x0000,
        0x0c11
    };

    uint8_t tempSSID[32];
    vector<SSIDParameter> ssidParameters;

    for (int i = 0; i < SSIDList.size(); i++) {

        for (int j = 0; j < SSIDList[i].size(); j++) {
            tempSSID[j] = SSIDList[i].data()[j];
        }

        SSIDParameter ssidParameter = {
            0x00,
            0x20,
            array<uint8_t, 32>{}
        };

        copy(tempSSID, tempSSID + SSIDList[i].size(), ssidParameter.SSID.begin());

        ssidParameters.push_back(ssidParameter);
    }

    SupportedRates supportedRates = {
        0x01,
        0x08,
        {0x82, 0x84, 0x0b, 0x16, 0x24, 0x30, 0x48, 0x6c}
    };

    TrafficIndicationMap trafficIndicationMap = {
        0x05,
        0x04,
        0x01,
        0x03,
        0x00,
        0x00
    };

    HTCapabilities htCapabilities = {
        0x2d,
        0x1a,
        0x187e,
        0x1b,
        {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        0x0000,
        0x00000000,
        0x00
    };

    HTInformation htInformation = {
        0x3d,
        0x16,
        0x06,
        0x0f,
        0x0004,
        0x0000,
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };

    // Send Packet
    // int pcap_sendpacket(pcap_t *p, const u_char *buf, int size);
    while (true) {
        for (int i = 0; i < SSIDList.size(); i++) {
            TestPacket testpacket = {
                radiotap,
                beaconFrame,
                ssidParameters[i],
                supportedRates,
                trafficIndicationMap,
                htCapabilities,
                htInformation,
                0x4321abcd
            };

            int res = pcap_sendpacket(pcap, reinterpret_cast<const u_char*>(&testpacket), sizeof(testpacket));
            if (res != 0) {
                fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
            }

            chrono::milliseconds sleepDuration(10);
            this_thread::sleep_for(sleepDuration);
        }
    }

	pcap_close(pcap);
}