cmake_minimum_required(VERSION 2.8.3)

project(MetaRuntime CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../CMake")

include(Precompiled)

set(HEADER_FILES
    VariantPolicy.h
    Argument.h
    ArgumentConfig.h
    Array.h
    ArrayVariantContainer.h
    ArrayWrapper.h
    ArrayWrapperBase.h
    ArrayWrapperContainer.h
    Constructor.h
    ConstructorInvoker.h
    ConstructorInvokerBase.h
    Destructor.h
    DestructorInvoker.h
    DestructorInvokerBase.h
    Enum.h
    EnumBase.h
    EnumContainer.h
    Field.h
    FieldGetter.h
    FieldGetterBase.h
    FieldSetter.h
    FieldSetterBase.h
    Function.h
    FunctionInvoker.h
    FunctionInvokerBase.h
    Global.h
    GlobalGetter.h
    GlobalGetterBase.h
    GlobalSetter.h
    GlobalSetterBase.h
    Invokable.h
    InvokableConfig.h
    Macros.h
    Meta.h
    MetaContainer.h
    MetaManager.h
    MetaProperty.h
    MetaTraits.h
    Method.h
    MethodInvoker.h
    MethodInvokerBase.h
    Object.h
    ObjectWrapper.h
    Precompiled.h
    ReflectionDatabase.h
    ReflectionModule.h
    RuntimeMetaProperties.h
    Type.h
    TypeCreator.h
    TypeConfig.h
    TypeData.h
    TypeInfo.h
    TypeID.h
    Variant.h
    VariantBase.h
    VariantContainer.h
)

set(SOURCE_FILES
    VariantContainer.cpp
    Argument.cpp
    ArrayWrapper.cpp
    Constructor.cpp
    Destructor.cpp
    Enum.cpp
    EnumBase.cpp
    Field.cpp
    Function.cpp
    Global.cpp
    Invokable.cpp
    MetaContainer.cpp
    MetaManager.cpp
    Method.cpp
    ObjectWrapper.cpp
    Precompiled.cpp
    ReflectionDatabase.cpp
    ReflectionModule.cpp
    Type.cpp
    TypeCreator.cpp
    TypeData.cpp
    Variant.cpp
    VariantBase.cpp
)



# allow use of Common and Library directories
include_directories(..)
include_directories(../Common)
include_directories(../Common/Lib)

# add json support
find_package(nlohmann_json CONFIG REQUIRED)

# add log support
find_package(spdlog CONFIG REQUIRED)
INCLUDE_DIRECTORIES(${spdlog_include_dirs})

# header files are superflous, but some IDEs (Visual Studio) don't include
# them in the solution explorer without them being added to the list of sources
add_library(MetaRuntime STATIC 
    ${HEADER_FILES} 
    ${SOURCE_FILES}


)
target_link_libraries(MetaRuntime PRIVATE nlohmann_json::nlohmann_json)

set_precompiled_header(MetaRuntime 
    Precompiled.h 
    Precompiled.cpp
)



if (MSVC)

    target_compile_options(MetaRuntime 
        PUBLIC
        # multi process compilation
        /MP
    )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else ()
    target_compile_options(MetaRuntime 
        PUBLIC
        -std=c++17
        -Wno-format-security
    )
endif ()

set(META_RUNTIME_INCLUDE_DIRS 
    "${CMAKE_CURRENT_LIST_DIR}"
    "${CMAKE_CURRENT_LIST_DIR}/../" 
	"${CMAKE_CURRENT_LIST_DIR}/../Common/"
    "${CMAKE_CURRENT_LIST_DIR}/../Common/Lib"
    CACHE STRING "Location of the runtime include directories."
)
