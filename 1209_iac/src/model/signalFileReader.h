typedef long long int ll;

class signalFileReader{

	private:

		static const int gidrophonesAmount = 178;

		int filesAmount;

		std::string filename;
		
		std::vector<FILE*> files;
		std::vector<int> filesSizes;
		std::vector<int> filesStartTimes;

		bool blockChecksumOk(int blockNumber);
		int countBlocksNFiles();

		int findFile(int time);
		int findPlaceInFile(int time);


	public:

		signalFileReader(signalFileReader const &other_reader);
		signalFileReader& operator= (signalFileReader const &other_reader);
		~signalFileReader();

		bool open(std::string filename);

		ll startTime();

		std::vector<int>* data(ll start, int length);

};