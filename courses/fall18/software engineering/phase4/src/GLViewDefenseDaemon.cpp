#include "GLViewDefenseDaemon.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "AftrGLRendererBase.h"
#include "WOInterceptorMissile.h"
#include "WORadarSensor.h"
#include "WORadarDish.h"
#include "WOTrackingRibbonBase.h"
//for gnuplots
#include "gnuplot-iostream.h"
#include "../PolyFit/PolyFit.h"
#include "MGLTrackingRibbon.h"

//If we want to use way points, we need to include this.
#include "DefenseDaemonWayPoints.h"

//for rand
#include <cstdlib>
#include <ctime>

using namespace Aftr;

GLViewDefenseDaemon* GLViewDefenseDaemon::New( const std::vector< std::string >& args )
{
   GLViewDefenseDaemon* glv = new GLViewDefenseDaemon( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->onCreate();
   return glv;
}


GLViewDefenseDaemon::GLViewDefenseDaemon( const std::vector< std::string >& args ) : GLView( args )
{
   //Generally, for the radar program, you do NOT need to add any code into this method - prefer
   //the loadMap()...
   //Initialize any member variables that need to be used inside of LoadMap() here; alternatively,
   //just initizlie those values in the header file or at the top of loadMap().
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewDefenseDaemon::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewDefenseDaemon::onCreate() is invoked after this module's LoadMap() is completed.
}


void GLViewDefenseDaemon::onCreate()
{
   //GLViewDefenseDaemon::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.

   if( this->pe != NULL )
   {
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1
}


GLViewDefenseDaemon::~GLViewDefenseDaemon()
{
   //Implicitly calls GLView::~GLView()
}


void GLViewDefenseDaemon::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.

   float radarDegRotation = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "radarDegRotation" ) );
   this->dish->rotateAboutRelZ( radarDegRotation * Aftr::DEGtoRAD );

   // if a red missile is close to a detonating blue missile, red's dead
   float blastRadius = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "blastRadius" ) );
   for( auto& interceptor : this->interceptors )
      for( auto blue : this->blueMissiles )
         if( std::get<0>( interceptor )->getPosition().distanceFrom( blue->getPosition() ) <= blastRadius && blue->isDetonating() )
            std::get<0>( interceptor )->detonate();
}

void GLViewDefenseDaemon::onResizeWindow( GLsizei width, GLsizei height )
{
   GLView::onResizeWindow( width, height ); //call parent's resize method.
}


void GLViewDefenseDaemon::onMouseDown( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseDown( e );
}


void GLViewDefenseDaemon::onMouseUp( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseUp( e );
}


void GLViewDefenseDaemon::onMouseMove( const SDL_MouseMotionEvent& e )
{
   GLView::onMouseMove( e );
}


