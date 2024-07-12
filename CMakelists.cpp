cmake_minimum_required(VERSION 3.21)
project(ONNX CXX)
 
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
 
set(CMAKE_CXX_FLAGS "-std=c++14 -mfma")
 
# Modify cmake module path if new .cmake files are required
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")
 
## System dependencies are found with CMake's conventions
set(Torch_DIR /home/yanglab/libtorch-cxx11-abi-shared-with-deps-1.9.1+cu111/libtorch/share/cmake/Torch)
find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)
message("OpenCV version is: ${OpenCV_VERSION}")
message(STATUS "PyTorch status:")
message(STATUS "libraries: ${TORCH_LIBRARIES}")
 
###########
## Build ##
###########
 
set(ONNX_INCLUDE_DIR /home/yanglab/onnxruntime-linux-x64-gpu-1.8.0/include)
set(ONNX_LIB_DIR /home/yanglab/onnxruntime-linux-x64-gpu-1.8.0/lib)
 
include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${ONNX_INCLUDE_DIR}
)
 
add_executable(onnx onnx.cpp)
# 链接OpenCV库
target_link_libraries(onnx ${OpenCV_LIBS} ${TORCH_LIBRARIES}
                      ${ONNX_LIB_DIR}/libonnxruntime.so 
                      ${ONNX_LIB_DIR}/libonnxruntime.so.1.8.0
                      ${ONNX_LIB_DIR}/libonnxruntime_providers_cuda.so
                      ${ONNX_LIB_DIR}/libonnxruntime_providers_shared.so)