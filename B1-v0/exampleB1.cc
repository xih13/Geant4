//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// 对于vim,开始编辑时，键入i，插入文本，或者键入a，在末尾添加文本。
// 单行注释以 // 开头，在行末结束。
/// \file exampleB1.cc
/// \brief Main program of the B1 example

// 预处理,以#号开头的代码行。#号必须是该行除了任何空白字符外的第一个字符。#后是指令关键字，关键字和#号之间允许存在任意个数的空白字符
// 预处理，.hh即源代码文件，.cc头文件
#include "B1DetectorConstruction.hh" // 调用探测器头文件
#include "B1ActionInitialization.hh" // 调用用户初始化头文件

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh" // 多线程管理类
#else
#include "G4RunManager.hh" // 管理类
#endif
// ifdef 条件编译指令，如果宏已经定义，则编译下面代码；决定哪些代码被编译，而哪些不被
// if 和 endif 相对应

#include "G4UImanager.hh" // UI管理类
#include "QBBC.hh" // 物理过程

#include "G4VisExecutive.hh" //可视化
#include "G4UIExecutive.hh" // UI操作数

#include "Randomize.hh" // 随机数

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 主函数, main() 是程序开始执行的地方
// 这里的 int 和 argc 之间必须有一个空格，这样编译器才能够区分它们
// g++, 则要求 main 函数的返回值必须为 int 型
// argc 表示运行程序时的参数个数，VS中默认为1，argv 表示参数，其中argv[0]是程序本身的全路径名，argv[1]是第一个参数
// 具体就是终端里输入的字符串的个数，如./example run1.mac 的argc为2； ./example的argc为1.

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  // 在 C++ 中，分号是语句结束符。也就是说，每个语句必须以分号结束。它表明一个逻辑实体的结束。
  // C++ 不以行末作为结束符的标识，因此，您可以在一行上放置多个语句。
  //
  G4UIExecutive* ui = 0;
  // 定义了一个ui操作指针（NULL），用于用户交互操作。
  // 如果一个 空指针常量 赋给了一个有类型的指针变量，那么这个指针就叫空指针。它不指向任何的对象或者函数
  // 对 ui 进行了初始化，使之可以访问 ui ，因为不能使用未初始化的内存；没指向元素之前是没给指针分配指向的内存的；
  // 假设有指针p，则p的值表示它指向的地址；
  // &p表示指针本身的地址（门牌号）,因为指针p同时也是个变量，既然是变量，编译器肯定要为其分配内存地址；
  // *p表示根据门派号，找到它指向的地址并取出里面存放的值。

  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
  // 给了一个判断，若argc == 1，则给ui指针申请新的空间，即不再是NULL指针。

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  // Construct the default run manager
  //
  // 不管是否多线程,我们都必须声明一个runManager，为运行管理器，可以认为是一个管理程序运行的大脑。
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes,开始构建强制类,这是 GEANT4 必需的部分
  //
  // Detector construction,探测器几何构建
  runManager->SetUserInitialization(new B1DetectorConstruction());
  // 运行管理指针调用SetUserInitialization()函数，分辟内存空间初始化B1DetectorConstruction的对象

  // Physics list,注册物理过程。同理，定义模块化物理过程QBBC，其对象physicsList，对该对象进行初始化
  G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
  //A->B则A为指针，->是成员提取，A->B是提取A中的成员B，A只能是指向类、结构、联合的指针
    
  // User action initialization, 1. 注册初始粒子生成,注册一些可选的 Action 类;
  // 2.  虽然不是 GEANT4 工作必需的,但是如果你需要提取模拟过程中粒子的信息,那就必须要它们。
  // 对B1ActionInitialization的对象初始化。
  runManager->SetUserInitialization(new B1ActionInitialization());
  // 注意，这里的B1DetectorConstruction和physicsList是强制类，必须设置它们
  // 还有一个强制类包含在B1ActionInitialization当中，后续会提到。
  
  // Initialize visualization，可视化初始化
  // 通过该类和UI操作类的结合，运行程序时能够看到几何模型和粒子输运过程。
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager, 用户操作界面管理类
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session，启用宏或开始UI会话
  //
  if ( ! ui ) { 
    // 定义UI管理类，前面定义的ui为null, 则执行if命令，此时为batch mode，批处理模式

    G4String command = "/control/execute ";
    G4String fileName = argv[1]; // argv[1] 指向在命令行中执行程序名后的第一个字符串。
    UImanager->ApplyCommand(command+fileName);
    // 由UI管理类指针指向命令行，输入"/control/execute/run.mac"
    // 假设输入run.mac，则argv[1]代表程序名后第一个字符串run.mac)
  }
  else { 
    // 若前面定义的ui不为null，则不执行if语句，此时为交互式模式，interactive mode
    // UI管理类会默认执行"/control/execute/init_vis.mac"

    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
    //打开对话框，如图所示，若注释掉这一句，可视化一闪就关闭了，因此有点像”pause”的作用。最后删除ui，释放内存。
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  delete visManager;
  delete runManager;
  // 最后删除两个管理器。
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
