#include <fstream>
#include <sstream>

#include "decoder.h"



timeSystem::time::time(unsigned char* data_input){
	for(int i = 0; i < 4; i++){
		_data[i] = (int)(*(data_input + i));
	}
}

timeSystem::time::~time(){
	
}

std::string timeSystem::time::toString(){
	std::string res = "";
	std::stringstream ss;
	for(int i = 0; i < 4; i++){
		if(i != 0){
			ss << " ";
		}
		ss << _data[i];
	}
	return ss.str();
}

int timeSystem::time::data(int i){
	return _data[i];
}







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
	if(type_input != timeSystem::Unknown){
		memcpy(data, data_input, typeSize[type]);
	}
}

void timeSystem::dataElement::swap(dataElement & elem){
	std::swap(data, elem.data);
	std::swap(type, elem.type);
}

timeSystem::dataElement& timeSystem::dataElement::operator=(dataElement const & elem){
	if(this != &elem){
		delete[] data;
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

timeSystem::dataElement::operator timeSystem::time*() const{
	if(type == timeSystem::Time){
		timeSystem::time* timeObject = new timeSystem::time(data);
		return timeObject;
	} else {
		return NULL;
	}
}









timeSystem::messageMasks::messageMasks(){
	std::ifstream masks_file;
	/*
	Маски форматов читаются из файла "masks.dat".
	Файл текстовый. Формат:

		Каждая строка содержит маску для одного значения ID, формат строки:
			<ID> <N> <parameter_1_type> <parameter_2_type> .. <parameter_N_type>

		Пример:
			149 19 2 1 1 2 2 3 3 3 5 5 5 4 4 4 4 4 4 4 4
	*/
	masks_file.open("masks.dat");
	
	int length;
	int ID;
	int buffer;
	while(!masks_file.eof()){
		masks_file >> ID >> length;
		std::vector<int>* mask = new std::vector<int>;
		mask->reserve(length);
		while(length){
			masks_file >> buffer;
			mask->push_back(buffer);
			--length;
		}
		masks.insert(std::pair<int, std::vector<int>*>(ID, mask));
	}
	masks_file.close();
}

timeSystem::messageMasks::~messageMasks(){
	for(std::map<int, std::vector<int>*>::iterator it = masks.begin(); it != masks.end(); ++it){
		delete (*it).second;
	}
}

std::vector<int>* timeSystem::messageMasks::find(int ID){
	std::map<int, std::vector<int>*>::iterator it = masks.find(ID);
	if(it != masks.end()){
		return (*it).second;
	} else {
		return NULL;
	}
}








timeSystem::dataArray::dataArray(unsigned char* data_input, int size_input, std::vector<int>* format_input, timeSystem::messageMasks* masks_input){
	
	// take masks
	masks = masks_input;
	
	int format_size;
	unsigned char* dataP = data_input;

	if(size_input < 4){
		ID = 0;
		return;
	} else {
		ID = (int)data_input[3];
	}

	if(format_input == NULL){
		format_input = masks->find(ID);
		if(format_input == NULL){
			return;
		}
		format_size = format_input->size();
	} else {
		// store format if given
		format_size = format_input->size();
		format.resize(format_input->size());
		memcpy(&(format[0]), &(*format_input)[0], format_input->size() * sizeof(int));
	}
		

	for(int i = 0; i < format_size; i++){
		timeSystem::dataElement el= timeSystem::dataElement(dataP, (*format_input)[i]);
		data.push_back(el);
		dataP += typeSize[(*format_input)[i]];
	}
}

timeSystem::dataArray::~dataArray(){}

timeSystem::dataElement timeSystem::dataArray::operator[](int number){
	if( (number >= 0) && (number < data.size()) ){
		return data[number];
	} else {
		return timeSystem::dataElement(NULL, 0);
	}
}

int timeSystem::dataArray::getID(){
	return ID;
}

int timeSystem::dataArray::size(){
	return data.size();
}








timeSystem::streamParser::streamParser(){
	// masks don't change
	masks = new timeSystem::messageMasks();
	
	// these things do
	dataStream = new std::vector<unsigned char>;
	messages = new std::vector< std::vector<unsigned char> >;
	messages_parsed = new std::vector<timeSystem::dataArray>;
}

timeSystem::streamParser::~streamParser(){
	delete dataStream;
	delete messages;
	delete messages_parsed;
	delete masks;
}

void timeSystem::streamParser::parse(unsigned char* data_input, int data_input_size){
	// clear everything
	dataStream->resize(data_input_size);
	messages->clear();
	messages_parsed->clear();

	// take stream
	memcpy(&((*dataStream)[0]), data_input, data_input_size * sizeof(unsigned char));

	// split into messages
	messages->push_back(std::vector<unsigned char>());
	for(int i = 0; i < data_input_size - 1; i++){
		if( (i > 0) && ((*dataStream)[i] == (unsigned char)87) && ((*dataStream)[i + 1] == (unsigned char)241) ){
			messages->push_back(std::vector<unsigned char>());
		}
		(*messages)[messages->size() - 1].push_back((*dataStream)[i]);
	}
	(*messages)[messages->size() - 1].push_back((*dataStream)[dataStream->size() - 1]);

	//for each message - parse and put into resulting array
	int messages_size = messages->size();
	messages_parsed->reserve(messages_size);
	for(int i = 0; i < messages_size; i++){
		messages_parsed->push_back(timeSystem::dataArray(&((*messages)[i][0]), (*messages)[i].size(), NULL, masks));
	}
}

timeSystem::dataArray* timeSystem::streamParser::getMessage(int i){
	if( (i >= 0) && (i < messages_parsed->size()) ){
		return &((*messages_parsed)[i]);
	} else {
		return NULL;
	}
}

int timeSystem::streamParser::parsedSize(){
	return messages_parsed->size();
}
