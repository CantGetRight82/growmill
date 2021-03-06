
#include "SVGText.h"
#include "core/StringTools.h"

using std::vector;
using std::string;
using std::cerr;
using std::endl;


vector<yy_glyph_info> SVGText::glyphs(Node* node, float maxw, float* outw, float* outh) {
    vector<yy_glyph_info> result;


    if(FT_Set_Pixel_Sizes( ftface, 0, fontSize ) != 0) {
        cerr<<"could not set pixelsize"<<endl;
    }
    
    double ascender = ftface->ascender * fontFactor;
    double descender = ftface->descender * fontFactor;
    double height = ftface->height * fontFactor;
    
    Node* hasKerning = node->has("kerning", true);
    double kerning = hasKerning ? node->number("kerning") : 0;
    
  
    vector<string> lines = StringTools::split(node->str("text"), "\n");

    double lastx = 0;
    double lasty = ascender;
    double bigx = 0;
    
    
    for(string str : lines) {
        lastx = 0;
        size_t len = str.size();

        for(int i=0; i<len; i++) {
            yy_glyph_info glyph;
            glyph.index = FT_Get_Char_Index(ftface, str.at(i));
            if(FT_Load_Glyph( ftface, glyph.index, 0 ) != 0) {
                std::cerr<<"Error loading glyph"<<std::endl;
            }
            
            

            if(lastx>0) {
                if(FT_HAS_KERNING(ftface)) {
                    FT_Vector vec;
                    FT_Get_Kerning( ftface,
                                   result.back().index, glyph.index,
                                   FT_KERNING_UNFITTED ,&vec);
                    if(vec.x != 0) {
                        double dx = (vec.x/64.0);
                        lastx += dx;
                    }
                }
                lastx += kerning;
            }
            
            glyph.x = lastx;
            glyph.y = lasty;
            
            
            lastx += (ftface->glyph->advance.x >> 6) * (ftface->units_per_EM/2048.0);
            result.push_back( glyph );
            
            if(maxw>0 && lastx > maxw) {
                //TODO: if str.at(i) == space
                int j = i;
                while(j>0 && str.at(j) != ' ') { j--; result.pop_back(); }
                if(j==0) {
                    throw std::runtime_error("cannot break at space");
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
        *outw = maxw != -1 ? maxw : bigx;
    }
    
    if(outh != NULL) {
        *outh = lasty - height - descender;
    }
     
    
    return result;
}
