#include "CappPCH.h"
#include "Cappuccino/Rendering/Text/Text.h"

using namespace Capp;

Text::Text(const std::string& text, const Ref<Font>& font) :
	_text(text), _font(font), _textColour(1.0f)
{
	std::array<float, 24> vertices = {
		0.0f, 0.0f,   0.0f, 0.0f,
		0.0f, 0.0f,   0.0f, 0.0f,
		0.0f, 0.0f,   0.0f, 0.0f,
		
		0.0f, 0.0f,   0.0f, 0.0f,
		0.0f, 0.0f,   0.0f, 0.0f,
		0.0f, 0.0f,   0.0f, 0.0f
	};

	std::array<uint32_t, 6> indices = {
		0, 1, 2,
		0, 2, 3
	};
	
	_vao = VertexArray::create();
	const auto vbo = VertexBuffer::create(vertices, BufferUsage::DynamicDraw);
	const auto ibo = IndexBuffer::create(indices, BufferUsage::DynamicDraw);

	BufferLayout layout = {
		{ ShaderDataType::Vec2, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
	};

	vbo->setLayout(layout);
	_vao->addVertexBuffer(vbo);
	_vao->setIndexBuffer(ibo);
}

Text& Text::setColour(const glm::vec4& colour) {
	_textColour = colour;
	return *this;
}

Text& Text::setPosition(const glm::vec2& position) {
	_transform.setPosition(position.x, position.y, 0.0f);
	return *this;
}

Text& Text::setScale(const float scale) {
	_transform.setScale(scale, scale, 1.0f);
	return *this;
}
