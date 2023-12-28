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
    GUI/ChangeModbusDeviceParameters
    GUI/AumaTestGUI
    GUI/FirmwareDetailGUI
    GUI/ActuatorResponseGUI

    ApplicationConstant
    SystemResult
    Singleton
    Device
    DeviceInterface
    Logger
    Modbus


    JSON
    JSON/JSONToModbusRegisterConventer
    JSON/JSONToModbusMapConverter
    JSON/JSONToDevicesConventer

    Modbus/ModbusRegister
    Modbus/ModbusStrategy
    Modbus/ModbusConnectionParameters
    Modbus/ModbusController
    Modbus/ModbusClientStrategy
    Modbus/ModbusTCPClient
    Modbus/ModbusRegisterTypeMapper
    Modbus/ModbusStateMapper
    Modbus/ModbusDeviceInterface

    Tests
    Tests/TestInterface
    Tests/LocalHostTest
    Tests/ActuatorResponseValidation
    Tests/ProcessControllerOperationAndStability
    Tests/FirmwareDetailVerification

    Images

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
        Modbus/ModbusStateMapper/ModbusStateMapper.hpp Modbus/ModbusStateMapper/ModbusStateMapper.cpp
        JSON/JSONToModbusMapConverter/JSONToModbusMapConverter.hpp JSON/JSONToModbusMapConverter/JSONToModbusMapConverter.cpp
        JSON/TestData.json
        JSON/Devices.json

        Images/LogoAuma.png
        Images/ConnectedState.png
        Images/DisconnectedState.png


        Modbus/ModbusRegister/ModbusRegister.hpp Modbus/ModbusRegister/ModbusRegister.cpp
        Tests/TestInterface/TestInterface.hpp Tests/TestInterface/TestInterface.cpp
        Modbus/ModbusDeviceInterface/ModbusDeviceInterface.hpp Modbus/ModbusDeviceInterface/ModbusDeviceInterface.cpp
        JSON/JSONToModbusRegisterConventer/JSONToModbusRegisterConventer.hpp JSON/JSONToModbusRegisterConventer/JSONToModbusRegisterConventer.cpp
        Tests/LocalHostTest/LocalHostTest.hpp Tests/LocalHostTest/LocalHostTest.cpp
        JSON/JSONToDevicesConventer/JSONToDevicesConventer.hpp JSON/JSONToDevicesConventer/JSONToDevicesConventer.cpp
        GUI/ChangeModbusDeviceParameters/ChangeModbusDeviceParameters.hpp GUI/ChangeModbusDeviceParameters/ChangeModbusDeviceParameters.cpp GUI/ChangeModbusDeviceParameters/ChangeModbusDeviceParameters.ui
        GUI/AumaTestGUI/AumaTestGUI.hpp GUI/AumaTestGUI/AumaTestGUI.cpp GUI/AumaTestGUI/AumaTestGUI.ui
        Tests/ActuatorResponseValidation/ActuatorResponseValidation.hpp Tests/ActuatorResponseValidation/ActuatorResponseValidation.cpp
        Tests/ProcessControllerOperationAndStability/ProcessControllerOperationAndStability.hpp Tests/ProcessControllerOperationAndStability/ProcessControllerOperationAndStability.cpp
        Tests/FirmwareDetailVerification/FirmwareDetailVerification.hpp Tests/FirmwareDetailVerification/FirmwareDetailVerification.cpp
        GUI/FirmwareDetailGUI/FirmwareDetailGUI.hpp GUI/FirmwareDetailGUI/FirmwareDetailGUI.cpp GUI/FirmwareDetailGUI/FirmwareDetailGUI.ui
        GUI/ActuatorResponseGUI/ActuatorResponseGUI.hpp GUI/ActuatorResponseGUI/ActuatorResponseGUI.cpp GUI/ActuatorResponseGUI/ActuatorResponseGUI.ui




    )


# Copy the AumaLogo.png file
set(AUMA_LOGO_SOURCE "${CMAKE_SOURCE_DIR}/Images/LogoAuma.png")
set(AUMA_LOGO_DESTINATION "${CMAKE_BINARY_DIR}/Images/LogoAuma.png")
set(CONNECTED_LOGO_SOURCE "${CMAKE_SOURCE_DIR}/Images/ConnectedState.png")
set(CONNECTED_LOGO_DESTINATION "${CMAKE_BINARY_DIR}/Images/ConnectedState.png")
set(DISCONNECTED_LOGO_SOURCE "${CMAKE_SOURCE_DIR}/Images/DisconnectedState.png")
set(DISCONNECTED_LOGO_DESTINATION "${CMAKE_BINARY_DIR}/Images/DisconnectedState.png")
add_custom_command(
    TARGET Modbus-Industrial-Diagnostic-Tool POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${AUMA_LOGO_SOURCE} ${AUMA_LOGO_DESTINATION}
    COMMAND ${CMAKE_COMMAND} -E copy ${CONNECTED_LOGO_SOURCE} ${CONNECTED_LOGO_DESTINATION}
    COMMAND ${CMAKE_COMMAND} -E copy ${DISCONNECTED_LOGO_SOURCE} ${DISCONNECTED_LOGO_DESTINATION}
)

# Define a list containing pairs of source and destination paths for each JSON file
set(JSON_FILES
    "${CMAKE_SOURCE_DIR}/JSON/TestData.json" "${CMAKE_BINARY_DIR}/JSON/TestData.json"
    "${CMAKE_SOURCE_DIR}/JSON/Devices.json" "${CMAKE_BINARY_DIR}/JSON/Devices.json"
)

# Loop through the list and copy each JSON file to the build directory during build
list(LENGTH JSON_FILES JSON_FILES_COUNT)
math(EXPR JSON_FILES_COUNT "${JSON_FILES_COUNT} - 1")
foreach(I RANGE 0 ${JSON_FILES_COUNT} 2)
    math(EXPR J "${I} + 1")
    list(GET JSON_FILES ${I} SOURCE_JSON)
    list(GET JSON_FILES ${J} DESTINATION_JSON)
    add_custom_command(
        TARGET Modbus-Industrial-Diagnostic-Tool POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_JSON} ${DESTINATION_JSON}
    )
endforeach()

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
