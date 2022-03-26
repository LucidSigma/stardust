#pragma once
#ifndef STARDUST_BLENDING_H
#define STARDUST_BLENDING_H

#include <ANGLE/GLES3/gl3.h>

#include "stardust/graphics/colour/Colour.h"
#include "stardust/types/Containers.h"

namespace stardust
{
    namespace graphics
    {
        enum class BlendFactor
            : GLenum
        {
            Zero = GL_ZERO,
            One = GL_ONE,
            SourceColour = GL_SRC_COLOR,
            OneMinusSourceColour = GL_ONE_MINUS_SRC_COLOR,
            DestinationColour = GL_DST_COLOR,
            OneMinusDestinationColour = GL_ONE_MINUS_DST_COLOR,
            SourceAlpha = GL_SRC_ALPHA,
            OneMinusSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
            DestinationAlpha = GL_DST_ALPHA,
            OneMinusDestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
            ConstantColour = GL_CONSTANT_COLOR,
            OneMinusConstantColour = GL_ONE_MINUS_CONSTANT_COLOR,
            ConstantAlpha = GL_CONSTANT_ALPHA,
            OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA,
            SourceAlphaSaturate = GL_SRC_ALPHA_SATURATE,
        };

        enum class BlendOperation
            : GLenum
        {
            Add = GL_FUNC_ADD,
            Subtract = GL_FUNC_SUBTRACT,
            ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT,
            Min = GL_MIN,
            Max = GL_MAX,
        };

        struct BlendMode final
        {
            BlendFactor sourceColourFactor;
            BlendFactor destinationColourFactor;
            BlendOperation colourOperation;

            BlendFactor sourceAlphaFactor;
            BlendFactor destinationAlphaFactor;
            BlendOperation alphaOperation;

            Optional<Colour> colourConstant;
        };

        namespace blend_modes
        {
            constexpr BlendMode Disabled{
                .sourceColourFactor = BlendFactor::One,
                .destinationColourFactor = BlendFactor::Zero,
                .colourOperation = BlendOperation::Add,

                .sourceAlphaFactor = BlendFactor::One,
                .destinationAlphaFactor = BlendFactor::Zero,
                .alphaOperation = BlendOperation::Add,

                .colourConstant = None,
            };

            constexpr BlendMode Alpha{
                .sourceColourFactor = BlendFactor::SourceAlpha,
                .destinationColourFactor = BlendFactor::OneMinusSourceAlpha,
                .colourOperation = BlendOperation::Add,

                .sourceAlphaFactor = BlendFactor::One,
                .destinationAlphaFactor = BlendFactor::OneMinusSourceAlpha,
                .alphaOperation = BlendOperation::Add,

                .colourConstant = None,
            };

            constexpr BlendMode Additive{
                .sourceColourFactor = BlendFactor::SourceAlpha,
                .destinationColourFactor = BlendFactor::One,
                .colourOperation = BlendOperation::Add,

                .sourceAlphaFactor = BlendFactor::Zero,
                .destinationAlphaFactor = BlendFactor::One,
                .alphaOperation = BlendOperation::Add,

                .colourConstant = None,
            };

            constexpr BlendMode ColourModulate{
                .sourceColourFactor = BlendFactor::Zero,
                .destinationColourFactor = BlendFactor::SourceColour,
                .colourOperation = BlendOperation::Add,

                .sourceAlphaFactor = BlendFactor::Zero,
                .destinationAlphaFactor = BlendFactor::One,
                .alphaOperation = BlendOperation::Add,

                .colourConstant = None,
            };
            
            constexpr BlendMode ColourMultiply{
                .sourceColourFactor = BlendFactor::DestinationColour,
                .destinationColourFactor = BlendFactor::OneMinusSourceAlpha,
                .colourOperation = BlendOperation::Add,

                .sourceAlphaFactor = BlendFactor::DestinationAlpha,
                .destinationAlphaFactor = BlendFactor::OneMinusSourceAlpha,
                .alphaOperation = BlendOperation::Add,

                .colourConstant = None,
            };
        }
    }
}

#endif
