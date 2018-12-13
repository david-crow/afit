#include "WOInterceptorMissile.h"
#include "Model.h"
#include "WOGJoint.h"
#include <cmath>
#include <iostream>

namespace Aftr
{

WOInterceptorMissile* WOInterceptorMissile::New()
{
   WOInterceptorMissile* wo = new WOInterceptorMissile();
   wo->onCreate( ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Launcher/Missile.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO );     
   return wo;
}

void WOInterceptorMissile::onCreate( const std::string& modelFileName, const Vector& scale, const MESH_SHADING_TYPE& mstMeshShadingType )
{
   WO::onCreate( modelFileName, scale, mstMeshShadingType );
}

WOInterceptorMissile::WOInterceptorMissile() : IFace( this ), WO()
{
}

WOInterceptorMissile::~WOInterceptorMissile()
{
}

void WOInterceptorMissile::setMotionStrategy( std::unique_ptr< MotionStrategyIFace > strategy, double range_m,
                                              double totalTime_sec, double headingDeg, const Vector& launchPos )
{
   this->motionAlgorithm = std::move(strategy);
   this->range_m = range_m;
   this->totalTime_sec = totalTime_sec;
   this->headingDeg = headingDeg;
   this->launchPosition = launchPos;
}

double WOInterceptorMissile::getMotionStrategyLaunchAngleDeg()
{
   return motionAlgorithm->getLaunchAngle(range_m, totalTime_sec);
}

std::string WOInterceptorMissile::getTrajectoryInfo()
{
   if( this->motionAlgorithm != nullptr )
      return std::move( this->motionAlgorithm->toString( this->range_m, this->totalTime_sec, this->headingDeg, this->launchPosition ) );
   return "Motion Algorithm is empty, cannot compute trajectory info...\n";
}

void WOInterceptorMissile::onUpdateWO()
{
   if( this->state == MISSILE_STATE::FLYING )
      this->updateMissileTrajectory();
   else if( this->state == MISSILE_STATE::DETONATING )
   {
      std::cout << "KAAAABOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM!!!!\n\n\n\n";
      this->state = MISSILE_STATE::INERT;
   }
}

void WOInterceptorMissile::updateMissileTrajectory()
{
   std::chrono::duration< double, std::micro > deltaT_uSec = std::chrono::system_clock::now() - this->launchTime;
   double deltaTSec = deltaT_uSec.count() / 1'000'000; //convert microseconds to seconds by dividing by a million

   auto r = motionAlgorithm->computePosition(deltaTSec, range_m, totalTime_sec, headingDeg, this->launchPosition);
   Vector newPosition = std::get<0>(r);
   bool reachedTarget = std::get<1>(r);

   if (reachedTarget)
      this->state = MISSILE_STATE::DETONATING;
   else
      this->setPosition(newPosition);
}

void WOInterceptorMissile::launch()
{
   if (this->state == MISSILE_STATE::AIMING)
   {
      this->launchTime = std::chrono::system_clock::now();
      this->state = MISSILE_STATE::FLYING;
   }
}

void WOInterceptorMissile::reset()
{
   this->setPosition(this->launchPosition);
   state = MISSILE_STATE::AIMING;
}

} //namespace Aftr
