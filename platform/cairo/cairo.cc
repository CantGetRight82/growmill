#include "core/parse.h"
#include "cairo-svg.h"
#include "cairo-ft.h"

#include "core/AbstractVirtualDisk.h"
#include "core/growmill.h"

#include "VirtualDisk.h"
#include "CairoText.h"

#include <fstream>

using std::stoi;
using std::cerr;

std::ofstream svg;

void rectPath(cairo_t* cr, Node* node, bool stroke, double x, double y, double width, double height) {
    
    if(node->has("border-radius")) {
        double radius = node->number("border-radius");
        if(stroke) { radius -= 0.5; }
        
        double degrees = M_PI / 180.0;
        
        cairo_new_sub_path (cr);
        cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
        cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
        cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
        cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
        cairo_close_path (cr);
    } else {
        cairo_rectangle(cr, x, y, width, height);
    }
}

void renderCore(cairo_t* cr, Node* node, CairoText& text) {
    Rect rect = node->rect();
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
}

void render(cairo_t* cr, Node* node, CairoText& text) {
    renderCore(cr, node, text);
    Rect rect = node->rect();

	if(node->has("fill")) {
        
		rectPath(cr, node, false, rect.x, rect.y, rect.width, rect.height);
		Color color = node->color("fill");
		cairo_set_source_rgb(cr, color.r, color.g, color.b );
		cairo_fill( cr );
	}
    
    if(node->has("stroke")) {
        rectPath(cr, node, true, rect.x + .5, rect.y + .5, rect.width - 1, rect.height -1);
        Color color = node->color("stroke");
        cairo_set_source_rgb(cr, color.r, color.g, color.b );
        cairo_set_line_width (cr, 1.0);
        cairo_stroke( cr );
    }

    
    if(node->has("text")) {
        
        text.render(node);
        
    }
	
	for(Node* n : node->subs) {
		render(cr, n, text);
	}
}


int main(int c, const char** argv) {
	if(c<5) {
        std::cerr<<"Usage: growc <in.mil> <out.svg|png> <target-width> <target-height>"<<std::endl;
		return 1;
	}

    

    
	const char* infile = argv[1];
	const char* outfile = argv[2];
	int width = stoi(argv[3]);
	int height = stoi(argv[4]);
    
	using std::endl;
	svg.open("../output/core.svg");
    svg << "<?xml version='1.0' encoding='UTF-8'?>" <<endl;
	svg << "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='360pt' height='640pt' viewBox='0 0 360 640' version='1.1'>" <<endl;
    
    cairo_t *cr;
    cairo_surface_t *surface;
    bool ispng = strstr(outfile, ".png");
    if(ispng) {
        printf("png\n");
        surface = (cairo_surface_t *)cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    } else {
        surface = (cairo_surface_t *)cairo_svg_surface_create(outfile, width, height);
    }
    cr = cairo_create(surface);

    CairoText text(cr);
	VirtualDisk disk;
	Node* root = GrowMill::parse(disk, text, infile, width, height);




	render(cr, root, text);
    
    
	svg << "</svg>" <<endl;
	svg.close();
    
    
    if(ispng) {
        cairo_surface_write_to_png( surface, outfile);
    }

	/*
     
     cairo_pattern_t *pattern;
     int x,y;

    pattern = cairo_pattern_create_radial(50, 50, 5, 50, 50, 50);
    cairo_pattern_add_color_stop_rgb(pattern, 0, 0.75, 0.15, 0.99);
    cairo_pattern_add_color_stop_rgb(pattern, 0.9, 1, 1, 1);

    cairo_set_source(cr, pattern);
    cairo_fill(cr);

    cairo_set_font_size (cr, 15);
    cairo_select_font_face (cr, "Georgia",
        CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_source_rgb (cr, 0, 0, 0);

    cairo_move_to(cr, 10, 25);
    cairo_show_text(cr, "Hallo");

    cairo_move_to(cr, 10, 75);
    cairo_show_text(cr, "Wikipedia!");
	*/

    cairo_destroy (cr);
    cairo_surface_destroy (surface);
}

