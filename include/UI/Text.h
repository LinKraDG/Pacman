#pragma once
#include <string>

namespace sf
{
	class RenderWindow;
}

void draw_text(bool i_center, float i_x, float i_y, const std::string& i_text, sf::RenderWindow& i_window);