void GLViewDefenseDaemon::onKeyDown( const SDL_KeyboardEvent& key )
{
   GLView::onKeyDown( key );

   if( key.keysym.sym == SDLK_l )
   {
      const Uint8* state = SDL_GetKeyboardState( nullptr );
      if( state[SDL_SCANCODE_LSHIFT] )
      {
         this->light->isDirectionalLight( !this->light->isDirectionalLight() );
         std::cout << "Light is now set to ";
         this->light->isDirectionalLight() ? std::cout << "directional " : std::cout << "point ";
         std::cout << "light...\n";
      }
      else
      {
         if( this->light->isDirectionalLight() )
         {
            std::cout << "Setting Light Direction to " << this->cam->getLookDirection() << "\n";
            this->light->getModel()->setLookDirection( this->cam->getLookDirection() );
         }
         else
         {
            std::cout << "Setting Light Position to " << this->cam->getPosition() << "\n";
            this->light->setPosition( this->cam->getPosition() );
         }
      }
   }
   else if( key.keysym.sym == SDLK_r )
   {
      const Uint8* state = SDL_GetKeyboardState( nullptr );

      if( state[SDL_SCANCODE_LSHIFT] )
      {
         std::cout << "Repopulated from conf file... New values are:\n";
         this->repopulateFromConfFile();
      }
   }
   else if( key.keysym.sym == SDLK_1 )
   {
      std::cout << "Launching Interceptors using params from conf file...\n";

      for( auto& i : this->interceptors )
         std::get<0>( i )->launch(); // launch the missile
   }
   else if( key.keysym.sym == SDLK_2 )
   {
      std::cout << "Resetting Interceptors back to launch positions...\n";

      // reset red missiles
      for( auto& i : this->interceptors )
      {
         std::get<0>( i )->reset();
         std::get<1>( i ).clear(); // clear its location history
         std::get<2>( i ).clear(); // clear its time history
         std::get<3>( i ) = false; // it's a threat again
      }

      // reset blue missiles
      for( auto& i : this->blueMissiles )
      {
         i->setLaunchPosition( this->launcher->getPosition() );
         i->reset();
      }
   }
   else if( key.keysym.sym == SDLK_p )
   {
      std::cout << "Printing Trajectory info...\n";

      for( auto& i : this->interceptors )
         std::cout << std::get<0>( i )->getTrajectoryInfo() << "\n";
   }
   else if( key.keysym.sym == SDLK_d )
   {
      std::cout << "Printing Radar Sensor info...\n";
      std::cout << this->sensor->getSensorInfo() << "\n";
   }
   else if( key.keysym.sym == SDLK_v )
   {
      this->sensor->isVisible = !this->sensor->isVisible;
   }
}

void GLViewDefenseDaemon::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}

