#include <string>

#ifdef MAGMA_VULKAN_ENABLED
#include <vulkan/vulkan.h>
#elif MAGMA_OPENGL_ENABLED
#include <glad/glad.h>
#endif

#include <stb_image.h>
#include "Magma/API/VulkanRenderAPI.h"
#include "Magma/Core/VulkanImpl.h"

#ifndef MAGMA_IMAGE_H
#define MAGMA_IMAGE_H

namespace mg {

    enum class ImageFormat
    {
        None = 0,
        RGBA,
        RGBA32F  
    };

    class Image
    {
    public:
        Image(std::string path);
        Image(uint32_t width, uint32_t height, ImageFormat format, const void* data = nullptr);
        ~Image();

        void SetData(const void* data);

        #ifdef MAGMA_VULKAN_ENABLED
        VkDescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }
        #elif MAGMA_OPENGL_ENABLED
        GLuint GetTextureID() const { return m_TextureID; }
        #endif

        // Cross API function
        void* GetImage()
        {
            #ifdef MAGMA_VULKAN_ENABLED
            return (void*)GetDescriptorSet();
            #elif MAGMA_OPENGL_ENABLED
            return (void*)GetTextureID();
            #endif
        }

        void Resize(uint32_t width, uint32_t height);

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
    private:
        void AllocateMemory(uint64_t size);
        void Release();
    private:

        #ifdef MAGMA_VULKAN_ENABLED

        VulkanRenderAPI* renderer;

        VkImage m_Image = nullptr;
        VkImageView m_ImageView = nullptr;
        VkDeviceMemory m_Memory = nullptr;
        VkSampler m_Sampler = nullptr;
        VkBuffer m_StagingBuffer = nullptr;
        VkDeviceMemory m_StagingBufferMemory = nullptr;
        size_t m_AlignedSize = 0;
        VkDescriptorSet m_DescriptorSet = nullptr;
        #elif MAGMA_OPENGL_ENABLED
        // OpenGL Specific
        GLuint m_TextureID = 0;

        void AllocateTexture();
        void ReleaseTexture();
        #endif

        
        uint32_t m_Width = 0, m_Height = 0;
        ImageFormat m_Format = ImageFormat::None;
        std::string m_Filepath;
    };

}
#endif
