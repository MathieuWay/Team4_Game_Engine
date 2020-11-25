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
		ImGuizmo::ViewManipulate(&camera.view[0][0], this->distance, ImVec2(0, 0), ImVec2(io.DisplaySize.x / 7.5, io.DisplaySize.x / 7.5), IM_COL32(0, 0, 0, 75));


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
			if (rb.showDebug) {
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
				Vector3D forceWorld = pos.local.sumVector(Vector3D::localToWorldDirn(rb.pointDebug.sumVector(rb.forceDebug.normalize()), rb.transforMatrix));
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
		//SceneDebugWindow();
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
		//if(ImGui::ImageButton("physics_start_stop", ))
		ImGui::End();
	}

	//void DearImGuiOverlay::SceneDebugWindow() {
	//	auto& camera = team4_game_engine::engine::SceneView::Instance().GetMainCameraComponent();
	//	ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - io.DisplaySize.x / 5, 0));
	//	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 5, io.DisplaySize.x / 4));
	//	ImGui::Begin("Debug", (bool*)false, ImGuiWindowFlags_NoSavedSettings);
	//	if (ImGui::CollapsingHeader("Instantiation"))
	//	{
	//		if (ImGui::BeginCombo("##combo2", Renderer3D::Instance().ms_meshs[selectedMesh]->m_name.c_str())) // The second parameter is the label previewed before opening the combo.
	//		{
	//			for (int i = 0; i < Renderer3D::Instance().ms_meshs.size(); i++) {
	//				bool is_selected = (selectedMesh == i); // You can store your selection however you want, outside or inside your objects
	//				if (ImGui::Selectable(Renderer3D::Instance().ms_meshs[i]->m_name.c_str(), is_selected))
	//					selectedMesh = i;
	//				if (is_selected)
	//					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
	//			}
	//			ImGui::EndCombo();
	//		}
	//		// Instantiation
	//		if (ImGui::Button("Instanciate Mesh")) {
	//			Instantiate(entt::null);
	//		}
	//		//ImGui::SameLine();
	//		//ImGui::Checkbox("Spawn continusly", &continuslySpawn);
	//		ImGui::Separator();
	//		// Initial Particule data
	//		if (ImGui::TreeNode("Position")) {
	//			if (!randomPos)
	//				ImGui::DragFloat3("Initial Position", &initialPosition[0]);
	//			else
	//				ImGui::DragFloatRange2("Min / Max Random Position", &minMaxPos.x, &minMaxPos.y, 0.1f, ImGuiSliderFlags_ClampOnInput);
	//			ImGui::Checkbox("Random Position", &randomPos);
	//			ImGui::TreePop();
	//		}
	//		if (ImGui::TreeNode("Velocity")) {
	//			if (!randomVel)
	//				ImGui::DragFloat3("Initial Velocity", &initialVelocity[0]);
	//			else
	//				ImGui::DragFloatRange2("Min / Max Random Velocity", &minMaxVel.x, &minMaxVel.y, 0.1f, ImGuiSliderFlags_ClampOnInput);
	//			ImGui::Checkbox("Random Velocity", &randomVel);
	//			ImGui::TreePop();
	//		}
	//		if (ImGui::TreeNode("Color")) {
	//			ImGui::ColorEdit3("Point Color", &particule_color.x);
	//			ImGui::TreePop();
	//		}
	//		if (ImGui::TreeNode("Radius")) {
	//			if (useRandomRadius)
	//				ImGui::DragFloatRange2("Min / Max Random Radius", &minRadius, &maxRadius, 0.1f, 0.1f, FLT_MAX, "%.3f", 0, ImGuiSliderFlags_ClampOnInput);
	//			else
	//				ImGui::DragFloat("Radius", &radius, 0.1f, 0.1f, FLT_MAX);
	//			ImGui::Checkbox("Random Radius", &useRandomRadius);
	//			ImGui::TreePop();
	//		}
	//		if (ImGui::TreeNode("Physique")) {
	//			if (ImGui::TreeNode("Mass")) {
	//				if (scaleWithSize) {
	//					ImGui::DragFloat("Size factor (radius * factor)", &scaleFactor, 0.01f, FLT_MAX);
	//				}
	//				else
	//				{
	//					if (useRandomRadius)
	//						ImGui::DragFloatRange2("Min / Max Random Radius", &minMass, &maxMass, 0.1f, 0, FLT_MAX, "%.3f", 0, ImGuiSliderFlags_ClampOnInput);
	//					else
	//						ImGui::DragFloat("Mass", &mass, 0.01f, FLT_MAX);
	//					ImGui::Checkbox("Random Mass", &useRandomMass);
	//				}
	//				ImGui::Checkbox("Scale With Size", &scaleWithSize);
	//				ImGui::TreePop();
	//			}
	//			if (ImGui::TreeNode("Restitution")) {
	//				ImGui::Checkbox("Random Restitution", &useRandomRestitution);
	//				if (!useRandomRestitution)
	//					ImGui::DragFloat("Restitution", &restitution, 0.1f, 0.f, 1.f);
	//				ImGui::TreePop();
	//			}
	//			ImGui::TreePop();
	//		}
	//		ImGui::Separator();
	//		for (int n = 0; n < IM_ARRAYSIZE(forceOpened); n++)
	//		{
	//			if (n > 0) { ImGui::SameLine(); }
	//			ImGui::Checkbox(forceNames[n], &forceOpened[n]);
	//		}
	//		// Forces Tabs
	//		if (ImGui::BeginTabBar("ForcesTabs", ImGuiTabBarFlags_AutoSelectNewTabs))
	//		{
	//			for (int n = 0; n < IM_ARRAYSIZE(forceOpened); n++) {
	//				if (forceOpened[n] && ImGui::BeginTabItem(forceNames[n], &forceOpened[n], ImGuiTabItemFlags_None))
	//				{
	//					switch (n)
	//					{
	//					case 0:
	//						ImGui::DragFloat3("Scene Gravity", &gravityForceVector.x, 0.1f);
	//						break;
	//					case 1:
	//						// Drag Value
	//						ImGui::Checkbox("Random drag", &useRandomDrag);
	//						if (useRandomDrag) {
	//							ImGui::DragFloatRange2("Min / Max Random Drag", &minDrag, &maxDrag, 0.1f, 0, FLT_MAX, "%.3f", 0, ImGuiSliderFlags_ClampOnInput);
	//						}
	//						else {
	//							ImGui::SliderFloat("Drag", &drag, 0, 1);
	//						}
	//						break;
	//					case 2:
	//						//
	//						ImGui::RadioButton("Anchored Spring", &useSpringType, 0); ImGui::SameLine();
	//						ImGui::RadioButton("Basic Spring", &useSpringType, 1); ImGui::SameLine();
	//						ImGui::RadioButton("Elasticity", &useSpringType, 2); ImGui::SameLine();
	//						ImGui::RadioButton("Stiff Spring", &useSpringType, 3);
	//						if (useSpringType == 0 || useSpringType == 3) ImGui::DragFloat3("Anchore Position", &anchorPosition.x);
	//						ImGui::DragFloat("Stiffness", &stiffness);
	//						if (useSpringType == 3) {
	//							ImGui::DragFloat("Damping", &damping, 0.01f);
	//						}
	//						else {
	//							ImGui::DragFloat("Rest Length	", &restLength);
	//						}
	//						break;
	//					case 3:
	//						ImGui::DragFloat("Maximum Depth", &maxDepth);
	//						ImGui::DragFloat("Volume", &volume);
	//						ImGui::DragFloat("Water Height", &waterHeight);
	//						ImGui::DragFloat("Liquid Density", &liquidDensity);
	//						break;
	//					default:
	//						break;
	//					}
	//					ImGui::EndTabItem();
	//				}
	//			}
	//			ImGui::EndTabBar();
	//			ImGui::Separator();
	//		}
	//	}
	//	if (ImGui::CollapsingHeader("Draw Settings")) {
	//		// Draw Mode Selector
	//		if (ImGui::BeginCombo("##combo", Draw_Modes[Renderer3D::Instance().ms_meshs[selectedMesh]->m_drawMode].name)) // The second parameter is the label previewed before opening the combo.
	//		{
	//			for (int n = 0; n < IM_ARRAYSIZE(Draw_Modes); n++)
	//			{
	//				bool is_selected = (Renderer3D::Instance().ms_meshs[selectedMesh]->m_drawMode == n); // You can store your selection however you want, outside or inside your objects
	//				if (ImGui::Selectable(Draw_Modes[n].name, is_selected))
	//					Renderer3D::Instance().ms_meshs[selectedMesh]->m_drawMode = n;
	//				if (is_selected)
	//					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
	//			}
	//			ImGui::EndCombo();
	//		}
	//		int tempPointSize = pointSize;
	//		ImGui::SliderInt("Point Size", &tempPointSize, 1, 20);
	//		if (tempPointSize != pointSize) {
	//			pointSize = tempPointSize;
	//			glPointSize(pointSize);
	//		}
	//	}
	//	if (ImGui::CollapsingHeader("Camera Settings")) {
	//		ImGui::SliderFloat("FOV", &camera.FOV, 1, 180);
	//		ImGui::DragFloatRange2("Near & Far Clip", &camera.nearClip, &camera.farClip, 0.25f, 0.1f, FLT_MAX, nullptr, nullptr, ImGuiSliderFlags_ClampOnInput);
	//		float distanceTemp = distance;
	//		ImGui::DragFloat("Distance from center", &distanceTemp, 1, 0, FLT_MAX);
	//		//Update Distance
	//		if (distanceTemp != distance) {
	//			float gap = distance - distanceTemp;
	//			distance = distance;
	//			if (distanceTemp > 0 || distanceTemp < FLT_MAX) {
	//				float* mat = glm::value_ptr(camera.view);
	//				//https://i.stack.imgur.com/KW71r.jpg
	//				camera.view = glm::translate(camera.view, glm::vec3(mat[2], mat[6], mat[10]) * gap);
	//			}
	//			distance = distanceTemp;
	//		}
	//		ImGui::ColorEdit3("Background Color", &camera.backgroundColor.x); // Edit 3 floats representing a color
	//		ImGui::Checkbox("Show Grid", &showGrid);
	//	}
	//	if (ImGui::CollapsingHeader("Debug"))
	//	{
	//		ImGui::Checkbox("Show Debug Text", &showDebugText);
	//		ImGui::Checkbox("Show Debug Ray", &showDebugRay);
	//		ImGui::Checkbox("Show Debug Tools", &showDebugTools);
	//	}
	//	//Framerate
	//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	ImGui::End();
	//	// Debug Overlay
	//	if (showDebugText || showDebugRay) {
	//		ImGuiStyle& style = ImGui::GetStyle();
	//		style.WindowBorderSize = 0.0f;
	//		ImGui::SetNextWindowBgAlpha(0);
	//		ImGui::SetNextWindowPos(ImVec2(0, 0));
	//		ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
	//		ImGui::Begin("test", (bool*)false,
	//			ImGuiWindowFlags_NoTitleBar |
	//			ImGuiWindowFlags_NoResize |
	//			ImGuiWindowFlags_NoMove |
	//			ImGuiWindowFlags_NoDecoration |
	//			ImGuiWindowFlags_NoScrollbar |
	//			ImGuiWindowFlags_NoSavedSettings |
	//			ImGuiWindowFlags_NoInputs);
	//		ImDrawList* draw_list = ImGui::GetWindowDrawList();
	//		auto world = Engine::Instance().GetWorld().lock();
	//		for (auto entity : selected) {
	//			if (!world->Registry().valid(entity)) world->Registry().destroy(entity);
	//			Position position = world->Registry().get<Position>(entity);
	//			RigidBody rb = world->Registry().get<RigidBody>(entity);
	//			ImVec2 screenCoordinate = WorldToScreenCoordinate(glm::vec3(position.local.x, position.local.y, position.local.z));
	//			if (showDebugText) {
	//				std::string text = "Position " + position.local.GetVectorData() + "\n";
	//				text += "Velocity " + rb.linearVelocity.GetVectorData() + "\n";
	//				text += "Acceleration " + rb.accumulateLinearForces.GetVectorData() + "\n";
	//				//text += "Mass " + std::to_string(particule->get()->GetMass()) + "\n";
	//				//text += "Radius " + std::to_string(particule->get()->GetRadius()) + "\n";
	//				//text += "Restitution " + std::to_string(particule->get()->GetRestitutionCoef()) + "\n";
	//				ImVec2 debugTextCoord = screenCoordinate;
	//				debugTextCoord.x += 25;
	//				debugTextCoord.y -= 50;
	//				DrawText(draw_list, text.c_str(), debugTextCoord);
	//			}
	//			if (showDebugRay) {
	//				//ImVec2 velDirection = WorldToScreenCoordinate(glm::vec3(pos.x + vel.x, pos.y + vel.y, pos.z + vel.z));
	//				//draw_list->AddLine(screenCoordinate, velDirection, IM_COL32(0, 0, 255, 75), pointSize);
	//			}
	//		}
	//		ImGui::End();
	//		style.WindowBorderSize = 1.0f;
	//	}
	//}

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
				if (ImGui::MenuItem("Cube")) {
					engine::primitive::Cube("Cube", parent, 1);
				}
				if (ImGui::MenuItem("Sphere")) {
					engine::primitive::Sphere("Sphere", parent, 2);
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
					if (world->Registry().has<Scale>(lastSelect))
						world->Registry().get<Scale>(lastSelect).OnInspectorGUI();
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

	// instantiate from tool
	//void DearImGuiOverlay::Instantiate(entt::entity parent, int mesh) {
	//	// Position
	//	Vector3D  pos, vel;
	//	if (randomPos)
	//		pos = Vector3D::Random(minMaxPos.x, minMaxPos.y, minMaxPos.x, minMaxPos.y, minMaxPos.x, minMaxPos.y);
	//	else
	//		pos = Vector3D(initialPosition.x, initialPosition.y, initialPosition.z);
	//	// Velocity
	//	if (randomVel)
	//		vel = Vector3D::Random(minMaxVel.x, minMaxVel.y, minMaxVel.x, minMaxVel.y, minMaxVel.x, minMaxVel.y);
	//	else
	//		vel = Vector3D(initialVelocity.x, initialVelocity.y, initialVelocity.z);
	//	Instantiate(parent, mesh >= 0 ? mesh : selectedMesh, pos, vel);
	//}

	// instantiate with position & velocity defined
	//void DearImGuiOverlay::Instantiate(entt::entity parent, int mesh, Vector3D  pos, Vector3D vel) {
	//	Renderer3D& instance = Renderer3D::Instance();
	//	std::weak_ptr<World> world = Engine::Instance().GetWorld();
	//	Entity* entity = new Entity(*world.lock(), Renderer3D::Instance().ms_meshs[mesh]->m_name);
	//	entity->GetComponent<Parent>().entity = parent;
	//	if (parent != entt::null) {
	//		world.lock()->Registry().get<Children>(parent).childrens.push_back(static_cast<entt::entity>(entity->EntityID()));
	//	}
	//	entity->Position() = Position(pos.x, pos.y, pos.z);
	//	entity->AddComponent<Color>(particule_color.x, particule_color.y, particule_color.z);
	//	//entity->AddComponent<BoxCollider>(glm::vec3(1,1,1));
	//	auto& rb = entity->AddComponent<RigidBody>(
	//		new SphereCollider(1),
	//		mass,
	//		useRandomRestitution ? engine::mathematics::RandomFloat(0, 1) : restitution,
	//		useRandomDrag ? engine::mathematics::RandomFloat(minDrag, maxDrag) : drag,
	//		1,
	//		Matrix3(),
	//		Vector3D(gravityForceVector.x, gravityForceVector.y, gravityForceVector.z),
	//		true);
	//	//auto& rb = entity->AddComponent<RigidBody>(entity->Position(), entity->AddComponent<SphereCollider>(1), mass);
	//	rb.linearVelocity = vel;
	//	auto meshs = std::vector<std::shared_ptr<team4_game_engine::renderer::Mesh>>();
	//	meshs.push_back(instance.ms_meshs[mesh]);
	//	entity->AddComponent<Model>(meshs);
	//	instance.ms_meshs[mesh]->AddInstance(entity);
	//}
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
}