void Aftr::GLViewDefenseDaemon::loadMap()
{
   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   ManagerOpenGLState::GL_CLIPPING_PLANE = 10000.0;
   ManagerOpenGLState::GL_NEAR_PLANE = 1.0f;
   ManagerOpenGLState::enableFrustumCulling = false;
   Axes::isVisible = true;
   // this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+

   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   this->cam->setPosition( 15, 15, 10 );
   this->cam->setPosition( 25.0f, -15.5f, 2.6f );
   this->cam->setCameraLookDirection( Vector{ -0.612123f, 0.765189f, 0.199524f } );

   //SkyBox Textures readily available
   std::vector< std::string > skyBoxImageNames; //vector to store texture paths
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/Sunset_Happy122.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/InTheClouds_Happy85.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/InTheCloudsAfternoon_Happy112.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/BlueGreenCloudy_Happy124.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/CumulusBlueAfternoon_Happy11.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/2KRes/GreenSkyOverOceanBlue_Happy72.png" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_water+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_dust+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_mountains+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_winter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/early_morning+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_afternoon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy3+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_deepsun+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_evening+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_noon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_warp+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_Hubble_Nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_gray_matter+6.jpg" );
   skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_easter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_hot_nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_ice_field+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_lemon_lime+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_milk_chocolate+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_solar_bloom+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_thick_rb+6.jpg" );

   {  //Setup lighting and shadows
      float ga = 0.2f; //Global Ambient Light level for this module
      ManagerLight::setGlobalAmbientLight( aftrColor4f( ga, ga, ga, 1.0f ) );
      this->light = WOLight::New();
      this->light->isDirectionalLight( true );
      //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
      //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
      this->light->getModel()->setDisplayMatrix( Mat4::rotateIdentityMat( { 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD ) );
      this->light->isPointLight( true );
      this->light->setPosition( Vector{ 157.132f, -41.497f, 307.859f } ); //place light in the sky roughly where the sun aligns w/ the skybox...
      this->light->setLabel( "Light" );
      worldLst->push_back( this->light );
   }

   // {  //if your graphics card doesn't support shadowing. Then comment out this block of code and run again.

   //    //enable shadow mapping, it will perform an orthographic projection from the camera's position, in its look direction, into a
   //    //texture who's size matches the frustum lengths. In this case 8192 / 16.
      this->light->isShadowGenerator( false );         //Toggle ME  to turn shadows ON or OFF!!!! False = off, True = on
      this->glRenderer->isUsingShadowMapping( false ); //Toggle ME  to turn shadows ON or OFF!!!! False = off, True = on
   //    if( this->glRenderer->isUsingShadowMapping() )
   //    {
   //       this->glRenderer->getShadowMapCamera().setCameraOrthographicSymmetricFrustumLengths( static_cast<float>( 8192 / 16 ), static_cast<float>( 8192 / 16 ) );
   //       this->glRenderer->getShadowMapCamera().setCameraNearClippingPlaneDistance( 1.0f );
   //       this->glRenderer->getShadowMapCamera().setCameraNearClippingPlaneDistance( 1000.0f );
   //    }
   // }

   {  //Create the SkyBox
      WO* wo = WOSkyBox::New( skyBoxImageNames.at( 0 ), this->getCameraPtrPtr() );
      wo->setPosition( Vector( 0, 0, 0 ) );
      wo->setLabel( "Sky Box" );
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
      worldLst->push_back( wo );
   }

   {  //Create the infinite grass plane (the floor)
      std::string grass( ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl" );
      WO* wo = WO::New( grass, Vector( 8, 8, 8 ), MESH_SHADING_TYPE::mstFLAT );
      wo->setPosition( Vector( 0, 0, 0 ) );
      wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
      ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at( 0 )->getSkins().at( 0 );
      grassSkin.getMultiTextureSet().at( 0 )->setTextureRepeats( 40.0f );
      grassSkin.setAmbient( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Color of object when it is not in any light
      grassSkin.setDiffuse( aftrColor4f( 1.0f, 1.0f, 1.0f, 1.0f ) ); //Diffuse color components (ie, matte shading color of this object)
      grassSkin.setSpecular( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Specular color component (ie, how "shiny" it is)
      grassSkin.setSpecularCoefficient( 10 ); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
      wo->setLabel( "Grass" );
      worldLst->push_back( wo );
   }

   {
      this->launcher = WO::New( ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Launcher/RLauncher.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO );
      float z = this->launcher->getModel()->getBoundingBox().getlxlylz().z / 2.0f;
      this->launcher->setPosition( Vector( ManagerEnvironmentConfiguration::getVariableValue( "launcherPosition" ) ) );
      std::cout << "RLauncher size is " << this->launcher->getModel()->getBoundingBox().getlxlylz().toString() << "\n";
      this->worldLst->push_back( this->launcher );
   }

   this->numMissiles = AftrUtilities::toInt( ManagerEnvironmentConfiguration::getVariableValue( "numMissiles" ) );

   // randomly place all red missiles
   // alternatively, place red missiles according to test parameters
   for( int i = 0; i < this->numMissiles; i++ )
   {
      auto interceptor = WOInterceptorMissile::New();
      interceptor->setPosition( Vector{ 25,100,0 } ); // this gets changed in repopulateFromConfFile, which is called at the end of this function
      std::cout << "Missile size is " << interceptor->getModel()->getBoundingBox().getlxlylz().toString() << "\n";
      this->worldLst->push_back( interceptor );

      WOTrackingRibbonBase* ribbon = WOTrackingRibbonBase::New( interceptor );
      ribbon->setParentWorldObject( interceptor );
      ribbon->setPlumblineRatio( UINT_MAX );
      interceptor->setTrackingRibbon( ribbon );
      interceptor->setDefaultColor( Aftr::aftrColor4ub( 255,0,255,100 ) );
      interceptor->getChildren().push_back( ribbon );

      // push the missile, a location history, a time history, and a flag for whether or not the missile is destroyed
      std::vector< std::tuple< float, float, float > > locationHistory;
      std::vector< std::chrono::system_clock::time_point > timeHistory;
      this->interceptors.push_back( std::make_tuple( interceptor, locationHistory, timeHistory, false ) );
   }

   // initialize all blue missiles
   for( int i = 0; i < this->numMissiles; i++ )
   {
      auto missile = WOInterceptorMissile::New();
      missile->setPosition( this->launcher->getPosition() );
      missile->setDetonationPosition( Vector{ 1000000, 1000000, 1000000 } ); // we change this pretty quickly, but we don't want the blue missile to accidentally get there before we change it
      std::cout << "Missile size is " << missile->getModel()->getBoundingBox().getlxlylz().toString() << "\n";
      this->worldLst->push_back( missile );

      WOTrackingRibbonBase* ribbon = WOTrackingRibbonBase::New( missile );
      ribbon->setParentWorldObject( missile );
      ribbon->setPlumblineRatio( UINT_MAX );
      missile->setTrackingRibbon( ribbon );
      missile->setDefaultColor( Aftr::aftrColor4ub( 0,255,0,100 ) );
      missile->getChildren().push_back( ribbon );

      std::vector< std::tuple< float, float, float > > locationHistory;
      this->blueMissiles.push_back( missile );
   }

   {
      {
         const float offset = 1.731f;

         this->dish = WORadarDish::New();
         this->dish->setPosition(Vector(ManagerEnvironmentConfiguration::getVariableValue("radarDishPosition")));
         this->worldLst->push_back(this->dish);

         WO* tower = WO::New(ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Radar/tower.3ds", Vector{1, 1, 1}, MESH_SHADING_TYPE::mstAUTO);
         this->dish->getChildren().push_back(tower);
         tower->setParentWorldObject(this->dish);
         tower->setPosition(
            this->dish->getPosition().x,
            this->dish->getPosition().y,
            this->dish->getPosition().z - offset - tower->getModel()->getBoundingBox().getlxlylz().z / 2.0f
         );
      }

      {
         this->sensor = WORadarSensor::New();
         this->sensor->setParentWorldObject(this->dish);
         this->sensor->setPosition(this->dish->getPosition());
         this->sensor->lockWRTparent();
         this->dish->getChildren().push_back(this->sensor);
         this->dish->attachSensor(this->sensor);
      }

      auto onFilterFunc = [&](const WO* wo, const std::chrono::system_clock::time_point &t) -> std::tuple<unsigned int, std::vector<Vector>>
      {
         return std::make_tuple(wo->getID(), std::vector<Vector>{Vector(
            this->sensor->getPosition().distanceFrom(wo->getPosition()),
            (wo->getPosition() - this->sensor->getPosition()).getTheta() * 180.0f / PI,
            (wo->getPosition() - this->sensor->getPosition()).getPhi() * 180.0f / PI
         )});
      };

      auto onScanDataAvailable = [&](unsigned int id, const std::chrono::system_clock::time_point &t, std::vector<Vector>&& v)
      {
         for (auto& location : v)
         {
            const float epsilon = 0.00001f;

            float horizontal_distance = location.x * cos( location.z * DEGtoRAD );
            float z_position = this->dish->getPosition().z - location.x * sin( location.z * DEGtoRAD );

            std::tuple< float, float, float > position = std::make_tuple(
               this->dish->getPosition().x + horizontal_distance * cos( location.y * DEGtoRAD ),
               this->dish->getPosition().y + horizontal_distance * sin( location.y * DEGtoRAD ),
               z_position > epsilon ? z_position : 0
            );

            // std::cout << "Missile SIG " << id << " is at " << location << " relative to the sensor - " << AftrUtilities::aftrConvertTimePointToString(t) << "\n";
            // std::cout << "Missile SIG " << id << " is at (" << std::get<0>(position) << ", " << std::get<1>(position) << ", " << std::get<2>(position) << ") absolute - " << AftrUtilities::aftrConvertTimePointToString(t) << "\n\n";

            int i = this->getMissileById( id );

            // we only care about points where the interceptor is in the air
            if( z_position > epsilon )
            {
               std::get<2>( this->interceptors[i] ).push_back( t );
               std::get<1>( this->interceptors[i] ).push_back( position );
            }

            // if we still need to fire at the interceptor and it has at least three observed points
            if( !std::get<3>( this->interceptors[i] ) && std::get<1>( this->interceptors[i] ).size() >= 3 )
            {
               std::pair< bool, std::vector< std::vector< float > > > missile = computeCoefficients( this->interceptors[i] );

               // if the missile is going to land within the base's radius, fire a blue missile at it
               if( missile.first )
                  this->launchBlueMissile( this->blueMissiles[i], computeIntersectionPoint( this->interceptors[i], missile.second ) );
               else
                  std::cout << "---------- Missile SIG " << id << " isn't going to land in the base\n";

               // regardless of whether or not we shot at the missile, we're done considering it
               std::get<3>( this->interceptors[i] ) = true;
            }
         }
      };

      auto onObjectIlluminated = [&]( const WO* wo )
      {
         if ( isInterceptor( wo ) )
         {
            int i = this->getMissileById( wo->getID() );
            std::get<0>( this->interceptors[i] )->currentlyDetectsRadarBeams( true );
         }
      };

      //Add targets, such as the interceptor and perhaps for ease of debugging, the camera, to the sensor's radar scan list
      for( auto& i : this->interceptors )
         this->sensor->addPossibleTargetToRadarScanList( std::get<0>( i ) , onObjectIlluminated );

      // this->sensor->addPossibleTargetToRadarScanList(this->getCamera(), onObjectIlluminated);

      //Set the callback functions to simple stubs that simply display the information without useful processing (just for phase 2)
      this->sensor->setFilteringOfRawScanDataAlgorithm(onFilterFunc);
      this->sensor->subscribeToRadarTrackingInfo(onScanDataAvailable);
   }

   {
      WO* wo = WO::New( ManagerEnvironmentConfiguration::getSMM() + "/models/DefenseDaemon/Doppler/doppler.3ds", Vector{ 1,1,1 }, MESH_SHADING_TYPE::mstAUTO );
      float z = wo->getModel()->getBoundingBox().getlxlylz().z / 2.0f;
      wo->setPosition( Vector{ -10,30,z } );
      std::cout << "Doppler size is " << wo->getModel()->getBoundingBox().getlxlylz().toString() << "\n";
      this->worldLst->push_back( wo );
   }
   
   createDefenseDaemonWayPoints();
   this->repopulateFromConfFile();
}

void GLViewDefenseDaemon::repopulateFromConfFile()
{
   ManagerEnvironmentConfiguration::populateFromFile();

   float maxRangeMeters = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "maxRangeMeters" ) );
   float minRangeMeters = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "minRangeMeters" ) );
   float maxTimeToImpact = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "maxTimeToImpact" ) );
   float minTimeToImpact = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "minTimeToImpact" ) );
   float redTimeToImpact = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "redTimeToImpact" ) );
   float maxDistance = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "maxDistance" ) );
   float initialRedLaunchZ = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "initialRedLaunchZ" ) );
   float radarFieldOfViewDeg = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "radarFieldOfViewDeg" ) );
   float radarScanRangeMeters = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "radarScanRangeMeters" ) );
   Vector radarDishPosition = Vector( ManagerEnvironmentConfiguration::getVariableValue( "radarDishPosition" ) );
   Vector launcherPosition = Vector( ManagerEnvironmentConfiguration::getVariableValue( "launcherPosition" ) );

   std::cout << "Repopulated from conf file... New values are:\n";
   std::cout << "maxRangeMeters = " << maxRangeMeters << "\n";
   std::cout << "minRangeMeters = " << minRangeMeters << "\n";
   std::cout << "maxTimeToImpact = " << maxTimeToImpact << "\n";
   std::cout << "minTimeToImpact = " << minTimeToImpact << "\n";
   std::cout << "maxDistance = " << maxDistance << "\n";
   std::cout << "initialRedLaunchZ = " << initialRedLaunchZ << "\n";
   std::cout << "radarFieldOfView = " << radarFieldOfViewDeg << "\n";
   std::cout << "radarScanRangeMeters = " << radarScanRangeMeters << "\n";
   std::cout << "radarDishPosition = " << radarDishPosition << "\n";
   std::cout << "launcherPosition = " << launcherPosition << "\n";

   // used for testing different parameters
   maxDistance = radarScanRangeMeters + 10;

   float minDistance = -1 * maxDistance;

   srand( static_cast< unsigned int >( time( NULL ) ) );

   // place all red missiles
   for( auto& i : this->interceptors )
   {
      auto strat = std::make_unique< MotionParabolic >();

      Vector initialRedLaunchPos(
         minDistance + static_cast< float >( rand() ) / ( static_cast< float >( RAND_MAX / ( maxDistance - minDistance ) ) ),
         minDistance + static_cast< float >( rand() ) / ( static_cast< float >( RAND_MAX / ( maxDistance - minDistance ) ) ),
         initialRedLaunchZ
      );

      // float rangeMeters = minRangeMeters + static_cast< float >( rand() ) / ( static_cast< float >( RAND_MAX / ( maxRangeMeters - minRangeMeters ) ) );
      float rangeMeters = radarScanRangeMeters / 2.0f;
      // float timeToImpact = minTimeToImpact + static_cast< float >( rand() ) / ( static_cast< float >( RAND_MAX / ( maxTimeToImpact - minTimeToImpact ) ) );
      float timeToImpact = redTimeToImpact;
      float headingDeg = static_cast< float >( rand() ) / ( static_cast< float >( RAND_MAX / 360.0 ) );

      std::get<0>( i )->setPositionIgnoringAllChildren( initialRedLaunchPos );
      std::get<0>( i )->setMotionStrategy( std::move( strat ), rangeMeters, timeToImpact, headingDeg, initialRedLaunchPos);
   }

   this->launcher->setPosition( launcherPosition );

   //you also have enough info to update your Radar sensors scanning parameters
   this->sensor->setScanRangeMeters( radarScanRangeMeters );
   this->sensor->setHorzFOVDeg( radarFieldOfViewDeg );
   this->dish->setPosition( radarDishPosition );
}


