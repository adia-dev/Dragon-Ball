#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <cstdlib>
#include <assert.h>


constexpr uint32_t W = 800;
constexpr uint32_t H = 600;

enum class PATHS
{
	NONE,
	CHARACTERS,
	ANIMATIONS,
	FONTS,
	IMAGES
};

class Utils
{
public:

	Utils();
	Utils(const sf::Vector2u& windowSize);

	template<typename T>
	static float magnitude(const sf::Vector2<T> v);

	static sf::Vector2f normalize(const sf::Vector2f v);

	template<typename T>
	static T lerp(const T& a, const T& b, const T& t);

	template<typename T>
	static sf::Vector2<T> vLerp(const sf::Vector2<T>& left, const sf::Vector2<T>& right, const T& t);

	template<typename T>
	static void printValue(const std::string& name, const T& value);

	template<typename T>
	static void printVector(const sf::Vector2<T>& v);

	static sf::Vector2u getWindowSize();

	static void setWindowSize(const sf::Vector2u windowSize);

	static std::string getFilePath(const std::string& fileName, PATHS path = PATHS::NONE);

private:

	static Utils* _instance;
	sf::Vector2u _windowSize;
};
