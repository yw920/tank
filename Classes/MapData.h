#ifndef _MAP_DATA_H_
#define _MAP_DATA_H_

#include <cmath>
#include <assert.h>
#include <2d/CCTMXLayer.h>

#include "MapLayer.h"
#define MAPDATA MapData::instance()

class MapData{
friend class MapLayer;
public:
	static MapData *instance(){
		return _mp;
	}

	static float tileX2worldX(const int &x);
	static float tileY2worldY(const int &y);
	static cocos2d::Point tilePos2worldPos(const int &x, const int& y);
	static cocos2d::Point getTileCenter(const int &x, const int& y);
	static int getNearestTileX(const float &curx);
	static int getNearestTileY(const float &cury);
	static float getNearestWorldX(const float &curx);
	static float getNearestWorldY(const float &cury);
	static float getMaxDistanceInMap();

	static std::vector<cocos2d::Vec2> getEnemyHomes();

	static cocos2d::Point tileCoordinateFromPosition(cocos2d::Point pos);

	static int tileIDFromPosition(cocos2d::Point pos);
	static int tileIDFromPosition(const int &x, const int &y);
	std::vector<cocos2d::Vec2> getAwardHomes();

	static float getTile_h_in_scene()  {
		assert(std::fabs(_tile_h_in_scene) > 1e-6);
		return _tile_h_in_scene; 
	}
	static float getTile_w_in_scene()  { 
		assert(std::fabs(_tile_w_in_scene) > 1e-6);
		return _tile_w_in_scene; 
	}
	static bool isBlank(int &tileX, int &tileY){
		assert(_bgLayer != NULL);
		uint32_t gid1 = _bgLayer->getTileGIDAt(cocos2d::Point(tileX, tileY));
		uint32_t gid2 = _waterLayer->getTileGIDAt(cocos2d::Point(tileX, tileY));
		if (0 == gid1 &&
			(gid2 == 17 || gid2 == 18 || gid2 == 35 || gid2 == 36)) return true;
		return false;
	}
	static float getScale()  { 
		assert(std::fabs(_scale) > 1e-6);
		return _scale; 
	}
	static void setScale(float &val) {
		_scale = val; 
	}

	static int getTiles_num_w()  { 
		assert(_tiles_num_w > 0);
		return _tiles_num_w; 
	}
	static void setTiles_num_w(int &val) { _tiles_num_w = val; }

	static int getTiles_num_h()  {
		assert(_tiles_num_h > 0);
		return _tiles_num_h; 
	}
	static void setTiles_num_h(int &val) { _tiles_num_h = val; }
	static cocos2d::TMXLayer* getWaterLayer(){
		return _waterLayer;
	}

	static cocos2d::Vec2 getHomePos();

	static int getHomeTileXStart(){
		return _home_tileX_start;
	}
	static int getHomeTileYStart(){
		return _home_tileY_start;
	}

	static int getHomeTileXEnd(){
		return _home_tileX_end;
	}

	static float getHeight(){
		return _size.height;
	}

	static float getWidth(){
		return _size.width;
	}

	static CollisionDetector *getCollDetector(){
		return _collDetector;
	}

	
	
private:
	MapData(){}
	//the left top corner coordinate of map
	static float _map_start_x;
	static float _map_start_y;

	//the size of tile when game playing
	static float _tile_h_in_scene;
	static float _tile_w_in_scene;
	//the size of tiles in map
	static int _tiles_num_w;
	static int _tiles_num_h;

	static int _home_tileX_start;
	static int _home_tileY_start;
	static int _home_tileX_end;

	static cocos2d::Size _size;
	static cocos2d::TMXLayer* _bgLayer;
	static cocos2d::TMXLayer* _waterLayer;
	static float _scale;
	static MapData *_mp;
	static CollisionDetector *_collDetector;
};
#endif