void GLViewDefenseDaemon::createDefenseDaemonWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 5000;
   params.useCamera = true;
   params.visible = true;
   WOWayPointSpherical* wayPt = WOWP1::New( params, 3 );
   wayPt->setPosition( Vector( 50, 0, 3 ) );
   worldLst->push_back( wayPt );
}

bool GLViewDefenseDaemon::isInterceptor( const WO* wo )
{
   for( auto& i : this->interceptors )
      if( std::get<0>( i ) == wo )
         return true;

   return false;
}

const int GLViewDefenseDaemon::getMissileById( const unsigned int id )
{
   for( auto i = 0; i < this->interceptors.size(); i++ )
      if( std::get<0>( this->interceptors[i] )->getID() == id )
         return i;

   return -1;
}

void GLViewDefenseDaemon::launchBlueMissile(
   WOInterceptorMissile* missile,
   std::tuple< Vector, float, float > intersection )
{
   const float gravity = 9.81f;
   auto strat = std::make_unique< MotionParabolic >();

   // account for the delay in sensing the red missile versus computing the intersection point
   float flightTime = std::get<1>( intersection ) - std::get<2>( intersection );

   // the motion algorithm assumes the missile goes to z = 0; because we want it to explode elsewhere, we need to use more physics
   float velocityX = ( std::get<0>( intersection ).x - missile->getPosition().x ) / flightTime, // V = s/t
         velocityY = ( -std::get<0>( intersection ).y + missile->getPosition().y ) / flightTime, // V = s/t
         velocityZ = ( gravity * flightTime * flightTime / 2 + std::get<0>( intersection ).z - missile->getPosition().z ) / flightTime; // Vi = ((1/2)at^2 - s) / t
   float timeToImpact = 2 * velocityZ / gravity; // what goes up must come down
   float headingDeg = atan2( velocityY, velocityX ) * 180.0f / PI;
   float rangeMeters = sqrt( velocityX * velocityX + velocityY * velocityY ) * timeToImpact; // s = Vt

   // don't launch if it's too late to stop the missile
   if( timeToImpact < 0 )
      return;

   missile->setMotionStrategy( std::move( strat ), rangeMeters, timeToImpact, headingDeg, missile->getPosition() );
   missile->setDetonationPosition( std::get<0>( intersection ) );
   missile->launch();
}

