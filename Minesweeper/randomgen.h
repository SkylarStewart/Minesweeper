#pragma once
#include <random>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <time.h>
#include <iostream>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
using namespace std;

//random number generator class (taken from the videos by Prof. Fox)
class Random {
	static std::mt19937 random;
public:
	static int Int(int min, int max) {

		std::uniform_int_distribution<int> dist(min, max);
		return dist(random);
	}
	static int Float(float min, float max) {

		std::uniform_real_distribution<float> dist(min, max);
		return dist(random);
	}
};

//texture manager (taken from the videos by Prof. Fox)
class TextureManager {

	static unordered_map<string, sf::Texture> textures;

public:
	static void LoadTexture(string filename) {
		string path = "images/";
		path += filename + ".png";

		textures[filename].loadFromFile(path);
	}

	static sf::Texture& GetTexture(string textureName) {
		if (textures.find(textureName) == textures.end()) {
			LoadTexture(textureName);

		}
		return textures[textureName];
	}
	static void Clear() {
		textures.clear();
	}
};