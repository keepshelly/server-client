#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <thread>
#include "GlobalData.h"
void server_thread_func(GlobalData* globalData);
int main() {
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(600, 400, "GPS Server", NULL, NULL);
    glfwMakeContextCurrent(window);
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    GlobalData data;
    std::thread s_thread(server_thread_func, &data);
    s_thread.detach();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("GPS status");
        GpsInfo info = data.get();
        if (info.isConnected) {
            ImGui::TextColored(ImVec4(0, 1, 0, 1), "STATUS: CONNECTED");
            ImGui::Separator();
            ImGui::Text("Latitude:  %.6f", info.lat);
            ImGui::Text("Longitude: %.6f", info.lon);
        } else {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "STATUS: NO SIGNAL");
            ImGui::Text("waiting for S25 client");
        }
        ImGui::Separator();
        ImGui::Text("raw data:");
        ImGui::TextWrapped("%s", info.text.c_str());
        ImGui::End();
        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}