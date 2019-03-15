#include "WOInterceptorMissile.h"
#include "MGLTrackingRibbon.h"
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

void WOInterceptorMissile::setLaunchTime( const std::chrono::system_clock::time_point& time_point )
{
   this->launchTime = time_point;
}

void WOInterceptorMissile::setLaunchPosition( const Vector& position )
{
   this->launchPosition = position;
}

void WOInterceptorMissile::setDetonationPosition( const Vector& position )
{
   this->detonationPosition = position;
}

void WOInterceptorMissile::setDefaultColor( const Aftr::aftrColor4ub& color )
{
   this->defaultColor = color;
}

void WOInterceptorMissile::detonate()
{
   this->state = MISSILE_STATE::DETONATING;
   std::cout << "---------- Missile SIG " << this->getID() << " successfully intercepted!\n";
}

void WOInterceptorMissile::onUpdateWO()
{
   if( this->state == MISSILE_STATE::FLYING )
   {
      Aftr::TrackInfo trackInfo( this->getPosition(), { 0,0,0 }, 0 );

      if ( this->currentlyDetectsRadarBeams() )
         this->ribbon->addTrackPoint( trackInfo, Aftr::aftrColor4ub( 255,255,0,100 ) );
      else
         this->ribbon->addTrackPoint( trackInfo, this->defaultColor );

      this->updateMissileTrajectory();
   }
   else if( this->state == MISSILE_STATE::DETONATING )
   {
      // std::cout << "KAAAABOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM!!!!\n\n\n\n";
      this->state = MISSILE_STATE::INERT;
   }

   this->currentlyDetectsRadarBeams( false );
}

void WOInterceptorMissile::updateMissileTrajectory()
{
   std::chrono::duration< double, std::micro > deltaT_uSec = std::chrono::system_clock::now() - this->launchTime;
   double deltaTSec = deltaT_uSec.count() / 1'000'000; //convert microseconds to seconds by dividing by a million

   auto r = motionAlgorithm->computePosition(deltaTSec, range_m, totalTime_sec, headingDeg, this->launchPosition);
   Vector newPosition = std::get<0>(r);

   this->setPositionIgnoringAllChildren(newPosition);

   if ( newPosition.distanceFrom( this->detonationPosition ) < 0.5f )
      this->state = MISSILE_STATE::DETONATING;
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
   this->setPositionIgnoringAllChildren( this->launchPosition );
   state = MISSILE_STATE::AIMING;

   if ( this->ribbon )
      this->ribbon->clear();
}

} //namespace Aftr
