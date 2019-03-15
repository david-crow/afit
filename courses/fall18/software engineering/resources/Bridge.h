#include <string>
#include <functional>
#include <future>

namespace Bridge
{

class Impl;

/**
 * This example shows the bridge pattern. In C++ this can sometimes be represented using the pimpl
 * idiom. In this case, as long as the public interface does not change, the client will never have
 * to recompile their code to use the latest version, they can simply update to the latest dynamically
 * linked library containing the CameraImageStreamer's implementation.
 */

class CameraImageStreamer
{
   public:
      CameraImageStreamer( std::string const& url );
      ~CameraImageStreamer();
      CameraImageStreamer( const CameraImageStreamer& ) = delete;
      CameraImageStreamer& operator=( const CameraImageStreamer& toCopyAssign ) = delete;
      CameraImageStreamer( CameraImageStreamer&& ) = delete;
      CameraImageStreamer& operator=( const CameraImageStreamer&& toMoveAssign ) = delete;

      bool connect();
      bool disconnect();
      std::future< int > start();
      void onImageReadyCallback( std::function< void( std::string const& img ) > );

   private:
      Impl* pimpl = nullptr; //C++ uses the PIMPL idiom to insulate the Bridge from its implementation
};

} //namespace Bridge