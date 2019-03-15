#pragma once

#include <chrono>
#include "GLView.h"
#include "gnuplot-iostream.h"

namespace Aftr
{
   class Camera;
   class WOLight;
   class WOInterceptorMissile;
   class WORadarDish;
   class WORadarSensor;

/**
   \class GLViewDefenseDaemon
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

using interceptorMissile = std::tuple<
   WOInterceptorMissile*,                                // a pointer to the missile itself
   std::vector< std::tuple< float, float, float > >,     // the missile's location histories
   std::vector< std::chrono::system_clock::time_point >, // the missile's time histories
   bool                                                  // whether or not the missile is destroyed
>;

class GLViewDefenseDaemon : public GLView
{
public:
   static GLViewDefenseDaemon* New( const std::vector< std::string >& outArgs );
   virtual ~GLViewDefenseDaemon();
   virtual void updateWorld(); ///< Called once per frame
   virtual void loadMap(); ///< Called once at startup to build this module's scene
   virtual void createDefenseDaemonWayPoints();
   virtual void onResizeWindow( GLsizei width, GLsizei height );
   virtual void onMouseDown( const SDL_MouseButtonEvent& e );
   virtual void onMouseUp( const SDL_MouseButtonEvent& e );
   virtual void onMouseMove( const SDL_MouseMotionEvent& e );
   virtual void onKeyDown( const SDL_KeyboardEvent& key );
   virtual void onKeyUp( const SDL_KeyboardEvent& key );

   void repopulateFromConfFile();

protected:
   GLViewDefenseDaemon( const std::vector< std::string >& args );
   virtual void onCreate();

   bool isInterceptor( const WO* wo );
   const int getMissileById( const unsigned int id );

   void launchBlueMissile( WOInterceptorMissile* missile, std::tuple< Vector, float, float > intersection );
   std::pair< bool, std::vector< std::vector< float > > > computeCoefficients( interceptorMissile& interceptor );
   std::tuple< Vector, float, float > computeIntersectionPoint(
      interceptorMissile& interceptor,
      std::vector< std::vector< float > >& interceptorCoefficients
   );

   std::vector< interceptorMissile > interceptors;
   std::vector< WOInterceptorMissile* > blueMissiles;
   WOLight* light = nullptr;
   WORadarSensor* sensor = nullptr;
   WORadarDish* dish = nullptr;
   WO* launcher = nullptr;
   int numMissiles = 0;
};

/** \} */

} //namespace Aftr
