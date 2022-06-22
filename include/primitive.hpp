/* 
 * Author: Felix Crazzolara
 */ 
#pragma once

#include <type_traits>
#include <string>
#include <functional>

#ifdef DEBUG_PRIMITIVE
#include <iostream>
#endif

#include "object.hpp"

namespace cs {

/* Type definitions */
using string = std::string;

class PrimitiveBase : public virtual Object {
public:

/* Constructors */
PrimitiveBase() {}

explicit PrimitiveBase(const PrimitiveBase& p) {
#ifdef DEBUG_PRIMITIVE
    std::cout << "PrimitiveBase(const PrimitiveBase& p) : this = " << (void*) this << std::endl;
#endif
}

/* Destructor */
~PrimitiveBase() override {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Entering ~PrimitiveBase() : this = " << (void*) this << std::endl;
    std::cout << "Not calling OBJECT_DESCTRUCT as PrimitiveBase is abstract." << std::endl;
    std::cout << "Leaving ~PrimitiveBase() : this = " << (void*) this << std::endl;
#endif
}

/* Miscellanous */
string to_string() const override {
    return "PrimitiveBase()";
}

virtual PrimitiveBase* clone() const = 0;

};

template <typename T>
requires std::is_arithmetic_v<std::remove_cvref_t<T>>
class Primitive : public PrimitiveBase {
public:

/* Constructors */
explicit Primitive(const T& v) : v_(v) {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Primitive(const T& v) : this = " << (void*) this << " v = " << v << std::endl;
#endif
}

explicit Primitive(const Primitive<T>& p) : v_(p.v_) {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Primitive(const T& v) : this = " << (void*) this << " v = " << p.v_ << std::endl;
#endif
}

/* Destructor */
~Primitive() override {
#ifdef DEBUG_PRIMITIVE
    std::cout << "Entering ~Primitive() : this = " << (void*) this << " v = " << v_ << std::endl;
#endif
    OBJECT_DESCTRUCT(this);
#ifdef DEBUG_PRIMITIVE
    std::cout << "Leaving ~Primitive() : this = " << (void*) this << " v = " << v_ << std::endl;
#endif
}

/* Assignment */
Primitive& operator=(const T& v) {
    v_ = v;

    return *this;
}

/* Getter */
T get() const {
    return v_;
}

/* Miscellanous */
bool is_equal(const Object* o) const override {
    return dynamic_cast<const Primitive<T>*>(o) != nullptr &&
           dynamic_cast<const Primitive<T>*>(o)->v_ == v_;
}

string to_string() const override {
    return std::to_string(v_);
}

// TODO: Use the Python hashing function instead
// https://stackoverflow.com/questions/2070276/where-can-i-find-source-or-algorithm-of-pythons-hash-function
size_t hash() const override {
    return hash_(v_);
}

PrimitiveBase* clone() const override {
    return new Primitive<T>(v_);
}

private:

T v_;
std::hash<T> hash_;

};

};
