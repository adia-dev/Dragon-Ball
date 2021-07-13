#pragma once

#include "AssetManager.h"

struct Animation {

	std::vector<sf::IntRect> frames;
	float frameDuration = 0.1f;

	int stopFrameCnt = -1;
	int desiredStopFrameCnt = -1;
	int stopFrame = -1;
	int repeatFramesBegin = -1;
	int repeatFramesEnd = -1;
	int repeatAnimationCnt = -1;
	int desiredRepeatAnimationCnt = -1;

	bool isLoop = false;
	bool isFlipped = false;
	bool isPlaying = false;
	bool isLocked = false;

	Animation()
	{
	}

	Animation(std::vector<sf::IntRect>& frames, const float& frameDuration = 0.1f, bool isLoop = false, bool isLocked = false)
		:frames(frames), frameDuration(frameDuration), isLoop(isLoop), isLocked(isLocked)
	{
		isPlaying = true;
	}

};

class Animator
{
public:

	Animator(sf::Sprite& s);

	void update(const float& deltaTime);

	Animation* CreateAnimation(const std::string& name, std::vector<sf::IntRect>& frames, const float& frameDuration = 0.1f, bool isLoop = false, bool isLocked = false);
	void SetAnimation(Animation* animation);
	void SetAnimation(const std::string& name);

	bool loadAnimation(const std::string& fileName, PATHS path = PATHS::NONE);

private:
	void forceUpdateAnimation();
	
	std::string getXMLAttribute(const std::string& attribute, const std::string& line);

	sf::Sprite& _s;
	Animation* _currentAnimation = nullptr;
	std::map<std::string, Animation> _animations;

	int _currentFrame = 0;
	float _animationTimer = 0.f;

};

