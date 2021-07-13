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
		SetAnimation(&_animations[name]);
	}

	return &_animations[name];
}

void Animator::update(const float& deltaTime)
{

	if (_currentAnimation == nullptr) return;

	_animationTimer += deltaTime;
}


void Animator::SetAnimation(Animation* animation)
{
	_currentAnimation = animation;
	forceUpdateAnimation();
}

void Animator::SetAnimation(const std::string& name)
{
	auto animation = _animations.find(name);
	if (animation != _animations.end())
	{
		_currentAnimation = &animation->second;
		forceUpdateAnimation();
	}
}


void Animator::forceUpdateAnimation()
{
	if (_currentAnimation == nullptr) return;

	const auto& frame = _currentAnimation->frames[_currentFrame];

	_s.setTextureRect(frame);
	//_s.setOrigin(frame.width / 2.f, frame.height / 2.f);

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
