#include <iostream>
#include <list>
#include <fstream>
#include <iomanip>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>



int main()
{
	sf::Image img;
	img.loadFromFile("console.png");
	
	int rows = img.getSize().y / 12;
	int columns = img.getSize().x / 8;


	const int height = 12;
	const int width = 8;

	std::list<unsigned char> bytelist;

	for(int row = 0; row < rows; ++row)
	{
		for(int column = 0; column < columns; ++column)
		{
			bool content = false;
			for(int y = 0; y < height; ++y)
			{
				unsigned char by = 0;
				unsigned char bit = 0x80;
				for(int x = 0; x < width; ++x)
				{
					if(img.getPixel(column * 8 + x, row * height + y) == sf::Color::White)
					{
						by |= bit;
					}
					bit >>= 1;
				}
				if(by)
					content = true;
				bytelist.push_back(by);
			}
			if(!content)
			{
				for(int i = 0; i < height; ++i)
					bytelist.pop_back();
			}
		}
	}


	std::ofstream file("out.txt");

	file << "size= " << bytelist.size() / height << "\n";
	file << "{\n\t";

	int num = 0;

	for(auto it = bytelist.begin();
		it != bytelist.end();
		++it)
	{
		file << "0x" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << int(*it) << ", ";
		if(++num > 20)
		{
			file << "\n\t";
			num = 0;
		}
	}
	file << "}\n";


	return 0;
}
