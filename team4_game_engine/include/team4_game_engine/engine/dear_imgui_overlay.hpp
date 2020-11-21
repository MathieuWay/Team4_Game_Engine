#pragma once
#include <team4_game_engine/common.hpp>
#include <team4_game_engine/systems/system.hpp>
using namespace team4_game_engine::systems;
#include <team4_game_engine/engine/entity.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>


#include <Dear_ImGui/imgui.h>
#include <Dear_ImGui/imgui_impl_glfw.h>
#include <Dear_ImGui/imgui_impl_opengl3.h>
#include <dear_imgui/ImGuizmo.h>
#include <glad/glad.h>

#include <team4_game_engine/engine/mathematics/vector3d.hpp>
using namespace team4_game_engine::engine::mathematics;

namespace team4_game_engine::debug {
	struct Draw_Mode {
		int mode;
		const char* name;
	};

	static const Draw_Mode Draw_Modes[] = {
		{GL_POINTS, "Points"},
		{GL_LINES, "Lines"},
		{GL_LINE_STRIP, "Lines Strip"},
		{GL_TRIANGLES, "Triangles"},
		{GL_TRIANGLE_STRIP, "Triangles Strip"}
	};
	class DearImGuiOverlay : public ISystem {
	public:
		DearImGuiOverlay();
		void Update(std::chrono::milliseconds deltatime, engine::World& world);
	private:
		void PhysicsDebugWindow();
		void SceneDebugWindow();
		void CreateBranche(entt::entity entity);
		void CreateEntityContextMenu(entt::entity parent);
		void Hierachy();
		void Inspector();
		Vector3D DearImGuiOverlay::WorldToScreenCoordinate(glm::vec3 pos);
		void DrawText(ImDrawList* draw_list, const char* text, ImVec2 pos, ImColor color = ImColor(255, 255, 255));

		// instantiate from tool
		void Instantiate(entt::entity parent, int mesh = -1);
		// instantiate with position & velocity defined
		void Instantiate(entt::entity parent, int mesh, Vector3D pos, Vector3D vel);

		ImGuiIO io;
		std::vector<entt::entity*> selectedEntities;
		//std::vector<std::shared_ptr<Mesh>>* Meshs;

		bool showGrid, continuslySpawn;
		int selectedMesh = 0;
		float distance = 10;
		//Vector3D clear_color;
		// Rendering
		int pointSize = 4;

		//Instantiation
		bool randomPos = false, randomVel = false;
		glm::vec3 initialPosition = glm::vec3(), initialVelocity = glm::vec3();
		glm::vec2 minMaxPos = glm::vec2(), minMaxVel = glm::vec2();

		float radius = 0.5f, minRadius = 0.25f, maxRadius = 0.75f;
		bool useRandomRadius = false;
		float mass = 1, minMass = 0, maxMass = 10, scaleFactor = 5;
		bool useRandomMass = false, scaleWithSize;

		float restitution = 1;
		bool useRandomRestitution;

		float initialMass = 1;
		glm::vec3 particule_color = glm::vec3(1);

		// Gravity
		glm::vec3 gravityForceVector = glm::vec3(0, -10, 0);

		// Drag
		float drag = 1, minDrag = 0, maxDrag = 10;
		bool useRandomDrag = false;

		// Spring
		float stiffness = 10, restLength = 2, damping = 1;
		int useSpringType; // 0 = Anchored Spring ; 1 = Basic Spring ; 2 = Elasticity Spring ; 3 = Stiff Spring
		glm::vec3 anchorPosition = glm::vec3();

		//Buoyancy
		float maxDepth = 1, volume = 0.1, waterHeight = 0;
		float liquidDensity = 1000;

		//Debug
		//Forces tabs
		const char* forceNames[4] = { "Gravity", "Drag", "Springs", "Buoyancy" };
		bool forceOpened[4] = { false, true, false, false }; // Persistent user state
		bool showDebugText = true, showDebugRay = true, showDebugTools = false;

		std::vector<entt::entity> selected;
	};
}