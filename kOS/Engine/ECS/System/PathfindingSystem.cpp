/******************************************************************/
/*!
\file      PathfindingSystem.cpp
\author    Yeo See Kiat Raymond, seekiatraymond.yeo, 2301268
\par       seekiatraymond.yeo@digipen.edu
\date      October 3, 2025
\brief     This file contains the pathfinding system to draw
			Octree wireframe


Copyright (C) 2025 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "Config/pch.h"
#include "PathfindingSystem.h"
#include "Inputs/Input.h"
#include "Pathfinding/OctreeGrid.h"
#include "Graphics/GraphicsManager.h"

#include <random>

namespace ecs {
	Octrees::Graph waypoints;
	Octrees::Octree octree;
	bool testing = true;
	bool testing2 = false;

	bool test = true;

	float maxTimer = 5.f;
	float currentTimer = 0.f;

	void PathfindingSystem::Init() {
		
	}

	void PathfindingSystem::Update() {
		ECS* ecs = ECS::GetInstance();
		const auto& entities = m_entities.Data();

		if (currentTimer < maxTimer) {
			currentTimer += ecs->m_GetDeltaTime();
			//std::cout << "TIMER: " << currentTimer << std::endl;
		}

		for (EntityID id : entities) {
			TransformComponent* trans = ecs->GetComponent<TransformComponent>(id);
			NameComponent* name = ecs->GetComponent<NameComponent>(id);
			//OctreeGeneratorComponent* oct = ecs->GetComponent<OctreeGeneratorComponent>(id);

			if (name->hide) { continue; }

			// Move all pathfinders
			const auto& otherEntities = m_entities.Data();
			for (EntityID otherId : otherEntities) {
				if (auto* pathfinderTarget = ecs->GetComponent<PathfinderTargetComponent>(otherId)) {
					if (auto* pathfinderComp = ecs->GetComponent<PathfinderComponent>(id)) {
						if (auto* pathfinderTrans = ecs->GetComponent<TransformComponent>(id)) {
							if (auto* pathfinderTargetTrans = ecs->GetComponent<TransformComponent>(otherId)) {
								if (pathfinderComp->chase) {
									if (currentTimer >= maxTimer) {
										Octrees::OctreeNode closestNodeFrom = octree.FindClosestNode(pathfinderTrans->LocalTransformation.position);
										Octrees::OctreeNode closestNodeTarget = octree.FindClosestNode(pathfinderTargetTrans->LocalTransformation.position);
										//octree.graph.AStar(&closestNodeFrom, &closestNodeTarget);

										if (test) {
											for (Octrees::Node nod : octree.graph.nodes) {
												if (nod.id == 88) {
													for (Octrees::Edge ed : nod.edges) {
														//std::cout << ed.b->octreeNode.bounds.center.x << ", "
														//	<< ed.b->octreeNode.bounds.center.y << ", "
														//	<< ed.b->octreeNode.bounds.center.z << std::endl;
													}
												}
											}
											test = false;
										}

										currentTimer = 0.f;
									}
									break;
								}
							}
						}
					}
				}
			}

			if (auto* oct = ecs->GetComponent<OctreeGeneratorComponent>(id)) {
				if (oct->drawWireframe) {
					if (testing) {
						testing = false;
						octree = Octrees::Octree(2.f, waypoints);
					}
					//octree = Octrees::Octree(1.f, waypoints);
					octree.root.DrawNode();
					// DRAWING TAKES A LOT OF LATENCY
					octree.graph.DrawGraph();
				}
				else {
					testing = true;
				}
			}

		}

		if (!testing2) {
			//octree.graph.AStar(currentNodePos, octree.graph.nodes[0].octreeNode);



			testing2 = true;
		}
	}
}