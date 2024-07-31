#pragma once

#ifndef COMMON_VALUES
#define COMMON_VALUES

/// <summary>
/// Should match the number in the fragment shader
/// </summary>
const int MAX_POINT_LIGHTS = 3;

/// <summary>
/// Should match the number in the fragment shader
/// </summary>
const int MAX_SPOT_LIGHTS = 3;

/// <summary>
/// Multiply an angle in degrees by this to get angle in radians
/// </summary>
const float TORADIANS = 3.14159265f / 180.0f;

#endif