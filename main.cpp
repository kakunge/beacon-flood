#include <cstdio>
#include <fstream>
#include <pcap.h>
#include <string>
#include <vector>
#include <tins/tins.h>
#include "radiotap.h"

using namespace std;
using namespace Tins;

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

    SSIDParameter ssidParameter = {
        0x00,
        0x07,
        {0x41, 0x62, 0x43, 0x64, 0x45, 0x66, 0x47}
    };

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

    TestPacket testpacket = {
        radiotap,
        beaconFrame,
        ssidParameter,
        supportedRates,
        trafficIndicationMap,
        htCapabilities,
        htInformation,
        0x4321abcd
    };

    // Generate Sample Packet
    // RadioTap tap;
    // Dot11::address_type ap        = "00:11:22:33:44:55";
    // Dot11::address_type broadcast = "ff:ff:ff:ff:ff:ff";
    // Dot11Beacon beacon(broadcast, ap);
    // beacon.addr4(ap);
    // beacon.ssid("TESTSSID");
    // beacon.ds_parameter_set(1);
    // beacon.supported_rates({ 1.0f, 5.5f, 11.0f });
    // tap.inner_pdu(beacon);
    // PacketSender sender("wlan0");
    // RadioTap radio = RadioTap() / Dot11Beacon();
    // PacketWriter writer("/tmp/output.pcap", PacketWriter::RADIOTAP);
    // writer.write(radio);




    // Send Packet
    // int pcap_sendpacket(pcap_t *p, const u_char *buf, int size);
    while (true) {
        int res = pcap_sendpacket(pcap, reinterpret_cast<const u_char*>(&testpacket), sizeof(testpacket));
        if (res != 0) {
            fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(pcap));
        }

        sleep(0.1);
    }

	pcap_close(pcap);
}