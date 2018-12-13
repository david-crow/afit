#include "WORadarSensor.h"
#include "Model.h"
#include "WOGJoint.h"
#include <cmath>
#include <iostream>
#include "MGLIndexedGeometry.h"
#include "IndexedGeometryTriangles.h"
#include "AftrGeometryPlane.h"

namespace Aftr
{

WORadarSensor* WORadarSensor::New()
{
   WORadarSensor* wo = new WORadarSensor();
   wo->onCreate();     
   return wo;
}

void WORadarSensor::onCreate()
{
   WO::onCreate();

   this->model = MGLIndexedGeometry::New( this ); //we must set the geometry of this object. This
            //is computed and set via computeSensedGeometry and updateModelGeometry, respectively.
   
   //Create two hyper planes defining a wedge shape. This shape will define the sensor's field of view.
   //use the computeSensedGeometry and updateModelGeometry method to instantiate a wedge
   auto tup = this->computeSensedGeometry( this->horzFieldOfView, this->scanRangeMeters );
   this->updateModelGeometry( tup );
}

WORadarSensor::WORadarSensor() : IFace( this ), WO()
{
}

WORadarSensor::~WORadarSensor()
{
}

void WORadarSensor::onUpdateWO()
{
   //Do NOT modify this code - manufacturer supplied
   WO::onUpdateWO();
   std::vector<WO*> targets = this->scanForTargetsWithinCurrentWedge();
   this->processScanData( targets, std::chrono::system_clock::now() );
}

void WORadarSensor::addPossibleTargetToRadarScanList( WO* objToSearchFor, const RadarCallback_onObjectIlluminatedByRadar& onRadarIlluminationOccurredCallback )
{
   if (objToSearchFor)
      this->possibleTargets.push_back( std::make_tuple( objToSearchFor, onRadarIlluminationOccurredCallback ) );
}

void WORadarSensor::setFilteringOfRawScanDataAlgorithm( const RadarCallback_onFilterRawScanData& filterFunc )
{
   this->callback_onFilterRawScanData = filterFunc;
}

void WORadarSensor::subscribeToRadarTrackingInfo( const RadarCallback_onScanDataAvailable& processedDataFunc )
{
   this->callback_onScanDataAvailable = processedDataFunc;
}

std::vector< WO* > WORadarSensor::scanForTargetsWithinCurrentWedge() const
{
   std::vector< WO* > insideWedge;

   //perform collision detection between all possible targets and the radar's wedge
   AftrGeometryPlane left = this->getLeftHyperplane();
   AftrGeometryPlane right = this->getRightHyperplane();

   for (auto& target : this->possibleTargets)
   {
      auto wo = std::get<0>(target);
      auto callback = std::get<1>(target);

      bool inside_left = left.getNormal().dotProduct(wo->getPosition()) <= left.getDisplacement() ? true : false;
      bool inside_right = right.getNormal().dotProduct(wo->getPosition()) <= right.getDisplacement() ? true : false;
      bool within_range = this->getPosition().distanceFrom(wo->getPosition()) <= scanRangeMeters ? true : false;

      if (inside_left && inside_right && within_range)
      {
         insideWedge.push_back(wo);
         callback(wo);
      }
   }

   return std::move( insideWedge );
}

void WORadarSensor::processScanData( const std::vector< WO* >& targets, const std::chrono::system_clock::time_point& t ) const
{
   for (auto& wo : targets)
   {
      auto result = this->callback_onFilterRawScanData(wo, t);
      unsigned int id = std::get<0>(result);
      std::vector<Vector> polar = std::get<1>(result);

      this->callback_onScanDataAvailable(id, t, std::move(polar));
   }
}

void WORadarSensor::setHorzFOVDeg( float horzFieldOfViewDeg )
{
   this->updateModelGeometry(this->computeSensedGeometry(horzFieldOfViewDeg, this->scanRangeMeters));
}

void WORadarSensor::setScanRangeMeters( float scanRangeMeters )
{
   this->updateModelGeometry(this->computeSensedGeometry(this->horzFieldOfView, scanRangeMeters));
}

std::string WORadarSensor::getSensorInfo() const
{
   std::stringstream ss;
   ss << "Apex Pos: " << this->getPosition() << "\n"
      << "Horz FOV: " << this->horzFieldOfView << " deg\n"
      << "Scan Rng: " << this->scanRangeMeters << " m\n"
      << "Curr Hdg: " << this->getLookDirection().getTheta() * Aftr::RADtoDEG << " deg\n"
      << "A call to scanForTargetsWithinCurrentWedge() returns the following:\n";
   auto v = this->scanForTargetsWithinCurrentWedge();
   for( auto& w : v )
      ss << "   woID: " << w->getID() << " at Pos " << w->getPosition() << "\n";

   return ss.str();
}

AftrGeometryPlane WORadarSensor::getLeftHyperplane() const
{
   AftrGeometryPlane p;

   auto result = computeSensedGeometry(this->horzFieldOfView, this->scanRangeMeters);
   auto verts = std::get<0>(result);

   Vector vec1(verts[3] - verts[0]);
   Vector vec2(verts[2] - verts[1]);
   Vector normal(vec1.crossProduct(vec2));
   p.setNormal(normal.normalizeMe().rotate({0, 0, 1}, this->getLookDirection().getTheta()));
   p.setDisplacement(p.getNormal().dotProduct(this->getPosition()));

   return p;
}

AftrGeometryPlane WORadarSensor::getRightHyperplane() const
{
   AftrGeometryPlane p;

   auto result = computeSensedGeometry(this->horzFieldOfView, this->scanRangeMeters);
   auto verts = std::get<0>(result);

   Vector vec1(verts[7] - verts[4]);
   Vector vec2(verts[6] - verts[5]);
   Vector normal(vec1.crossProduct(vec2));
   p.setNormal(normal.normalizeMe().rotate({0, 0, 1}, this->getLookDirection().getTheta()));
   p.setDisplacement(p.getNormal().dotProduct(this->getPosition()));

   return p;
}

std::tuple< std::vector< Vector >, std::vector< unsigned int >, float, float > WORadarSensor::computeSensedGeometry( float horzFieldOfViewDeg, float rangeMeters ) const
{
   //Do NOT modify this code - manufacturer supplied. However, I recommend you carefully read through this code and
   //understand how it works... This may be your first exposure to modern OpenGL indexed geometry. If you've ever played
   //a 3D game, an approach similar to this code was used to create the geometry rendered by the graphics card.

   //             \        /
   //              \      /
   // Normal vec <--\    /--> Normal vec  (Note: real normals are perpendicular to each face, not parallel with each other).
   //        Face A  \  / Face B
   //                 \/
   //          

   constexpr float H = 15.0f;

   //a is the face A side
   Vector a = Vector{ 1,0,0 }.rotate( Vector{ 0,0,1 }, horzFieldOfViewDeg / 2.0f * Aftr::DEGtoRAD );
   a *= rangeMeters;

   //b is the face B side
   Vector b = Vector{ 1,0,0 }.rotate( Vector{ 0,0,1 }, -horzFieldOfViewDeg / 2.0f * Aftr::DEGtoRAD );
   b *= rangeMeters;

   Vector faceA_TopNear = { 0, 0, H };
   Vector  faceA_TopFar = { a.x, a.y, H };
   Vector faceA_BotNear = { 0,0,0 };
   Vector  faceA_BotFar = { a.x, a.y, 0 };

   Vector faceB_TopNear = { 0, 0, H };
   Vector  faceB_TopFar = { b.x, b.y, H };
   Vector faceB_BotNear = { 0,0,0 };
   Vector  faceB_BotFar = { b.x, b.y, 0 };

   //vertex order is:
   //faceA top/near, faceA top far, faceA bottom near, faceA bottom far,
   //faceB top/near, faceB top far, faceB bottom near, faceB bottom far
   std::vector< Vector > verts = { faceA_TopNear, faceA_TopFar, faceA_BotNear, faceA_BotFar,
                                   faceB_TopNear, faceB_BotNear, faceB_TopFar, faceB_BotFar };

   //Index determines the order that the triangles in Vertex are rendered - we use counter clockwise notation.
   std::vector< unsigned int > indices = {0,1,2,   //TriA1 Normal points left-ish
                                          2,1,3,   //TriA2 Normal points left-ish
                                          4,5,6,   //TriA2 Normal points right-ish
                                          5,7,6 }; //TriA2 Normal points right-ish

   return std::make_tuple( verts, indices, horzFieldOfViewDeg, rangeMeters );
}


void WORadarSensor::updateModelGeometry( std::tuple< std::vector< Vector >, std::vector< unsigned int >, float, float >& geomData )
{
   //Do NOT modify this code - manufacturer supplied
   this->updateModelGeometry( std::get<0>( geomData ), std::get<1>( geomData ), std::get<2>( geomData ), std::get<3>( geomData ) );
}

void WORadarSensor::updateModelGeometry( const std::vector< Vector >& verts, const std::vector< unsigned int >& indices, float horzFieldOfView, float rangeMeters )
{
   //Do NOT modify this code - manufacturer supplied
   this->horzFieldOfView = horzFieldOfView;
   this->scanRangeMeters = rangeMeters;

   IndexedGeometryTriangles* geom = IndexedGeometryTriangles::New( verts, indices );
   this->getModelT<MGLIndexedGeometry>()->setIndexedGeometry( geom );
}

} //namespace Aftr
