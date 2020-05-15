#include "CappPCH.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

using namespace Capp;

TextureParams::TextureParams(const WrapMode wrapMode, const MinFilter minFilter, const MagFilter magFilter, const Anisotropy anisotropyLevel, const glm::vec4& borderColour) :
	wrapS(wrapMode), wrapT(wrapMode), wrapR(wrapMode),
	minFilter(minFilter), magFilter(magFilter),
	anisotropyLevel(anisotropyLevel), borderColour(borderColour) {}