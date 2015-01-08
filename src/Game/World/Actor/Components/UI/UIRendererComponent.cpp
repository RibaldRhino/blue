#include <GL/glew.h>
#include <log.hpp>
#include "UIRendererComponent.hpp"
#include <Event/EventManager.hpp>
#include <Event/Game/Window/OnWindowClose.hpp>

namespace game {

    FT_Library UIRendererComponent::library;
    FT_Face UIRendererComponent::face;

    FT_GlyphSlot UIRendererComponent::slot;
    FT_UInt UIRendererComponent::glyph_index;
    FT_Vector UIRendererComponent::pen;            /* untransformed origin  */
    FT_Error UIRendererComponent::error;
    bool UIRendererComponent::initiated = false;
    UIRendererComponent::Glyph UIRendererComponent::texGlyphs[127];

    void UIRendererComponent::Render()
    {
        DrawString("test", 400, 300, 128);
    }

    UIRendererComponent::UIRendererComponent(game::ActorWPtr actorWPtr, GLuint program) {
        _actorWPtr = actorWPtr;
        _program = program;

        struct {
            GLubyte* bitmap;
            GLubyte* resizedBitmap;
            GLubyte width;
            GLubyte height;
            GLubyte xAdvance;
            GLubyte yBearing;
            GLubyte xBearing;
        } glyphs[127];
        GLuint textures[127];
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(127, textures);
        initiated = true;
        unsigned int i;
        unsigned int j;
        unsigned int k;
        unsigned int s;
        unsigned int n;

        // s represents the resolution of the texture
        // n represents the mipmap level of the texture
        s = 128;
        n = 0;
        while (s > 1) {
            s /= 2;
            error = FT_Init_FreeType( &library );
            if (error) {
                LOG(ERROR) << "Error:  Could not initiate FreeType 2" << std::endl;
                event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
            }
            error = FT_New_Face(library, "assets/arial.ttf", 0, &face);
            if (error) {
                LOG(ERROR) << "Error:  Could not create font face from file" << std::endl;
                FT_Done_FreeType(library);
                event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
            }
            error = FT_Set_Pixel_Sizes(face, s, s);
            if (error) {
                LOG(ERROR) << "Error: Could not set pixel size" << std::endl;
                FT_Done_FreeType(library);
                event::EventManager::getInstance().TriggerEvent(std::make_shared<event::OnWindowClose>());
            }
            pen.x = 0;
            pen.y = 0;
            slot = face->glyph;

            for (i = 0; i < 127; i++) {
                FT_Set_Transform(face, NULL, &pen);

                error = FT_Load_Char(face, i, FT_LOAD_RENDER);
                if (error) {
                    continue;
                }

                if (i != 32) {
                    glyphs[i].width = slot->bitmap.width;
                    glyphs[i].height = slot->bitmap.rows;
                    glyphs[i].bitmap = new GLubyte[glyphs[i].width * glyphs[i].height];
                    glyphs[i].xAdvance = slot->metrics.horiAdvance / 64;
                    glyphs[i].yBearing = slot->metrics.horiBearingY / 64;
                    glyphs[i].xBearing = slot->metrics.horiBearingX / 64;
                    DrawGlyph(&slot->bitmap, glyphs[i].bitmap, glyphs[i].width, glyphs[i].height);
                }
                else {
                    glyphs[i].width = 0;
                    glyphs[i].height = 0;
                    glyphs[i].bitmap = new GLubyte[0];
                    glyphs[i].xAdvance = slot->metrics.horiAdvance / 64;
                    glyphs[i].yBearing = slot->metrics.horiBearingY / 64;
                    glyphs[i].xBearing = slot->metrics.horiBearingX / 64;
                }
            }
            static GLboolean texInited = false;
            for (i = 0; i < 127; i++) {
                glBindTexture(GL_TEXTURE_2D, textures[i]);
                glyphs[i].resizedBitmap = new GLubyte[s * s];
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

                /*gluScaleImage(GL_ALPHA,
                              glyphs[i].width,
                              glyphs[i].height,
                              GL_UNSIGNED_BYTE,
                              glyphs[i].bitmap,
                              s,
                              s,
                              GL_UNSIGNED_BYTE,
                              glyphs[i].resizedBitmap);*/

                for (unsigned int y = 0; y < s; y++) {
                    for (unsigned int x = 0; x < s; x++) {
                        if ((y < glyphs[i].height) && (x < glyphs[i].width)) {
                            glyphs[i].resizedBitmap[(y * s) + x] = glyphs[i].bitmap[(y * glyphs[i].width) + x];
                        }
                        else {
                            glyphs[i].resizedBitmap[(y * s) + x] = 0;
                        }
                    }
                }

                glTexImage2D(GL_TEXTURE_2D, n, GL_ALPHA, s, s, 0, GL_ALPHA, GL_UNSIGNED_BYTE, glyphs[i].resizedBitmap);
                if (!texInited) {
                    texGlyphs[i].texture = textures[i];
                    texGlyphs[i].width = (GLfloat)glyphs[i].width / (GLfloat)s;
                    texGlyphs[i].height = (GLfloat)glyphs[i].height / (GLfloat)s;
                    texGlyphs[i].xAdvance = (GLfloat)glyphs[i].xAdvance / (GLfloat)s;
                    texGlyphs[i].xBearing = (GLfloat)glyphs[i].xBearing / (GLfloat)s;
                    texGlyphs[i].yBearing = (GLfloat)glyphs[i].yBearing / (GLfloat)s;
                }
                glBindTexture(GL_TEXTURE_2D, 0);
                delete[] glyphs[i].bitmap;
                delete[] glyphs[i].resizedBitmap;
            }
            texInited = true;
            FT_Done_FreeType(library);
            n++;
        }
    }

