#ifndef ARRAYS_H
#define ARRAYS_H

#include <vector>

template< class T >
	class dualVector{
		private:
			std::vector< std::vector<T> > content;
		public:
			T& operator()(int x, int y){
				return content[x][y];
			}

			std::vector<T>* operator[](int x){
				return &(content[x]);
			}
		
			
			dualVector(int x = 0, int y = 0){
				content.resize(x);
				for(int i = 0; i < x; i++){
					content[i].resize(y);
				}
			}

			void resize(int x = 0, int y = 0){
				content.resize(x);
				for(int i = 0; i < x; i++){
					content[i].resize(y);
				}
			}
			
			int sizex(){
				return content.size();
			}
			
			
			int sizey(){
				if(content.size()){
					return content[0].size();
				} else {
					return 0;
				}
			}
	};
	
template< class T >
	class tripleVector{
		private:
			std::vector< std::vector< std::vector<T> > > content;
		public:
			T& operator()(int x, int y, int z){
				return content[x][y][z];
			}
			
			tripleVector(int x = 0, int y = 0, int z = 0){
				content.resize(x);
				for(int i = 0; i < x; i++){
					content[i].resize(y);
					for(int j = 0; j < y; j++){
						content[i][j].resize(z);
					}
				}
			}
			
			void resize(int x = 0, int y = 0, int z = 0){
				content.resize(x);
				for(int i = 0; i < x; i++){
					content[i].resize(y);
					for(int j = 0; j < y; j++){
						content[i][j].resize(z);
					}
				}
			}
			
			int sizex(){
				return content.size();
			}
			
			int sizey(){
				if(content.size()){
					return content[0].size();
				} else {
					return 0;
				}
			}
			
			int sizez(){
				if(content.size()){
					if(content[0].size()){
						return content[0][0].size();
					} else {
						return 0;
					}
				} else {
					return 0;
				}
			}
	};

#endif
