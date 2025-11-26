#pragma once

#include "transform.h"

struct  Rigibody
{
    float mass = 1.0f;

    vec::Vector2 velocity = { 0.0f, 0.0f };
    vec::Vector2 force = { 0.0f, 0.0f };

    float angularVelocity = 0.0f;
    float torque = 0.0f;
};