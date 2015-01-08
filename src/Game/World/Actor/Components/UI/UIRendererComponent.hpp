#pragma once

#include <memory>
#include <Game/World/Actor/Actor.hpp>
#include <Game/World/Actor/Components/IComponent.hpp>
#include <c++/bits/unique_ptr.h>

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace game {
    class UIRendererComponent : public IComponent {
        private:
        ActorWPtr _actorWPtr;
        GLuint _program;

    public:
        UIRendererComponent(ActorWPtr actorWPtr, GLuint program);
        ~UIRendererComponent();
        void Render();

        virtual ComponentType VGetComponentType() { return ComponentType::RENDER_COMPONENT; }

    protected:
        static FT_Library library;
        static FT_Face face;

        static FT_GlyphSlot slot;
        static FT_UInt glyph_index;
        static FT_Vector pen;                    /* untransformed origin  */
        static FT_Error error;

        struct Glyph
        {
            // All of the float values are ratios compared to the given size
            GLuint texture;
            GLfloat width;
            GLfloat height;
            GLfloat xAdvance;
            GLfloat yBearing;
            GLfloat xBearing;
        };
        static Glyph texGlyphs[127];

        static bool initiated;

        unsigned int i;
        unsigned int j;
        unsigned int k;

        static void DrawGlyph(FT_Bitmap* bitmap, GLubyte* image, unsigned int width, unsigned int height);

    public:

        void DrawString(std::string text, GLfloat x, GLfloat y, GLfloat size);
    };
}