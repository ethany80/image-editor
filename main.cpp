#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <functional>
using namespace std;

#include "Image.h"

Image Manipulate(Image tLayer, Image bLayer, function<Image::Pixel(Image::Pixel, Image::Pixel)> func)
{
	vector<Image::Pixel> newPixels;
	vector<Image::Pixel> tLayerPixels = tLayer.getPixels();
	vector<Image::Pixel> bLayerPixels = bLayer.getPixels();
	
	for (unsigned int i = 0; i < tLayerPixels.size(); ++i)
	{
		Image::Pixel newPixel = func(tLayerPixels.at(i), bLayerPixels.at(i));
		newPixels.push_back(newPixel);
	}

	Image newImage(tLayer.getHeader(), newPixels);

	return newImage;
}

Image::Pixel Multiply(Image::Pixel tPixel, Image::Pixel bPixel) { return tPixel.Multiply(bPixel); }
Image::Pixel Subtract(Image::Pixel tPixel, Image::Pixel bPixel) { return bPixel.Subtract(tPixel); }
Image::Pixel Screen(Image::Pixel tPixel, Image::Pixel bPixel) { return tPixel.Screen(bPixel); }
Image::Pixel Overlay(Image::Pixel tPixel, Image::Pixel bPixel) { return tPixel.Overlay(bPixel); }

void loadImage(map<string, Image>& images, ifstream& inFile, string name)
{
	string fileName = "input/" + name + ".tga";
	inFile.open(fileName, ios_base::binary);
	Image image(inFile);
	images.emplace(name, image);
	inFile.close();
	inFile.clear();
}

void Tasks(map<string, Image>& images, ofstream& oFile, int part, ifstream& inFile)
{
	if (part < 6)
	{
		Image result;

		if (part == 1)
		{
		oFile.open("output/part1.tga", ios_base::binary);
		result = Manipulate(images.at("layer1"), images.at("pattern1"), &Multiply);

		}
		else if (part == 2)
		{
			oFile.open("output/part2.tga", ios_base::binary);
			result = Manipulate(images.at("layer2"), images.at("car"), &Subtract);
		}
		else if (part == 3)
		{
			oFile.open("output/part3.tga", ios_base::binary);
			Image temp = Manipulate(images.at("layer1"), images.at("pattern2"), &Multiply);
			result = Manipulate(images.at("text"), temp, &Screen);
		}
		else if (part == 4)
		{
			oFile.open("output/part4.tga", ios_base::binary);
			Image temp = Manipulate(images.at("layer2"), images.at("circles"), &Multiply);
			result = Manipulate(images.at("pattern2"), temp, &Subtract);
		}
		else if (part == 5)
		{
			oFile.open("output/part5.tga", ios_base::binary);
			result = Manipulate(images.at("layer1"), images.at("pattern1"), &Overlay);
		}

		result.writeToFile(oFile);
	}
	else if (part < 8)
	{
		if (part == 6)
		{
			oFile.open("output/part6.tga", ios_base::binary);
			images.at("car").manipulateChannel(200, 'g', '+');
			images.at("car").writeToFile(oFile);
			images.erase("car");
			loadImage(images, inFile, "car");
		}
		else if (part == 7)
		{
			oFile.open("output/part7.tga", ios_base::binary);
			images.at("car").manipulateChannel(4, 'r', '*');
			images.at("car").manipulateChannel(0, 'b', '*');
		}

		images.at("car").writeToFile(oFile);
		images.erase("car");
		loadImage(images, inFile, "car");
	}
	else if (part == 8)
	{
		char colors[3] = { 'r', 'g', 'b' };

		for (int i = 0; i < 3; ++i)
		{
			string fileName = "output/part8_.tga";
			fileName.insert(13, 1, colors[i]);
			oFile.open(fileName, ios_base::binary);
			images.at("car").singleChannel(colors[i]);
			images.at("car").writeToFile(oFile);
			images.erase("car");
			loadImage(images, inFile, "car");
			if (i < 2)
			{
				oFile.close();
				oFile.clear();
			}
		}
	}
	else if (part == 9)
	{
		Image result = images.at("layer_red").Combine(images.at("layer_green"), images.at("layer_blue"));
		oFile.open("output/part9.tga", ios_base::binary);
		result.writeToFile(oFile);
	}
	else if (part == 10)
	{
		oFile.open("output/part10.tga", ios_base::binary);
		images.at("text2").Flip();
		images.at("text2").writeToFile(oFile);
		images.erase("text2");
		loadImage(images, inFile, "text2");
	}

	oFile.close();
	oFile.clear();
}

int main()
{
	ifstream inFile;
	map<string, Image> images;
	string imageNames[] = { "car", "circles", "layer_blue", "layer_green", "layer_red", "layer1", "layer2", "pattern1", "pattern2", "text", "text2" };
	
	for (int i = 0; i < 11; ++i)
	{
		loadImage(images, inFile, imageNames[i]);
	}

	ofstream oFile;
	
	for (int i = 1; i <= 10; ++i)
	{
		Tasks(images, oFile, i, inFile);
	}

	return 0;
}