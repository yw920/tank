#include "MapData.h"

USING_NS_CC;
using namespace std;

float MapData::_map_start_x = 0.0f;
 float MapData::_map_start_y = 0.0f;

//the size of tile when game playing
 float MapData::_tile_h_in_scene = 0.0f;
 float MapData::_tile_w_in_scene = 0.0f;
//the size of tiles in map
 int MapData::_tiles_num_w = 0;
 int MapData::_tiles_num_h = 0;

 Size MapData::_size = Size(0, 0);
 TMXLayer* MapData::_bgLayer = NULL;
 TMXLayer* MapData::_waterLayer = NULL;
 float MapData::_scale = 0.0f;
 MapData *MapData::_mp = new MapData();

 int MapData::_home_tileX_start = 0;
 int MapData::_home_tileY_start = 0;
 int MapData::_home_tileX_end = 0;

float MapData::tileX2worldX(const int &x)
{
	assert(x >= 0 && x <= _tiles_num_w);
	return x * _tile_w_in_scene + _map_start_x;
}

float MapData::tileY2worldY(const int &y)
{
	assert(y >= 0 && y <= _tiles_num_h);
	return -y * _tile_h_in_scene + _map_start_y;
}
/**
return the world position of the left top corner of tile(x, y) .
*/
Point MapData::tilePos2worldPos(const int &x, const int& y){


	float res_x = tileX2worldX(x);
	float res_y = tileY2worldY(y);
	return Point(res_x, res_y);
}


/**
return the world position of the center of tile(x, y) .
*/
Point MapData::getTileCenter(const int &x, const int& y){
	Point start = tilePos2worldPos(x, y);

	return Point(start.x + _tile_w_in_scene / 2.0f, start.y - _tile_h_in_scene / 2.0f);
}


float MapData::getNearestWorldX(const float &curx)
{
	int curTilex = getNearestTileX(curx);
	return tileX2worldX(curTilex);

}

int MapData::getNearestTileX(const float &curx)
{
	int preTile = (curx - _map_start_x) / _tile_w_in_scene;
	float remain = (curx - _map_start_x) - _tile_w_in_scene * preTile;
	if (remain > _tile_w_in_scene / 2.0f)
	{
		return preTile + 1;
	}
	return preTile;

}

float MapData::getNearestWorldY(const float &cury)
{
	int curTiley = getNearestTileY(cury);
	return tileY2worldY(curTiley);
}


int MapData::getNearestTileY(const float &cury)
{
	int preTile = (_map_start_y - cury) / _tile_h_in_scene;
	float remain = (_map_start_y - cury) - _tile_h_in_scene * preTile;
	if (remain > _tile_h_in_scene / 2.0f)
	{
	return (preTile + 1);
	}
	return  preTile;
}

float MapData::getMaxDistanceInMap()
{
	return _size.width + _size.height;
}


vector<Vec2> MapData::getEnemyHomes(){
	assert(_bgLayer != NULL);
	vector<Vec2> res;
	for (int y = 1; y < _tiles_num_w; y = y + 2)
	{
		uint32_t gid = _bgLayer->getTileGIDAt(Point(y, 1));
		if (0 == gid)
		{
			res.emplace_back(tilePos2worldPos(y, 1));
		}
	}
	for (int x = 1; x < _tiles_num_h >> 1; x = x + 2)
	{
		uint32_t gid1 = _bgLayer->getTileGIDAt(Point(1, x));

		if (0 == gid1)
		{
			res.emplace_back(tilePos2worldPos(1, x));
		}
		uint32_t gid2 = _bgLayer->getTileGIDAt(Point(_tiles_num_w - 1, x));
		if (0 == gid2)
		{
			res.emplace_back(tilePos2worldPos(_tiles_num_w - 1, x));
		}
	}
	return res;
}

vector<Vec2> MapData::getAwardHomes(){
	assert(_bgLayer != NULL);
	vector<Vec2> res;
	int halfy = _tiles_num_h >> 1;
	if ((halfy & 1) == 0)
	{
		++halfy;
	}
	for (int y = halfy; y < _home_tileY_start; y = y + 2)
	{
		for (int x = 1; x < _tiles_num_w; x = x + 2)
		{
			if (isBlank(x, y))
			{
				res.emplace_back(tilePos2worldPos(x, y));
			}
		}
	}
	for (int y = _tiles_num_h - 1 ; y >= _home_tileY_start; y = y - 2)
	{
		for (int x = 1; x < _home_tileX_start; x = x + 2)
		{
			if (isBlank(x, y))
			{
				res.emplace_back(tilePos2worldPos(x, y));
			}
				
		}

		for (int x = _tiles_num_w - 1; x > _home_tileX_end; x = x - 2)
		{
			if (isBlank(x, y))
			{
				res.emplace_back(tilePos2worldPos(x, y));
			}
		}
	}
	return res;
}

//convert touch Coordinate to map Coordinate
Point MapData::tileCoordinateFromPosition(Point pos)
{
	assert(_bgLayer != NULL);
	int cox, coy;

	//Size szLayer=_bg1Layer->getLayerSize();
	Size szLayer = _bgLayer->getLayerSize();

	//Size szTile = _gameMap->getTileSize();

	//cox = pos.x / szTile.width;
	//与地图坐标相反，所以减
	//coy = szLayer.height - pos.y / szTile.height;
	cox = pos.x / _tile_w_in_scene;
	coy = szLayer.height - pos.y / _tile_h_in_scene;

	if ((cox >= 0) && (cox < szLayer.width) && (coy >= 0) && (coy < szLayer.height)) {

		return  Point(cox, coy);
	}
	else {

		return Point(-1, -1);
	}

}


//convert map x,y to tile value;
int MapData::tileIDFromPosition(Point pos)
{
	assert(_bgLayer != NULL);
	Point cpt = tileCoordinateFromPosition(pos);
	if (cpt.x < 0) return -1;
	if (cpt.y < 0) return -1;
	//if (cpt.x >= _bg1Layer->getLayerSize().width) return -1;
	//if (cpt.y >= _bg1Layer->getLayerSize().height) return -1;


	//return _bg1Layer->getTileGIDAt(cpt);

	if (cpt.x >= _bgLayer->getLayerSize().width) return -1;
	if (cpt.y >= _bgLayer->getLayerSize().height) return -1;


	return _bgLayer->getTileGIDAt(cpt);
}

cocos2d::Vec2 MapData::getHomePos()
{
	return tilePos2worldPos(_home_tileX_start - 1, _tiles_num_h - 1);
}

CollisionDetector * MapData::_collDetector = NULL;

