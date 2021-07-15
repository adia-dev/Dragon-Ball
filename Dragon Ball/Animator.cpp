#include "Animator.h"
#include <fstream>


Animator::Animator(sf::Sprite& s)
	:_s(s)
{

}


Animation* Animator::CreateAnimation(const std::string& name, std::vector<sf::IntRect>& frames, const float& frameDuration, bool isLoop, bool isLocked)
{

	if (_animations.find(name) != _animations.end())
		return nullptr;

	_animations[name] = Animation(frames, frameDuration, isLoop, isLocked);

	if (_currentAnimation == nullptr) {
		playAnimation(&_animations[name]);
	}

	return &_animations[name];
}

void Animator::playAnimation(Animation* animation, bool isLoop, bool isLocked)
{


	if (_currentAnimation != nullptr ) {
		if (_currentAnimation == animation) {
			_currentAnimation->isPlaying = true;
			return;
		}

		if (_currentAnimation->isLocked)
			return;
	}

	_currentAnimation = animation;
	if (_currentAnimation != nullptr) {
		_currentFrame = 0;
		_animationTimer = 0.f;
		_currentAnimation->isPlaying = true;
		_currentAnimation->isLocked = isLocked;
		_currentAnimation->isLoop = isLoop;
		forceUpdateAnimation();
	}
}

void Animator::playAnimation(const std::string& name, bool isLoop, bool isLocked)
{
	if (_currentAnimation != nullptr && _currentAnimation->isLocked) return;

	auto animation = _animations.find(name);
	if (animation != _animations.end())
	{
		if (_currentAnimation == &animation->second) {
			_currentAnimation->isPlaying = true;
			return;
		}

		_currentAnimation = &animation->second;
		if (_currentAnimation != nullptr) {
			_currentFrame = 0;
			_animationTimer = 0.f;
			_currentAnimation->isPlaying = true;
			_currentAnimation->isLocked = isLocked;
			_currentAnimation->isLoop = isLoop;
			forceUpdateAnimation();
		}
	}
}

void Animator::playComboAnimation(const std::string& name, int* comboIndex, bool isLoop, bool isLocked)
{
	if (_currentAnimation != nullptr && _currentAnimation->isLocked) return;

	auto animation = _animations.find(name);
	if (animation != _animations.end())
	{
		if (_currentAnimation == &animation->second) {
			_currentAnimation->isPlaying = true;
			return;
		}

		_currentAnimation = &animation->second;
		if (_currentAnimation != nullptr) {
			_currentFrame = 0;
			_animationTimer = 0.f;
			_currentAnimation->isPlaying = true;
			_currentAnimation->isLocked = isLocked;
			_currentAnimation->isLoop = isLoop;
			forceUpdateAnimation();
		}
	}
}

sf::IntRect Animator::getAnimationFrame(const std::string& name, int frame)
{
	auto animation = _animations.find(name);
	if (animation != _animations.end())
	{
		if (frame >= 0 && frame < animation->second.frames.size())
			return animation->second.frames[frame];
		return animation->second.frames[0];
	}

	return sf::IntRect(0, 0, 0, 0);
}

void Animator::flip(bool status)
{
	if (_currentAnimation != nullptr) {
		_currentAnimation->isFlipped = status;
		forceUpdateAnimation();
	}
}

void Animator::update(const float& deltaTime)
{

	if (_currentAnimation == nullptr) return;

	_animationTimer += deltaTime;

	// If the animation timer is greater than the desired frame duration, go to the next frame
	if (_animationTimer >= _currentAnimation->frameDuration)
	{
		_currentFrame++;
		_animationTimer = 0.f;

		// Check the currentFrame index so we don't go out of bounds
		if (_currentFrame >= _currentAnimation->frames.size())
		{
			if (_currentAnimation->isLoop)
			{
				// If we want to repeat certain frames for a certain count, or forever execute this
				if (_currentAnimation->getRepeatFrames() && _currentAnimation->desiredRepeatAnimationCnt != _currentAnimation->repeatAnimationCnt)
				{
					_currentFrame = _currentAnimation->repeatFramesBegin;
					_currentAnimation->repeatAnimationCnt++;
				}
				else {
					_currentFrame = 0;
				}
			}
			else {
				// set the index to be the last frame of the animation
				_currentFrame = _currentAnimation->frames.size() - 1;
				_currentAnimation->isLocked = false;
			}
		}


		// If the animation is flipped, pick a frame from the flippedFrames collection
		const auto& frame = (_currentAnimation->isFlipped ? _currentAnimation->flippedFrames[_currentFrame] : _currentAnimation->frames[_currentFrame]);

		_s.setTextureRect(frame);
		//_s.setOrigin(frame.width / 2.f, frame.height / 2.f);
		_s.setOrigin(frame.width / 2.f, frame.height);

	}

}


void Animator::forceUpdateAnimation()
{
	if (_currentAnimation == nullptr) return;

	const auto& frame = (_currentAnimation->isFlipped ? _currentAnimation->flippedFrames[_currentFrame] : _currentAnimation->frames[_currentFrame]);
	_s.setTextureRect(frame);
	//_s.setOrigin(frame.width / 2.f, frame.height / 2.f);
	_s.setOrigin(frame.width / 2.f, frame.height);

}


bool Animator::loadAnimation(const std::string& fileName, PATHS path)
{

	std::ifstream file(Utils::getFilePath(fileName, path));

	if (!file.is_open()) return false;

	std::string line;
	std::string animationName;
	std::vector<sf::IntRect> frames;
	bool write = false;

	while (std::getline(file, line))
	{
		auto animation = line.find("<animation title=");
		if (animation != std::string::npos)
		{
			write = true;
			animationName = getXMLAttribute("title", line);
			continue;
		}

		auto endAnimation = line.find("</animation>");
		if (endAnimation != std::string::npos)
		{
			write = false;
			if (animationName.find("Transform") != std::string::npos)
			{
				_maxTransformation++;
			}
			CreateAnimation(animationName, frames);
			frames.clear();
		}

		if (write)
		{
			int x, y, w, h;
			x = std::stoi(getXMLAttribute("x", line));
			y = std::stoi(getXMLAttribute("y", line));
			w = std::stoi(getXMLAttribute("w", line));
			h = std::stoi(getXMLAttribute("h", line));

			frames.push_back(sf::IntRect(x, y, w, h));
		}


	}

	file.close();
	return true;
}

std::string Animator::getXMLAttribute(const std::string& attribute, const std::string& line)
{

	std::string value;
	auto seekAttribute = line.find(attribute + "=");
	if (seekAttribute != std::string::npos)
	{
		auto openingQuote = seekAttribute + attribute.length() + 2;
		auto seekClosingQuote = openingQuote + line.substr(openingQuote).find("\"");
		if (seekClosingQuote != std::string::npos) {
			for (auto i = openingQuote; i < seekClosingQuote; ++i) {
				value += line[i];
			}
		}


	}

	return value;
}
