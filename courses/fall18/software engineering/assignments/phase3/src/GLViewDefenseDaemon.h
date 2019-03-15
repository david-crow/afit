#pragma once

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

   WOLight* light = nullptr;
   WOInterceptorMissile* interceptor = nullptr;
   WORadarSensor* sensor = nullptr;
   WORadarDish* dish = nullptr;
   Gnuplot* gp = nullptr;
   Gnuplot* pf = nullptr;
   std::vector< std::pair< double, double > > locationHistory;
};

/** \} */

} //namespace Aftr
