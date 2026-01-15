// maggie.h
// maggie: Maggie's Demonstration Library
// Copyright (C) 2026 Jeffrey M. Engelmann

#pragma once
#ifndef MAGGIE_H
#define MAGGIE_H

#include <Windows.h>
#include <cstdint>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif  // __cplusplus

#ifdef MAGGIE_SHARED
#ifdef MAGGIE_EXPORTS
#define MAGGIE_API EXTERN_C __declspec(dllexport)
#else
#define MAGGIE_API EXTERN_C __declspec(dllimport)
#endif  // MAGGIE_EXPORTS
#else
#define MAGGIE_API EXTERN_C
#endif  // MAGGIE_SHARED

#ifdef _WIN64
#define MAGGIE_CC
#else
#define MAGGIE_CC __cdecl
#endif  // _WIN64

MAGGIE_API HRESULT MAGGIE_CC hi_maggie(int32_t n);

#endif  // MAGGIE_H

///////////////////////////////////////////////////////////////////////////////
