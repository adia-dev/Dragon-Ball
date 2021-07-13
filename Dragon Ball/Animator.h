#pragma once

#include "AssetManager.h"

struct Animation {

	std::vector<sf::IntRect> frames;
	std::vector<sf::IntRect> flippedFrames;
	float frameDuration = 0.1f;

	int stopFrameCnt = 0;
	int desiredStopFrameCnt = -1;
	int stopFrame = -1;
	int repeatFramesBegin = -1;
	int repeatFramesEnd = -1;
	int repeatAnimationCnt = 0;
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
		for (const auto& frame : frames)
		{
			flippedFrames.push_back(sf::IntRect(frame.width + frame.left, frame.top, -frame.width, frame.height));
		}
	}

	bool getRepeatFrames() { return repeatFramesBegin > 0 && repeatFramesEnd < frames.size(); }


};

class Animator
{
public:

	Animator(sf::Sprite& s);

	void update(const float& deltaTime);

	Animation* CreateAnimation(const std::string& name, std::vector<sf::IntRect>& frames, const float& frameDuration = 0.1f, bool isLoop = false, bool isLocked = false);
	void playAnimation(Animation* animation, bool isLoop = false, bool isLocked = false);
	void playAnimation(const std::string& name, bool isLoop = false, bool isLocked = false);

	void playComboAnimation(const std::string& name, int* comboIndex, bool isLoop = false, bool isLocked = false);

	std::string getCurrentAnimationName()
	{
		for (const auto& animation : _animations)
			if (&animation.second == _currentAnimation)
				return animation.first;

		return "No animation equipped";
	}

	void flip(bool status);

	int getCurrentFrame()
	{
		return _currentFrame;
	}


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

