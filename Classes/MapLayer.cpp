#include "MapLayer.h"
#include "SimpleAudioEngine.h"
#include "TankSprite.h"
#include "Config.h"
#include "TileNode.h"
#include "TileIron.h"
#include "TileGrass.h"
#include "TileBrick.h"
#include "TileWater.h"
#include "TileHomeWall.h"
#include "TileHome.h"
#include "MapData.h"
#include "TankLayer.h"
#include <cocos2d.h>



USING_NS_CC;
using namespace CocosDenshion;
using namespace std;



// on "init" you need to initialize your instance
bool MapLayer::init()
{
	// 1. super init first
	if (!Node::init())
	{
		return false;
	}
	return true;
}

void MapLayer::initHome()
{
    //找到老巢位置
	for (int x = 0; x < MapData::_tiles_num_w; ++x)
	{
		bool startFound = false;
		for (int y = 0; y < MapData::_tiles_num_h; y++)
		{
			uint32_t gid = MapData::_bgLayer->getTileGIDAt(Point(x, y));
			if (13 == gid || 14 == gid || 31 == gid || 32 == gid)
			{
				MapData::_home_tileX_start = x - 1;
				MapData::_home_tileY_start = y - 1;
				startFound = true;
				break;
			}
		}
		if (startFound)
		{
			break;
		}
	}
    //横着的砖头
	for (int x = MapData::_home_tileX_start; x < MapData::_tiles_num_w; ++x)
	{
		uint32_t gid = MapData::_bgLayer->getTileGIDAt(Point(x, MapData::_home_tileY_start + 1));
		if (0 == gid)
		{
			MapData::_home_tileX_end = x;
			break;
		}
	}
    //老巢
	TileHome *homeNode = TileHome::create(MapData::_collDetector, MapData::_home_tileX_start + 2, MapData::_tiles_num_h - 1, MapData::_bgLayer);
	this->getParent()->addChild(homeNode, 0);
}

//初始化map
void MapLayer::initWithMap(int leve)
{
	_leve = leve;
	_tankLayer = NULL;
	MapData::_collDetector = dynamic_cast<CollisionDetector *>(this->getParent()->getChildByName("collDetector"));
    //添加地图
	_gameMap = TMXTiledMap::create(String::createWithFormat("tankMap%d.tmx", leve)->getCString());
	
	//缩放比例
	MapData::_scale = wSize.height / _gameMap->getContentSize().height;
	_gameMap->setScale(MapData::_scale);
	
    //map在屏幕中的size
	MapData::_size = Size(_gameMap->getContentSize().width*MapData::_scale, _gameMap->getContentSize().height*MapData::_scale);
	
    //_gameMap的位置是相对MapLayer的，MapLayer的anchor是屏幕正中间,_gmaeMap的anchor是0，0，所以位置是-_size
	_gameMap->setPosition(-MapData::_size.width / 2, -MapData::_size.height / 2);
    this->addChild(_gameMap, 0);

    
    //取得地图中设置的数据
	MapData::_bgLayer = _gameMap->getLayer("layer1");
	MapData::_waterLayer = _gameMap->getLayer("waterLayer");
	MapData::_waterLayer->setGlobalZOrder(-10.0);
	//this->getParent()->order
	//_objects = _gameMap->getObjectGroup("objects");

	
	

    //attention please! createEdgeBox and createBox are different
	

    //the left top corner coordinate of map
	MapData::_map_start_x = (wSize.width - MapData::_size.width) / 2.0f;
	MapData::_map_start_y = (wSize.height + MapData::_size.height) / 2.0f;
    
	MapData::_tiles_num_w = _gameMap->getMapSize().width;
	MapData::_tiles_num_h = _gameMap->getMapSize().height;
	MapData::_tile_h_in_scene = MapData::_size.width / MapData::_tiles_num_w;
	MapData::_tile_w_in_scene = MapData::_size.height / MapData::_tiles_num_h;


	initHome();
	
	_left_edge = (EdgeLine *)EdgeVertical::create(MapData::_collDetector, Vec2(MapData::_map_start_x, MapData::_map_start_y - MapData::_size.height));
	_left_edge->setTag(NodeWithCollision::COLL_EDGE_LEFT);
	this->getParent()->addChild(_left_edge);
	
	_right_edge = (EdgeLine*)EdgeVertical::create(MapData::_collDetector, Vec2(MapData::_map_start_x + MapData::_size.width, MapData::_map_start_y - MapData::_size.height));
	_right_edge->setTag(NodeWithCollision::COLL_EDGE_RIGHT);
	this->getParent()->addChild(_right_edge);

	_top_edge = (EdgeLine*)EdgeHorizontal::create(MapData::_collDetector, Vec2(MapData::_map_start_x, MapData::_map_start_y));
	_top_edge->setTag(NodeWithCollision::COLL_EDGE_TOP);
	this->getParent()->addChild(_top_edge);

	_bottom_edge = (EdgeLine*)EdgeHorizontal::create(MapData::_collDetector, Vec2(MapData::_map_start_x, MapData::_map_start_y - MapData::_size.height));
	_bottom_edge->setTag(NodeWithCollision::COLL_EDGE_BOTTOM);
	this->getParent()->addChild(_bottom_edge);

	//设置MapLayer在屏幕的正中间
	this->setPosition(Point(wSize.width / 2.0f, wSize.height / 2.0f));

	//register tiles for collision，
	for (int x = 0; x < MapData::_tiles_num_w; x++)
	{
		for (int y = 0; y < MapData::_tiles_num_h; ++y)
		{
			//_tiles[curi] = NULL;
			uint32_t gid = MapData::_bgLayer->getTileGIDAt(Point(x, y));
			//if (0 == gid) continue;
			if (x > MapData::_home_tileX_start && x < MapData::_home_tileX_end -1
				&& y > MapData::_home_tileY_start)
					continue;
            //讲tile坐标改为针对MapLayer的坐标
			TileNode *tileNode = createTile(gid, x, y);
			if (tileNode != nullptr)
			{
				this->getParent()->addChild(tileNode, 0);
				continue;
			}

			uint32_t waterGid = MapData::_waterLayer->getTileGIDAt(Point(x, y));
			switch (waterGid)
			{
			case 9:
			case 10:
			case 27:
			case 28:
				tileNode = (TileNode *)(TileWater::create(MapData::_collDetector, x, y, MapData::_waterLayer));
				this->getParent()->addChild(tileNode, 0);
				break;
			default:
				break;
			}
		}
	}
}

