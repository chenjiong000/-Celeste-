#pragma once
#include<vector>
#include<memory>
#include "cocos2d.h"
#include<string>


enum MARIO_DIR { DIR_NONE, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN };
class Common
{
public:
	static void loadMarioAnimation(const char* pngFile, int width, int height,
		int frameCount, const char* aniName, float delay, int startIdx = 0,int reverse=0)
	{
		cocos2d::Texture2D *texture = cocos2d::TextureCache::sharedTextureCache()->addImage(pngFile);
		auto *anim = cocos2d::Animation::create();
		for (int i = startIdx; i < frameCount + startIdx; i++)
		{
			auto *sf= cocos2d::SpriteFrame::createWithTexture(texture, cocos2d::Rect(i * (width+2)+1, 0, width, height));
			if(reverse)
				sf= cocos2d::SpriteFrame::createWithTexture(texture, cocos2d::Rect((frameCount-i-1) * (width+2)+1, 0, width, height));
			anim->addSpriteFrame(sf);
		}		
		anim->setDelayPerUnit(delay);
		anim->setLoops(-1);
		cocos2d::AnimationCache::sharedAnimationCache()->addAnimation(anim, aniName);
	}
	static void loadMarioSpriteFrame(const char* pngFile, int width, int height, int idx, const char* frameName,int spandx=0,int orgy=0 )
	{
		cocos2d::Texture2D* texture = cocos2d::TextureCache::sharedTextureCache()->addImage(pngFile);
		cocos2d::SpriteFrame* spriteFrame = cocos2d::SpriteFrame::createWithTexture(texture, cocos2d::Rect(idx * (width+ spandx), 0+orgy, width, height));
		cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(spriteFrame, frameName);
	}
	static void loadMarioClimbAnimation(const char* pngFile, int width, int height, cocos2d::ValueVector nums, const char* aniName, float delay, int orgx=0, int orgy = 1, int spandx = 1) {
		cocos2d::Texture2D* texture = cocos2d::TextureCache::sharedTextureCache()->addImage(pngFile);
		auto* anim = cocos2d::Animation::create();
		for (int i = 0; i < nums.size(); i++) {
			auto* sf = cocos2d::SpriteFrame::createWithTexture(texture, cocos2d::Rect(nums.at(i).asInt() * (width + spandx)+ orgx, orgy, width, height));
			anim->addSpriteFrame(sf);
		}
		anim->setDelayPerUnit(delay);
		anim->setLoops(-1);
		cocos2d::AnimationCache::sharedAnimationCache()->addAnimation(anim, aniName);
	}


	static bool isBlock(const cocos2d::Point& pt, cocos2d::TMXTiledMap* map)
	{
		// ���Ȼ�ȡ��ͼ�������
		cocos2d::Point ptMap = GL2map(pt, map);
		// �����ͼ�鲻�ڵ�ͼ�У�˵��������˶�����ͼ�⣬�˶�����ͼ���ʾû���赲
		if (!isInMap(ptMap, map))
			return false;
		// ��������ͼ�㣬������ͼ�㶼�����赲Ԫ�ص�ͼ��
		static const char* layerNames[] = { "block","sharprock"};
		for (int i = 0; i < sizeof(layerNames) / sizeof(*layerNames); i++)
		{
			// ��ȡͼ���е�GID�����GID������0����ô��ʾ���赲
			cocos2d::TMXLayer* layer = map->layerNamed(layerNames[i]);
			int gid = layer->tileGIDAt(ptMap);
			if (gid != 0)
			{
				//if (blockSprite)* blockSprite = layer->tileAt(ptMap);
				//if (layerName)* layerName = layerNames[i];
				//if (pgid)* pgid = gid;
				return true;
			}
		}
		return false;
	}
	static cocos2d::Point GL2map(const cocos2d::Point& ptGL, cocos2d::TMXTiledMap* map)  //��Ļ��ת����Ƭ��ͼ�㣨��ƬΪ��λ��,ע����Ƭ��ͼ���Ͻ�ԭ��
	{
		cocos2d::Point ptUI = ptGL;
		ptUI.y = map->getContentSize().height - ptGL.y;

		int x = ptUI.x / map->getTileSize().width;
		int y = ptUI.y / map->getTileSize().height;
		return cocos2d::Point(x, y);
	}
	static bool isInMap(const cocos2d::Point& pos, cocos2d::TMXTiledMap* map)
	{
		auto mapSize = map->getMapSize();
		return pos.x < mapSize.width
			&& pos.y < mapSize.height
			&& pos.x >= 0
			&& pos.y >= 0;
	}

	static int isBlock3Point(cocos2d::Point pt[3], cocos2d::TMXTiledMap* map)
	{
		int bolckstate = 0;
		cocos2d::ValueVector v = { cocos2d::Value(4),cocos2d::Value(2),cocos2d::Value(1) };
		for (int i = 0; i < 3; i++)
		{
			if (isBlock(pt[i], map))
				bolckstate += v.at(i).asInt();
		}
		return bolckstate;
	}
	

};