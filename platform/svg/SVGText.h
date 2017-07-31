#pragma once

#include <assert.h>
#include <string>
#include <vector>
#include "../src/core/Node.h"
#include "../src/core/AbstractTextSizer.h"
#include <ftoutln.h>

#include <fstream>
#include <iomanip>

typedef struct {
    int index;
    double x, y;
} yy_glyph_info;

class SVGText : public AbstractTextSizer {
	private:
		FT_Library library;
        FT_Face ftface;
    
    void setContext(Node* node) {
        
        if( FT_New_Face( library, "/Library/Fonts/Arial.ttf", 0, &ftface ) != 0) {
            std::cerr<<"load issue";
        }

    }
    
	public:
    

    SVGText() {
        if( FT_Init_FreeType( &library ) != 0) {
            std::cerr<<"lib issue";
        }
    }

    ~SVGText() {
    }
 
    
    std::vector<yy_glyph_info> glyphs(Node* node, float maxw, float* outw, float* outh);
    
    
    void measure( Node* node, float maxw, float* outw, float* outh ) {
        setContext(node);
        this->glyphs( node, maxw, outw, outh );
    }

    
    static int moveto( const FT_Vector*  to, void* user ) {
        using std::ofstream;
        ofstream& os = *(static_cast<ofstream*>(user));
        os << "M" << (to->x) << "," << (to->y)  << " ";
        return 0;
    }
    static int lineto( const FT_Vector*  to, void* user ) {
        using std::ofstream;
        ofstream& os = *(static_cast<ofstream*>(user));
        os << "L" << (to->x) << "," << (to->y)  << " ";
        return 0;
    }
    static int quadto( const FT_Vector* c1, const FT_Vector* to, void* user ) {
        using std::ofstream;
        ofstream& os = *(static_cast<ofstream*>(user));
        os << "Q" <<
            (c1->x) << "," << (c1->y) << " " <<
            (to->x) << "," << (to->y) << " ";
        return 0;
    }
    static int cubeto( const FT_Vector* c1, const FT_Vector* c2, const FT_Vector* to, void* user ) {
        using std::ofstream;
        ofstream& os = *(static_cast<ofstream*>(user));
        os << "C" <<
            (c1->x) << "," << (c1->y) << " " <<
            (c2->x) << "," << (c2->y) << " " <<
            (to->x) << "," << (to->y) << " ";
        return 0;
    }
    void render(Node* node, std::ofstream& os) {
        Rect rect = node->rect();
        
        setContext(node);
        std::vector<yy_glyph_info> glyphs = this->glyphs( node, rect.width, NULL, NULL );

        FT_Outline_Funcs funcs;
        funcs.move_to = SVGText::moveto;
        funcs.line_to = SVGText::lineto;
        funcs.conic_to = SVGText::quadto;
        funcs.cubic_to = SVGText::cubeto;
        funcs.shift = 0;
        funcs.delta = 0;
        
 
        os << "<g stroke='none'";
        if(node->has("color")) {
            os << " fill='" << node->color("color").str() <<"'";
        }
        os << " transform='translate(" << rect.x << "," <<rect.y << ")'";
        os << ">";
        for(yy_glyph_info cg : glyphs) {
            int err = FT_Load_Glyph( ftface, cg.index, FT_LOAD_NO_SCALE );
            assert( err == 0);
            
            double fontSize = 10;
            double fontFactor = fontSize/2048.0;

            double x = cg.x;
            double y = cg.y;
            
            double scale = fontFactor;
            os << "<path transform='translate("<<x<<","<<y<<") scale("<<scale<<",-"<<scale<<")' d='";
            FT_Outline_Decompose( &ftface->glyph->outline, &funcs, (void*)&os);

            os << "' />" << std::endl;
//            break;
            

        }
        os << "</g>" << std::endl;
    }
};
