#pragma once
//#include <Brainstorm/io/logger.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <vector>
//#include <array>
//
//enum class SimplexType : uint8_t {
//	Null = 0, Line = 2, Triangle = 3, Tetrahedron = 4
//};
//
//class Simplex {
//public:
//	Simplex(const std::initializer_list<glm::vec3>& list) : type(SimplexType(list.size())) {
//		if (list.size() < static_cast<int>(SimplexType::Line)) {
//			BS::Logger::fatal("Simplex must have at least 2 points!");
//			this->type = SimplexType::Null;
//
//			return;
//		}
//		if (list.size() > static_cast<int>(SimplexType::Tetrahedron)) {
//			BS::Logger::fatal("Simplex must have maximum 4 points!");
//			this->type = SimplexType::Null;
//
//			return;
//		}
//
//		this->points = new glm::vec3[list.size()];
//		if (this->points == nullptr) {
//			BS::Logger::fatal("Could not allocate memory for Simplex.");
//			this->type = SimplexType::Null;
//
//			return;
//		}
//
//		std::copy(list.begin(), list.end(), this->points);
//	}
//	~Simplex() {
//		delete[] this->points;
//	}
//
//	const glm::vec3* getPoints() const {
//		return this->points;
//	}
//	const SimplexType getType() const {
//		return this->type;
//	}
//private:
//	SimplexType type;
//	glm::vec3* points = nullptr;
//};
//
//template<size_t Size>
//class ConvexCollider {
//private:
//	glm::vec3 getFurthestPoint(glm::vec3 direction) const {
//		glm::vec3 furthestPoint{};
//		float furthestDistance = -std::numeric_limits<float>::max();
//
//		glm::mat4 matrix = this->getMatrix();
//		direction = glm::normalize(glm::vec3(glm::inverse(matrix) * glm::vec4(direction, 1.0f)));
//
//		for (const glm::vec3& point : this->points) {
//			float distance = glm::dot(point, direction);
//			if (distance > furthestDistance) {
//				furthestDistance = distance;
//				furthestPoint = point;
//			}
//		}
//
//		return glm::vec3(matrix * glm::vec4(furthestPoint, 0.0f)) + this->position;
//	}
//	glm::vec3 getSupport(const ConvexCollider* other, const glm::vec3& direction) const {
//		return this->getFurthestPoint(direction) - other->getFurthestPoint(-direction);
//	}
//
//	std::pair<bool, Simplex> checkGjkCollision(const Ga)
//	inline const Simplex* lineCase(const glm::vec3* points, const SimplexType type) const {
//		
//	}
//public:
//	const Simplex* nextSimplex(const Simplex* simplex) {
//		switch (simplex->getType()) {
//		case SimplexType::Line:
//			return this->lineCase(simplex->getPoints(), simplex->getType());
//		case SimplexType::Triangle:
//			return this->triangleCase(simplex->getPoints(), simplex->getType());
//		case SimplexType::Tetrahedron:
//			return this->tetrahedronCase(simplex->getPoints(), simplex->getType());
//		default:
//			BS::Logger::error("ConvexCollider::nextSimplex took Simplex that is out of SimplexType range. No action performed.");
//			break;
//		}
//
//		return simplex;
//	}
//public:
//	glm::vec3 position{}, rotation{}, scale{ 1.0f };
//	std::array<glm::vec3, Size> points{};
//
//	glm::mat4 getMatrix() const {
//		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(this->position));
//		matrix = glm::rotate(matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//		matrix = glm::rotate(matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//		matrix = glm::rotate(matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
//		matrix = glm::scale(matrix, this->scale);
//
//		return matrix;
//	}
//};