#include "core/parse.h"
#include "cairo-svg.h"

#include "core/AbstractVirtualDisk.h"
#include "core/growmill.h"

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
		string result = buffer;
		delete [] buffer;
		return result;
	}

};


void render(cairo_t* cr, Node* node) {
	bool stroke = node->has("stroke");
	bool fill = node->has("fill");

	Rect rect = node->rect();

		if(stroke) {
			cairo_rectangle(cr, rect.x, rect.y, rect.width, rect.height);
			Color color = node->color("stroke");
			cairo_set_source_rgb(cr, color.r, color.g, color.b );
			cairo_set_line_width (cr, 1.0);
			cairo_stroke( cr );
		}
		if(fill) {
			cairo_rectangle(cr, rect.x, rect.y, rect.width, rect.height);
			Color color = node->color("fill");
			cairo_set_source_rgb(cr, color.r, color.g, color.b );
			cairo_fill( cr );
		}
	
	for(Node* n : node->subs) {
		render(cr, n);
	}
}


int main(int c, const char** argv) {
	if(c<5) {
		cerr<<"Usage: growc <in.mil> <out.svg> <target-width> <target-height>"<<endl;
		return 1;
	}

	const char* infile = argv[1];
	const char* outfile = argv[2];
	int width = stoi(argv[3]);
	int height = stoi(argv[4]);

	VirtualDisk disk;
	Node* root = GrowMill::parse(disk, infile, width, height);

	if(width == 0) {
		width = root->right();
		height = root->bottom();
	}

    cairo_t *cr;
    cairo_surface_t *surface;
    cairo_pattern_t *pattern;
    int x,y;

    surface = (cairo_surface_t *)cairo_svg_surface_create(outfile, width, height);
    cr = cairo_create(surface);

	render(cr, root);

	/*
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

