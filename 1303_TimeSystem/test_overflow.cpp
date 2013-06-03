#include <cstdio>
#include <iostream>


int main(){
	unsigned char ch;
	
	ch = 1;
	for(int i = 0; i < 300; i++){
		std::cout << (int)ch++ << std::endl;
	}
	
	return 0;
}