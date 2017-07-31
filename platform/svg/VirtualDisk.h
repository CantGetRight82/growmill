#pragma once

class VirtualDisk : public AbstractVirtualDisk {
	public:
	VirtualDisk() {
	}
	std::string getContents(std::string path) {
		FILE* fh = fopen( path.c_str(), "rb");
		if(fh == NULL) {
			throw path + " does not exist";
		}

		fseek(fh, 0, SEEK_END);
		int size = ftell(fh);
		fseek(fh, 0, SEEK_SET);


		char* buffer = new char[ size+1 ];
		fread(buffer, 1, size, fh);
		fclose(fh);
		buffer[size] = '\0';
		std::string result = buffer;
		delete [] buffer;
		return result;
	}

};
