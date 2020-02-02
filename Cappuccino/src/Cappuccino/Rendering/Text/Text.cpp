#include "CappPCH.h"
#include "Cappuccino/Rendering/Text/Text.h"

using namespace Capp;

Text::Text(const std::string& text, Font* font) :
	_text(text), _font(font), _textColour(1.0f) {

	std::vector<float> vertices = {
		0.0f, 0.0f,   0.0f, 0.0f,
		0.0f, 0.0f,   0.0f, 0.0f,
		0.0f, 0.0f,   0.0f, 0.0f,
		
		0.0f, 0.0f,   0.0f, 0.0f,
		0.0f, 0.0f,   0.0f, 0.0f,
		0.0f, 0.0f,   0.0f, 0.0f
	};

	std::vector<unsigned> indices = {
		0, 1, 2,
		0, 2, 3
	};
	
	_vao = new VertexArray;
	const auto vbo = new VertexBuffer(vertices, BufferUsage::DynamicDraw);
	const auto ibo = new IndexBuffer(indices, BufferUsage::DynamicDraw);

	BufferLayout layout = {
		{ ShaderDataType::Vec2, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
	};

	vbo->setLayout(layout);
	_vao->addVertexBuffer(vbo);
	_vao->setIndexBuffer(ibo);
}

Text::~Text() {
	delete _vao;
}

const std::string& Text::getText() const { return _text; }
void Text::setText(const std::string& text) { _text = text; }

Font* Text::getFont() const { return _font; }
void Text::setFont(Font* font) { _font = font; }

const glm::vec4& Text::getTextColour() const { return _textColour; }
void Text::setTextColour(const glm::vec4& colour) { _textColour = colour; }
void Text::setTextColour(const glm::vec3& colour) { setTextColour(colour.r, colour.g, colour.b); }
void Text::setTextColour(const float r, const float g, const float b, const float a) { setTextColour({ r, g ,b, a }); }

Text& Text::setTextPosition(const glm::vec2& position) {
	_transform.setPosition(position.x, position.y, 0.0f);
	return *this;
}
Text& Text::setTextPosition(const float x, const float y) { return setTextPosition({ x, y }); }

Text& Text::setTextScale(const float scale) {
	_transform.setScale(scale, scale, 1.0f);
	return *this;
}

const Transform& Text::getTransform() const { return _transform; }
VertexArray* Text::getVAO() const { return _vao; }
