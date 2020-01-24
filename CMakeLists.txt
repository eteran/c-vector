cmake_minimum_required(VERSION 3.0.0)
project(c-vector VERSION 0.1.0)

add_library(c-vector INTERFACE)

target_sources(stdcxx-headers INTERFACE 
	cvector.h
)
