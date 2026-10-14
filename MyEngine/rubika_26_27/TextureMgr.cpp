#include "TextureMgr.h"

#include "Logger.h"
#include "Globals.h"

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
#include <SFML/Graphics/Image.hpp>

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif

#include <charconv>
#include <cstring>

namespace
{
	bool CheckFileExists(const std::filesystem::path& path)
	{
		std::error_code ec;
		if (!std::filesystem::is_regular_file(path, ec))
		{
			Logger::Error("TextureMgr: file not found: " + path.string());
			return false;
		}
		return true;
	}

	bool LoadTextureFile(const std::filesystem::path& path, TextureData& data)
	{
		if (!data.Texture.loadFromFile(path))
		{
			Logger::Error("TextureMgr: SFML failed to load " + path.string());
			return false;
		}
		return true;
	}

	bool ReadInt(rapidxml::xml_node<>* parent, const char* childName, int& outValue)
	{
		rapidxml::xml_node<>* node = parent->first_node(childName);
		if (!node)
		{
			Logger::Error(std::string("TextureMgr: missing node <") + childName + ">");
			return false;
		}

		const char* begin = node->value();
		const char* end = begin + node->value_size();
		const auto result = std::from_chars(begin, end, outValue);
		if (result.ec != std::errc() || result.ptr != end)
		{
			Logger::Error(std::string("TextureMgr: invalid integer in <") + childName + ">");
			return false;
		}
		return true;
	}

	bool ReadBool(rapidxml::xml_node<>* parent, const char* childName, bool& outValue)
	{
		int value = 0;
		if (!ReadInt(parent, childName, value))
		{
			return false;
		}
		outValue = (value != 0);
		return true;
	}

	bool ReadName(rapidxml::xml_node<>* node, std::string& outName)
	{
		rapidxml::xml_attribute<>* attribute = node->first_attribute("Name");
		if (!attribute || attribute->value_size() == 0)
		{
			Logger::Error("TextureMgr: node without a 'Name' attribute");
			return false;
		}
		outName.assign(attribute->value(), attribute->value_size());
		return true;
	}

	bool ReadOptionalFloat(rapidxml::xml_node<>* parent, const char* childName, float& outValue)
	{
		rapidxml::xml_node<>* node = parent->first_node(childName);
		if (!node)
		{
			return true;
		}

		const char* begin = node->value();
		const char* end = begin + node->value_size();
		const auto result = std::from_chars(begin, end, outValue);
		if (result.ec != std::errc() || result.ptr != end)
		{
			Logger::Error(std::string("TextureMgr: invalid number in <") + childName + ">");
			return false;
		}
		return true;
	}

	bool ParseAnimation(rapidxml::xml_node<>* node, AnimationData& out)
	{
		return ReadInt(node, "X", out.StartX)
			&& ReadInt(node, "Y", out.StartY)
			&& ReadInt(node, "SizeX", out.SizeX)
			&& ReadInt(node, "SizeY", out.SizeY)
			&& ReadInt(node, "OffsetX", out.OffsetX)
			&& ReadInt(node, "OffsetY", out.OffsetY)
			&& ReadInt(node, "SpriteNum", out.AnimationSpriteCount)
			&& ReadInt(node, "SpritesOnLine", out.SpriteOnLine)
			&& ReadBool(node, "Reverted", out.IsReverted)
			&& ReadOptionalFloat(node, "TimeBetweenAnimation", out.TimeBetweenAnimationInS);
	}

	bool ParseTile(rapidxml::xml_node<>* node, StaticTileData& out)
	{
		return ReadInt(node, "X", out.StartX)
			&& ReadInt(node, "Y", out.StartY)
			&& ReadInt(node, "SizeX", out.SizeX)
			&& ReadInt(node, "SizeY", out.SizeY)
			&& ReadBool(node, "RevertedX", out.IsRevertedX)
			&& ReadBool(node, "RevertedY", out.IsRevertedY);
	}

	bool ParseAnimations(rapidxml::xml_node<>* root, TextureData& data)
	{
		for (rapidxml::xml_node<>* node = root->first_node("Animation"); node; node = node->next_sibling("Animation"))
		{
			std::string name;
			AnimationData animation;
			if (!ReadName(node, name) || !ParseAnimation(node, animation))
			{
				return false;
			}

			if (!data.AnimationsData.emplace(name, animation).second)
			{
				Logger::Error("TextureMgr: duplicate animation name '" + name + "'");
				return false;
			}


		}
		return true;
	}

