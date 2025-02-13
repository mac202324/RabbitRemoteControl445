## Compiling for Linux
Author: Kang Lin <kl222@126.com>  
Translator: Allan Nordhøy <epost@anotheragency.no>

### Environment
#### Operating system 

- Ubuntu

      ~$ lsb_release -a
      No LSB modules are available.
      Distributor ID:	Ubuntu
      Description:	Ubuntu 22.04.3 LTS
      Release:	22.04
      Codename:	jammy

- Debian

      ~$ lsb_release -a
      No LSB modules are available.
      Distributor ID:	Debian
      Description:	Debian GNU/Linux 12 (bookworm)
      Release:	12
      Codename:	bookworm

#### Qt Creator
Version: v9.0.2  
It is recommended to use version v5.0.2 or later.  
Prior versions don't have CMake support.

### Tools

- Install the development tools package

      ~$ sudo apt install build-essential

  - The development tools package already includes the installation of the following:
    - Compiler
      - GCC/g++
  
            ~$ sudo apt install g++ gcc
            
      - automake、autoconf、make、fakeroot

            ~$ sudo apt install automake autoconf make fakeroot

- Git: [https://www.git-scm.com](https://www.git-scm.com/)

      ~$ sudo apt install git
        
- CMake: [https://cmake.org](https://cmake.org/)

      ~$ sudo apt install cmake

- Package tools: debhelper

      ~$ sudo apt install debhelper

- Qt
  + Qt (official release): https://download.qt.io/official_releases/qt/
  + Qt5: Current version: 5.12.12
    + System build-in:

          ~$ sudo apt install qttools5-dev qttools5-dev-tools qtbase5-dev qtbase5-dev-tools qtmultimedia5-dev qtlocation5-dev libqt5svg5-dev libqtermwidget5-0-dev

  + Qt6: Current version: 6.4.2
    + System build-in:

          ~$ sudo apt install qt6-tools-dev qt6-tools-dev-tools qt6-base-dev qt6-base-dev-tools qt6-qpa-plugins libqt6svg6-dev qt6-l10n-tools qt6-translations-l10n qt6-scxml-dev qt6-multimedia-dev libqt6serialport6-dev qt6-webengine-dev qt6-webengine-dev-tools

- [OPTIONAL] IDE: Qt Creator. It is recommended to use version v5.0.2 or later.  
  Prior versions don't have CMake support.

      ~$ sudo apt install qtcreator

- Doxygen: [http://www.doxygen.nl/](http://www.doxygen.nl/)

      ~$ sudo apt install doxygen

### Library dependencies

- [MUST] RabbitCommon: [https://github.com/KangLin/RabbitCommon](https://github.com/KangLin/RabbitCommon)
- [OPTIONAL] FreeRDP: [https://github.com/FreeRDP/FreeRDP](https://github.com/FreeRDP/FreeRDP)
- [OPTIONAL] RFB
  + [Optional] RabbitVNC: [https://github.com/KangLin/RabbitVNC](https://github.com/KangLin/RabbitVNC)
  + [OPTIONAL] LibVNCServer: [https://github.com/LibVNC/libvncserver](https://github.com/LibVNC/libvncserver)
  + [OPTIONAL] TigerVNC: [https://github.com/KangLin/tigervnc](https://github.com/KangLin/tigervnc)
- [OPTIONAL] QTermWidget: [https://github.com/lxqt/qtermwidget](https://github.com/lxqt/qtermwidget)
- [OPTIONAL] [SSH]
  + libssh: [https://www.libssh.org](https://www.libssh.org/)
  + libssh2:
    - [https://www.libssh2.org](https://www.libssh2.org/)
    - [https://github.com/libssh2/libssh2](https://github.com/libssh2/libssh2/)
- [OPTIONAL] libtelnet: [https://github.com/seanmiddleditch/libtelnet](https://github.com/seanmiddleditch/libtelnet)
- [OPTIONAL] libdatachannel: [https://github.com/paullouisageneau/libdatachannel](https://github.com/paullouisageneau/libdatachannel)
- [OPTIONAL] QXmpp: [https://github.com/qxmpp-project/qxmpp](https://github.com/qxmpp-project/qxmpp)
- [OPTIONAL] QtService: [https://github.com/KangLin/qt-solutions](https://github.com/KangLin/qt-solutions)

#### RabbitCommon
This library is placed in the same directory level as the project by default.
If not, you must specify the CMake parameters:
-DRabbitCommon_DIR=[RabbitCommon installation path]

    ~$ git clone https://github.com/KangLin/RabbitCommon.git

#### FreeRDP
- Use the system-packaged development library

      ~$ sudo apt install freerdp2-dev
    
- Use vcpkg
  + Source-code location: https://github.com/microsoft/vcpkg/
  
        ~$ git clone https://github.com/microsoft/vcpkg.git
        ~$ cd vcpkg
        ~/vcpkg$ ./bootstrap-vcpkg.sh
        ~/vcpkg$ vcpkg install freerdp

  + Specify the CMake parameters:
    -DCMAKE_TOOLCHAIN_FILE=[vcpkg installation path]/scripts/buildsystems/vcpkg.cmake
  + With vcpk, the FreeRDP service is not a default feature,
    so you need to specify it manually.

- Compile from source code
  + Source-code location: https://github.com/FreeRDP/FreeRDP
  + Compilation instructions: https://github.com/FreeRDP/FreeRDP/wiki/Compilation
  
          ~$ git clone https://github.com/FreeRDP/FreeRDP.git
          ~$ cd FreeRDP
          ~/FreeRDP$ mkdir build
          ~/FreeRDP/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install -DWITH_SERVER=ON
          ~/FreeRDP/build$ cmake --build . --config Release --target install
          
  + Specify the CMake parameters:
    - -DBUILD_FREERDP=ON
    - -DFreeRDP-Client_DIR=[freerdp installation path]/lib/cmake/FreeRDP-Client2
    - -DFreeRDP_DIR=[freerdp installation path]/lib/cmake/FreeRDP2
    - -DWinPR_DIR=[freerdp installation path]/lib/cmake/WinPR2
  
#### LibVNCServer
- Use the system-packaged development library

      ~$ sudo apt install libvncserver-dev

- Compile from source code
  + Source-code location: [https://github.com/LibVNC/libvncserver](https://github.com/LibVNC/libvncserver)  
  It is recommended to use the patches from: https://github.com/KangLin/libvncserver
  + Specify CMake parameters: -DLibVNCServer_DIR=[LibVNCServer installation path]/lib/cmake/LibVNCServer

#### RabbitVNC
- Compile from source code
Source-code location: https://github.com/KangLin/RabbitVNC  

      ~$ sudo apt install libpixman-1-dev
      ~$ git clone https://github.com/KangLin/RabbitVNC.git
      ~$ cd RabbitVNC
      ~/RabbitVNC$ mkdir build
      ~/RabbitVNC$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install
      ~/RabbitVNC$ cmake --build . --config Release --target install
    
- Specify the CMake parameters: -DRabbitVNC_DIR=[RabbitVNC installation path]/lib/cmake/tigervnc

#### TigerVNC
- Compile from source code

The official program does not support libraries.
See: https://github.com/TigerVNC/tigervnc/issues/1123  
The KangLin fork has support.
Source-code location: https://github.com/KangLin/tigervnc  

    ~$ sudo apt install libpixman-1-dev
    ~$ git clone https://github.com/KangLin/tigervnc.git
    ~$ cd tigervnc
    ~/tigervnc$ mkdir build
    ~/tigervnc$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install
    ~/tigervnc$ cmake --build . --config Release --target install
    
- Specify the CMake parameters: -Dtigervnc_DIR=[TigerVNC installation path]/lib/cmake/tigervnc

#### libdatachannel
- Use vcpkg
  + Source-code location: https://github.com/microsoft/vcpkg/

        ~$ git clone https://github.com/microsoft/vcpkg.git
        ~$ cd vcpkg
        ~/vcpkg$ vcpkg install libdatachannel
      
  + Specify the CMake parameters:
    -DCMAKE_TOOLCHAIN_FILE=[vcpkg installation path]/scripts/buildsystems/vcpkg.cmake
  
- Compile from source code
  + Source-code location: [https://github.com/paullouisageneau/libdatachannel](https://github.com/paullouisageneau/libdatachannel)
  + Compilation: [https://github.com/paullouisageneau/libdatachannel/blob/master/BUILDING.md](https://github.com/paullouisageneau/libdatachannel/blob/master/BUILDING.md)
  
        ~$ git clone https://github.com/paullouisageneau/libdatachannel.git
        ~$ cd libdatachannel
        ~/libdatachannel$ git submodule update --init --recursive
        ~/libdatachannel$ mkdir build
        ~/libdatachannel$ cd build
        ~/libdatachannel/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install
        ~/libdatachannel/build$ cmake --build . --config Release --target install

  + Specify the CMake parameters: -DLibDataChannel_DIR=[libdatachannel installation path]/lib/cmake/LibDataChannel

#### QXmpp
- Compile from source code
  + Source-code location: [https://github.com/qxmpp-project/qxmpp](https://github.com/qxmpp-project/qxmpp)
  
        ~$ git clone https://github.com/qxmpp-project/qxmpp.git
        ~$ cd qxmpp
        ~/qxmpp$ mkdir build
        ~/qxmpp$ cd build
        ~/qxmpp/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install -DQt5_DIR=[Qt install path]/lib/cmake/Qt5
        ~/qxmpp/build$ cmake --build . --config Release --target install

  + Specify the CMake parameters: -DQXmpp_DIR=[libdatachannel installation path]/lib/cmake/qxmpp

#### QTermWidget
- Use the system-packaged development library

      ~$ sudo apt install libqtermwidget5-0-dev
      
- Compile from source code
  + Source-code location: [https://github.com/lxqt/qtermwidget](https://github.com/lxqt/qtermwidget)
  + Specify the CMake parameters: -Dqtermwidget5_DIR=[qtermwidget installation path]/lib/cmake/qtermwidget5

#### libssh
- Use the system-packaged development library

      ~$ sudo apt install libssh-dev 

- Use vcpkg
  + Source-code location: https://github.com/microsoft/vcpkg/
 
        ~$ git clone https://github.com/microsoft/vcpkg.git
        ~$ cd vcpkg
        ~/vcpkg$ vcpkg install libssh

  + Specify the CMake parameters:
  -DCMAKE_TOOLCHAIN_FILE=[vcpkg installation path]/scripts/buildsystems/vcpkg.cmake

- Compile from source code
  + Source-code location: [https://www.libssh.org](https://www.libssh.org)
  + Specify the CMake parameters: -Dlibssh_DIR=[libssh installation path]/lib/cmake/libssh

#### QtService
- Compile from source code
  + Source-code location: https://github.com/KangLin/qt-solutions/
  
        ~$ git clone https://github.com/KangLin/qt-solutions.git
        ~$ cd qt-solutions
        ~/qt-solutions$ mkdir build
        ~/qt-solutions$ cd build
        ~/qt-solutions/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install
        ~/qt-solutions/build$ cmake --build . --config Release --target install
  
  + Specify the CMake parameters: -DQtService_DIR=[QtService installation path]/lib/cmake/QtService
  
### Compile this project
- Project location: [https://github.com/KangLin/RabbitRemoteControl](https://github.com/KangLin/RabbitRemoteControl)
- Download the source code:

      ~$ git clone https://github.com/KangLin/RabbitRemoteControl.git

- The CMake parameters:
  + RabbitCommon_DIR: RabbitCommon source code location
  + BUILD_CLIENT: Compile client. the default is ON
  + BUILD_SERVICE: Compile service. the default dependency is whether there is a QtService
  + BUILD_PLUGINS: Compile plugins. the default is ON
  + BUILD_APP: Compile applaction. the default is ON
  + BUILD_DOCS: Build docmenets. the default is OFF
  + BUILD_SHARED_LIBS: Compile shared libraries. the default is ON
  + BUILD_FREERDP：If compile FreeRDP. the default is OFF
  + WinPR_DIR: [freerdp installation path]/lib/cmake/WinPR2
  + FreeRDP_DIR: [freerdp installation path]/lib/cmake/FreeRDP2
  + FreeRDP-Client_DIR: [freerdp installation path]/lib/cmake/FreeRDP-Client2
  + BUILD_RABBITVNC: Compile RabbitVNC. the default is ON
  + RabbitVNC_DIR: [RabbitVNC installation path]/lib/cmake/RabbitVNC
  + BUILD_TigerVNC: Compile TigerVNC. the default is ON
  + TigerVNC_DIR: [TigerVNC installation path]/lib/cmake/tigervnc
  + BUILD_LibVNCServer: Compile LibVNCServer. the default is ON
  + LibVNCServer_DIR: [libvncserver installation path]/lib/cmake/LibVNCServer
  + LibDataChannel_DIR: [libdatachannel installation path]/lib/cmake/LibDataChannel
  + QXmpp_DIR=[QXmpp installation path]/lib/cmake/qxmpp
  + qtermwidget5_DIR: [qtermwidget installation path]/lib/cmake/qtermwidget5
  + libssh_DIR: [libssh installation path]/lib/cmake/libssh
  + QtService_DIR:[QtService installation path]/lib/cmake/QtService
  
- If using vcpkg, please set the CMake parameters:
  + CMAKE_TOOLCHAIN_FILE: [vcpkg installation path]/scripts/buildsystems/vcpkg.cmake

- Compilation
  + Compile from the command-line
    - Not using vcpkg

          ~$ cd RabbitRemoteControl
          ~/RabbitRemoteControl$ mkdir build
          ~/RabbitRemoteControl/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install
          ~/RabbitRemoteControl/build$ cmake --build . --config Release --target install

    -  If using vcpkg

           ~$ cd RabbitRemoteControl
           ~/RabbitRemoteControl$ mkdir build
           ~/RabbitRemoteControl/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install [options libraries] -DCMAKE_TOOLCHAIN_FILE=[vcpkg installation path]/scripts/buildsystems/vcpkg.cmake
           ~/RabbitRemoteControl/build$ cmake --build . --config Release --target install

  + Using an IDE (Qt Creator)
    - Open project: Menu→ File→ Open File or project, Select the CMakeLists.txt of the project
    - Configure: Click Project→ "Build & Run" in the toolbar on the left to configure CMake parameters
    - Compile and run: Click "Start Debugging of startup project" in the left toolbar, or press the shortcut key (F5)
    - If using vcpkg: Menu→ Options→ Kits→ Cmake Configureration: add CMAKE_TOOLCHAIN_FILE=[vcpkg installation path]/scripts/buildsystems/vcpkg.cmake


  + Use script build_debpackage.sh
    - Set [Compile this project](#Compile-this-project) → The CMake parameters as environment variable. eg:
  
          export RabbitVNC_DIR=[RabbitVNC installation path]/lib/cmake/RabbitVNC
          export tigervnc_DIR=[TigerVNC installation path]/lib/cmake/tigervnc
        
    - Use build_debpackage.sh

          ./build_debpackage.sh $QT_ROOT $RabbitCommon_DIR

- Run
  + The application is installed in install/bin
  
        ~$ cd RabbitRemoteControl
        ~/RabbitRemoteControl$ cd build/install/bin
        ~/RabbitRemoteControl$ ./RabbitRemoteControl.sh

**Note:** If the plugin does not load.
It may be that the plugin's dependencies are not installed on the system.
You can add the path of the dependent library to the environment variable PKG_CONFIG_PATH.
You can also add the path of the dependent library to the /etc/ld.so.conf file,
and then run ldconfig to add the dependent library to the system.

### Compilation

```bash
    #Install the development tools package
    ~$ sudo apt install build-essential
    # Install tools
    ~$ sudo apt install git cmake debhelper doxygen
    # Install Qt5
    ~$ sudo apt install qttools5-dev qttools5-dev-tools qtbase5-dev qtbase5-dev-tools qtmultimedia5-dev qtlocation5-dev libqt5svg5-dev libqtermwidget5-0-dev

    # Install library dependencies
    ~$ sudo apt install freerdp2-dev libvncserver-dev libssh-dev libtelnet-dev
    # Install X development libraries
    ~$ sudo apt install libxkbcommon-dev libxkbcommon-x11-dev libx11-xcb-dev libx11-dev libxfixes-dev
    ~$ sudo apt install libutf8proc-dev libpam0g-dev # Need by compile qtermwidget
    # Install libpixman, RabbitVNC and TigerVNC need it.
    ~$ sudo apt install libpixman-1-dev
    # Compile RabbitVNC
    ~$ git clone https://github.com/KangLin/RabbitVNC.git
    ~$ cd RabbitVNC
    ~$ mkdir build
    ~/RabbitVNC/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install
    ~/RabbitVNC/build$ cmake --build . --config Release --target install
    ~/RabbitVNC/build$ cd ~
    # Compile TigerVNC
    ~$ git clone https://github.com/KangLin/tigervnc.git
    ~$ cd tigervnc
    ~$ mkdir build
    ~/tigervnc/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install
    ~/tigervnc/build$ cmake --build . --config Release --target install
    ~/tigervnc/build$ cd ~
    ~$ sudo apt install libqxmpp-dev
    # Compile libdatachannel
    ~$ git clone https://github.com/paullouisageneau/libdatachannel.git
    ~$ cd libdatachannel
    ~/libdatachannel$ git submodule update --init --recursive
    ~/libdatachannel$ mkdir build
    ~/libdatachannel$ cd build
    ~/libdatachannel/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install
    ~/libdatachannel/build$ cmake --build . --config Release --target install
    ~/libdatachannel/build$ cd ~
    # Compile QtService
    ~$ git clone https://github.com/KangLin/qt-solutions.git
    ~$ cd qt-solutions/qtservice
    ~/qt-solutions/qtservice$ mkdir build
    ~/qt-solutions/qtservice$ cd build
    ~/qt-solutions/qtservice/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install
    ~/qt-solutions/qtservice/build$ cmake --build . --config Release --target install
    ~/qt-solutions/qtservice/build$ cd ~
    ~$ git clone https://github.com/KangLin/RabbitCommon.git
    ~$ git clone https://github.com/KangLin/RabbitRemoteControl.git
    ~$ cd RabbitRemoteControl
    ~/RabbitRemoteControl$ mkdir build
    ~/RabbitRemoteControl$ cd build
    ~/RabbitRemoteControl/build$ cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=`pwd`/install -DRabbitVNC_DIR=~/RabbitVNC/build/install/lib/cmake/RabbitVNC -Dtigervnc_DIR=~/tigervnc/build/install/lib/cmake/tigervnc -DBUILD_FREERDP=ON -DLibDataChannel_DIR=~/libdatachannel/build/install/lib/cmake/LibDataChannel -DQtService_DIR=~/qt-solutions/qtservice/build/lib/cmake/QtService
    ~/RabbitRemoteControl/build$ cmake --build . --config Release --target install
```

See: [Compile integration](../../.github/workflows/ubuntu.yml)
