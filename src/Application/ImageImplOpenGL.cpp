#ifdef MAGMA_OPENGL_ENABLED
#include "Magma/Application/Image.h"
#include "glad/glad.h"
#include "stb_image.h"
#include "Magma/Core/Logging.h"

namespace mg 
{
    namespace Utils
    {
        static unsigned int GetGLFromFormat(ImageFormat fmt)
        {
            switch (fmt)
            {
                case ImageFormat::RGBA:     return GL_RGBA;
                case ImageFormat::RGBA32F:  return GL_RGBA32F;
            
                case ImageFormat::None:
                    MAGMA_CORE_WARN("ImageFormat::None");
                    return 0;
            }
        }
    
        static unsigned int GetGLFromFilter(ImageFilter filter)
        {
            switch (filter)
            {
            case ImageFilter::Nearest:      return GL_NEAREST;
            case ImageFilter::Linear:       return GL_LINEAR;
            //case ImageFilter::Anisotropic:  return GL_LINEAR;
            //case ImageFilter::Mipmap:       return GL_LINEAR_MIPMAP_LINEAR;
            case ImageFilter::None:
                MAGMA_CORE_WARN("ImageFilter::None - using default Filter(GL_LINEAR)");
                return GL_LINEAR;
            }
        }
    }

    Image::Image(std::string path, ImageFilter filter)
        : m_Filepath(std::move(path)), m_Filter(filter)
    {
        int width, height, channels;
        stbi_uc* data = nullptr;
        if (stbi_is_hdr(m_Filepath.c_str()))
        {
            data = (stbi_uc*)stbi_loadf(m_Filepath.c_str(), &width, &height, &channels, 0);
            m_Format = ImageFormat::RGBA32F;
        }
        else
        {
            data = stbi_load(m_Filepath.c_str(), &width, &height, &channels, 0);
            m_Format = ImageFormat::RGBA;
        }

        if (data)
        {
            m_Width = width;
            m_Height = height;
            AllocateTexture();
            SetData(data);
            stbi_image_free(data);
        }
        else
        {
            MAGMA_CORE_ERROR("Error loading image data from file '{0}'", m_Filepath);
        }
    }

    Image::Image(uint32_t width, uint32_t height, ImageFormat format, const void* data, ImageFilter filter)
        : m_Width(width), m_Height(height), m_Format(format), m_Filter(filter)
    {
        AllocateTexture();
        if (data)
            SetData(data);
    }

    Image::~Image()
    {
        ReleaseTexture();
    }

    void Image::SetData(const void* data)
    {
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, Utils::GetGLFromFormat(m_Format), m_Width, m_Height, 0, Utils::GetGLFromFormat(m_Format), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Image::Resize(uint32_t width, uint32_t height)
    {
        if (m_TextureID && m_Width == width && m_Height == height)
            return;

        m_Width = width;
        m_Height = height;

        ReleaseTexture();
        AllocateTexture();
    }

    void Image::AllocateTexture()
    {
        glGenTextures(1, &m_TextureID);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Utils::GetGLFromFilter(m_Filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Utils::GetGLFromFilter(m_Filter));
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Image::ReleaseTexture()
    {
        if (m_TextureID != 0) {
            glDeleteTextures(1, &m_TextureID);
            m_TextureID = 0;
        }
    }


}
#endif
