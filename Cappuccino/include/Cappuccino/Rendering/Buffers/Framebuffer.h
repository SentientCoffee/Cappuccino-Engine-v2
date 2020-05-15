#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Buffers/FramebufferAttachment.h"
#include "Cappuccino/Rendering/Buffers/FramebufferParams.h"
#include "Cappuccino/Rendering/RenderParams.h"

namespace Capp {
	
	class Framebuffer {

		using AttachmentMap = std::unordered_map<AttachmentTarget, Attachment>;
		using DrawTargetList = std::vector<AttachmentTarget>;
		
	public:
		
		Framebuffer() = default;
		Framebuffer(unsigned width, unsigned height);
		~Framebuffer();

		static Ref<Framebuffer> create(unsigned width, unsigned height) {
			return Memory::createRef<Framebuffer>(width, height);
		}

		unsigned getWidth() const { return _width; }
		unsigned getHeight() const { return _height; }
		
		bool validateFramebuffer();
		bool isValid() const { return _isValid; }

		void setName(const std::string& name) { _name = name; }
		const std::string& getName() const { return _name; }
		
		void resize(unsigned width, unsigned height);

		void addAttachment(AttachmentTarget target, const Attachment& attachment);
		Ref<Texture2D> getTextureAttachment(AttachmentTarget target);
		Ref<TextureCubemap> getCubemapAttachment(AttachmentTarget target);

		void bind(FramebufferBinding binding = FramebufferBinding::DrawOnly);
		void unbind();

		static void blitBufferData(const glm::ivec4& sourceBounds, const glm::ivec4& destinationBounds, ClearFlags flags = ClearFlags::All, MagFilter magFilter = MagFilter::Linear);
		
	private:
		
		unsigned _id = 0;
		std::string _name;
		
		unsigned _width = 0, _height = 0;
		bool _isValid = false;
		FramebufferBinding _binding = FramebufferBinding::None;
		
		AttachmentMap _attachments;
		DrawTargetList _drawTargets;
		
	};
	
}
