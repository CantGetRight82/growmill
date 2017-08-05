#include "core/parse.h"
#include "core/AbstractVirtualDisk.h"
#include "core/growmill.h"

#include "VirtualDisk.h"
#include "SVGText.h"

#include <fstream>
#include <sstream>
#include "core/StringTools.h"

typedef struct {
    double x, y;
} GradientPoint;

GradientPoint parseGradientPoint( std::string str ) {
    const char* cstr = str.c_str();
    GradientPoint result;
    result.x = strtod(cstr+1, NULL);
    result.y = strtod( strchr(cstr, ',') + 1, NULL);
    return result;
}

using std::stoi;
using std::cerr;
using std::endl;


using std::stringstream;



void renderRectOpen(Node* node, stringstream& body) {
    Rect rect = node->rect();
    body << "<rect" <<
    " x='"<<rect.x<<"'" <<
    " y='"<<rect.y<<"'" <<
    " width='"<<rect.width<<"'" <<
    " height='"<<rect.height<<"'";
    
    if(node->has("border-radius")) {
        double radius = node->number("border-radius");
        body << " rx='"<<radius<<"' ry='"<<radius<<"'";
    }
}

void render(Node* node, SVGText& text, stringstream& head, stringstream& body) {
    if(node->has("shadow")) {
        std::vector<std::string> parts = StringTools::split(node->str("shadow"), " ");
        GradientPoint offset = parseGradientPoint(parts[0]);
        
        double blur = strtod(parts[1].c_str(), NULL);
        Color color = Color::fromString(parts[2]);
        

        double minx = fmin( -blur, -blur + offset.x);
        double miny = fmin( -blur, -blur + offset.y);
        
        Rect rect = node->rect();
        double maxx = fmax( blur, blur + offset.x) + rect.width;
        double maxy = fmax( blur, blur + offset.y) + rect.height;
        
		head << "<filter id='shadow'"
            " x='"<<minx<<"' width='"<<maxx<<"'"
            " y='"<<miny<<"' height='"<<maxy<<"'>"
			
        "<feOffset in='SourceAlpha' result='offOut' dx='"<<offset.x<<"' dy='"<<offset.y<<"' />"
        "<feColorMatrix in='offOut' result='colorOut' type='matrix' values='"
        " 0 0 0 0 "<<color.r<<
        " 0 0 0 0 "<<color.g<<
        " 0 0 0 0 "<<color.b<<
        " 0 0 0 1 0' />"
        "<feGaussianBlur in='colorOut' result='blurOut' stdDeviation='"<<blur<<"' />"
        "<feBlend in='SourceGraphic' in2='blurOut' mode='normal' />"
			"</filter>" << endl;
        
    }

    body << "<g>" << endl;
    if(node->has("fill") || node->has("stroke")) {
        renderRectOpen(node, body);
        body << " style='";
        if(node->has("stroke")) {
            body << " stroke: "<<node->color("stroke").str()<<";";
        }

		if(node->has("shadow")) {
			body << " filter: url(#shadow);";
		}
      
        if(node->has("fill")) {
            std::string fill = node->str("fill");
            if(fill[0] == '(') {
                std::string gradientId = "grad1";
                std::vector<std::string> parts = StringTools::split(fill," ");
            
                GradientPoint pt1 = parseGradientPoint(parts[0]);
                GradientPoint pt2 = parseGradientPoint(parts[1]);
                head << "<linearGradient id='"<<gradientId<<"'"<<
                " x1='"<<pt1.x<<"%' y1='"<<pt1.y<<"%' x2='"<<pt2.x<<"%' y2='"<<pt2.y<<"%'>";
                
               
                int max = parts.size();
                for(int i=2; i<max; i++) {
                    double offset = ((i-2) / (max-3.0))*100.0;
                    Color c = Color::fromString(parts[i]);
                    head << "<stop offset='"<<offset<<"%' style='stop-color:"<<c.str()<<";stop-opacity:1' />";

                }
                head << "</linearGradient>";
                
                body << " fill: url(#"<<gradientId<<");";
                
            } else {
                body << " fill: "<<node->color("fill").str()<<";";
            }
        } else {
            body << " fill: none;";
        }
        body << "' />";
        
    }
    
    if(node->has("text")) {
        
        text.render(node, head,  body);
        
    }
    
    
    body << "</g>" << endl;
    
	for(Node* n : node->subs) {
        if(n->tag == "gradient") { continue; }
		render(n, text, head, body);
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
    
    std::ofstream debug;
    debug.open("/tmp/debug.json");
    
    std::ofstream svg;
    
	svg.open(outfile);
    svg << "<?xml version='1.0' encoding='UTF-8'?>" <<endl;
	svg << "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink'"
        << " width='" << width << "px' height='" << height << "px' viewBox='0 0 "<<width<<" "<<height<<"' version='1.1'>" <<endl;
 
    SVGText text;
	VirtualDisk disk;
    
    
    stringstream head;
    stringstream body;
    
    Node* root = GrowMill::parse(disk, text, infile, width, height);
    root->renderJSON(debug);
    
	render(root, text, head, body);
    
    svg << "<defs>" << head.str() << "</defs>";
    svg << body.str() << endl;
	svg << "</svg>" <<endl;
	svg.close();

}

