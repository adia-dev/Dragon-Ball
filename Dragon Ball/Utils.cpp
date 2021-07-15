#include "Utils.h"

Utils* Utils::_instance = nullptr;

Utils::Utils()
{
	assert(_instance == nullptr);

	_instance = this;
}

Utils::Utils(const sf::Vector2u& windowSize)
{
	assert(_instance == nullptr);

	_instance = this;
	_instance->_windowSize = windowSize;
}

template<typename T>
float Utils::magnitude(const sf::Vector2<T> v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}


sf::Vector2f Utils::normalize(const sf::Vector2f v)
{
	auto vMag = magnitude(v);
	if (vMag <= 0.f)
		return v;
	return sf::Vector2f(v.x / vMag, v.y / vMag);
}

template<typename T>
T Utils::lerp(const T& a, const T& b, const T& t)
{
	return a * (1 - t) + b * t;
}

sf::Vector2f Utils::vLerp(const sf::Vector2f& left, const sf::Vector2f& right, const float& t)
{
	return sf::Vector2f(lerp(left.x, right.x, t), lerp(left.y, right.y, t));
}

template<typename T>
inline void Utils::printValue(const std::string& name, const T& value)
{
	std::cout << name << " : " << std::to_string(value) << std::endl;
}

template<typename T>
inline void Utils::printVector(const sf::Vector2<T>& v)
{
	std::cout << "(" << v.x << ", " << v.y << ")" << std::endl;
}


sf::Vector2u Utils::getWindowSize()
{
	if (_instance != nullptr)
		return _instance->_windowSize;
	return sf::Vector2u(0,0);
}

void Utils::setWindowSize(const sf::Vector2u windowSize)
{
	if (_instance != nullptr)
		_instance->_windowSize = windowSize;
}

std::string Utils::getFilePath(const std::string& fileName, PATHS path)
{
	std::string filePath;

	switch (path)
	{
	case PATHS::NONE:
		filePath = fileName;
		break;
	case PATHS::CHARACTERS:
		filePath = "assets/characters/" + fileName;
		break;
	case PATHS::ANIMATIONS:
		filePath = "assets/animations/" + fileName;
		break;
	case PATHS::FONTS:
		filePath = "assets/fonts/" + fileName;
		break;
	case PATHS::IMAGES:
		filePath = "assets/images/" + fileName;
		break;
	default:
		filePath = fileName;
		break;
	}

	return filePath;
}
