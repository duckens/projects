#include "stringDecoder.h"

int main(){
	std::string input_03_05 = "57 F1 23 95 16 79 7F 2A 0C 1B 0A 00 95 82 09 2D 0D 03 05 00 00 00 00 D9 7F C1 F0 3F 00 00 00 75 A2 F5 E0 3F 00 00 00 00 00 88 48 40 00 00 00 00 00 00 00 00 00 00 00 00 F9 19 0A 48 40 24 3B 45 80 96 18 4B 00 00 00 00 00 00 00 00";
	std::string input_03_06 = "57 F1 23 95 96 CA 3F 2E0D 0C 27 00 F7 C9 9A 3B 0D 03 06 00 00 00 00 6F 7C C1 F0 3F 00 00 00 C9 B0 F5 E0 3F 00 00 00 00 00 2C 45 40 00 00 00 00 00 00 00 00 00 00 00 00 00 40 56 41 00 20 85 41 00 00 E2 42 00 00 00 00 00 00 00 00";
	std::string ID_149_format = "wccwwtttdddffffffff";
	
	//parseFormat(input_03_05, ID_149_format);
	parseFormat(input_03_06, ID_149_format);
	
	std::cout << "float = " << decode_FP("0000E242") << std::endl;
	
	
	return 0;
}