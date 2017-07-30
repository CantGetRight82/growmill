
#include "CairoText.h"

using std::vector;
using std::string;


vector<cairo_glyph_t> CairoText::glyphs(Node* node, float maxw, float* outw, float* outh) {
    vector<cairo_glyph_t> result;

    double fontSize = 10;
    double fontFactor = fontSize/2048.0;
    assert( FT_Set_Pixel_Sizes( ftface, 0, fontSize ) == 0);
    
 
    
    
    double ascender = ftface->ascender * fontFactor;
    double descender = ftface->descender * fontFactor;
    double height = ftface->height * fontFactor;
    
    vector<string> lines = node->attsplit("text", "\n");

    double lastx = 0;
    double lasty = ascender;
    double bigx = 0;
    for(string str : lines) {
        lastx = 0;
        size_t len = str.size();

        for(int i=0; i<len; i++) {
            cairo_glyph_t glyph;
            glyph.index = FT_Get_Char_Index(ftface, str.at(i));
            int err = FT_Load_Glyph( ftface, glyph.index, 0 );
            assert( err == 0);
            

            if(FT_HAS_KERNING(ftface) && lastx>0) {
                FT_Vector vec;
                FT_Get_Kerning( ftface,
                               result.back().index, glyph.index,
                               FT_KERNING_UNFITTED ,&vec);
                if(vec.x != 0) {
                    double dx = (vec.x/64.0);
                    lastx += dx;
                }
            }
            
            
            glyph.x = lastx;
            glyph.y = lasty;
            
            
            lastx += ftface->glyph->advance.x >> 6;
                                  result.push_back( glyph );
            
            if(maxw>0 && lastx > maxw) {
                //TODO: if str.at(i) == space
                int j = i;
                while(j>0 && str.at(j) != ' ') { j--; result.pop_back(); }
                if(j==0) {
                    throw "cannot break at space";
                }
                i=j;
                lastx = 0;
                lasty += height;
            }
            
        }
        
        bigx = fmaxf(bigx, lastx);
        lasty += height;
    }
    if(outw != NULL) {
        if(maxw != -1) {
            *outw = maxw;
        } else {
            *outw = bigx;
        }
    }
    if(outh != NULL) {
        *outh = lasty - height - descender;
    }
    return result;
}
