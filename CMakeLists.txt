cmake_minimum_required(VERSION 3.6)
project(TNCG15_proj)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BINARY_DIR   ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH    ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR})

set(SOURCE_FILES main.cpp Renderer.cpp Renderer.h Pixel.cpp Pixel.h Ray.cpp Ray.h ColorRGB.cpp ColorRGB.h scene/Scene.cpp scene/Scene.h scene/Triangle.cpp scene/Triangle.h Sphere.cpp Sphere.h Camera.cpp Camera.h RayTracer.cpp RayTracer.h material/BaseMaterial.cpp material/BaseMaterial.h material/SpecularMaterial.cpp material/SpecularMaterial.h material/DiffuseMaterial.cpp material/DiffuseMaterial.h photon_mapping/PhotonMapper.cpp photon_mapping/PhotonMapper.h photon_mapping/Photon.cpp photon_mapping/Photon.h photon_mapping/PhotonKdTree.cpp photon_mapping/PhotonKdTree.h photon_mapping/PhotonOctree.cpp photon_mapping/PhotonOctree.h scene/BoundingBox.cpp scene/BoundingBox.h material/EmissionMaterial.cpp material/EmissionMaterial.h material/TransmissionMaterial.cpp material/TransmissionMaterial.h scene/Object3d.cpp scene/Object3d.h)
add_executable(TNCG15_proj ${SOURCE_FILES})
