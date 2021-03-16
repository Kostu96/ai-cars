#include "imgui_sfml.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui.h>

namespace im_sf {

static bool s_windowHasFocus = false;
static bool s_mouseMoved = false;
static bool s_mousePressed[3] = { false, false, false };
static sf::Texture* s_fontTexture = nullptr;
static sf::RenderWindow* s_window = nullptr;

sf::Cursor* s_mouseCursors[ImGuiMouseCursor_COUNT];
bool s_mouseCursorLoaded[ImGuiMouseCursor_COUNT];

ImTextureID convertGLTextureHandleToImTextureID(GLuint glTextureHandle) {
    ImTextureID textureID = nullptr;
    std::memcpy(&textureID, &glTextureHandle, sizeof(GLuint));
    return textureID;
}

GLuint convertImTextureIDToGLTextureHandle(ImTextureID textureID) {
    GLuint glTextureHandle;
    std::memcpy(&glTextureHandle, &textureID, sizeof(GLuint));
    return glTextureHandle;
}

void loadMouseCursor(ImGuiMouseCursor imguiCursorType, sf::Cursor::Type sfmlCursorType) {
    s_mouseCursors[imguiCursorType] = new sf::Cursor();
    s_mouseCursorLoaded[imguiCursorType] =
        s_mouseCursors[imguiCursorType]->loadFromSystem(sfmlCursorType);
}

void updateMouseCursor(sf::Window& window) {
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0) {
        ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
        if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None) {
            window.setMouseCursorVisible(false);
        }
        else {
            window.setMouseCursorVisible(true);

            sf::Cursor& c = s_mouseCursorLoaded[cursor] ? *s_mouseCursors[cursor] :
                *s_mouseCursors[ImGuiMouseCursor_Arrow];
            window.setMouseCursor(c);
        }
    }
}

