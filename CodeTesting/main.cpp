#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

enum class GameState
{

	MAIN,
	PAUSE,


};

int main()
{
	std::cout << "Hello world";

	int size;
	std::vector<int> num;

	std::cout << "Enter the size of your number container-> ";
	std::cin >> size;
	
	for (int i = 0; i < size; i++)
	{
		printf("Insert value of num[%d]: ", i);
		int tmp;
		std::cin >> tmp;
		num.push_back(tmp);
	}

	std::sort(num.begin(), num.end());

	std::cout << "Sorted list: \n";

	for (int i = 0; i < size; i++)
	{
		std::cout << num[i] << std::endl;
	}
	return 0;
}