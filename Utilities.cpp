#include "precomp.h"

namespace SLL
{
	void PrintNumber(Tmpl8::Surface* screen, int number, int x, int y, Pixel color)
	{
		char buffer[33];
		sprintf(buffer, "%d", number);
		screen->Print(buffer, x, y, color);
	}

	void PrintNumber(Tmpl8::Surface* screen, float number, int x, int y, Pixel color)
	{
		char buffer[33];
		sprintf(buffer, "%f", number);
		screen->Print(buffer, x, y, color);
	}

	void PrintNumber(Tmpl8::Surface* screen, size_t number, int x, int y, Pixel color)
	{
		char buffer[33];
		sprintf(buffer, "%zd", number);
		screen->Print(buffer, x, y, color);
	}
}