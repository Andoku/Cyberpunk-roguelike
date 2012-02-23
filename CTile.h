#ifndef _CTILE_H_
    #define _CTILE_H_
 
#include "Define.h"
 
enum {
    TILE_TYPE_NONE = 0,
    TILE_TYPE_NORMAL = 1,
    TILE_TYPE_BLOCK = 2
};
 
class CTile {
    public:
        int TileID;
        int TypeID;
        CTile();
};

CTile::CTile() {
    TileID = 0;
    TypeID = TILE_TYPE_NONE;
}

#endif
