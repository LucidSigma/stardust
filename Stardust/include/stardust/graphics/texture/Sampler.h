#pragma once
#ifndef STARDUST_SAMPLER_H
#define STARDUST_SAMPLER_H

#include <glad/glad.h>

#include "stardust/data/Types.h"
#include "stardust/graphics/Colour.h"

namespace stardust
{
	struct Sampler
	{
		enum class Wrap
			: GLint
		{
			Repeat = GL_REPEAT,
			MirroredRepeat = GL_MIRRORED_REPEAT,
			ClampToEdge = GL_CLAMP_TO_EDGE,
			MirroredClampToEdge = GL_MIRROR_CLAMP_TO_EDGE,
			ClampToBorder = GL_CLAMP_TO_BORDER,
		};

		enum class MinFilter
			: GLint
		{
			Nearest = GL_NEAREST,
			Linear = GL_LINEAR,
			NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
			NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
			LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
			LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
		};

		enum class MagFilter
			: GLint
		{
			Nearest = GL_NEAREST,
			Linear = GL_LINEAR,
		};

		Wrap sWrap = Wrap::Repeat;
		Wrap tWrap = Wrap::Repeat;
		Optional<Colour> borderColour = NullOpt;

		MinFilter minFilter = MinFilter::Nearest;
		MagFilter magFilter = MagFilter::Nearest;

		bool generateMipmaps = false;
		bool enableAnisotropicFiltering = false;
	};
}

#endif