#include "core/parse.h"
#include "core/AbstractVirtualDisk.h"
#include "core/growmill.h"

#include "VirtualDisk.h"
#include "SVGText.h"

#include <fstream>

using std::stoi;
using std::cerr;
using std::endl;

std::ofstream svg;

void render(Node* node, SVGText& text) {
    Rect rect = node->rect();
    
    svg << "<g>" << endl;
    if(node->has("fill") || node->has("stroke")) {
        svg << "<rect" <<
        " x='"<<rect.x<<"'" <<
        " y='"<<rect.y<<"'" <<
        " width='"<<rect.width<<"'" <<
        " height='"<<rect.height<<"'";
        
        if(node->has("border-radius")) {
            double radius = node->number("border-radius");
            svg << " rx='"<<radius<<"' ry='"<<radius<<"'";
        }
        
        svg << " style='";
        if(node->has("stroke")) {
            svg << " stroke: "<<node->color("stroke").str()<<";";
        }
        if(node->has("fill")) {
            svg << " fill: "<<node->color("fill").str()<<";";
        } else {
            svg << " fill: none;";
        }
        svg << "' />";
        
    }
    
    if(node->has("text")) {
        
        text.render(node, svg);
        
    }
    
    svg << "</g>" << endl;
    
	for(Node* n : node->subs) {
		render(n, text);
	}
}


int main(int c, const char** argv) {
	if(c<5) {
        std::cerr<<"Usage: yy <in.mil> <out.svg> <target-width> <target-height>"<<std::endl;
		return 1;
	}

    
	const char* infile = argv[1];
	const char* outfile = argv[2];
	int width = stoi(argv[3]);
	int height = stoi(argv[4]);
    
	using std::endl;
	svg.open(outfile);
    svg << "<?xml version='1.0' encoding='UTF-8'?>" <<endl;
	svg << "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink'"
        << " width='" << width << "px' height='" << height << "px' viewBox='0 0 "<<width<<" "<<height<<"' version='1.1'>" <<endl;
 
    SVGText text;
	VirtualDisk disk;
    
	Node* root = GrowMill::parse(disk, text, infile, width, height);

	render(root, text);
	svg << "</svg>" <<endl;
	svg.close();

}

