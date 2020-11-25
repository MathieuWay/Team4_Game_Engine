#pragma once
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/mathematics/matrix3.hpp>
#include <team4_game_engine/engine/mathematics/matrix4.hpp>
using namespace team4_game_engine::engine::mathematics;
#include <team4_game_engine/components/position.hpp>
#include <team4_game_engine/components/colliders/collider.hpp>
#include <team4_game_engine/components/colliders/box_collider.hpp>
#include <team4_game_engine/components/colliders/sphere_collider.hpp>
#include <map>

namespace team4_game_engine::components {
	enum class RestitutionCombine { Average };
	static std::map<RestitutionCombine, const char*> RestitutionCombineLabel = {
		{ RestitutionCombine::Average, "Average" }
	};
	struct RigidBody {
	public:
		Collider* collider;
		Vector3D linearVelocity;
		Vector3D angularVelocity;
		Vector3D acceleration;
		Vector3D accumulateLinearForces;
		Vector3D accumulateAngularForces;
		Vector3D massCenter;
		float inverseMass;
		float mass;
		float restitutionCoef;
		RestitutionCombine restitutionCombine;


		Matrix4 transforMatrix;
		Matrix3 inverseInertiaTensor;
		Matrix3 inverseInertiaTensorWorld;

		bool isKinematic;

		//Gravity
		bool useGravity;
		engine::mathematics::Vector3D gravity;

		float linearDrag; // aka damping
		float angularDrag; // aka damping

		bool isAwake = false;
		// Debug
		bool showDebug = true;
		Vector3D forceDebug = Vector3D();
		Vector3D pointDebug = Vector3D();

		RigidBody(Collider* col, float _mass, float _restitution, float _linearDrag, float _angularDrag, Matrix3 _inverseInertiaTensor, engine::mathematics::Vector3D _gravity, bool _useGravity = true) :
			collider(col),
			restitutionCoef(_restitution),
			transforMatrix(Matrix4()),
			inverseInertiaTensor(_inverseInertiaTensor),
			inverseInertiaTensorWorld(Matrix3()),
			linearDrag(_linearDrag),
			angularDrag(_angularDrag),
			gravity(_gravity),
			isKinematic(false),
			useGravity(_useGravity),
			restitutionCombine(RestitutionCombine::Average)
		{
			if (_mass == 0) {
				mass = 0;
				inverseMass = FLT_MAX;
			}
			else {
				mass = _mass;
				inverseMass = 1 / _mass;
			}
		}

		void OnInspectorGUI() {
			if (collider) {
				switch (collider->GetShape())
				{
				case Shape::Box:
					if (ImGui::CollapsingHeader("Box Collider")) {
						BoxCollider* boxCollider = (BoxCollider*)collider;
						Vector3D size = Vector3D(boxCollider->shapeData.max.x * 2, boxCollider->shapeData.max.y * 2, boxCollider->shapeData.max.z * 2);
						ImGui::DragFloat3("Size", &size.x, 0.1f, 0, FLT_MAX);
						boxCollider->shapeData.max = Vector3D(size.x / 2, size.y / 2, size.z / 2);
						boxCollider->shapeData.min = Vector3D(size.x / 2, size.y / 2, size.z / 2).invert();
					}
					break;
				case Shape::Sphere:
					if (ImGui::CollapsingHeader("Sphere Collider")) {
						SphereCollider* sphereCollider = (SphereCollider*)collider;
						ImGui::DragFloat("Radius", &sphereCollider->shapeData.radius);
					}
					break;
				default:
					break;
				}
			}
			if (ImGui::CollapsingHeader("RigidBody")) {
				ImGui::Checkbox("Is Kinematic", &isKinematic);

				// Mass
				if (ImGui::TreeNode("Mass")) {
					ImGui::DragFloat("Mass", &mass, 0.1f, 0, FLT_MAX);
					ImGui::DragFloat3("Center", &massCenter.x, 0.01f);
					inverseMass = mass > 0 ? 1 / mass : FLT_MAX;
					ImGui::TreePop();
				}

				// Rectitution
				if (ImGui::TreeNode("Restitution")) {
					if (ImGui::BeginCombo("Type", RestitutionCombineLabel[restitutionCombine])) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < RestitutionCombineLabel.size(); n++)
						{
							bool is_selected = ((int)restitutionCombine == n); // You can store your selection however you want, outside or inside your objects
							if (ImGui::Selectable(RestitutionCombineLabel[(RestitutionCombine)n], is_selected))
								restitutionCombine = (RestitutionCombine)n;
							if (is_selected)
								ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
						}
						ImGui::EndCombo();
					}
					ImGui::DragFloat("Coefficient", &restitutionCoef, 0.05f, 0.001f, 1);
					ImGui::TreePop();
				}

				// Velocity
				if (ImGui::TreeNode("Velocity")) {
					ImGui::DragFloat3("Linear", &linearVelocity.x);
					ImGui::DragFloat3("Angular", &angularVelocity.x);
					ImGui::TreePop();
				}

				// Acceleration
				if (ImGui::TreeNode("Acceleration")) {
					ImGui::DragFloat3("Linear##test", &accumulateLinearForces.x);
					ImGui::DragFloat3("Angular##test2", &accumulateAngularForces.x);
					ImGui::TreePop();
				}

				//Gravity
				if (ImGui::TreeNode("Gravity")) {
					ImGui::Checkbox("Use Gravity", &useGravity);
					ImGui::DragFloat3("Gravity Vector", &gravity.x);
					ImGui::TreePop();
				}

				// Drag
				if (ImGui::TreeNode("Drag")) {
					ImGui::DragFloat("Linear##test3", &linearDrag, 0.01f, 0, 1);
					ImGui::DragFloat("Angular##test4", &angularDrag, 0.01f, 0, 1);
					ImGui::TreePop();
				}

			}
		};
	};
}