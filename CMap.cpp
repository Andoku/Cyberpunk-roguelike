#include <SDL/SDL.h>
#include <vector>
#include "Define.h"
#include "CTile.h"
#include "CSurface.h"
#include "CMap.h"

CMap CMap::MapControl;

CMap::CMap() {
    Surf_Tileset = NULL;
    LeftEdgePositionX = 0;
    LeftEdgePositionY = 0;
}
 
bool CMap::OnLoad(const char *File) {
    TileList.clear();
    FILE *FileHandle = fopen(File, "r");
    if(FileHandle == NULL) return false;
    
    fscanf(FileHandle, "%d:%d\n", &mapsizey, &mapsizex);
    
    char TilesetFile[255];
    fscanf(FileHandle, "%s\n", TilesetFile);
    if(!(Surf_Tileset = CSurface::OnLoad(TilesetFile))) {
        fclose(FileHandle);
        return false;
    }

    Map.resize(mapsizey);
    for(int Y = 0; Y < mapsizey; Y++) {
        for(int X = 0; X < mapsizex; X++) {
            CTile tempTile;
            fscanf(FileHandle, "%d:%d ", &tempTile.TileID, &tempTile.TypeID);
            Map[Y].push_back(tempTile);
        }
        fscanf(FileHandle, "\n");
    }   
    fclose(FileHandle);
    return true;
}
 
void CMap::OnRender(SDL_Surface *Surf_Display, int HeroX, int HeroY)
{
    if(Surf_Tileset == NULL) return;

    int cdiffx, cdiffy;
    cdiffx = HeroX - LeftEdgePositionX;
    cdiffy = HeroY - LeftEdgePositionY;

    if (cdiffx < 10 && LeftEdgePositionX > 0)
        LeftEdgePositionX--;
    if (cdiffx > SCREEN_WIDTH - 10 && LeftEdgePositionX < mapsizex - SCREEN_WIDTH)
        LeftEdgePositionX++;
    if (cdiffy < 10 && LeftEdgePositionY > 0)
        LeftEdgePositionY--;
    if (cdiffy > SCREEN_HEIGHT - 10 && LeftEdgePositionY < mapsizey - SCREEN_WIDTH)
        LeftEdgePositionY++;

    DrawTiles(Surf_Display);
}

void CMap::DrawTiles(SDL_Surface *Surf_Display)
{
    int TilesetWidth  = Surf_Tileset->w / TILE_SIZE;
    int TilesetHeight = Surf_Tileset->h / TILE_SIZE;

    for(int Y = 0; Y < SCREEN_HEIGHT; Y++)
    {
        for(int X = 0; X < SCREEN_WIDTH; X++)
        {
            //КОСТЫЛИ & КОЛЯСКИ
            int tX = MAP_X + (X * TILE_SIZE);
            int tY = MAP_Y + (Y * TILE_SIZE);

            int TilesetX = (Map[Y + LeftEdgePositionY][X + LeftEdgePositionX].TileID % TilesetWidth) * TILE_SIZE;
            int TilesetY = (Map[Y + LeftEdgePositionY][X + LeftEdgePositionX].TileID / TilesetWidth) * TILE_SIZE;

            CSurface::OnDraw(Surf_Display, Surf_Tileset, tX, tY, TilesetX, TilesetY, TILE_SIZE, TILE_SIZE);
        }
    }
}

void CMap::OnCleanup() {
    SDL_FreeSurface(Surf_Tileset);
    TileList.clear();
}
