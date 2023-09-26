#include "ModuleEditor.h"

#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"

#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"

//#include "External/Parson/parson.h"
#include <Windows.h>

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app,start_enabled)
{
    FPSvec.reserve(30);
    DTvec.reserve(30);
    MSvec.reserve(30);
}

ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init()
{
    bool ret = true;

    // Setup Dear ImGui context

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

        UpdateAndRenderAdditionalPlatformWindows = true;

    }

    // Setup Dear ImGui style

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.

    ImGuiStyle& style = ImGui::GetStyle();
    
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    // ----------------------------------------------
    
    // JSON parser quick implementation (Parson)

    //JSON_Value* root_value = json_value_init_object();
    //JSON_Object* root_object = json_value_get_object(root_value);

    //json_object_set_string(root_object, "name", "John Doe");
    //json_object_set_number(root_object, "age", 30);
    //json_object_set_boolean(root_object, "isStudent", false);

    //FILE* file = fopen("output.json", "w");
    //if (file) {
    //    char* serialized_json = json_serialize_to_string_pretty(root_value); // Pretty-printed JSON
    //    if (serialized_json) {
    //        fprintf(file, "%s\n", serialized_json);
    //        json_free_serialized_string(serialized_json);
    //    }
    //    fclose(file);
    //}

    //json_value_free(root_value);

	return ret;
}

void ModuleEditor::DrawEditor()
{
    // Start the Dear ImGui frame

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Here starts the code for the editor

    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("File")) {

            ImGui::SeparatorText("Scene");

            if (ImGui::MenuItem("New Scene")) {



            }

            if (ImGui::MenuItem("Open Scene")) {



            }

            ImGui::SeparatorText("Save");

            if (ImGui::MenuItem("Save")) {



            }

            if (ImGui::MenuItem("Save As...")) {



            }

            ImGui::SeparatorText("Project");

            if (ImGui::MenuItem("New Project")) {



            }

            if (ImGui::MenuItem("Open Project")) {



            }

            ImGui::SeparatorText("Exit");

            if (ImGui::MenuItem("Exit")) {

                App->input->quit = true;

            }

            ImGui::EndMenu();
       
        }

        if (ImGui::BeginMenu("Edit")) {




            ImGui::EndMenu();

        }

        if (ImGui::BeginMenu("View")) {




            ImGui::EndMenu();

        }

        if (ImGui::BeginMenu("GameObject")) {




            ImGui::EndMenu();

        }

        if (ImGui::BeginMenu("Help")) {

            if (ImGui::MenuItem("About")) {

                

            }

            if (ImGui::MenuItem("Repository")) {

                RequestBrowser("https://github.com/francesctr4/Ymir-Engine");

            }

            if (ImGui::MenuItem("Documentation")) {

                

            }

            if (ImGui::MenuItem("Releases")) {



            }

            if (ImGui::MenuItem("Bug report")) {



            }

            if (ImGui::MenuItem("Show ImGui demo")) {



            }

            ImGui::EndMenu();

        }

        ImGui::EndMainMenuBar();

    }

    /*if (ImGui::Begin("Configuration"), true) {

        char title[50];

        sprintf_s(title, 50, "Framerate (FPS): %.3f", FPSvec[FPSvec.size() - 1]);
        ImGui::PlotHistogram("## Framerate", &FPSvec[0], FPSvec.size(), 0, title, 0.0f, 250.0f, ImVec2(300, 100));

        sprintf_s(title, 50, "DeltaTime (DT): %.3f", DTvec[DTvec.size() - 1]);
        ImGui::PlotHistogram("## DeltaTime", &DTvec[0], DTvec.size(), 0, title, 0.0f, 0.032f, ImVec2(300, 100));

        sprintf_s(title, 50, "Milliseconds (MS): %.3f", MSvec[MSvec.size() - 1]);
        ImGui::PlotHistogram("## Milliseconds", &MSvec[0], MSvec.size(), 0, title, 0.0f, 32.0f, ImVec2(300, 100));

        ImGui::End();
    }*/

    if (ImGui::Begin("Application"), true) {

        if (ImGui::CollapsingHeader("Configuration")) {

            char title[50];

            sprintf_s(title, 50, "Framerate (FPS): %.3f", FPSvec[FPSvec.size() - 1]);
            ImGui::PlotHistogram("## Framerate", &FPSvec[0], FPSvec.size(), 0, title, 0.0f, 250.0f, ImVec2(300, 100));

            sprintf_s(title, 50, "DeltaTime (DT): %.3f", DTvec[DTvec.size() - 1]);
            ImGui::PlotHistogram("## DeltaTime", &DTvec[0], DTvec.size(), 0, title, 0.0f, 0.032f, ImVec2(300, 100));

            sprintf_s(title, 50, "Milliseconds (MS): %.3f", MSvec[MSvec.size() - 1]);
            ImGui::PlotHistogram("## Milliseconds", &MSvec[0], MSvec.size(), 0, title, 0.0f, 32.0f, ImVec2(300, 100));

        }

        if (ImGui::CollapsingHeader("Window")) {



        }

        ImGui::End();
    }

    ImGui::ShowDemoWindow();

    // Here finishes the code for the editor
    
    // Rendering

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
       // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
       //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)

    if (UpdateAndRenderAdditionalPlatformWindows) {

        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();

        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);

    }

}

bool ModuleEditor::CleanUp()
{
    bool ret = true;

    // ImGui CleanUp

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

	return ret;
}

void ModuleEditor::AddFPS(const float aFPS)
{
    if (FPSvec.size() < 30) {

        FPSvec.push_back(aFPS);

    }
    else {

        for (int i = 0; i < FPSvec.size(); i++) {

            if (i + 1 < FPSvec.size()) {

                float copy = FPSvec[i + 1];
                FPSvec[i] = copy;

            }
            
        }
        FPSvec[FPSvec.capacity() - 1] = aFPS;
    }

}

void ModuleEditor::AddDT(const float aDT)
{
    if (DTvec.size() < 30) {

        DTvec.push_back(aDT);

    }
    else {

        for (int i = 0; i < DTvec.size(); i++) {

            if (i + 1 < DTvec.size()) {

                float copy = DTvec[i + 1];
                DTvec[i] = copy;

            }

        }
        DTvec[DTvec.capacity() - 1] = aDT;
    }

}

void ModuleEditor::AddMS(const float aMS)
{
    if (MSvec.size() < 30) {

        MSvec.push_back(aMS);

    }
    else {

        for (int i = 0; i < MSvec.size(); i++) {

            if (i + 1 < MSvec.size()) {

                float copy = MSvec[i + 1];
                MSvec[i] = copy;

            }

        }
        MSvec[MSvec.capacity() - 1] = aMS;
    }

}

void ModuleEditor::RequestBrowser(const char* url)
{
    HINSTANCE result = ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
}
