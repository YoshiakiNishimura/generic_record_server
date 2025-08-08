cmake_minimum_required(VERSION 3.14)
project(GreeterGrpc)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTO_BASENAME sample)
set(PROTO_PRIMITIV primitive_types)
set(PROTO_COMPLEX complex_types)
set(PROTO_FILES proto/${PROTO_BASENAME}.proto proto/${PROTO_PRIMITIV}.proto
                proto/${PROTO_COMPLEX}.proto)

set(PROTO_GEN_SRCS "")
set(PROTO_GEN_HDRS "")
set(GRPC_GEN_SRCS "")
set(GRPC_GEN_HDRS "")
set(PROTO_GEN_ALL "")

foreach(PROTO_PATH ${PROTO_FILES})
  get_filename_component(PROTO_FILENAME ${PROTO_PATH} NAME_WE)
  set(PROTO_GEN_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_FILENAME}.pb.cc")
  set(PROTO_GEN_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_FILENAME}.pb.h")
  list(APPEND PROTO_GEN_SRCS ${PROTO_GEN_SRC})
  list(APPEND PROTO_GEN_HDRS ${PROTO_GEN_HDR})
  set(GRPC_GEN_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_FILENAME}.grpc.pb.cc")
  set(GRPC_GEN_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_FILENAME}.grpc.pb.h")
  list(APPEND GRPC_GEN_SRCS ${GRPC_GEN_SRC})
  list(APPEND GRPC_GEN_HDRS ${GRPC_GEN_HDR})
  list(APPEND PROTO_GEN_ALL ${GRPC_GEN_SRC} ${GRPC_GEN_HDR})
  add_custom_command(
    OUTPUT ${PROTO_GEN_SRC} ${PROTO_GEN_HDR} ${GRPC_GEN_SRC} ${GRPC_GEN_HDR}
    COMMAND
      protobuf::protoc ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
      --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
      --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin> -I
      ${CMAKE_CURRENT_SOURCE_DIR}/proto
      ${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_PATH}
    DEPENDS ${PROTO_PATH})
endforeach()
add_custom_target(proto_gen DEPENDS ${PROTO_GEN_ALL})
add_executable(
  rpc_server
  ${CMAKE_CURRENT_SOURCE_DIR}/server/rpc_server.cpp ${PROTO_GEN_SRCS}
  ${PROTO_GEN_HDRS} ${GRPC_GEN_SRCS} ${GRPC_GEN_HDRS})
add_dependencies(rpc_server proto_gen)
target_include_directories(rpc_server PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(rpc_server gRPC::grpc++ protobuf::libprotobuf)

add_executable(
  plugin_loader
  ${CMAKE_CURRENT_SOURCE_DIR}/plugin_loader/plugin.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/udf/udf_loader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/udf/generic_record_impl.cpp)
target_include_directories(
  plugin_loader PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                         ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(plugin_loader gRPC::grpc++ protobuf::libprotobuf)

add_executable(
  rpc_client
  rpc_client.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/udf/generic_record_impl.cpp
  rpc_client_factory.cpp
  main.cpp
  ${PROTO_GEN_SRCS}
  ${PROTO_GEN_HDRS}
  ${GRPC_GEN_SRCS}
  ${GRPC_GEN_HDRS})

add_library(
  plugin_api SHARED
  rpc_client.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/udf/generic_record_impl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/udf/plugin_api_impl.cpp
  rpc_client_factory.cpp
  ${PROTO_GEN_SRCS}
  ${PROTO_GEN_HDRS}
  ${GRPC_GEN_SRCS}
  ${GRPC_GEN_HDRS})

target_include_directories(
  plugin_api
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
          ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(plugin_api gRPC::grpc++ protobuf::libprotobuf)

target_include_directories(
  rpc_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
                     ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(rpc_client gRPC::grpc++ protobuf::libprotobuf)
