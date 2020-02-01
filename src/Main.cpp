#define _USE_MATH_DEFINES
#include "App.h"

#include "PathIntegrator.h"

#include <glm/glm.hpp>

#include <iostream>
#include <cmath>

#include "Sampler.h"
#include "Stratified.h"
#include "Sampling.h"

// TODO(achal): Make a debug symbol for Hikari

int main()
{
    // Hikari::App app;
    // app.Start();

    Hikari::StratifiedSampler ss(4u, 4u, true, 2u);
    return 0;
}