	bool ParseTiles(rapidxml::xml_node<>* root, TextureData& data)
	{
		for (rapidxml::xml_node<>* node = root->first_node("Tile"); node; node = node->next_sibling("Tile"))
		{
			std::string name;
			StaticTileData tile;
			if (!ReadName(node, name) || !ParseTile(node, tile))
			{
				return false;
			}

			if (!data.StaticTilesData.emplace(name, tile).second)
			{
				Logger::Error("TextureMgr: duplicate tile name '" + name + "'");
				return false;
			}
		}
		return true;
	}

	bool LoadMetadata(const std::filesystem::path& path, TextureData& data)
	{
		try
		{
			rapidxml::file<> xmlFile(path.string().c_str());
			rapidxml::xml_document<> document;
			document.parse<0>(xmlFile.data());

			rapidxml::xml_node<>* root = document.first_node();
			if (!root)
			{
				Logger::Error("TextureMgr: empty metadata file " + path.string());
				return false;
			}

			if (std::strcmp(root->name(), "Animations") == 0)
			{
				return ParseAnimations(root, data);
			}
			if (std::strcmp(root->name(), "Backgrounds") == 0)
			{
				return ParseTiles(root, data);
			}

			Logger::Error(std::string("TextureMgr: unknown root node <") + root->name() + "> in " + path.string());
			return false;
		}
		catch (const std::exception& e)
		{
			Logger::Error("TextureMgr: cannot read metadata " + path.string() + " (" + e.what() + ")");
			return false;
		}
	}

	constexpr unsigned int DefaultTextureSize = 64;
	constexpr unsigned int DefaultCellSize = 16;

	bool CreateDefaultTexture(TextureData& data)
	{
		sf::Image image(sf::Vector2u(DefaultTextureSize, DefaultTextureSize), sf::Color::Black);

		for (unsigned int y = 0; y < DefaultTextureSize; ++y)
		{
			for (unsigned int x = 0; x < DefaultTextureSize; ++x)
			{
				if (((x / DefaultCellSize) + (y / DefaultCellSize)) % 2 == 0)
				{
					image.setPixel({ x, y }, sf::Color::Magenta);
				}
			}
		}

		if (!data.Texture.loadFromImage(image))
		{
			Logger::Error("TextureMgr: failed to create the default texture");
			return false;
		}

		AnimationData animation;
		animation.SizeX = DefaultTextureSize;
		animation.SizeY = DefaultTextureSize;
		animation.AnimationSpriteCount = 1;
		animation.SpriteOnLine = 1;
		data.AnimationsData.emplace("Default", animation);

		StaticTileData tile;
		tile.SizeX = DefaultTextureSize;
		tile.SizeY = DefaultTextureSize;
		data.StaticTilesData.emplace("Default", tile);

		return true;
	}

}

TextureData::TextureData() {}
TextureData::~TextureData() {}

void TextureMgr::Init()
{
	CreateDefaultTexture(DefaultTexture);
	gData.DebugMgr->RegisterDebugableWindow("Textures", this);
}

void TextureMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("Textures");
	Textures.clear();
}

bool TextureMgr::LoadTexture(const std::filesystem::path& path)
{
	const std::string name = path.stem().string();
	if (Textures.find(name) != Textures.end())
	{
		Logger::Warning("TextureMgr: texture '" + name + "' is already loaded");
		return true;
	}

	std::filesystem::path metadataPath = path;
	metadataPath.replace_extension(".xml");

	if (!CheckFileExists(path) || !CheckFileExists(metadataPath))
	{
		return false;
	}

	auto [it, inserted] = Textures.try_emplace(name);
	TextureData& data = it->second;

	if (!LoadTextureFile(path, data) || !LoadMetadata(metadataPath, data))
	{
		Textures.erase(it);   
		return false;
	}

	Logger::Info("TextureMgr: loaded '" + name + "' ("
		+ std::to_string(data.AnimationsData.size()) + " animations, "
		+ std::to_string(data.StaticTilesData.size()) + " tiles)");
	return true;
}

const TextureData& TextureMgr::GetTextureData(const std::string& name) const
{
	auto it = Textures.find(name);
	if (it == Textures.end())
	{
		Logger::Warning("TextureMgr: unknown texture '" + name + "', returning default");
		return DefaultTexture;
	}
	return it->second;
}

void TextureMgr::DrawDebug()
{
#ifdef _USE_IMGUI
	

#endif
}