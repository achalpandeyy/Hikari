cmake_minimum_required (VERSION 3.14.5)

project (Hikari)

set (CMAKE_CXX_STANDARD 14)

############################################################################
# glm
############################################################################

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/ext/glm/)

############################################################################
# tbb
############################################################################

set (CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/ext/embree/tbb)
find_package(TBB REQUIRED)

############################################################################
# Embree
############################################################################

set (embree_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/embree)
find_package (embree 3.0 REQUIRED)
include_directories(${EMBREE_INCLUDE_DIRS})

############################################################################
# tinyobjloader
############################################################################

include_directories (
	${CMAKE_CURRENT_SOURCE_DIR}/ext/tinyobjloader
)

############################################################################
# Hikari
############################################################################

include_directories (src/)

set (HIKARI_SOURCE

	# BxDFs
	src/BxDFs/OrenNayar.cpp

	# Core
	src/Core/BSDF.cpp
	src/Core/BxDF.cpp
	src/Core/Camera.cpp
	src/Core/Integrator.cpp
	src/Core/Interaction.cpp
	src/Core/Light.cpp
	src/Core/Sampler.cpp
	src/Core/Scene.cpp

	# Integrators
	src/Integrators/DirectLighting.cpp
	src/Integrators/Path.cpp
	src/Integrators/Whitted.cpp

	# Lights
	src/Lights/Directional.cpp
	src/Lights/Diffuse.cpp
	src/Lights/Point.cpp
	src/Lights/Spot.cpp

	# Materials
	src/Materials/Matte.cpp

	# Math
	src/Math/Transform.cpp

	# Shapes
	src/Shapes/Sphere.cpp
	src/Shapes/TriangleMesh.cpp

	src/App.cpp
	src/BlockGenerator.cpp

	src/ImageBlock.cpp
	src/Main.cpp

	src/ObjLoader.cpp
	src/Renderer.cpp
)

add_executable (Hikari ${HIKARI_SOURCE})

# Copy embree3.dll as a post build step
add_custom_command(TARGET Hikari POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${CMAKE_SOURCE_DIR}/ext/embree/bin/embree3.dll"      # <--this is in-file
        "${CMAKE_BINARY_DIR}/$<CONFIG>")

# Copy tbb.dll as a post build step
add_custom_command(TARGET Hikari POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${CMAKE_SOURCE_DIR}/ext/embree/tbb/bin/intel64/vc14/tbb.dll"      # <--this is in-file
        "${CMAKE_BINARY_DIR}/$<CONFIG>")

# Copy tbb_debug.dll as a post build step
add_custom_command(TARGET Hikari POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${CMAKE_SOURCE_DIR}/ext/embree/tbb/bin/intel64/vc14/tbb_debug.dll"      # <--this is in-file
        "${CMAKE_BINARY_DIR}/$<CONFIG>")

# Copy tbbmalloc.dll as a post build step
add_custom_command(TARGET Hikari POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${CMAKE_SOURCE_DIR}/ext/embree/tbb/bin/intel64/vc14/tbbmalloc.dll"      # <--this is in-file
        "${CMAKE_BINARY_DIR}/$<CONFIG>")

# Copy tbbmalloc_debug.dll as a post build step
add_custom_command(TARGET Hikari POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${CMAKE_SOURCE_DIR}/ext/embree/tbb/bin/intel64/vc14/tbbmalloc_debug.dll"      # <--this is in-file
        "${CMAKE_BINARY_DIR}/$<CONFIG>")

target_link_libraries (Hikari ${TBB_IMPORTED_TARGETS} ${EMBREE_LIBRARY})