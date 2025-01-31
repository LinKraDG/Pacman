#include <Core/AssetManager.h>
#include <UI/Text.h>
#include <Utils/Constants.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>


void draw_text(bool center, float x, float y, const std::string& text, sf::RenderWindow& window)
{
	short character_x = x;
	short character_y = y;

	char character_width;

	sf::Sprite character_sprite;

	sf::Texture* font_texture = AssetManager::getInstance()->loadTexture(FONT_SPRITES);;

	character_width = font_texture->getSize().x / 96;

	character_sprite.setTexture(*font_texture);

	if (1 == center)
	{
		character_x = static_cast<float>(round(0.5f * (SPRITES_SIZE * MAP_WIDTH - character_width * text.substr(0, text.find_first_of('\n')).size())));
		character_y = static_cast<float>(round(0.5f * (SPRITES_SIZE * MAP_HEIGHT - FONT_HEIGHT * (1 + std::count(text.begin(), text.end(), '\n')))));
	}

	for (std::string::const_iterator a = text.begin(); a != text.end(); a++)
	{
		if ('\n' == *a)
		{
			if (1 == center)
			{
				character_x = static_cast<float>(round(0.5f * (SPRITES_SIZE * MAP_WIDTH - character_width * text.substr(1 + a - text.begin(), text.find_first_of('\n', 1 + a - text.begin()) - (1 + a - text.begin())).size())));
			}
			else
			{
				character_x = x;
			}

			character_y += FONT_HEIGHT;

			continue;
		}

		character_sprite.setPosition(character_x, character_y);
		character_sprite.setTextureRect(sf::IntRect(character_width * (*a - 32), 0, character_width, FONT_HEIGHT));

		character_x += character_width;

		window.draw(character_sprite);
	}
}