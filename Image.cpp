#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include "Image.h"

Image::Image() {}

Image::Image(ifstream& inFile)
{
	readFile(inFile);
}

Image::Image(Header h, const vector<Pixel> &pixels)
{
	this->h.idLength = h.idLength;
	this->h.colorMapType = h.colorMapType;
	this->h.dataTypeCode = h.dataTypeCode;
	this->h.colorMapOrigin = h.colorMapOrigin;
	this->h.colorMapLength = h.colorMapLength;
	this->h.colorMapDepth = h.colorMapDepth;
	this->h.xOrigin = h.xOrigin;
	this->h.yOrigin = h.yOrigin;
	this->h.width = h.width;
	this->h.height = h.height;
	this->h.bitsPerPixel = h.bitsPerPixel;
	this->h.imageDescriptor = h.imageDescriptor;
	for (unsigned int i = 0; i < pixels.size(); ++i)
	{
		Pixel pixel(pixels.at(i).red, pixels.at(i).green, pixels.at(i).blue);
		this->pixels.push_back(pixel); 
	}
}

void Image::readFile(ifstream& inFile)
{
	inFile.read(&h.idLength, sizeof(h.idLength));
	inFile.read(&h.colorMapType, sizeof(h.colorMapType));
	inFile.read(&h.dataTypeCode, sizeof(h.dataTypeCode));
	inFile.read((char*)&h.colorMapOrigin, sizeof(h.colorMapOrigin));
	inFile.read((char*)&h.colorMapLength, sizeof(h.colorMapLength));
	inFile.read(&h.colorMapDepth, sizeof(h.colorMapDepth));
	inFile.read((char*)&h.xOrigin, sizeof(h.xOrigin));
	inFile.read((char*)&h.yOrigin, sizeof(h.yOrigin));
	inFile.read((char*)&h.width, sizeof(h.width));
	inFile.read((char*)&h.height, sizeof(h.height));
	inFile.read(&h.bitsPerPixel, sizeof(h.bitsPerPixel));
	inFile.read(&h.imageDescriptor, sizeof(h.imageDescriptor));
	for (int i = 0; i < ((int)h.height * (int)h.width); ++i)
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		inFile.read((char*)&blue, sizeof(blue));
		inFile.read((char*)&green, sizeof(green));
		inFile.read((char*)&red, sizeof(red));
		Pixel pixel(red, green, blue);
		pixels.push_back(pixel);
	}
}

void Image::writeToFile(ofstream& oFile)
{
	oFile.write(&h.idLength, sizeof(h.idLength));
	oFile.write(&h.colorMapType, sizeof(h.colorMapType));
	oFile.write(&h.dataTypeCode, sizeof(h.dataTypeCode));
	oFile.write((char*)&h.colorMapOrigin, sizeof(h.colorMapOrigin));
	oFile.write((char*)&h.colorMapLength, sizeof(h.colorMapLength));
	oFile.write(&h.colorMapDepth, sizeof(h.colorMapDepth));
	oFile.write((char*)&h.xOrigin, sizeof(h.xOrigin));
	oFile.write((char*)&h.yOrigin, sizeof(h.yOrigin));
	oFile.write((char*)&h.width, sizeof(h.width));
	oFile.write((char*)&h.height, sizeof(h.height));
	oFile.write(&h.bitsPerPixel, sizeof(h.bitsPerPixel));
	oFile.write(&h.imageDescriptor, sizeof(h.imageDescriptor));
	for (unsigned int i = 0; i < pixels.size(); ++i)
	{
		oFile.write((char*)&pixels.at(i).blue, sizeof(pixels.at(i).blue));
		oFile.write((char*)&pixels.at(i).green, sizeof(pixels.at(i).green));
		oFile.write((char*)&pixels.at(i).red, sizeof(pixels.at(i).red));
	}
}

void Image::manipulateChannel(int num, char color, char oper)
{
	for (unsigned int i = 0; i < pixels.size(); ++i)
	{
		if (oper == '+')
		{
			pixels.at(i).addToChannel(num, color);
		}
		else if (oper == '*')
		{
			pixels.at(i).multiplyChannel(num, color);
		}
	}
}

void Image::singleChannel(char color)
{
	unsigned char temp;
	
	for (unsigned int i = 0; i < pixels.size(); ++i)
	{
		if (color == 'r')
		{
			temp = pixels.at(i).red;
			pixels.at(i).green = temp;
			pixels.at(i).blue = temp;
		}
		else if (color == 'g')
		{
			temp = pixels.at(i).green;
			pixels.at(i).red = temp;
			pixels.at(i).blue = temp;
		}
		else if (color == 'b')
		{
			temp = pixels.at(i).blue;
			pixels.at(i).red = temp;
			pixels.at(i).green = temp;
		}
		
	}
}

Image Image::Combine(Image image1, Image image2)
{
	Image newImage;
	newImage.setHeader(this->h);
	vector<Pixel> pixels1 = image1.getPixels();
	vector<Pixel> pixels2 = image2.getPixels();

	for (unsigned int i = 0; i < pixels.size(); ++i)
	{
		newImage.addPixel(pixels.at(i).Combine(pixels1.at(i), pixels2.at(i)));
	}

	return newImage;
}

void Image::Flip()
{
	vector<Pixel> newPixels;

	for (int i = (pixels.size() - 1); i >= 0; --i)
	{
		newPixels.push_back(pixels.at(i));
	}

	pixels = newPixels;
}