    UIRendererComponent::~UIRendererComponent() {
        for (unsigned int i = 0; i < 127; i++) {
            glDeleteTextures(1, &texGlyphs[i].texture);
        }
    }

    void UIRendererComponent::DrawString(std::string text, GLfloat x, GLfloat y, GLfloat size)
    {
        static GLfloat xLocation, yLocation;
        static GLint winWidth = 800, winHeight = 600;
        glPushAttrib(GL_COLOR_BUFFER_BIT | GL_LIGHTING_BIT | GL_ENABLE_BIT);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, winWidth, 0, winHeight, 1.0f, 10.0f);
        gluLookAt(0, 0, 5.0f, 0, 0, 0.0f, 0.0f, 1.0f, 0.0f);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        xLocation = x;
        yLocation = winHeight - y;

        for (j = 0; j < text.length(); j++) {
            glPushMatrix();
            if (text[j] == '\n') {
                yLocation -= size;
                xLocation = x;
                continue;
            }
            glBindTexture(GL_TEXTURE_2D, texGlyphs[text[j]].texture);
            glTranslatef(xLocation + (texGlyphs[text[j]].xBearing * size), yLocation - ((texGlyphs[text[j]].height - texGlyphs[text[j]].yBearing)* size), 0.0f);
            glScalef(size, size, 1.0f);
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(0.0f, 1.0f);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(1.0f, 0.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(1.0f, 1.0f);
            glEnd();
            xLocation += (texGlyphs[text[j]].xAdvance * size);
            glBindTexture(GL_TEXTURE_2D, 0);
            glPopMatrix();
        }
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glPopAttrib();
    }

    void UIRendererComponent::DrawGlyph(FT_Bitmap* bitmap, GLubyte* image, unsigned int width, unsigned int height)
    {
        unsigned int i;
        unsigned int j;
        unsigned int k;
        for (j = 0; j < height; j++) {
            for (k = 0; k < width; k++) {
                if (k > bitmap->width) {
                    //continue;
                }
                image[(j * width) + k] = bitmap->buffer[((height - 1 - j) * width) + k];
            }
        }
    }
}
