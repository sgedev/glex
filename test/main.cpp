//
//
#include <cstdio>
#include <cstring>
#include <chrono>
#include <thread>
#include <functional>

#include <HandmadeMath.h>

#include <GL/gl3w.h>
#include <GL/glex.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace std::chrono_literals;

using Clock = std::chrono::high_resolution_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

struct ScopeGuard final {
	ScopeGuard(std::function<void (void)> fn): m_fn(fn) { }
	~ScopeGuard(void) { if (m_fn) m_fn(); }
	void dismiss(void) { m_fn = std::function<void (void)>(); }
	std::function<void (void)> m_fn;
};

static GLFWwindow *mainWindow;
static int mainWindowSize[2];
static bool showDemoWindow;
static bool guiMode;
static GLEXContext *glexContext;
static hmm_vec3 viewPosition;
static hmm_vec3 viewCenter;
static hmm_vec3 viewUp;

static bool init(void)
{
	viewPosition = HMM_Vec3(0.0f, 0.0f, 0.0f);
	viewCenter = HMM_Vec3(0.0f, 0.0f, -1.0f);
	viewUp = HMM_Vec3(0.0f, 1.0f, 0.0f);

	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

	showDemoWindow = true;

	return true;
}

static void render(Duration elapsed)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, mainWindowSize[0], mainWindowSize[1]);

	glexBeginFrame(mainWindowSize[0], mainWindowSize[1]);

	glexLookAt(viewPosition.X, viewPosition.Y, viewPosition.Z,
		viewCenter.X, viewCenter.Y, viewCenter.Z, viewUp.X, viewUp.Y, viewUp.Z);

	glexEndFrame();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow(&showDemoWindow);
	// TODO

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

static void onError(int error, const char *description)
{
	fprintf(stderr, "glfw error %d: %s\n", error, description);
}

static void onFocus(GLFWwindow *window, int focused)
{
	if (guiMode) {
		ImGui_ImplGlfw_WindowFocusCallback(window, focused);
		return;
	}
}

static void onCursorEnter(GLFWwindow *window, int entered)
{
	if (guiMode) {
		ImGui_ImplGlfw_CursorEnterCallback(window, entered);
		return;
	}
}

static void onScroll(GLFWwindow *window, double xoffset, double yoffset)
{
	if (guiMode) {
		ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
		return;
	}
}

static void onSize(GLFWwindow *window, int width, int height)
{
	mainWindowSize[0] = width;
	mainWindowSize[1] = height;
}

static void onKey(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
	if (guiMode) {
		ImGui_ImplGlfw_KeyCallback(window, key, scanCode, action, mods);
		return;
	}
}

static void onChar(GLFWwindow *window, unsigned int c)
{
	if (guiMode) {
		ImGui_ImplGlfw_CharCallback(window, c);
		return;
	}
}

static void onMonitor(GLFWmonitor *monitor, int event)
{
	ImGui_ImplGlfw_MonitorCallback(monitor, event);
}

static void onMouseMove(GLFWwindow *window, double x, double y)
{

}

static void onMouseButton(GLFWwindow *window, int button, int action, int mods)
{
	if (guiMode) {
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
		return;
	}
}

int main(int argc, char *argv[])
{
	printf("===================================\n");
	printf("glex test " GLEX_VERSION_STR "\n");
	printf("===================================\n\n");

	glfwSetErrorCallback(&onError);

	printf("initializing glfw...\n");
	int ret = glfwInit();
	if (ret != GLFW_TRUE) {
		fprintf(stderr, "failed to init glfw.\n");
		return EXIT_FAILURE;
	}

	ScopeGuard glfwGuard([] { glfwTerminate(); });

	glfwSetMonitorCallback(&onMonitor);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

#ifdef GLEX_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

	mainWindowSize[0] = 800;
	mainWindowSize[1] = 600;

	printf("creating main window...\n");
	mainWindow = glfwCreateWindow(mainWindowSize[0], mainWindowSize[1], "glex test", NULL, NULL);
	if (mainWindow == nullptr) {
		fprintf(stderr, "failed to create main window.\n");
		return EXIT_FAILURE;
	}

	ScopeGuard mainWindowGuard([] { glfwDestroyWindow(mainWindow); });

	glfwMakeContextCurrent(mainWindow);
	glfwSwapInterval(1);

	printf("initializing imgui...\n");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	ImGui_ImplGlfw_InitForOpenGL(mainWindow, false);
	ImGui_ImplOpenGL3_Init("#version 130");

	glfwSetWindowFocusCallback(mainWindow, &onFocus);
	glfwSetScrollCallback(mainWindow, &onScroll);
	glfwSetWindowFocusCallback(mainWindow, &onFocus);
	glfwSetWindowSizeCallback(mainWindow, &onSize);
	glfwSetKeyCallback(mainWindow, &onKey);
	glfwSetCursorPosCallback(mainWindow, &onMouseMove);
	glfwSetMouseButtonCallback(mainWindow, &onMouseButton);
	glfwSetCharCallback(mainWindow, &onChar);
	glfwSetCursorEnterCallback(mainWindow, &onCursorEnter);

	guiMode = true;

	printf("initializing glex context...\n");

	GLEXConfig config;
	memset(&config, 0, sizeof(config));

	config.getProc = (GLEXGetProcAddressProc)glfwGetProcAddress;

	glexContext = glexCreateContext(&config);
	if (glexContext == nullptr) {
		fprintf(stderr, "failed to create glex context.\n");
		return EXIT_FAILURE;
	}

	ScopeGuard glexGuard([] { glexDeleteContext(glexContext); });

	glexMakeCurrent(glexContext);

	if (!init())
		return EXIT_FAILURE;

	TimePoint last = Clock::now();
	TimePoint curr;

	printf("entering main loop...\n");
	while (!glfwWindowShouldClose(mainWindow)) {
		glfwPollEvents();
		curr = Clock::now();
		if (curr > last) {
			auto elapsed = curr - last;
			if (elapsed >= 16666us) {
				render(elapsed);
				glfwSwapBuffers(mainWindow);
				last = curr;
			}  else
				std::this_thread::sleep_for(16666us - elapsed);
		} else {
			last = curr;
			std::this_thread::yield();
		}
	}

	printf("exiting...\n");

	return EXIT_SUCCESS;
}
