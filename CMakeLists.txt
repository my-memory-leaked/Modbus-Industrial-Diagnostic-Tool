cmake_minimum_required(VERSION 3.5)

project(Modbus-Industrial-Diagnostic-Tool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS SerialBus)

set(PROJECT_SOURCES
        main.cpp
)
include_directories(
    GUI/MainGUI
    GUI/AddModbusDeviceGUI

    ApplicationConstant
    SystemResult
    Singleton
    DeviceInterface
    Logger
    Modbus

    JSON

    Modbus/ModbusStrategy
    Modbus/ModbusConnectionParameters
    Modbus/ModbusController
    Modbus/ModbusClientStrategy
    Modbus/ModbusTCPClient
    Modbus/ModbusRegisterTypeMapper
    Modbus/ModbusStateMapper
    Modbus/ModbusParser


    ExceptionHandling/ModbusException

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Modbus-Industrial-Diagnostic-Tool
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        GUI/MainGUI/MainGUI.hpp GUI/MainGUI/MainGUI.cpp GUI/MainGUI/MainGUI.ui
        GUI/AddModbusDeviceGUI/AddModbusDeviceGUI.hpp GUI/AddModbusDeviceGUI/AddModbusDeviceGUI.cpp GUI/AddModbusDeviceGUI/AddModbusDeviceGUI.ui


        ApplicationConstant/ApplicationConstant.hpp

        ExceptionHandling/ModbusException/ModbusException.hpp ExceptionHandling/ModbusException/ModbusException.cpp
        Modbus/ModbusConnectionParameters/ModbusConnectionParameters.hpp Modbus/ModbusConnectionParameters/ModbusConnectionParameters.cpp
        Modbus/ModbusStrategy/ModbusStrategy.hpp Modbus/ModbusStrategy/ModbusStrategy.cpp
        Singleton/Singleton.hpp
        SystemResult/SystemResult.hpp
        DeviceInterface/DeviceInterface.hpp
        Modbus/ModbusController/ModbusController.hpp Modbus/ModbusController/ModbusController.cpp
        Logger/Logger.hpp Logger/Logger.cpp
        Modbus/ModbusTCPClient/ModbusTCPClient.hpp Modbus/ModbusTCPClient/ModbusTCPClient.cpp
        DeviceInterface/DeviceInterface.cpp
        Modbus/ModbusClientStrategy/ModbusClientStrategy.hpp Modbus/ModbusClientStrategy/ModbusClientStrategy.cpp
        Modbus/ModbusRegisterTypeMapper/ModbusRegisterTypeMapper.hpp Modbus/ModbusRegisterTypeMapper/ModbusRegisterTypeMapper.cpp
        Modbus/ModbusParser/ModbusParser.hpp Modbus/ModbusParser/ModbusParser.cpp
        Modbus/ModbusStateMapper/ModbusStateMapper.hpp Modbus/ModbusStateMapper/ModbusStateMapper.cpp
        JSON/JsonToModbusConverter.hpp JSON/JsonToModbusConverter.cpp
        JSON/TestData.json


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Modbus-Industrial-Diagnostic-Tool APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Modbus-Industrial-Diagnostic-Tool SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Modbus-Industrial-Diagnostic-Tool
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Modbus-Industrial-Diagnostic-Tool PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Modbus-Industrial-Diagnostic-Tool PRIVATE Qt6::SerialBus)

set_target_properties(Modbus-Industrial-Diagnostic-Tool PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Modbus-Industrial-Diagnostic-Tool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Modbus-Industrial-Diagnostic-Tool)
endif()
