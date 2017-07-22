#pragma once
#include "Node.h"
#include "AbstractVirtualDisk.h"

#include <iostream>
#include <vector>


class NodeParser {
	public:
		static Node* parse(AbstractVirtualDisk& disk, std::string str);
};

