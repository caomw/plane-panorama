include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/)

file(GLOB_RECURSE h_s *.h)
file(GLOB_RECURSE x_s *.cpp)
file(GLOB_RECURSE u_s *.ui)

QT4_WRAP_UI(u_h_s ${u_s})
doQtMOC(h_s moc_s)

add_library(RexImp ${h_s} ${x_s} ${moc_s} ${u_h_s})

target_link_libraries(RexImp ${LIBS})

