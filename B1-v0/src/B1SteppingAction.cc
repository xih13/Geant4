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
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction), // 当前event下的 eventAction 地址赋给 fEventAction ，初始化
  fScoringVolume(0) // 赋值 null
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// const 指针内存空间变量值无法修改
void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  // 第一次step到来时， fScoringVolume 为假，执行 if (!fScoringVolume)语句
  if (!fScoringVolume) { 
    const B1DetectorConstruction* detectorConstruction // 定义了对象 detectorConstruction
      = static_cast<const B1DetectorConstruction*>
        // 通过运行管理器， 得到用户定义的探测器构造
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    // 把探测器类 G4RunManager 强制转换成 B1DetectorConstruction 类
    // detectorConstruction 调用函数 GetScoringVolume（） 获取logicShape2(在 B1DetectorConstruction.cc 中定义)，
    // 并将它的地址赋给指针 fScoringVolmue ，（ fScoringVolmue 是 B1SteppingAction 类的私有变量
    // 与 B1DetectorConstruction 类中私有变量 fScoringVolmue ，是通过指针传递建立联系）
    // 可以简单认为此时的 fScoringVolmue 已经是指代 logicShape2 了。在下一个step开始以及以后的循环中，if语句不再执行直至下一个event到来。
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }

  // get volume of the current step
  // 定义逻辑体指针 volume , 获取该 step 的逻辑体
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  // check if we are in scoring volume
  // 若 volume 不在 logicShape2 中，跳出循环，
  // 若在，则执行下一行命令，获取该 step 的能量沉积， 并赋值给一个临时变量 edepStep
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit(); // 这一步的能量沉积
  // 一个event下step不断循环， fEdep 则不断累加。直到最后一个step结束， fEdep 代表了该event的能量沉积。
  fEventAction->AddEdep(edepStep); // EventAction 类调用 AddEdep 函数 得到provite变量 fEdep
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

