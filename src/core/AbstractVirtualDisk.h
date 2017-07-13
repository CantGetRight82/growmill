
#pragma once

class AbstractVirtualDisk {
	public:
	AbstractVirtualDisk() {
	}
	virtual std::string getContents(std::string path) = 0;
};
