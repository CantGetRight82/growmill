#pragma once

class Node;

class AbstractTextSizer {
	public:
		virtual void measure( Node* node, float maxw, float* outw, float* outh ) = 0;
};
