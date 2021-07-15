#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <iomanip>
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

	static sf::Vector2f vLerp(const sf::Vector2f& left, const sf::Vector2f& right, const float& t);

	template<typename T>
	static void printValue(const std::string& name, const T& value);

	template<typename T>
	static void printVector(const sf::Vector2<T>& v);

	static double distance(const sf::Vector2f left, const sf::Vector2f right)
	{
		return std::sqrt(std::pow(right.x - left.x, 2) + std::pow(left.y - right.y, 2));
	}

	static sf::Vector2f direction(const sf::Vector2f left, const sf::Vector2f right)
	{
		auto dir = right - left;
		return normalize(dir);
	}

	static sf::Vector2u getWindowSize();

	static void setWindowSize(const sf::Vector2u windowSize);

	static std::string getFilePath(const std::string& fileName, PATHS path = PATHS::NONE);

	template<typename T>
	static bool contains(const sf::Vector2<T> v, const sf::Rect<T> r)
	{
		return(
			v.x > r.left
			&& v.x < r.left + r.width
			&& v.y > r.top
			&& v.y < r.top + r.height
			);
	}

	template<typename T>
	static bool intersects(const sf::Rect<T> r1, const sf::Rect<T> r2)
	{
		return(
			(
				r1.left < r2.left
			&& r1.left + r1.width > r2.left
			&& r1.top < r2.top
			&& r1.top + r1.height > r2.top)
			||
			(
				r1.left < r2.left + r2.width
				&& r1.left + r1.width > r2.left + r2.width
				&& r1.top < r2.top
				&& r1.top + r1.height > r2.top
			)
			||
			(
				r1.left < r2.left
				&& r1.left + r1.width > r2.left
				&& r1.top > r2.top
				&& r1.top + r1.height > r2.top + r2.height
			)
			||
			(
				r1.left < r2.left + r2.width
				&& r1.left + r1.width > r2.left + r2.width
				&& r1.top > r2.top
				&& r1.top + r1.height > r2.top + r2.height
			)

			);
	}

private:

	static Utils* _instance;
	sf::Vector2u _windowSize;
};
