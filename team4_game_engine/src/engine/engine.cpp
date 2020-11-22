#include <team4_game_engine/common.hpp>

// Engine
#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/world.hpp>
#include <team4_game_engine/engine/physics/physics.hpp>

// Renderer
#include <team4_game_engine/engine/renderer/renderer3D.hpp>
#include <team4_game_engine/engine/renderer/shape.hpp>
using namespace team4_game_engine::renderer;

// Systems
#include <team4_game_engine/systems/system.hpp>
#include <team4_game_engine/systems/fps_counter.hpp>
#include <team4_game_engine/systems/entity_viewer.hpp>
#include <team4_game_engine/systems/physics_system.hpp>
#include <team4_game_engine/systems/collision_system.hpp>

#include <team4_game_engine/engine/dear_imgui_overlay.hpp>

#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <team4_game_engine/engine/scene_view.hpp>

namespace team4_game_engine::engine {
	void APIENTRY glDebugOutput(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei length,
		const char* message,
		const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::cout << "---------------" << std::endl;
		std::cout << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		} std::cout << std::endl;

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		} std::cout << std::endl;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		} std::cout << std::endl;
		std::cout << std::endl;
	}

	class EngineImpl{
	public:
		EngineImpl() {
			m_world = std::make_shared<World>("team4_game_engine");

			int success = glfwInit();
			if (success == GLFW_FALSE) {
				std::cout << "Failed to initialize GLFW" << std::endl;
				Stop();
				return;
			}

			auto monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
			m_window = glfwCreateWindow(mode->width, mode->height, "Team4 Game Engine", NULL, NULL);
			glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
				Engine::Instance().Stop();
			});
			glfwSwapInterval(1);
			glfwMakeContextCurrent(m_window);
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cout << "Failed to initialize OpenGL context" << std::endl;
				return;
			}

			std::cout << glGetString(GL_VERSION) << std::endl;

			// Enable Debug Callback
			int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(glDebugOutput, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			}

			///Init & Attach Dear ImGui Debug Tool
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImFontAtlas* atlas = new ImFontAtlas();
			ImGui::CreateContext(atlas);
			ImGuiIO& io = ImGui::GetIO();
			ImGui::StyleColorsClassic();

			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForOpenGL(m_window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
			/*
			auto cube = m_world->CreateEntity("cube");
			components::Position cubePos = m_world->AddComponent<components::Position>(cube, 0.0f, -0.5f, 0.0f);
			m_world->AddComponent<components::Velocity>(cube, 0.0f, 0.0f, 0.0f);

			entt::entity sphere = m_world->CreateEntity("sphere");
			components::Position spherePos = m_world->AddComponent<components::Position>(sphere, 0.0f, 20.0f, 0.0f);
			m_world->AddComponent<components::Velocity>(sphere, 0.0f, 10.0f, 0.0f);
			*/
		}

		~EngineImpl() {
			glfwDestroyWindow(m_window);
		}

		void ParseCommandeLine(int argc, char* argv[]) {

		}

		void StartUp() {
			//Init Camera
			int width, height;
			glfwGetWindowSize(m_window, &width, &height);

			debugOverlay = team4_game_engine::debug::DearImGuiOverlay();
			SceneView::Instance().UpdateProjection(width, height);
			#pragma region GPU_data

#pragma region Particule_data
			float singleVertex[] = {
				0.0f,  0.0f,  0.0f
			};
			const BufferLayout particule_Vertexlayout = { {ShaderDataType::Float3, "a_Pos", components::ComponentType::NotInstanced} };
			const BufferLayout particule_Instancelayout = { {ShaderDataType::Float3, "a_Color", components::ComponentType::Color}, {ShaderDataType::Float3, "a_Offset", components::ComponentType::Position} };
			Shader* particule_shader = new Shader("particuleShader.vs", "particuleShader.fs");
			Renderer3D::Instance().ms_meshs.push_back(std::make_shared<Mesh>(
				"Particule",
				std::vector<float>(singleVertex, singleVertex + sizeof singleVertex / sizeof singleVertex[0]),
				std::vector<uint32_t>(),
				particule_Vertexlayout,
				particule_Instancelayout,
				particule_shader,
				0
				));
#pragma endregion

#pragma region Cube_data
			// https://encrypted-tbn0.gstatic.com/images?q=tbn%3AANd9GcSChUMsWvzjIEAFE0smdkDk7ZkYoBb_hItqtg&usqp=CAU
			float vertices[] = {
				-0.5f,  0.5f,  0.5f, 0, 1, 1,
				 0.5f,  0.5f,  0.5f, 1, 1, 1,
				-0.5f, -0.5f,  0.5f, 0, 0, 1,
				 0.5f, -0.5f,  0.5f, 1, 0, 1,

				 0.5f, -0.5f, -0.5f, 1, 0, 0,
				 0.5f,  0.5f, -0.5f, 1, 1, 0,

				-0.5f, -0.5f, -0.5f, 0, 0, 0,
				-0.5f,  0.5f, -0.5f, 0, 1, 0
			};

			uint32_t indices[] = {  // Triangles Strip indices
				0, 1,
				2, 3,
				4, 1,
				5, 0,
				7, 2,
				6, 4,
				7, 5
			};

			// VAO: Vertex Array Object / VBO: Vertex Buffer Object / EBO: Element Buffer Object
			// https://learnopengl.com/img/getting-started/vertex_array_objects.png
			// Create Vertex Array
			const BufferLayout cube_Vertexlayout = { {ShaderDataType::Float3, "a_Pos", components::ComponentType::NotInstanced}, {ShaderDataType::Float3, "a_Color", components::ComponentType::NotInstanced} };
			const BufferLayout cube_Instancelayout = { {ShaderDataType::Mat4, "model", components::ComponentType::Transform}};
			Shader* cube_shader = new Shader("shader.vs", "shader.fs");
			// Push VAO to VAO List
			Renderer3D::Instance().ms_meshs.push_back(std::make_shared<Mesh>(
				"Cube",
				std::vector<float>(vertices, vertices + sizeof vertices / sizeof vertices[0]),
				std::vector<uint32_t>(indices, indices + sizeof indices / sizeof indices[0]),
				cube_Vertexlayout,
				cube_Instancelayout,
				cube_shader,
				4
				));
#pragma endregion

			std::shared_ptr<Mesh> sphere(shape::GenerateSphere(36, 18, 0.5f));
			Renderer3D::Instance().ms_meshs.push_back(sphere);
#pragma endregion

			// Enable depth buffer
			glEnable(GL_DEPTH_TEST);


			spdlog::set_level(spdlog::level::debug);
			m_systems.emplace_back(std::make_unique<systems::FPSCounter>());
			m_systems.emplace_back(std::make_unique<systems::PhysicsSystem>());
			m_systems.emplace_back(std::make_unique<systems::CollisionSystem>(8));
			//m_systems.emplace_back(std::make_unique<systems::EntityViewer>());
		}

		void Run() {
			if (!m_stop) return;
			m_stop = false;
			std::chrono::milliseconds deltaTime(0);
			while (!m_stop)
			{
				auto start = std::chrono::high_resolution_clock::now();

				glfwPollEvents();
				int width, height;
				glfwGetWindowSize(m_window, &width, &height);
				glViewport(0, 0, width, height);
				//update all systems
				for (auto& system : m_systems) {
					system->Update(deltaTime, *m_world);
				}
				if (physics::Physics::doNextStep)
					physics::Physics::doNextStep = false;

				//Draw Frame
				SceneView::Instance().UpdateProjection(width, height);
				const auto& sceneView = engine::SceneView::Instance().GetMainCameraComponent();
				glm::mat4 view = engine::SceneView::Instance().GetMainCameraComponent().view;
				glm::mat4 projection = engine::SceneView::Instance().GetProjection();

				glClearColor(sceneView.backgroundColor.x, sceneView.backgroundColor.y, sceneView.backgroundColor.z, 1);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				for (auto mesh : renderer::Renderer3D::Instance().ms_meshs)
				{
					mesh->UpdateInstanceBuffer();
					mesh->Draw(projection, view);
				}
				debugOverlay.Update(deltaTime, *m_world);
				glfwSwapBuffers(m_window);

				auto end = std::chrono::high_resolution_clock::now();
				deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			}
		}

		void Stop() {
			m_stop = true;
		}

		std::weak_ptr<World> GetWorld() {
			return m_world;
		}
	private:
		std::atomic_bool m_stop = true;
		std::vector<std::unique_ptr<systems::ISystem>> m_systems;
		std::shared_ptr<World> m_world;
		team4_game_engine::debug::DearImGuiOverlay debugOverlay;
		GLFWwindow* m_window;
	};

	Engine::Engine() {
		m_impl = std::make_unique<EngineImpl>();
	}

	Engine::~Engine() {
		m_impl = nullptr;

	}

	void Engine::ParseCommandeLine(int argc, char* argv[]) {
		m_impl->ParseCommandeLine(argc, argv);

	}

	void Engine::Startup() {
		m_impl->StartUp();
	}

	void Engine::Run() {
		m_impl->Run();

	}
	void Engine::Stop() {
		m_impl->Stop();
	}
	std::weak_ptr<World> Engine::GetWorld()
	{
		return m_impl->GetWorld();
	}
}