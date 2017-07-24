#pragma once

#include <assert.h>
#include <string>
#include <vector>
#include "../src/core/Node.h"
#include "../src/core/AbstractTextSizer.h"
#include "cairo-ft.h"

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
};
