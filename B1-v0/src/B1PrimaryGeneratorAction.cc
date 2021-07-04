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
/// \file B1PrimaryGeneratorAction.cc
/// \brief Implementation of the B1PrimaryGeneratorAction class

#include "B1PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
// 写入需要的头文件

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 在构造函数中进行初始化， 把 fParticleGun 和 fEnvelopeBox 指针赋值为null
B1PrimaryGeneratorAction::B1PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fEnvelopeBox(0)
{
  // 定义 fParticleGun ，每次发射一个粒子。
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  // 定义 G4ParticleTable 类对象 particleTable ，用于获取粒子列表
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // 定义G4ParticleDefinition 类对象 particle
  // particleTable 列表查找gama粒子 并赋值给 particle
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="gamma");
  // fParticleGun 对粒子进行一些设置
  fParticleGun->SetParticleDefinition(particle); // 设置粒子种类
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.)); // 粒子的动量和方向
  fParticleGun->SetParticleEnergy(6.*MeV); // 粒子的能量
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PrimaryGeneratorAction::~B1PrimaryGeneratorAction()
{
  // 在析构函数中，删除了fParticleGun，这是因为fParticleGun new了一个新的内存空间，在粒子射出后要删除这部分内存。
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// GeneratePrimaries(G4Event* anEvent) 函数每次 event 都调用一次，用于发射粒子
// 在其中用户可以设置粒子的能谱分布、角度分布、源的位置分布等等。
void B1PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.
  
  G4double envSizeXY = 0;
  G4double envSizeZ = 0;

  // 第一次初始化 fEnvelopeBox=null ,执行一次
  if (!fEnvelopeBox) // if 语句判断 fEnvelopeBox 的真假， 第一个 event 到来时，该语句执行为真
  {
    G4LogicalVolume* envLV // 在逻辑体存储容器中，找到名为 Envelope 的逻辑体（ logical ）， 指针 envLV 指向其地址
      = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
    // 再通过 envLV 的 GetSolid 把 Envelope 实体( solid )找出来， fEnvelopeBox 指向其地址，（强制G4Box类）
    // 注意： 第二个 event 到来的时候就不执行了，因为以后 fEnvelopeBox!=null 了，
    // 以后每次 event 循环，一直执行下面 if(fEnvelopeBox) 的内容，即得到 fEnvelopeBox 几何参数
  }

  if ( fEnvelopeBox ) // 以后每次 event 循环， fEnvelopeBox ！= null ,一直执行
  {
    envSizeXY = fEnvelopeBox->GetXHalfLength()*2.; // 得到 fEnvelopeBox 的几何参数
    envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
  }  
  // 为了保护程序的完整性，给了一个 else ，指的是如果找不到 fEnvelopeBox ，输出提示语。
  else // 不存在 fEnvelopeBox 时的输出
  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }

  // 把得到的 fEnvelopeBox 几何参数给到x0,y0,z0变量，通过 fParticleGun 设置粒子位置分布
  G4double size = 0.8; 
  G4double x0 = size * envSizeXY * (G4UniformRand()-0.5);
  G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
  G4double z0 = -0.5 * envSizeZ;
  
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0)); // 粒子位置，服从（-0.5，0.5）均匀分布

  // fParticleGun 调用了一个 GeneratePrimaryVertex(anEvent) 函数，类比开关，每个 event 过来，这个粒子发生器就发射一次。
  fParticleGun->GeneratePrimaryVertex(anEvent); // 开始发射
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

