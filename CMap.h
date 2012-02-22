#ifndef _CMAP_H_
	#define _CMAP_H_
 
#include <SDL/SDL.h>
#include <vector>
#include "CTile.h"
#include "CSurface.h"
#include "CUnit.h"

enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    LEFTUP,
    LEFTDOWN,
    RIGHTUP,
    RIGHTDOWN
};

class CMap {
	public:
		static CMap MapControl;

	private:
		SDL_Surface *Surf_Tileset;
        SDL_Surface *Body_Tileset;
        SDL_Surface *fogofwar;
		std::vector<CTile> TileList;
        std::vector< std::vector<CTile> > Map ;
        std::vector< CUnit > UnitList;
        int mapsizex, mapsizey;
        int leftedgepositionx, leftedgepositiony; //РАССТРЭЛЯТЬ!

	public:
		CMap();
		bool OnLoad(char *File);
		void OnRender(SDL_Surface *Surf_Display, int MapX, int MapY);
		void OnCleanup();
        void Move(int direction);
        bool CanMove(int direction);
};

CMap CMap::MapControl;

CMap::CMap() {
	Surf_Tileset = NULL;
    UnitList.resize(1);
    CUnit yourself;
    yourself.posx = 1;
    yourself.posy = 1;
    yourself.facing = RIGHT;
    UnitList.clear();
    //Примем, что на нулевом элементе всегда хранится игрок
    UnitList.push_back(yourself);
    leftedgepositionx = 0;
    leftedgepositiony = 0;
}
 
bool CMap::OnLoad(char *File) {
	TileList.clear();
	FILE *FileHandle = fopen(File, "r");
	if(FileHandle == NULL) {
		return false;
	}
	
    fscanf(FileHandle, "%d:%d\n", &mapsizey, &mapsizex);

	char TilesetFile[255];
	fscanf(FileHandle, "%s\n", TilesetFile);
	if(!(Surf_Tileset = CSurface::OnLoad(TilesetFile))) {
		fclose(FileHandle);
		return false;
	}
    fscanf(FileHandle, "%s\n", TilesetFile);
    if(!(Body_Tileset = CSurface::OnLoad(TilesetFile))) {
        fclose(FileHandle);
        return false;
    }
    if(!(fogofwar = CSurface::OnLoad("media/fogofwar.png"))) {
        fclose(FileHandle);
        return false;
    }
    Map.resize(mapsizey);
    for(int Y = 0; Y < mapsizey; Y++)
    {
        for(int X = 0; X < mapsizex; X++)
        {
            CTile tempTile;
            fscanf(FileHandle, "%d:%d ", &tempTile.TileID, &tempTile.TypeID);
            Map[Y].push_back(tempTile);
        }
		fscanf(FileHandle, "\n");
	}	
    fclose(FileHandle);
	return true;
}
 
