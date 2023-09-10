cmake_minimum_required(VERSION 3.5)

project(MyFinance VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_USE_FIXED_QT_ADD_RESOURCE_BASE TRUE)  #
find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core) # special case
find_package(Qt6 ${PROJECT_VERSION} CONFIG OPTIONAL_COMPONENTS Gui Widgets OpenGL OpenGLWidgets Multimedia Quick Qml QuickTest Test) # special case
find_package(Qt6Charts)
#










find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Charts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        structurs.h
        currency_window.h
        currency_window.cpp
        currency_window.ui
        currencytablemodel.h
        currencytablemodel.cpp
        currency_add.h
        currency_add.cpp
        currency_add.ui
        accounts_window.h
        accounts_window.cpp
        accounts_window.ui
        accounttablemodel.h
        accounttablemodel.cpp
        account_add.h
        account_add.cpp
        account_add.ui
        income_window.h
        income_window.cpp
        income_window.ui
        expense_window.h
        expense_window.cpp
        expense_window.ui
        transfer_window.h
        transfer_window.cpp
        transfer_window.ui
        incometablemodel.h
        incometablemodel.cpp
        expensetablemodel.h
        expensetablemodel.cpp
        transfertablemodel.h
        transfertablemodel.cpp
        income_add.h
        income_add.cpp
        income_add.ui
        expence_add.h
        expence_add.cpp
        expence_add.ui
        transfer_add.h
        transfer_add.cpp
        transfer_add.ui
        mainwindowtablemodel.h
        mainwindowtablemodel.cpp
        Res.qrc
        turnover.cpp
        turnover.ui
        turnover.h
        turnovertablemodel.h
        turnovertablemodel.cpp
        chart_incomes.h
        chart_incomes.cpp
        chart_incomes.ui
        chart_expenses.h
        chart_expenses.cpp
        chart_expenses.ui
        chart_accounts.h
        chart_accounts.cpp
        chart_accounts.ui
        info.h
        info.cpp
        info.ui


)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MyFinance
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MyFinance APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MyFinance SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MyFinance
            ${PROJECT_SOURCES}
        )
    endif()
endif()

#target_link_libraries(MyFinance PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
# Use the Widgets and Charts modules from Qt 5.
target_link_libraries(MyFinance PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Charts)

set_target_properties(MyFinance PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)




install(TARGETS MyFinance
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MyFinance)
endif()
