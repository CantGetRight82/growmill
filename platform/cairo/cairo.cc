#include "core/parse.h"
#include "cairo-svg.h"
#include "cairo-ft.h"

#include "core/AbstractVirtualDisk.h"
#include "core/growmill.h"

#include "VirtualDisk.h"
#include "CairoText.h"


using std::stoi;
using std::cerr;



void render(cairo_t* cr, Node* node, CairoText& text) {
	
    Rect rect = node->rect();
	if(node->has("stroke")) {
		cairo_rectangle(cr, rect.x, rect.y, rect.width, rect.height);
		Color color = node->color("stroke");
		cairo_set_source_rgb(cr, color.r, color.g, color.b );
		cairo_set_line_width (cr, 1.0);
		cairo_stroke( cr );
	}
	if(node->has("fill")) {
		cairo_rectangle(cr, rect.x, rect.y, rect.width, rect.height);
		Color color = node->color("fill");
		cairo_set_source_rgb(cr, color.r, color.g, color.b );
		cairo_fill( cr );
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

