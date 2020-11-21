#pragma once
#include <string>
#include <team4_game_engine/serialization/input_memory_stream.hpp>
#include <team4_game_engine/serialization/output_memory_stream.hpp>
#include <dear_imgui/imgui.h>
#include <glm/glm.hpp>
#include <random>

namespace team4_game_engine::engine::mathematics {
    float static RandomFloat(float min, float max) {
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_real_distribution<> randfloat(min, max);
        return randfloat(e2);
    }
    class Matrix4;
    class Vector3D {
    public:
	    float x, y, z;

	    Vector3D() {
		    x = 0;
		    y = 0;
		    z = 0;
	    };

	    Vector3D(float _x, float _y, float _z = 0) {
		    x = _x;
		    y = _y;
		    z = _z;
	    }


        virtual void Write(serialization::OutputMemoryStream& stream);
        virtual void Read(serialization::InputMemoryStream& stream);
        // Cast Operator
        operator glm::vec3() const { return glm::vec3(x, y, z); };
        operator ImVec2 () const { return ImVec2(x, y); };

        /*
        Multiplies this Vector with a given scalar and return the result in a new Vector
       */
        virtual Vector3D scalarMultiplication(float scalar);
        /*Vector3D operator*(const float scalar);
        Vector3D& operator*=(const float scalar);*/

        /*
            Sums this Vector with a given Vector and return the result in a new Vector
        */
        virtual Vector3D sumVector(Vector3D v);
        /*Vector3D operator+(const Vector3D v);
        Vector3D& operator+=(const Vector3D v);*/

        /*
             Substracts this Vector with a given Vector and return the result in a new Vector
        */
        virtual Vector3D subVector(Vector3D v);
        /*Vector3D operator-(const Vector3D v);
        Vector3D& operator-=(const Vector3D v);*/

        /*
            Multiplied each components of this Vector with the components of given Vector and return the result in a new Vector
        */
        virtual Vector3D VectorMultiplication(Vector3D v);
       /* Vector3D operator*(const Vector3D v);
        Vector3D& operator*=(const Vector3D v);*/

        /*
        Invert the given Vector
        */
        virtual Vector3D invert();

        /*
        return the magnitude of the given Vector
        */
        virtual float magnitude();

        /*
        return the squared magnitude of this Vector
        */
        virtual float squareMagnitude();

        /*
        Turns a non-zero vector to a vector with a magnitude of 1
        */
        virtual Vector3D normalize();

        /*
           Sums a given Vector with this Vector multiplied by a given scalar and return the result in a new Vector
        */
        virtual Vector3D sumScalarVector(Vector3D v, float scalar);

        /*
            Calculates and return the scalar Product of this Vector and a given Vector
        */
        virtual float scalarProduct(Vector3D v);

        /*
            Calculates and return the Vector Product of this Vector and a given Vector
        */
        virtual Vector3D vectorProduct(Vector3D v);

        virtual void printVector();
        virtual std::string GetVectorData();
        // Random Vector
        static Vector3D Random(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
        /*
        make a Orthonormal Basis with the given Vector
        */
        static void makeOrthonormalBasis(Vector3D* u, Vector3D* v, Vector3D* w);

        static Vector3D localToWorld(const Vector3D& local, const Matrix4& transform);

        static Vector3D worldToLocal(const Vector3D& world, const Matrix4& transform);

        static Vector3D localToWorldDirn(const Vector3D& local, const Matrix4& transform);

        static Vector3D worldToLocalDirn(const Vector3D& world, const Matrix4& transform);
    };
}