void CMap::OnRender(SDL_Surface *Surf_Display, int MapX, int MapY) {
	if(Surf_Tileset == NULL) return;
 
	int TilesetWidth  = Surf_Tileset->w / TILE_SIZE;
	int TilesetHeight = Surf_Tileset->h / TILE_SIZE;

    int cdiffx, cdiffy;
    cdiffx = UnitList[0].posx - leftedgepositionx;
    cdiffy = UnitList[0].posy - leftedgepositiony;

    if (cdiffx < 10 && leftedgepositionx > 0)
        leftedgepositionx--;
    if (cdiffx > SCREEN_WIDTH - 10 && leftedgepositionx < mapsizex - SCREEN_WIDTH)
        leftedgepositionx++;
    if (cdiffy < 10 && leftedgepositiony > 0)
        leftedgepositiony--;
    if (cdiffy > SCREEN_HEIGHT - 10 && leftedgepositiony < mapsizey - SCREEN_WIDTH)
        leftedgepositiony++;

    for(int Y = 0; Y < SCREEN_HEIGHT; Y++)
    {
        for(int X = 0; X < SCREEN_WIDTH; X++)
        {
            //КОСТЫЛИ & КОЛЯСКИ
            int tX = MapX + (X * TILE_SIZE);
            int tY = MapY + (Y * TILE_SIZE);

            int TilesetX = (Map[Y + leftedgepositiony][X + leftedgepositionx].TileID % TilesetWidth) * TILE_SIZE;
            int TilesetY = (Map[Y + leftedgepositiony][X + leftedgepositionx].TileID / TilesetWidth) * TILE_SIZE;

            CSurface::OnDraw(Surf_Display, Surf_Tileset, tX, tY, TilesetX, TilesetY, TILE_SIZE, TILE_SIZE);
        }
    }
    //Туман войны в области, где гг никого не видит. Или типо того.
    int startx, starty, endx, endy;
    switch(UnitList[0].facing)
    {
    case UP:
        starty = UnitList[0].posy + 1 - leftedgepositiony;
        endy = SCREEN_HEIGHT;
        startx = 0;
        endx = SCREEN_WIDTH;
        break;
    case DOWN:
        starty = 0;
        endy = UnitList[0].posy - leftedgepositiony;
        startx = 0;
        endx = SCREEN_WIDTH;
        break;
    case LEFT:
        startx = UnitList[0].posx + 1 - leftedgepositionx;
        endx = SCREEN_WIDTH;
        starty = 0;
        endy = SCREEN_HEIGHT;
        break;
    case RIGHT:
        startx = 0;
        endx = UnitList[0].posx - leftedgepositionx;
        starty = 0;
        endy = SCREEN_HEIGHT;
        break;
    default:
        break;
    }

    for(int Y = starty; Y < endy; Y++) {
        for(int X = startx; X < endx; X++)
        {
            int tX = MapX + (X * TILE_SIZE);
            int tY = MapY + (Y * TILE_SIZE);

            int TilesetX = 0;
            int TilesetY = 0;

            CSurface::OnDraw(Surf_Display, fogofwar, tX, tY, TilesetX, TilesetY, TILE_SIZE, TILE_SIZE);
        }
    }
    //Тут сделать отрисовку всех юнитов в видимой части
    CSurface::OnDraw(Surf_Display, Body_Tileset, MapX + (UnitList[0].posx - leftedgepositionx) * TILE_SIZE,
                                                 MapY + (UnitList[0].posy - leftedgepositiony) * TILE_SIZE,
                                                 0, 0, TILE_SIZE, TILE_SIZE);
    //Разъебать это говно на отдельные методы, пизда получается
}

void CMap::OnCleanup() {
	SDL_FreeSurface(Surf_Tileset);
	TileList.clear();
}

void CMap::Move(int direction)
{
    //Вставить проверку на возможность перемещения
    if (!CanMove(direction))
        return;
    switch(direction)
    {
        case UP:
            UnitList[0].posy--;
            break;
        case DOWN:
            UnitList[0].posy++;
            break;
        case LEFT:
            UnitList[0].posx--;
            break;
        case RIGHT:
            UnitList[0].posx++;
            break;
        case LEFTUP:
            UnitList[0].posx--;
            UnitList[0].posy--;
            break;
        case LEFTDOWN:
            UnitList[0].posx--;
            UnitList[0].posy++;
            break;
        case RIGHTUP:
            UnitList[0].posx++;
            UnitList[0].posy--;
            break;
        case RIGHTDOWN:
            UnitList[0].posx++;
            UnitList[0].posy++;
            break;
    }
    UnitList[0].facing = direction;
}

bool CMap::CanMove(int direction)
{
    int x = UnitList[0].posx;
    int y = UnitList[0].posy;
        switch(direction)
        {
        case UP:
            return (y > 0) && (Map[y-1][x].TypeID == TILE_TYPE_NORMAL);
            break;
        case DOWN:
            return (y < mapsizey - 1) && (Map[y+1][x].TypeID == TILE_TYPE_NORMAL);
            break;
        case LEFT:
            return (x > 0) && (Map[y][x-1].TypeID == TILE_TYPE_NORMAL);
            break;
        case RIGHT:
            return (x < mapsizex - 1) && (Map[y][x+1].TypeID == TILE_TYPE_NORMAL);
            break;
        case LEFTUP:
            return (y > 0) && (x > 0) && (Map[y-1][x-1].TypeID == TILE_TYPE_NORMAL);
            break;
        case LEFTDOWN:
            return (x > 0) && (y < mapsizey - 1) && (Map[y+1][x-1].TypeID == TILE_TYPE_NORMAL);
            break;
        case RIGHTUP:
            return (x < mapsizex - 1) && (y > 0) && (Map[y-1][x+1].TypeID == TILE_TYPE_NORMAL);
            break;
        case RIGHTDOWN:
            return (x < mapsizex - 1) && (y < mapsizey - 1) && (Map[y+1][x+1].TypeID == TILE_TYPE_NORMAL);
            break;
        }
}

#endif
