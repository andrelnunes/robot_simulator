# robot_simulator
Robot simulator: mapping, graph search and position control.

================================================== ==============================
    MFC Library: MobileRobotSimulator project overview
================================================== =============================

In the AppWizard, open the MobileRobotSimulator application
I made it. This application not only shows the basic usage of MFC,
Provides a basic structure for writing.

This file contains information about each file that makes up the MobileRobotSimulator application.
A summary description is included.

Algorithm.cpp
    Contains the positioning, mapping and target checking.
    
Robot.cpp
    Initialize and updates the robot parameters (speed  and local position). Also sets up the optima path.
    
MobileRobotSimulator.vcproj
    This is the main project file for a VC ++ project created using the AppWizard.
    Information about the version of Visual C ++ that generated the file was selected using the AppWizard
    Contains information about platform, configuration, and project features.

MobileRobotSimulator.h
    The application's default header file. This includes other project related
    Header (including Resource.h), and the CMobileRobotSimulatorApp application
    Declare the class.

MobileRobotSimulator.cpp
    The default application that contains the application class CMobileRobotSimulatorApp
    Source file.

MobileRobotSimulator.rc
    A list of all Microsoft Windows resources used by the program.
 This includes icons, bitmaps, and cursors stored in the RES subdirectory
    Included. This file is directly in Microsoft Visual C ++
    Can be edited. The project resources are at 1042.

Res \ MobileRobotSimulator.ico
    Icon file used as the icon for the application. This icon
    Included by the main resource file, MobileRobotSimulator.rc.

Res \ MobileRobotSimulator.rc2
    This file contains resources edited by tools other than Microsoft Visual C ++
    I have. All resources that can not be edited with the Resource Editor
    You need to put it in this file.


//////////////////////////////////////////////////////// ///////////////////////////////

One dialog class is created in the AppWizard.

MobileRobotSimulatorDlg.h, MobileRobotSimulatorDlg.cpp? Dialog box
    This file contains the CMobileRobotSimulatorDlg class. This class
    Defines the main dialog behavior of the application. The dialog's template
    It is located in MobileRobotSimulator.rc, which can be edited in Microsoft Visual C ++.


//////////////////////////////////////////////////////// ///////////////////////////////

Other features:

ActiveX control
    Supports the use of ActiveX controls in applications.

Windows Sockets
    The application supports communication using a TCP / IP network.

//////////////////////////////////////////////////////// ///////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    This file contains the precompiled header file (PCH) MobileRobotSimulator.pch
    Used to build a precompiled format file, StdAfx.obj.

Resource.h
    A standard header file that defines a new resource ID.
    Microsoft Visual C ++ reads and updates this file.

MobileRobotSimulator.manifest
The application manifest file is a version of Side-by-Side
Used to describe application dependencies on assemblies. The loader can
To load the appropriate assembly from the assembly cache,
Load the assembly. The application manifest is the same as the application executable.
The external .manifest file installed in the folder will be included for redistribution or in the form of a resource.
It can be included in the executable file.
//////////////////////////////////////////////////////// ///////////////////////////////

Other Notes:

Application Wizard should be added or customized using "TODO:"
Represents the source code part.

If the application uses MFC in a shared DLL, the corresponding MFC DLL
Redistribution is required. And if the application has an operating system locale
If you use a different language, you can use the localized resource MFC80XXX.DLL
Redistribution is required. For more information on these two topics,
Visual C ++ Application Redistribution Entries in the MSDN documentation
Please see.

//////////////////////////////////////////////////////// ///////////////////////////////
