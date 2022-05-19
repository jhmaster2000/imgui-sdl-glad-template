// don't listen to MS complains, we want cross-platform code
#define _CRT_SECURE_NO_DEPRECATE

// C++
//#include <filesystem>
#include <vector>
// SDL
#include <glad/glad.h>
#include <SDL.h>
// ImGui
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include "imgui-style.h"
// Misc
#include "functions.h"

int windowWidth = 1280, windowHeight = 720;

int main(int argc, char* argv[]) {
    std::cout << "[" << currentTime(std::chrono::system_clock::now()) << "] " << "Start\n- - -\n\n";

    // std::vector<std::string> files;
    // auto currentPath = std::filesystem::current_path();
    // //std::cout << currentPath.string();
    // for (const auto &entry : std::filesystem::directory_iterator(currentPath)) {
    //     files.push_back(entry.path().filename().string().data());
    //     //std::cout << entry.path().filename() << std::endl;
    // }

    // initiate SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("[ERROR] %s\n", SDL_GetError());
        return -1;
    }

    SDL_version compiled;
    SDL_VERSION(&compiled);
    std::ostringstream compiledVal;
    compiledVal << "Compiled with "
                << std::to_string(compiled.major)
                << "." << std::to_string(compiled.minor)
                << "." << std::to_string(compiled.patch);
    //std::cout << compiledVal.str() << std::endl;

    SDL_version linked;
    SDL_GetVersion(&linked);
    std::ostringstream linkedVal;
    linkedVal << "Linked with "
              << std::to_string(linked.major)
              << "." << std::to_string(linked.minor)
              << "." << std::to_string(linked.patch);
    //std::cout << linkedVal.str() << std::endl;

    // setup SDL window

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    std::string glsl_version = "";

