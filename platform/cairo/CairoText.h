#pragma once

#include <assert.h>
#include <string>
#include <vector>
#include "../src/core/Node.h"
#include "../src/core/AbstractTextSizer.h"
#include "cairo-ft.h"

#include <ftoutln.h>

#include <fstream>
#include <iomanip>

class CairoText : public AbstractTextSizer {
	private:
        cairo_t* cr;
		FT_Library library;
        FT_Face ftface;
    
    void setContext(Node* node) {
        
        assert( FT_New_Face( library, "/Library/Fonts/Arial.ttf", 0, &ftface ) == 0);
        
        //FT_Set_Char_Size(
        cairo_font_face_t* face = cairo_ft_font_face_create_for_ft_face(ftface, 0);
        cairo_set_font_face(cr, face);
        cairo_set_font_size(cr, 10);
       
        
        cairo_set_source_rgb(cr, 0,0,0);

    }
    
	public:
    

    CairoText(cairo_t* cr) : cr(cr) {
        assert( FT_Init_FreeType( &library ) == 0);
    }

    ~CairoText() {
    }
 
    
    std::vector<cairo_glyph_t> glyphs(Node* node, float maxw, float* outw, float* outh);
    
    
    void measure( Node* node, float maxw, float* outw, float* outh ) {
        
        setContext(node);
        this->glyphs( node, maxw, outw, outh );
    }
    
    void render(Node* node) {
        Rect rect = node->rect();
        
        setContext(node);
        std::vector<cairo_glyph_t> glyphs = this->glyphs( node, rect.width, NULL, NULL );

        cairo_matrix_t offset;
        cairo_matrix_init_translate(&offset, rect.x, rect.y);
        cairo_set_matrix(cr, &offset);
        
        cairo_show_glyphs(cr, &glyphs[0], glyphs.size());
        cairo_fill( cr );
    
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
    void renderCore(Node* node, std::ofstream& os) {
        Rect rect = node->rect();
        
        setContext(node);
        std::vector<cairo_glyph_t> glyphs = this->glyphs( node, rect.width, NULL, NULL );

        FT_Outline_Funcs funcs;
        funcs.move_to = CairoText::moveto;
        funcs.line_to = CairoText::lineto;
        funcs.conic_to = CairoText::quadto;
        funcs.cubic_to = CairoText::cubeto;
        funcs.shift = 0;
        funcs.delta = 0;
        
        os << std::setprecision(8);
        for(cairo_glyph_t cg : glyphs) {
            int err = FT_Load_Glyph( ftface, cg.index, FT_LOAD_NO_SCALE );
            assert( err == 0);
            
            double fontSize = 10;
            double fontFactor = fontSize/2048.0;

            double x = rect.x + cg.x;
            double y = rect.y + cg.y;
            
            double scale = fontFactor;
            os << "<path transform='translate("<<x<<","<<y<<") scale("<<scale<<",-"<<scale<<")' fill='black' stroke='none' d='";
            FT_Outline_Decompose( &ftface->glyph->outline, &funcs, (void*)&os);

            os << "' />" << std::endl;
//            break;
            

        }
        cairo_show_glyphs(cr, &glyphs[0], glyphs.size());
        cairo_fill( cr );
    }
};