TileNode * MapLayer::createTile(uint32_t gid, int x, int y)
{
	if ((y == MapData::_home_tileY_start && x >= MapData::_home_tileX_start && x < MapData::_home_tileX_end)
		|| (y > MapData::_home_tileY_start && (MapData::_home_tileX_start == x || MapData::_home_tileX_end  - 1 == x)))
	{
		return TileHomeWall::create(MapData::_collDetector, x, y, MapData::_bgLayer);
	}
	
	switch (gid)
	{
	case 0:
		break;
	case 5:
	case 6:
	case 23:
	case 24:
		return TileIron::create(MapData::_collDetector, x, y, MapData::_bgLayer);
	case 7:
	case 8:
	case 25:
	case 26:
		return TileGrass::create(MapData::_collDetector, x, y, MapData::_bgLayer);
	case 1:
	case 2:
	case 19:
	case 20:
		return TileBrick::create(MapData::_collDetector, x, y, MapData::_bgLayer);
	default:
		break;
	}
	return nullptr;
}

void MapLayer::destroyTile(TileNode *tile)
{
	int tileX = tile->getTileX();
	int tileY = tile->getTileY();
	int key = abs(MapData::_home_tileX_start + 2 - tileX) + abs(MapData::_tiles_num_h - 1 - tileY);
	_destroyedTiles.emplace(DestroyedTile(tileX, tileY, tile->getGid(), key));
}

bool MapLayer::repairTile()
{
	if (_destroyedTiles.empty()) return false;
	DestroyedTile curTile = _destroyedTiles.top();
	MapData::_bgLayer->setTileGID(curTile.gid, Vec2(curTile.x, curTile.y));
	TileNode *tileNode = createTile(curTile.gid, curTile.x, curTile.y);
	this->getParent()->addChild(tileNode);
	_destroyedTiles.pop();
	return true;
}

MapLayer::~MapLayer()
{
	if (_tankLayer != nullptr)
	{
		_tankLayer->setMapLayer(nullptr);
	}
}

//int MapLayer::removeTile(Vec2 tilexy)
//{
//	MapData::_bgLayer->removeTileAt(tilexy);
//	return 0;
//}

bool EdgeLine::init(CollisionDetector *detector, cocos2d::Vec2 pos)
{
	setPosition(pos);
	if (!NodeWithCollision::init(detector))
	{
		return false;
	}
	return true;
}

b2Body * EdgeVertical::createBody()
{
	return _collDetector->createEdgeLine(this, getPositionX(), getPositionY(), 0,  MAPDATA->getHeight());
}



b2Body * EdgeHorizontal::createBody()
{
	return _collDetector->createEdgeLine(this, getPositionX(), getPositionY(), MAPDATA->getWidth(), 0);
}
