#pragma once

#include "Cappuccino/Objects/Transform.h"
#include "Cappuccino/Rendering/VertexArray.h"
#include "Cappuccino/Resource/FontLibrary.h"

namespace Capp {

	class Text {
	public:

		Text(const std::string& text, const Ref<Font>& font);
		~Text() = default;

		static Ref<Text> create(const std::string& text, const Ref<Font>& font) {
			return Memory::createRef<Text>(text, font);
		}

		Text& setColour(const glm::vec4& colour);
		Text& setPosition(const glm::vec2& position);
		Text& setScale(float scale);

		const std::string& getText() const { return _text; }
		void setText(const std::string& text) { _text = text; }
		
		Ref<Font> getFont() const { return _font; }
		void setFont(const Ref<Font>& font) { _font = font; }

		const glm::vec4& getTextColour() const { return _textColour; }
		
		Text& setColour(const glm::vec3& colour) {
			return setColour({ colour.r, colour.g, colour.b, 1.0f });
		}
		Text& setColour(const float r, const float g, const float b, const float a = 1.0f) {
			return setColour({ r, g ,b, a });
		}

		Text& setPosition(const float x, const float y) {
			return setPosition({ x, y });
		}

		const Transform& getTransform() const { return _transform; }

		Ref<VertexArray> getVAO() const { return _vao; }

	private:

		std::string _text;
		Ref<Font> _font;

		glm::vec4 _textColour;
		Transform _transform;
		Ref<VertexArray> _vao;
		
	};
	
}
