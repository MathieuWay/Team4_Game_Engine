#pragma once
#include <team4_game_engine/engine/mathematics/vector3d.hpp>
#include <team4_game_engine/engine/mathematics/matrix4.hpp>
#include <iostream>
#include <sstream>
#include <random>

namespace team4_game_engine::engine::mathematics {
    void Vector3D::Write(serialization::OutputMemoryStream& stream) {
        stream.Write(x);
        stream.Write(y);
        stream.Write(z);
    }

    void Vector3D::Read(serialization::InputMemoryStream& stream) {
        x = stream.Read<float>();
        y = stream.Read<float>();
        z = stream.Read<float>();
    }

    Vector3D Vector3D::scalarMultiplication(float scalar) {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }
  /*  Vector3D Vector3D::operator*(const float scalar) {
        return Vector3D(x * scalar, y * scalar, z * scalar);
    }
    Vector3D& Vector3D::operator*=(const float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }*/


    Vector3D Vector3D::sumVector(Vector3D v) {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }
    /*Vector3D Vector3D::operator+(const Vector3D v) {
        return Vector3D(x + v.x, y + v.y, z + v.z);
    }
    Vector3D& Vector3D::operator+=(const Vector3D v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }*/

    Vector3D Vector3D::subVector(Vector3D v) {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }
    /*Vector3D Vector3D::operator-(const Vector3D v) {
        return Vector3D(x - v.x, y - v.y, z - v.z);
    }
    Vector3D& Vector3D::operator-=(const Vector3D v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }*/

    Vector3D Vector3D::VectorMultiplication(Vector3D v) {
        return Vector3D(x * v.x, y * v.y, z * v.z);
    }
    /*Vector3D Vector3D::operator*(const Vector3D v) {
        return Vector3D(x * v.x, y * v.y, z * v.z);
    }
    Vector3D& Vector3D::operator*=(const Vector3D v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }*/

    Vector3D Vector3D::invert() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    float Vector3D::magnitude() {
        return sqrtf(x * x + y * y + z * z);
    }

    float Vector3D::squareMagnitude() {
        return x * x + y * y + z * z;
    }

    Vector3D Vector3D::normalize() {
        float m = this->magnitude();
        if (m > 0) {
            (*this) = this->scalarMultiplication(1 / m);
        }
        return *this;
    }

    Vector3D Vector3D::sumScalarVector(Vector3D v, float scalar) {
        return  Vector3D(x + v.x * scalar, y + v.y * scalar, z + v.z * scalar);
    }

    float Vector3D::scalarProduct(Vector3D v) {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3D Vector3D::vectorProduct(Vector3D v) {
        return Vector3D(y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }

    void Vector3D::printVector() {
        std::cout << "x= " << x << " y= " << y << " z = " << z << std::endl;
    }

    std::string Vector3D::GetVectorData() {
        std::stringstream data;
        data << "x: " << (float)((int)(x * 1000 + .5)) / 1000 << " y: " << (float)((int)(y * 1000 + .5)) / 1000 << " z: " << (float)((int)(z * 1000 + .5)) / 1000;
        return data.str();
    }

    Vector3D Vector3D::Random(float xMin, float xMax, float yMin, float yMax, float zMin, float zMax) {
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_real_distribution<> x(xMin, xMax);
        std::uniform_real_distribution<> y(yMin, yMax);
        std::uniform_real_distribution<> z(zMin, zMax);
        return Vector3D(x(e2), y(e2), z(e2));
    }

    Vector3D Vector3D::localToWorld(const Vector3D& local, const Matrix4& transform) {
        return transform.transform(local);
    }

    Vector3D Vector3D::worldToLocal(const Vector3D& world, const Matrix4& transform) {
        return transform.transformInverse(world);
    }

    Vector3D Vector3D::localToWorldDirn(const Vector3D& local, const Matrix4& transform) {
        return transform.transformDirection(local);
    }

    Vector3D Vector3D::worldToLocalDirn(const Vector3D& world, const Matrix4& transform) {
        return transform.transformInverseDirection(world);
    }
}