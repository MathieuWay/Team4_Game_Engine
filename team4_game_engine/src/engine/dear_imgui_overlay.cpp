#include <team4_game_engine/engine/dear_imgui_overlay.hpp>
#include <glad/glad.h>

#include <entt/entt.hpp>
#include <team4_game_engine/engine/physics/physics.hpp>
using namespace team4_game_engine::physics;

#include <team4_game_engine/components/name.hpp>
#include <team4_game_engine/components/parent.hpp>
#include <team4_game_engine/components/children.hpp>
#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/rotation.hpp>
#include <team4_game_engine/components/scale.hpp>
#include <team4_game_engine/components/rigidbody.hpp>
#include <team4_game_engine/components/color.hpp>
#include <team4_game_engine/components/model.hpp>
#include <team4_game_engine/components/colliders/box_collider.hpp>
#include <team4_game_engine/components/colliders/sphere_collider.hpp>
using namespace team4_game_engine::components;

#include <team4_game_engine/engine/scene_view.hpp>

#include <team4_game_engine/engine/renderer/renderer3d.hpp>
using namespace team4_game_engine::renderer;
#include <team4_game_engine/engine/engine.hpp>
#include <team4_game_engine/engine/primitive.hpp>
#include <GLFW\glfw3.h>
using namespace team4_game_engine::engine;
namespace team4_game_engine::debug {
	DearImGuiOverlay::DearImGuiOverlay() {

	}

