cmake_minimum_required(VERSION 3.14)
project(GreeterGrpc)

find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTO_BASENAME greeter)
set(PROTO_SRC ${PROTO_BASENAME}.proto)

set(PROTO_GEN_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_BASENAME}.pb.cc")
set(PROTO_GEN_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_BASENAME}.pb.h")
set(GRPC_GEN_SRC "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_BASENAME}.grpc.pb.cc")
set(GRPC_GEN_HDR "${CMAKE_CURRENT_BINARY_DIR}/${PROTO_BASENAME}.grpc.pb.h")

add_custom_command(
  OUTPUT ${PROTO_GEN_SRC} ${PROTO_GEN_HDR} ${GRPC_GEN_SRC} ${GRPC_GEN_HDR}
  COMMAND protobuf::protoc
  ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
       --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
       --plugin=protoc-gen-grpc=$<TARGET_FILE:gRPC::grpc_cpp_plugin>
       -I ${CMAKE_CURRENT_SOURCE_DIR}
       ${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_SRC}
  DEPENDS ${PROTO_SRC}
)

add_executable(greeter_server greeter_server.cpp
  ${PROTO_GEN_SRC} ${PROTO_GEN_HDR}
  ${GRPC_GEN_SRC} ${GRPC_GEN_HDR}
)
target_include_directories(greeter_server PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(greeter_server gRPC::grpc++ protobuf::libprotobuf)

add_executable(greeter_client greeter_client.cpp
  generic_record_impl.cpp
  greeter_client_factory.cpp
  main.cpp
  ${PROTO_GEN_SRC} ${PROTO_GEN_HDR}
  ${GRPC_GEN_SRC} ${GRPC_GEN_HDR}
)
target_include_directories(greeter_client PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(greeter_client gRPC::grpc++ protobuf::libprotobuf)