void renderDrawLists(ImDrawData* draw_data) {
    ImGui::GetDrawData();
    if (draw_data->CmdListsCount == 0) {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();
    assert(io.Fonts->TexID != (ImTextureID)NULL); // You forgot to create and set font texture

    // scale stuff (needed for proper handling of window resize)
    int fb_width = static_cast<int>(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = static_cast<int>(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0) {
        return;
    }
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    for (int n = 0; n < draw_data->CmdListsCount; ++n) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();

        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert),
            (void*)(vtx_buffer + offsetof(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert),
            (void*)(vtx_buffer + offsetof(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert),
            (void*)(vtx_buffer + offsetof(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); ++cmd_i) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else {
                GLuint textureHandle = convertImTextureIDToGLTextureHandle(pcmd->TextureId);
                glBindTexture(GL_TEXTURE_2D, textureHandle);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w),
                    (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                    (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT,
                    idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    glPopAttrib();
}

void init(sf::RenderWindow& window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "imgui_impl_sfml";

    io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
    io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
    io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
    io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
    io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
    io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
    io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
    io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
    io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
    io.KeyMap[ImGuiKey_Insert] = sf::Keyboard::Insert;
    io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
    io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
    io.KeyMap[ImGuiKey_Space] = sf::Keyboard::Space;
    io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
    io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
    io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
    io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
    io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
    io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
    io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
    io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;

    auto displaySize = window.getSize();
    io.DisplaySize = ImVec2(displaySize.x, displaySize.y);

    // load mouse cursors
    for (int i = 0; i < ImGuiMouseCursor_COUNT; ++i) {
        s_mouseCursorLoaded[i] = false;
    }
    loadMouseCursor(ImGuiMouseCursor_Arrow, sf::Cursor::Arrow);
    loadMouseCursor(ImGuiMouseCursor_TextInput, sf::Cursor::Text);
    loadMouseCursor(ImGuiMouseCursor_ResizeAll, sf::Cursor::SizeAll);
    loadMouseCursor(ImGuiMouseCursor_ResizeNS, sf::Cursor::SizeVertical);
    loadMouseCursor(ImGuiMouseCursor_ResizeEW, sf::Cursor::SizeHorizontal);
    loadMouseCursor(ImGuiMouseCursor_ResizeNESW, sf::Cursor::SizeBottomLeftTopRight);
    loadMouseCursor(ImGuiMouseCursor_ResizeNWSE, sf::Cursor::SizeTopLeftBottomRight);
    loadMouseCursor(ImGuiMouseCursor_Hand, sf::Cursor::Hand);

    // Default Font stuff
    if (s_fontTexture) {
        delete s_fontTexture;
    }
    s_fontTexture = new sf::Texture;
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    s_fontTexture->create(width, height);
    s_fontTexture->update(pixels);
    io.Fonts->TexID = convertGLTextureHandleToImTextureID(s_fontTexture->getNativeHandle());

    s_windowHasFocus = window.hasFocus();

    window.resetGLStates();

    s_window = &window;
}

void processEvents(const sf::Event& e)
{
    if (s_windowHasFocus) {
        ImGuiIO& io = ImGui::GetIO();

        switch (e.type) {
        case sf::Event::MouseMoved:
            s_mouseMoved = true;
            break;
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseButtonReleased: {
            int button = e.mouseButton.button;
            if (e.type == sf::Event::MouseButtonPressed && button >= 0 && button < 3)
                s_mousePressed[e.mouseButton.button] = true;
        } break;
        case sf::Event::MouseWheelScrolled:
            if (e.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel ||
                (e.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel && io.KeyShift)) {
                io.MouseWheel += e.mouseWheelScroll.delta;
            }
            else if (e.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
                io.MouseWheelH += e.mouseWheelScroll.delta;
            }
            break;
        case sf::Event::KeyPressed:
        case sf::Event::KeyReleased:
            io.KeysDown[e.key.code] = (e.type == sf::Event::KeyPressed);
            break;
        case sf::Event::TextEntered:
            if (e.text.unicode < ' ' || e.text.unicode == 127) {
                break; // Don't handle the event for unprintable characters
            }
            io.AddInputCharacter(e.text.unicode);
            break;
        }
    }

    switch (e.type) {
    case sf::Event::LostFocus:
        s_windowHasFocus = false;
        break;
    case sf::Event::GainedFocus:
        s_windowHasFocus = true;
        break;
    }
}

void update(const sf::Time& dt) {
    updateMouseCursor(*s_window);

    ImGuiIO& io = ImGui::GetIO();

    auto displaySize = s_window->getSize();
    io.DisplaySize = ImVec2(displaySize.x, displaySize.y);

    io.DeltaTime = dt.asSeconds();
    auto mousePos = sf::Mouse::getPosition(*s_window);

    if (s_windowHasFocus) {
        if (io.WantSetMousePos) {
            sf::Vector2i newMousePos(static_cast<int>(io.MousePos.x),
                static_cast<int>(io.MousePos.y));
            sf::Mouse::setPosition(newMousePos);
        }
        else {
            io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        }
        for (unsigned int i = 0; i < 3; i++) {
            io.MouseDown[i] = s_mousePressed[i] || sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
            s_mousePressed[i] = false;
        }
    }

    // Update Ctrl, Shift, Alt, Super state
    io.KeyCtrl = io.KeysDown[sf::Keyboard::LControl] || io.KeysDown[sf::Keyboard::RControl];
    io.KeyAlt = io.KeysDown[sf::Keyboard::LAlt] || io.KeysDown[sf::Keyboard::RAlt];
    io.KeyShift = io.KeysDown[sf::Keyboard::LShift] || io.KeysDown[sf::Keyboard::RShift];
    io.KeySuper = io.KeysDown[sf::Keyboard::LSystem] || io.KeysDown[sf::Keyboard::RSystem];

    assert(io.Fonts->Fonts.Size > 0); // You forgot to create and set up font atlas (see createFontTexture)

    ImGui::NewFrame();
}

void render() {
    ImGui::Render();
    renderDrawLists(ImGui::GetDrawData());
}

void shutdown() {
    ImGui::GetIO().Fonts->TexID = nullptr;

    if (s_fontTexture) {
        delete s_fontTexture;
        s_fontTexture = nullptr;
    }

    for (unsigned int i = 0; i < ImGuiMouseCursor_COUNT; ++i) {
        if (s_mouseCursorLoaded[i]) {
            delete s_mouseCursors[i];
            s_mouseCursors[i] = nullptr;
            s_mouseCursorLoaded[i] = false;
        }
    }

    ImGui::DestroyContext();
}

}
