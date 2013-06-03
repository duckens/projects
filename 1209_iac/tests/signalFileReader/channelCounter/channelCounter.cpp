#include <iostream>

const int UDP_PACKET_NUMBER_SIZE = 4;
const int UDP_PACKET_SYSINFO_SIZE = 2;
const int UDP_PACKET_SIZE = 260;

int channelSequently(int i){
	if(i < 120){
		if(i < 60){
			return 60 - i;
		} else {
			return 119 + (i - 60);
		}
	} else {
		if(i < 149){
			return 90 + (i - 120);
		} else {
			return 89 - (i - 149);
		}
	}
}

int shiftSequently(int i){
	int shiftForPacketInTriple;
	if(i < 120){
		if(i < 60){
			shiftForPacketInTriple = 0;
		} else {
			shiftForPacketInTriple = UDP_PACKET_SIZE;
			i -= 60;
		}
	} else {
		shiftForPacketInTriple = 2 * UDP_PACKET_SIZE;
		if(i < 149){
			i -= 119;
		} else {
			i -= 118;
		}
	}
	return UDP_PACKET_NUMBER_SIZE + ( (i / 10) + 1 ) * UDP_PACKET_SYSINFO_SIZE + 3 * i;// + shiftForPacketInTriple;
}

int main(){
	int array[179];
	// std::cout << "channelSequently(i) -- shiftSequently(i)" << std::endl;
	// for(int i = 0; i < 178; i++){
	// 	std::cout << channelSequently(i) << " -- " << shiftSequently(i) << std::endl;
	for(int i = 0; i < 178; i++){
		array[channelSequently(i)] = shiftSequently(i);
	}
	for(int i = 1; i <= 178; i++){
		std::cout << i << " -- " << array[i] << std::endl;
	}

}