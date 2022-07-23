#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

class Image
{
public:
	struct Header
	{
		char idLength;
		char colorMapType;
		char dataTypeCode;
		short colorMapOrigin;
		short colorMapLength;
		char colorMapDepth;
		short xOrigin;
		short yOrigin;
		short width;
		short height;
		char bitsPerPixel;
		char imageDescriptor;
	};
	struct Pixel
	{
		Pixel(unsigned char red, unsigned char green, unsigned char blue) : red(red), green(green), blue(blue) {}
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		int clamp(int num)
		{
			if (num > 255)
			{
				return 255;
			}
			else if (num < 0)
			{
				return 0;
			}

			return num;
		}
		Pixel Multiply(Pixel pixel)
		{			
			unsigned char newRed = (unsigned char)((((float)this->red * (float)pixel.red) / 255.0) + 0.5f);
			unsigned char newGreen = (unsigned char)((((float)this->green * (float)pixel.green) / 255.0) + 0.5f);
			unsigned char newBlue = (unsigned char)((((float)this->blue * (float)pixel.blue) / 255.0) + 0.5f);
			Pixel newPixel(newRed, newGreen, newBlue);
			return newPixel;
		}
		Pixel Subtract(Pixel pixel)
		{
			unsigned char newRed = (unsigned char)clamp((int)this->red - (int)pixel.red);
			unsigned char newGreen = (unsigned char)clamp((int)this->green - (int)pixel.green);
			unsigned char newBlue = (unsigned char)clamp((int)this->blue - (int)pixel.blue);
			Pixel newPixel(newRed, newGreen, newBlue);
			return newPixel;
		}
		Pixel Screen(Pixel pixel)
		{
			unsigned char newRed = (unsigned char)((((1.0 - (1.0 - (float)this->red / 255.0) * (1.0 - (float)pixel.red / 255.0))) * 255.0) + 0.5f);
			unsigned char newGreen = (unsigned char)((((1.0 - (1.0 - (float)this->green / 255.0) * (1.0 - (float)pixel.green / 255.0))) * 255.0) + 0.5f);
			unsigned char newBlue = (unsigned char)((((1.0 - (1.0 - (float)this->blue / 255.0) * (1.0 - (float)pixel.blue / 255.0))) * 255.0) + 0.5f);
			Pixel newPixel(newRed, newGreen, newBlue);
			return newPixel;
		}
		Pixel Overlay(Pixel pixel)
		{
			unsigned char newRed, newGreen, newBlue;

			if (((float)pixel.red / 255.0) <= 0.5)
			{
				newRed = (unsigned char)(((2.0 * (float)this->red * (float)pixel.red) / 255.0) + 0.5f);
			}
			else
			{
				newRed = (unsigned char)((((1.0 - 2.0 * (1.0 - (float)this->red / 255.0) * (1.0 - (float)pixel.red / 255.0))) * 255.0) + 0.5f);
			}

			if (((float)pixel.green / 255.0) <= 0.5)
			{
				newGreen = (unsigned char)(((2.0 * (float)this->green * (float)pixel.green) / 255.0) + 0.5f);
			}
			else
			{
				newGreen = (unsigned char)((((1.0 - 2.0 * (1.0 - (float)this->green / 255.0) * (1.0 - (float)pixel.green / 255.0))) * 255.0) + 0.5f);
			}
			if (((float)pixel.blue / 255.0) <= 0.5)
			{
				newBlue = (unsigned char)(((2.0 * (float)this->blue * (float)pixel.blue) / 255.0) + 0.5f);
			}
			else
			{
				newBlue = (unsigned char)((((1.0 - 2.0 * (1.0 - (float)this->blue / 255.0) * (1.0 - (float)pixel.blue / 255.0))) * 255.0) + 0.5f);
			}
			Pixel newPixel(newRed, newGreen, newBlue);
			return newPixel;
		}
		Pixel Combine(Pixel pixel1, Pixel pixel2)
		{
			unsigned char newRed = this->red;
			unsigned char newGreen = pixel1.green;;
			unsigned char newBlue = pixel2.blue;
			Pixel newPixel(newRed, newGreen, newBlue);
			return newPixel;
		}
		void addToChannel(int num, char color)
		{
			if (color == 'r')
			{
				red = (unsigned char)clamp((int)red + num);
			}
			else if (color == 'g')
			{
				green = (unsigned char)clamp((int)green + num);
			}
			else if (color == 'b')
			{
				blue = (unsigned char)clamp((int)blue + num);
			}
		}
		void multiplyChannel(int num, char color)
		{
			if (color == 'r')
			{
				red = (unsigned char)clamp((int)red * num);
			}
			else if (color == 'g')
			{
				green = (unsigned char)clamp((int)green * num);
			}
			else if (color == 'b')
			{
				blue = (unsigned char)clamp((int)blue * num);
			}
		}
	};
	Image();
	Image(ifstream& inFile);
	Image(Header h, const vector<Pixel>& pixels);
	void readFile(ifstream& inFile);
	void writeToFile(ofstream& oFile);
	Header getHeader() { return h; }
	void setHeader(Header header) { h = header;  }
	const vector<Pixel>& getPixels() const { return pixels; }
	void addPixel(Pixel pixel) { pixels.push_back(pixel); }
	void manipulateChannel(int num, char color, char oper);
	void singleChannel(char color);
	Image Combine(Image image1, Image image2);
	void Flip();
private:
	vector<Pixel> pixels;
	Header h;
};

#endif 