#ifdef __linux__
    // GL 3.2 Core + GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif _WIN32
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif
    
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    SDL_Window* window = SDL_CreateWindow(
        "ImGui SDL Template",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        window_flags
    );
    // limit to which minimum size user can resize the window
    SDL_SetWindowMinimumSize(window, 500, 300);
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    
    // enable VSync
    SDL_GL_SetSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "[ERROR] Couldn't initialize glad" << std::endl;
    } else{
        std::cout << "[INFO] glad initialized\n";
    }

    std::cout << "[INFO] OpenGL renderer: "
              << glGetString(GL_RENDERER)
              << std::endl;

    // apparently, that shows maximum supported version
    std::cout << "[INFO] OpenGL from glad: "
              << GLVersion.major
              << "."
              << GLVersion.minor
              << std::endl;

    // int sdlOpenGLmajor = 0, sdlOpenGLminor = 0;
    // SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &sdlOpenGLmajor);
    // SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &sdlOpenGLminor);
    // std::cout << "[INFO] OpenGL from SDL: "
    //           << sdlOpenGLmajor
    //           << "."
    //           << sdlOpenGLminor
    //           << std::endl;

    glViewport(0, 0, windowWidth, windowHeight);

    // setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("data/fonts/verdana.ttf", 18.0f, NULL, NULL);

    // setup ImGui style
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    setImGuiStyle();

    // setup platform/renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    bool show_demo_window = false;
    bool show_another_window = false;
    // colors are set in RGBA, but as float
    ImVec4 background = ImVec4(35 / 255.0f, 35 / 255.0f, 35 / 255.0f, 1.0f);

    glClearColor(background.x, background.y, background.z, background.w);
    // --- rendering loop
    bool loop = true;
    while (loop) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // without it you won't have keyboard input and other things
            ImGui_ImplSDL2_ProcessEvent(&event);
            // you might also want to check io.WantCaptureMouse and io.WantCaptureKeyboard
            // before processing events

            switch (event.type) {
                case SDL_QUIT:
                    loop = false;
                    break;

                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            windowWidth = event.window.data1;
                            windowHeight = event.window.data2;
                            // std::cout << "[INFO] Window size: "
                            //           << windowWidth
                            //           << "x"
                            //           << windowHeight
                            //           << std::endl;
                            glViewport(0, 0, windowWidth, windowHeight);
                            break;
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            loop = false;
                            break;
                    }
                    break;
            }
        }

        // start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // standard demo window
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // a window is defined by Begin/End pair
        {
            static int counter = 0;

            int sdl_width = 0, sdl_height = 0, controls_width = 0;
            // get the window size as a base for calculating widgets geometry
            SDL_GetWindowSize(window, &sdl_width, &sdl_height);
            controls_width = sdl_width;
            // make controls widget width to be 1/3 of the main window width
            if ((controls_width /= 3) < 300) controls_width = 300;

            // position the controls widget in the top-right corner with some margin
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
            // here we set the calculated width and also make the height to be
            // be the height of the main window also with some margin
            ImGui::SetNextWindowSize(
                ImVec2(static_cast<float>(controls_width), static_cast<float>(sdl_height - 20)), ImGuiCond_Always
            );
            // create a window and append into it
            ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize);

            ImGui::Dummy(ImVec2(0.0f, 1.0f));
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Time");
            ImGui::Text("%s", currentTime(std::chrono::system_clock::now()).c_str());

            ImGui::Dummy(ImVec2(0.0f, 3.0f));
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
            ImGui::Text("%s", SDL_GetPlatform());
            ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
            ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

            ImGui::Dummy(ImVec2(0.0f, 3.0f));
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Application");
            ImGui::Text("Main window width: %d", sdl_width);
            ImGui::Text("Main window height: %d", sdl_height);

            ImGui::Dummy(ImVec2(0.0f, 3.0f));
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "SDL");
            ImGui::Text("%s", compiledVal.str().c_str());
            ImGui::Text("%s", linkedVal.str().c_str());

            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
            
            // buttons and most other widgets return true when clicked/edited/activated
            if (ImGui::Button("Counter button")) {
                std::cout << "counter button clicked\n";
                counter++;
                if (counter == 9) ImGui::OpenPopup("Easter egg");
            }
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            if (ImGui::BeginPopupModal("Easter egg", NULL)) {
                ImGui::Text("Ho-ho, you found me!");
                if (ImGui::Button("Buy Ultimate Orb")) ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            ImGui::Dummy(ImVec2(0.0f, 15.0f));
            if (!show_demo_window) {
                if (ImGui::Button("Open standard demo")) show_demo_window = true;
            }

            ImGui::Checkbox("show a custom window", &show_another_window);
            if (show_another_window) {
                ImGui::SetNextWindowSize(
                    ImVec2(400.0f, 350.0f),
                    ImGuiCond_FirstUseEver // after first launch it will use values from imgui.ini
                );
                // the window will have a closing button that will clear the bool variable
                ImGui::Begin("A custom window", &show_another_window);
                
                ImGui::Dummy(ImVec2(0.0f, 1.0f));
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Files in the current folder");
                                
                //ImGui::TextColored(ImVec4(128 / 255.0f, 128 / 255.0f, 128 / 255.0f, 1.0f), "%s", currentPath.string().data());
                ImGui::Dummy(ImVec2(0.0f, 0.5f));

                // static int currentFile = 0;
                // ImVec2 windowSize = ImGui::GetWindowSize();
                // ImGui::PushItemWidth(windowSize.x - 15);
                // ImGui::ListBox(
                //     "",
                //     &currentFile,
                //     vectorGetter,
                //     &files,
                //     static_cast<int>(files.size())
                // );

                ImGui::Dummy(ImVec2(0.0f, 1.0f));
                if (ImGui::Button("Close")) {
                    std::cout << "close button clicked\n";
                    show_another_window = false;
                }

                ImGui::End();
            }

            ImGui::End();
        }
        
        // rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "\n- - -\n" << "[" << currentTime(std::chrono::system_clock::now()) << "] " << "Quit\n";
    
    return 0;
}
