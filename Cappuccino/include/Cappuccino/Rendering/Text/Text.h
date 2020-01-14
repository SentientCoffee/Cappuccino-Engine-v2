#pragma once

#include "Cappuccino/Rendering/Text/FontManager.h"
#include "Cappuccino/Objects/Transform.h"
#include "Cappuccino/Rendering/VertexArray.h"

namespace Capp {

	class Text {
	public:

		Text(const std::string& text, Font* font);
		~Text();

		const std::string& getText() const;
		void setText(const std::string& text);
		
		Font* getFont() const;
		void setFont(Font* font);

		const glm::vec4& getTextColour() const;
		void setTextColour(const glm::vec4& colour);
		void setTextColour(const glm::vec3& colour);
		void setTextColour(float r, float g, float b, float a = 1.0f);

		void setTextPosition(const glm::vec2& position);
		void setTextPosition(float x, float y);
		void setTextScale(float scale);

		const Transform& getTransform() const;

		VertexArray* getVAO() const;

	private:

		std::string _text;
		Font* _font;
		glm::vec4 _textColour;
		Transform _transform;

		VertexArray* _vao;
		
	};
	
}