	void DearImGuiOverlay::Update(std::chrono::milliseconds deltatime, engine::World& world) {
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
		static bool toolManipulation = false;
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		io = ImGui::GetIO();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		glm::mat4 projection = engine::SceneView::Instance().GetProjection();
		components::Camera& camera = team4_game_engine::engine::SceneView::Instance().GetMainCameraComponent();
		if (showGrid) ImGuizmo::DrawGrid(&camera.view[0][0], &projection[0][0], &glm::mat4(1.0f)[0][0], 10 * glm::ceil(distance / 10));
		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::ViewManipulate(&camera.view[0][0], SceneView::Instance().GetDistance(), ImVec2(0, 0), ImVec2(io.DisplaySize.x / 7.5, io.DisplaySize.x / 7.5), IM_COL32(0, 0, 0, 75));


		if (ImGui::IsKeyPressed(GLFW_KEY_R)) // r Key
			toolManipulation = false;
		if (ImGui::IsKeyPressed(GLFW_KEY_Q)) {
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			toolManipulation = true;
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_W)) {
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
			toolManipulation = true;
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_E)) {
			mCurrentGizmoOperation = ImGuizmo::SCALE;
			toolManipulation = true;
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_X)) {
			if(mCurrentGizmoMode == ImGuizmo::LOCAL)
				mCurrentGizmoMode = ImGuizmo::WORLD;
			else
				mCurrentGizmoMode = ImGuizmo::LOCAL;
		}
		for (auto& entity : selected) {
			Position& position = world.Registry().get<Position>(entity);
			Rotation& rotation = world.Registry().get<Rotation>(entity);
			Scale& scale = world.Registry().get<Scale>(entity);
			if(toolManipulation){
				glm::mat4 matrice = glm::mat4(1.0f);
				matrice = glm::translate(matrice, glm::vec3(position.local.x, position.local.y, position.local.z));
				matrice = matrice * glm::toMat4(glm::quat(rotation.w, rotation.i, rotation.j, rotation.k));
				matrice = glm::scale(matrice, glm::vec3(scale.x, scale.y, scale.z));
				ImGuizmo::Manipulate(&camera.view[0][0], &projection[0][0], mCurrentGizmoOperation, mCurrentGizmoMode, &matrice[0][0], NULL, NULL, NULL, NULL);
				float matrixTranslation[3], matrixRotation[3], matrixScale[3];
				ImGuizmo::DecomposeMatrixToComponents(&matrice[0][0], matrixTranslation, matrixRotation, matrixScale);
				position.local = Vector3D(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
				for (int i = 0; i < 3; i++)  matrixRotation[i] = matrixRotation[0] * glm::pi<float>() / 180;
				double c1 = glm::cos(matrixRotation[0] / 2);
				double c2 = glm::cos(matrixRotation[1] / 2);
				double c3 = glm::cos(matrixRotation[2] / 2);
				double s1 = glm::sin(matrixRotation[0] / 2);
				double s2 = glm::sin(matrixRotation[1] / 2);
				double s3 = glm::sin(matrixRotation[2] / 2);
				float x = s1 * c2 * c3 + c1 * s2 * s3;
				float y = c1 * s2 * c3 - s1 * c2 * s3;
				float z = c1 * c2 * s3 + s1 * s2 * c3;
				float w = c1 * c2 * c3 - s1 * s2 * s3;
				rotation = Quaternion(w, x, y, z);
				scale = { matrixScale[0], matrixScale[1], matrixScale[2] };
			}
		}
		world.Registry().view<Position, RigidBody>().each([this, io](Position& pos, RigidBody& rb) {
			// Debug Overlay
			if (rb.showDebug && rb.mass > 0) {
				ImGuiStyle& style = ImGui::GetStyle();
				style.WindowBorderSize = 0.0f;
				ImGui::SetNextWindowBgAlpha(0);
				ImGui::SetNextWindowPos(ImVec2(0, 0));
				ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
				ImGui::Begin("test", (bool*)false,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoDecoration |
					ImGuiWindowFlags_NoScrollbar |
					ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoInputs);
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				auto world = Engine::Instance().GetWorld().lock();
				Vector3D massCenterWorld = pos.local;
				Vector3D pointWorld = pos.local.sumVector(Vector3D::localToWorldDirn(rb.pointDebug, rb.transforMatrix));
				Vector3D forceWorld = pos.local.sumVector(Vector3D::localToWorldDirn(rb.pointDebug.sumVector(rb.forceDebug), rb.transforMatrix));
				ImVec2 originscreenCoordinate = WorldToScreenCoordinate(glm::vec3(massCenterWorld.x, massCenterWorld.y, massCenterWorld.z));
				ImVec2 pointscreenCoordinate = WorldToScreenCoordinate(glm::vec3(pointWorld.x, pointWorld.y, pointWorld.z));
				ImVec2 forceDirection = WorldToScreenCoordinate(glm::vec3(forceWorld.x, forceWorld.y, forceWorld.z));
				draw_list->AddCircleFilled(originscreenCoordinate, 8, IM_COL32(0, 0, 255, 128));
				draw_list->AddCircleFilled(pointscreenCoordinate, 8, IM_COL32(255, 0, 0, 128));
				draw_list->AddLine(pointscreenCoordinate, forceDirection, IM_COL32(0, 255, 0, 128), 8);
				ImGui::End();
				style.WindowBorderSize = 1.0f;
			}
			});
		//ImGui Debug Window
		Inspector();
		Hierachy();
		PhysicsDebugWindow();
		//draw imgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void DearImGuiOverlay::PhysicsDebugWindow() {
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2 - 370 / 2, 0));
		ImGui::SetNextWindowSize(ImVec2(370, 80), ImGuiCond_Once);
		ImGui::Begin("Physics Window", (bool*)false, ImGuiWindowFlags_NoSavedSettings);
		const char* label;
		if (Physics::doPhysicsStep) label = "Stop";
		else label = "Play";
		if (ImGui::Button(label)) {
			Physics::doPhysicsStep = !Physics::doPhysicsStep;
		}
		ImGui::SameLine();
		if (ImGui::Button("Next Step")) {
			Physics::doPhysicsStep = false;
			Physics::doNextStep = true;

		}
		ImGui::SameLine();
		ImGui::Checkbox("Fixed Deltatime", &Physics::useFixedDeltatime);
		ImGui::SameLine();
		ImGui::Checkbox("Collision", &Physics::doCollisionStep);
		if (Physics::useFixedDeltatime) {
			ImGui::SetNextItemWidth(100);
			ImGui::DragInt("Time Step per second", &Physics::physicsStepPerSecond, 1, 100);
			Physics::physicsStepTime = (float)1 / (float)Physics::physicsStepPerSecond;
		}
		ImGui::End();
	}

	void DearImGuiOverlay::CreateBranche(entt::entity entity) {
		std::shared_ptr<World> world = Engine::Instance().GetWorld().lock();
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		std::string name = world->Registry().get<Name>(entity);
		std::string id = "##" + name;

		//If is Selected
		auto it = std::find(selected.begin(), selected.end(), entity);
		if (it != selected.end()) node_flags |= ImGuiTreeNodeFlags_Selected;

		// If entity don't have children = is leaf
		auto& childrens = world->Registry().get<Children>(entity).childrens;
		if (childrens.size() == 0) {
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx(id.c_str(), node_flags, name.c_str());
			if (ImGui::IsItemClicked()) {
				if (it != selected.end()) {
					selected.erase(it);
				}
				else {
					if (selected.size() > 0 && !ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL))
						selected.clear();
					selected.push_back(entity);
				}
			}
			CreateEntityContextMenu(entity, (int)entity +2);
		}
		else {
			if (ImGui::TreeNodeEx(id.c_str(), node_flags, name.c_str())) {
				if (ImGui::IsItemClicked()) {
					if (it != selected.end()) {
						selected.erase(it);
					}
					else {
						if (selected.size() > 0 && !ImGui::IsKeyDown(GLFW_KEY_LEFT_CONTROL))
							selected.clear();
						selected.push_back(entity);
					}
				}
				CreateEntityContextMenu(entity, (int)entity + 2);
				for (entt::entity children : childrens) {
					CreateBranche(children);
				}
				ImGui::TreePop();
			}
		}
	}
	void DearImGuiOverlay::CreateEntityContextMenu(entt::entity parent, int id, ImGuiPopupFlags flags) {
		if (ImGui::BeginPopupContextItem(std::string("Create Entity##" + std::to_string(id)).c_str(), flags))
		{
			if (ImGui::BeginMenu("Create Entity")) {
				if (ImGui::MenuItem("Empty")) {
					engine::primitive::Empty("GameObject", parent);
				}
				if (ImGui::MenuItem("Plane")) {
					engine::primitive::Plane("Plane", parent, 1);
				}
				if (ImGui::MenuItem("Cube")) {
					engine::primitive::Cube("Cube", parent, 2);
				}
				if (ImGui::MenuItem("Sphere")) {
					engine::primitive::Sphere("Sphere", parent, 3);
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
	}
	void DearImGuiOverlay::Hierachy() {
		std::shared_ptr<World> world = Engine::Instance().GetWorld().lock();
		if (!world) return;
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 7.5, io.DisplaySize.y - io.DisplaySize.x / 7.5));
		ImGui::SetNextWindowPos(ImVec2(0, io.DisplaySize.x / 7.5));
		ImGui::Begin("Hierachy", (bool*)false, ImGuiWindowFlags_NoSavedSettings);
		if (ImGui::Button("Clean Scene")) {
			world->Clear();
			selected.clear();
		}
		ImGui::SameLine();
		ImGui::Button("Add entity");
		CreateEntityContextMenu(entt::null, 0, ImGuiPopupFlags_MouseButtonLeft);
		ImGui::SameLine();
		if (ImGui::Button("Build Room")) {
			BuildRoom();
		}
		std::string label = "Scene (" + std::to_string(world->Registry().alive()) + " Objects)";
		if (ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			CreateEntityContextMenu(entt::null, 1);
			/* Hierarchy */
			Engine::Instance().GetWorld().lock()->Registry().each([this, world](entt::entity entity) {
				if (world->Registry().get<Parent>(entity).entity == entt::null) {
					CreateBranche(entity);
				}
			});
			/*			 */
			ImGui::TreePop();
		}
		else {
			CreateEntityContextMenu(entt::null, 1);
		}
		ImGui::End();
	}

	void AddComponentContextMenu(entt::entity target) {
		if (ImGui::BeginPopupContextItem("Add Component ContextMenu", ImGuiPopupFlags_MouseButtonLeft))
		{
			for (int i = 0; i < (int)ComponentType::Count; i++) {
				if (ComponentsLabels.find((ComponentType)i) != ComponentsLabels.end()) {
					if (ImGui::MenuItem(ComponentsLabels[(ComponentType)i])) {
						std::shared_ptr<World> world = Engine::Instance().GetWorld().lock();
						switch ((ComponentType)i)
						{
						case team4_game_engine::components::ComponentType::Color:
							if (!world->Registry().has<Color>(target))
								world->AddComponent<Color>(target, 1.0f, 1.0f, 1.0f);
							break;
						case team4_game_engine::components::ComponentType::Model:
							if (!world->Registry().has<Model>(target))
								world->AddComponent<Model>(target, std::vector<std::shared_ptr<Mesh>>());
								break;
						case team4_game_engine::components::ComponentType::RigidBody:
							if (!world->Registry().has<RigidBody>(target))
								world->AddComponent<RigidBody>(target, nullptr, 1.0f, 0.5f, 1.0f, 1.0f, Matrix3(), Vector3D(0, -10, 0));
							break;
						case team4_game_engine::components::ComponentType::BoxCollider:
							if (world->Registry().has<RigidBody>(target))
								world->GetComponent<RigidBody>(target).collider = new BoxCollider(Vector3D(1, 1, 1));
							break;
						case team4_game_engine::components::ComponentType::SphereCollider:
							if (world->Registry().has<RigidBody>(target))
								world->GetComponent<RigidBody>(target).collider = new SphereCollider(0.5f);
							break;
						}
					}
				}
			}
			ImGui::EndPopup();
		}
	}

	void DearImGuiOverlay::Inspector()
	{
		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 5, io.DisplaySize.y));
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - io.DisplaySize.x / 5, 0));
		ImGui::Begin("Inspector", (bool*)false, ImGuiWindowFlags_NoSavedSettings);
		if (selected.size()) {
			std::shared_ptr<World> world = Engine::Instance().GetWorld().lock();
			entt::entity lastSelect = selected[selected.size() - 1];
			for (int i = 0; i < (int)components::ComponentType::Count; i++)
			{
				switch ((components::ComponentType)i)
				{
				case team4_game_engine::components::ComponentType::Name:
					if (world->Registry().has<Name>(lastSelect))
						ImGui::InputText("Name", &world->Registry().get<Name>(lastSelect)[0], 1024);
					break;
				case team4_game_engine::components::ComponentType::Position:
					if (world->Registry().has<Position>(lastSelect))
						world->Registry().get<Position>(lastSelect).OnInspectorGUI();
					break;
				case team4_game_engine::components::ComponentType::Rotation:
					if (world->Registry().has<Rotation>(lastSelect))
						world->Registry().get<Rotation>(lastSelect).OnInspectorGUI();
					break;
				case team4_game_engine::components::ComponentType::Scale:
					if (world->Registry().has<Scale>(lastSelect)) {
						Scale& scale = world->Registry().get<Scale>(lastSelect);
						scale.OnInspectorGUI();
						scale.x; scale.y; scale.z;//Tu a accès aux valeur du scale
						if (world->Registry().has<BoundingVolume>(lastSelect)) {
							BoundingVolume& boudingVolume = world->Registry().get<BoundingVolume>(lastSelect);
							if (scale.x >= scale.y && scale.x >= scale.z) {
								boudingVolume.radius = scale.x;
							}
							else if (scale.y >= scale.x && scale.y >= scale.z) {
								boudingVolume.radius = scale.y;
							}
							else {
								boudingVolume.radius = scale.z;
							}		
						}
					}
					break;
				case team4_game_engine::components::ComponentType::Color:
					if (world->Registry().has<Color>(lastSelect))
						world->Registry().get<Color>(lastSelect).OnInspectorGUI();
					break;
				case team4_game_engine::components::ComponentType::Model:
					if (world->Registry().has<Model>(lastSelect))
						world->Registry().get<Model>(lastSelect).OnInspectorGUI();
					break;
				case team4_game_engine::components::ComponentType::RigidBody:
					if (!world->Registry().has<RigidBody>(lastSelect)) continue;
					auto& rb = world->Registry().get<RigidBody>(lastSelect);
					auto& pos = world->Registry().get<Position>(lastSelect);
					rb.OnInspectorGUI();

					if (ImGui::CollapsingHeader("Debug Force")) {
						ImGui::Checkbox("Show Debug", &rb.showDebug);
						ImGui::DragFloat3("Force", &rb.forceDebug.x);
						ImGui::DragFloat3("Point", &rb.pointDebug.x, 0.01f);
						if (ImGui::Button("Add Force")) {

							Vector3D worldForce = Vector3D().localToWorldDirn(rb.forceDebug, rb.transforMatrix);
							Physics::AddForceAtBodyPoint(pos, rb, worldForce, rb.pointDebug);
						}
					}
					break;
				}
			}
			ImGui::Button("Add Component");
			AddComponentContextMenu(lastSelect);
		}
		ImGui::End();
	}
	// World position to screen coordinate
	Vector3D DearImGuiOverlay::WorldToScreenCoordinate(glm::vec3 pos) {
		glm::mat4 projection = team4_game_engine::engine::SceneView::Instance().GetProjection();
		const components::Camera& camera = engine::SceneView::Instance().GetMainCameraComponent();
		glm::vec4 clipSpacePos = projection * camera.view * glm::vec4(pos, 1.0);
		glm::vec2 viewportCoordinate = glm::vec3(clipSpacePos) / clipSpacePos.w;
		viewportCoordinate.y *= -1;
		viewportCoordinate = ((viewportCoordinate + 1.0f) / 2.0f) * glm::vec2(io.DisplaySize.x, io.DisplaySize.y);
		return Vector3D(viewportCoordinate.x, viewportCoordinate.y, 0);
	}
	// Function to text on overlay with defaults parameters
	void DearImGuiOverlay::DrawText(ImDrawList* draw_list, const char* text, ImVec2 pos, ImColor color) {

		draw_list->AddText(
			ImGui::GetFont(),
			ImGui::GetFontSize(),
			pos,
			color,
			text,
			0, 0.0f, 0);
	}

	void DearImGuiOverlay::BuildRoom() {
		entt::entity walls = static_cast<entt::entity>(engine::primitive::Empty("Walls", entt::null)->EntityID());
		Entity* plane;
		float planeSize = 10;
		plane = engine::primitive::Plane("ground", walls, 1);
		//plane->Position() = { 0, 0, 0 };
		plane->Scale() = { planeSize, planeSize, planeSize };

		plane = engine::primitive::Plane("right", walls, 1);
		plane->Position() = { planeSize/2, planeSize/4, 0 };
		plane->Rotation() = { 0.707, 0, 0, 0.7070 };
		plane->Scale() = { planeSize/2, planeSize, planeSize };

		plane = engine::primitive::Plane("Left", walls, 1);
		plane->Position() = { -planeSize/2, planeSize/4, 0 };
		plane->Rotation() = { 0.707, 0, 0, -0.7070 };
		plane->Scale() = { planeSize/2, planeSize, planeSize };

		plane = engine::primitive::Plane("front", walls, 1);
		plane->Position() = { 0, planeSize/4, planeSize/2 };
		plane->Rotation() = { 0.707, -0.7070, 0, 0 };
		plane->Scale() = { planeSize, planeSize, planeSize/2 };

		plane = engine::primitive::Plane("back", walls, 1);
		plane->Position() = { 0, planeSize/4, -planeSize/2 };
		plane->Rotation() = { 0.707, 0.7070, 0, 0 };
		plane->Scale() = { planeSize, planeSize, planeSize/2 };

		Entity* cube = engine::primitive::Cube("Cube", entt::null, 2);
		cube->Position() = { 0, planeSize/4, 0 };
		cube->GetComponent<RigidBody>().useGravity = false;
	}
}