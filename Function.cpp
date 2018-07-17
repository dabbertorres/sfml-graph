#include "Function.hpp"

#include <cmath>
#include <vector>

#include <SFML/OpenGL.hpp>
#include <SFML/System/Err.hpp>
    
#if defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_MACOS)
    #include <dlfcn.h>
#elif defined(SFML_SYSTEM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#else
    #error "Unsupported OS"
#endif

namespace sfx
{
namespace graph
{

static void(*glBindBuffer)(GLenum, GLuint) = nullptr;
static void*(*glMapBuffer)(GLenum, GLbitfield) = nullptr;
static GLboolean(*glUnmapBuffer)(GLenum) = nullptr;

static void loadGL();

Function::Function(Signature func, sf::Color color, bool line)
    : function{ func },
    color{ color },
    buffer{ line ? sf::PrimitiveType::LineStrip : sf::PrimitiveType::Points, sf::VertexBuffer::Usage::Dynamic }
{
    loadGL();
}

bool Function::operator==(const Function& other) const
{
    return buffer.getNativeHandle() == other.buffer.getNativeHandle();
}

bool Function::operator!=(const Function& other) const
{
    return !(*this == other);
}

void Function::setColor(sf::Color c)
{
    color = c;

    if (buffer.getNativeHandle() == 0)
        return;

    auto handle  = buffer.getNativeHandle();
    size_t count = buffer.getVertexCount();

    glBindBuffer(GL_ARRAY_BUFFER, handle);
    sf::Vertex* data = static_cast<sf::Vertex*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE));

    for (size_t i = 0; i < count; i++)
        data->color = color;

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Function::setLine(bool line)
{
    buffer.setPrimitiveType(line ? sf::PrimitiveType::LineStrip : sf::PrimitiveType::Points);
}

bool Function::update(sf::FloatRect area, sf::Vector2f step)
{
    std::vector<sf::Vertex> vertices;
    vertices.reserve(static_cast<size_t>(std::ceil((area.width + 1) / step.x)));

    for (float x = area.left; x <= area.left + area.width; x += step.x)
        vertices.emplace_back(sf::Vector2f{ x, function(x) }, color);

    if (buffer.getVertexCount() != vertices.size())
    {
        if (!buffer.create(vertices.size()))
            return false;
    }

    return buffer.update(vertices.data());
}


void Function::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(buffer, states);
}

void loadGL()
{
    // we might already be done
    if (glBindBuffer != nullptr)
        return;

#define GET_SYM(handle, sym) reinterpret_cast<decltype(sym)>(GET_SYM_FUNC(handle, #sym))

#if defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_MACOS)
    // symbols *should* already be loaded in our program. just need to find them
    auto handle = dlopen(nullptr, RTLD_LAZY);
#define GET_SYM_FUNC dlsym
#elif defined(SFML_SYSTEM_WINDOWS)
    // symbols *should* already be loaded in our program. just need to find them
    auto handle = GetModuleHandle(nullptr);
#define GET_SYM_FUNC GetProcAddress
#endif

    glBindBuffer = GET_SYM(handle, glBindBuffer);
    glMapBuffer  = GET_SYM(handle, glMapBuffer);
    glUnmapBuffer  = GET_SYM(handle, glUnmapBuffer);

    if (glBindBuffer == nullptr || glMapBuffer == nullptr || glUnmapBuffer == nullptr)
    {
        sf::err() << "sfx::graph: Unable to find OpenGL function symbols\n";
        glBindBuffer  = nullptr;
        glMapBuffer   = nullptr;
        glUnmapBuffer = nullptr;
    }

#undef GET_SYM_FUNC
#undef GET_SYM
}

}
}
