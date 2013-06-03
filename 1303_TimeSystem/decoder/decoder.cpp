#include "decoder.h"

	

timeSystem::dataElement::dataElement(){
	type = timeSystem::Unknown;
	data = new unsigned char[0];
}

timeSystem::dataElement::dataElement(dataElement const & elem){
	type = elem.type;
	data = new unsigned char[typeSize[type]];
	memcpy(data, elem.data, typeSize[type]);
	
}

timeSystem::dataElement::dataElement(unsigned char* data_input, int type_input){
	type = type_input;
	data = new unsigned char[typeSize[type]];
	memcpy(data, data_input, typeSize[type]);
}
void timeSystem::dataElement::swap(dataElement & elem){
	std::swap(data, elem.data);
	std::swap(type, elem.type);
}
timeSystem::dataElement& timeSystem::dataElement::operator=(dataElement const & elem){
	if(this != &elem){
		dataElement(elem).swap(*this);
	}
	
	return *this;
}

int timeSystem::dataElement::getType(){
	return type;
}

timeSystem::dataElement::operator float() const{
	float* f = (float*)data;	
	return *f;
}

timeSystem::dataElement::operator double() const{
	double* f = (double*)data;	
	return *f;
}

timeSystem::dataElement::operator short int() const{
	short int* f = (short int*)data;
	return *f;
}

timeSystem::dataElement::operator unsigned char() const{
	return *data;
}

timeSystem::dataElement::operator std::vector<int>*() const{
	std::vector<int>* result = new std::vector<int>;
	result->resize(4);
	for(int i = 0; i < 4; i++){
		(*result)[i] = (int)data[i];
	}
	
	return result;
}







timeSystem::dataArray::masks = {
	std::vector<int>()
}

timeSystem::dataArray::dataArray(unsigned char* data_input, int size_input, std::vector<int>* format_input){
	
	int format_size = format_input->size();
	unsigned char* dataP = data_input;
	
	ID = (int)data_input[3];
	
	for(int i = 0; i < format_size; i++){
		timeSystem::dataElement el= timeSystem::dataElement(dataP, (*format_input)[i]);
		data.push_back(el);
		dataP += typeSize[(*format_input)[i]];
	}
	
	std::vector<timeSystem::dataElement> data2;
	unsigned char* data_in = new unsigned char[4];
	data_in[0] = 0;
	data_in[1] = 0;
	data_in[2] = 0;
	data_in[3] = 0;
	data2.push_back(timeSystem::dataElement(data_in, timeSystem::Time));

	
	format.resize(format_input->size());
	memcpy(&(format[0]), &(*format_input)[0], format_input->size() * sizeof(int));
	
}

timeSystem::dataArray::~dataArray(){}

timeSystem::dataElement timeSystem::dataArray::operator[](int number){
	return data[number];
}

int timeSystem::dataArray::getID(){
	return ID;
}