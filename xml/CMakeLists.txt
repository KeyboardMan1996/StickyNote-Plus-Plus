cmake_minimum_required(VERSION 3.14)
project(XML)

set(SRC
	pugiconfig.hpp
	pugixml.cpp
	pugixml.hpp                                                   
)
add_definitions(-D_XML_)
add_library(XML SHARED ${SRC})
source_group("src" FILES ${SRC})
SET_BIN_DIR(XML XML)