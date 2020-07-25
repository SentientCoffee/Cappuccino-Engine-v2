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
		Framebuffer(uint32_t width, uint32_t height);
		~Framebuffer();

		static Ref<Framebuffer> create(uint32_t width, uint32_t height) {
			return Memory::createRef<Framebuffer>(width, height);
		}

		uint32_t getWidth() const { return _width; }
		uint32_t getHeight() const { return _height; }
		
		bool validateFramebuffer();
		bool isValid() const { return _isValid; }

		void setName(const std::string& name) { _name = name; }
		const std::string& getName() const { return _name; }
		
		void resize(uint32_t width, uint32_t height);

		void addAttachment(AttachmentTarget target, const Attachment& attachment);
		Ref<Texture2D> getTextureAttachment(AttachmentTarget target);
		Ref<TextureCubemap> getCubemapAttachment(AttachmentTarget target);

		void bind(FramebufferBinding binding = FramebufferBinding::DrawOnly);
		void unbind();

		static void blitBufferData(const glm::ivec4& sourceBounds, const glm::ivec4& destinationBounds, ClearFlags flags = ClearFlags::All, MagFilter magFilter = MagFilter::Linear);
		
	private:
		
		uint32_t _id = 0;
		std::string _name;
		
		uint32_t _width = 0, _height = 0;
		bool _isValid = false;
		FramebufferBinding _binding = FramebufferBinding::None;
		
		AttachmentMap _attachments;
		DrawTargetList _drawTargets;
		
	};
	
}
