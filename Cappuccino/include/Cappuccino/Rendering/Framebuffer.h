#pragma once

#include "Cappuccino/Rendering/FramebufferAttachment.h"
#include "Cappuccino/Rendering/FramebufferParams.h"
#include "Cappuccino/Rendering/RenderParams.h"

namespace Capp {
	
	class Framebuffer {
	public:

		Framebuffer(unsigned width, unsigned height);
		~Framebuffer();

		unsigned getWidth() const;
		unsigned getHeight() const;
		bool isValid() const;

		void setName(const std::string& name);
		const std::string& getName() const;
		
		void resize(unsigned width, unsigned height);

		void addAttachment(AttachmentTarget target, const Attachment& attachment);
		Texture2D* getAttachment(AttachmentTarget target);

		void bind(FramebufferBinding binding = FramebufferBinding::DrawOnly);
		void unbind();

		static void blit(const glm::ivec4& sourceBounds, const glm::ivec4& destinationBounds, ClearFlags flags = ClearFlags::All, MagFilter magFilter = MagFilter::Linear);
		
	private:

		bool validateFramebuffer();
		
		unsigned _id = 0;
		std::string _name;
		
		unsigned _width = 0, _height = 0;
		bool _isValid = false;
		FramebufferBinding _binding = FramebufferBinding::None;
		
		std::unordered_map<AttachmentTarget, Attachment> _attachments;
		
	};
	
}
