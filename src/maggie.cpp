// maggie.cpp
// maggie: Maggie's Demonstration Library
// Copyright (C) 2026 Jeffrey M. Engelmann

#include "maggie/maggie.h"
#include <fmt/core.h>
#include <wil/resource.h>

// Use an SRWLOCK to demonstrate dependency on wil
static wil::srwlock s_srw_lock;

HRESULT hi_maggie(int32_t const n) try
{
    auto const lock = s_srw_lock.lock_exclusive();

    if (n < 0) return -1;

    for (int32_t i = 0; i < n; ++i)
    {
        for (int32_t j = 0; j <= i; ++j)
        {
            if (j < i) fmt::print("Maggie ");
            else fmt::print("Maggie\n");
        }
    }
    fmt::print("Boykin Spaniel!\n");

    return S_OK;
}
CATCH_RETURN();

///////////////////////////////////////////////////////////////////////////////
