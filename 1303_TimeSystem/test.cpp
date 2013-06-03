#include <cstdio>
#include <iostream>


int main(){
	char ch[8];
	
	ch[0] = 255;
	ch[1] = 0;
	ch[2] = 0;
	ch[3] = 0;
	ch[4] = 0;
	ch[5] = 0;
	ch[6] = 0;
	ch[7] = 0;
	
	
	/*
	ch[0] = 0;
	ch[1] = 0;
	ch[2] = 0;
	ch[3] = 0;
	ch[4] = 0;
	ch[5] = 88;
	ch[6] = 48;
	ch[7] = 40;
	*/
	
	
	
	double* f;
	f = (double*)(&(ch[0]));
	
	double x;
	x = *f;
	std::cout << x << std::endl;
	
	return 0;
}