#pragma once
#ifndef BUFFER_USAGE_H
#define BUFFER_USAGE_H

#include <glad/glad.h>

namespace stardust
{
	enum class BufferUsage
		: GLenum
	{
		Static = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW,
		Stream = GL_STREAM_DRAW,
	};
}

#endif