std::pair< bool, std::vector< std::vector< float > > > GLViewDefenseDaemon::computeCoefficients( interceptorMissile& interceptor )
{
   std::vector< float > x_pts, y_pts, z_pts, t_pts;

   // save xyz location history
   for( auto& location : std::get<1>( interceptor ) )
   {
      x_pts.push_back( std::get<0>( location ) );
      y_pts.push_back( std::get<1>( location ) );
      z_pts.push_back( std::get<2>( location ) );
   }

   // save time history
   for( auto& t : std::get<2>( interceptor ) )
   {
      std::chrono::duration< float, std::micro > microseconds = t - std::get<2>( interceptor )[0];
      float seconds = microseconds.count() / 1'000'000; //convert microseconds to seconds by dividing by a million
      t_pts.push_back( seconds );
   }

   // compute the coefficients of the three lines
   std::vector< float > x_coefficients = PolyFit::PolyFit( t_pts, x_pts, 1 ),
                        y_coefficients = PolyFit::PolyFit( t_pts, y_pts, 1 ),
                        z_coefficients = PolyFit::PolyFit( t_pts, z_pts, 2 );

   // compute the time and location of impact
   float a = z_coefficients[2], b = z_coefficients[1], c = z_coefficients[0];
   std::vector< float > impact_time = { ( -b - sqrt( b * b - 4 * a * c ) ) / ( 2 * a ) };
   std::vector< float > x_impact_location = PolyFit::PolyVal( x_coefficients, impact_time ),
                        y_impact_location = PolyFit::PolyVal( y_coefficients, impact_time );

   std::vector< std::vector< float > > interceptorCoefficients = { x_coefficients, y_coefficients, z_coefficients };
   float baseRadius = AftrUtilities::toFloat( ManagerEnvironmentConfiguration::getVariableValue( "baseRadius" ) );

   // always return interceptorCoefficients
   // also, return true if the interceptor will land within the base's radius
   if( abs( x_impact_location[0] ) <= baseRadius && abs( y_impact_location[0] ) <= baseRadius )
      return std::make_pair( true, interceptorCoefficients );
   return std::make_pair( false, interceptorCoefficients );
}

std::tuple< Vector, float, float > GLViewDefenseDaemon::computeIntersectionPoint(
   interceptorMissile& interceptor,
   std::vector< std::vector< float > >& interceptorCoefficients )
{
   // compute the time since we first observed the interceptor
   std::chrono::duration< float, std::micro > d = std::chrono::system_clock::now() - std::get<2>( interceptor )[0];
   float delay = d.count() / 1'000'000; //convert microseconds to seconds by dividing by a million

   // compute the time of impact
   float a = interceptorCoefficients[2][2], b = interceptorCoefficients[2][1], c = interceptorCoefficients[2][0];
   std::vector< float > impact_time = { ( -b - sqrt( b * b - 4 * a * c ) ) / ( 2 * a ) };

   // intercept the missile half a second before it strikes
   std::vector< float > i_time = { ( impact_time[0] - 0.5f ) };

   // compute the point of intersection
   Vector i_point(
      PolyFit::PolyVal( interceptorCoefficients[0], i_time )[0],
      PolyFit::PolyVal( interceptorCoefficients[1], i_time )[0],
      PolyFit::PolyVal( interceptorCoefficients[2], i_time )[0]
   );

   return std::make_tuple( i_point, i_time[0], delay );
}