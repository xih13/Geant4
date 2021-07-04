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
//
/// \file B1DetectorConstruction.hh
/// \brief Definition of the B1DetectorConstruction class

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh" // base class
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
// 文件涉及到逻辑体、物理体指针变量，所以添加上 G4VPhysicalVolume 和 G4LogicalVolume 两个大类

/// Detector construction class to define materials and geometry.

class B1DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    B1DetectorConstruction();
    // 声明构造函数。创建对象时，需要添加初始化代码（即为对象成员变量赋初始值）时，则需要定义自己的构造函数。
    // 总与new运算符一起使用在创建对象的语句中；
    // 特别的一个类可以有多个构造函数 可根据其参数个数的不同或参数类型的不同来区分它们，即构造函数的重载

    virtual ~B1DetectorConstruction();
    // 声明析构函数。对象撤销时，需要自己添加清理工作的代码时，则需要定义自己的析构函数。
    // 当对象脱离其作用域时（例如对象所在的函数已调用完毕），系统自动执行析构函数。
    // 析构函数往往用来做“清理善后” 的工作（例如在建立对象时用new开辟了一片内存空间，应在退出前在析构函数中用delete释放）。

    // 构造函数是生成一个新的实例,让你操作它里面的属性或方法;析构函数是当你不用这个对象时释放系统资源。
    // new 就是构造，dispose 就是析构

    virtual G4VPhysicalVolume* Construct();
    // 声明了返回值是物理体指针 G4VPhysicalVolume 的 Construct 函数，描述探测器
    // 一般返回值是 world 物理体指针，所谓 world 是G4定义的几何世界母体，可以认为是虚拟的实验大厅。
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    // 自定义计数函数 GetScoringVolume() ，返回 fScoringVolume 指针， 指针类型是 G4LogicalVolume 逻辑体

  protected:
    G4LogicalVolume*  fScoringVolume;
    // fScoringVolume 在protected封装中了，他是一个逻辑体指针，保护成员变量，仅在该类中允许调用。
    // 封装，protected和private成员在类外（在此将这两种限定符都可以看成是私有的，在继承出区别）不能够访问。
    // 类外：即脱离了类的作用域或者说访问时不在类的成员函数中。
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

