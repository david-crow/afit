#include "Bridge.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <sstream>

namespace Bridge
{

class Impl
{
public:
   Impl( std::string const& url );
   ~Impl();
   bool connect();
   bool disconnect();
   std::future< int > start();
   void onImageReadyCallback( std::function< void( std::string const& img ) > );
private:
   int waitForImage( std::chrono::seconds t );
   std::string url = "";
   std::function< void( std::string const& img ) > func;
};

Impl::Impl( std::string const& url ){ this->url = url; }
Impl::~Impl() { this->disconnect(); }

bool Impl::connect() { std::cout << "Connecting to camera at " << this->url << "\n"; }
bool Impl::disconnect() { std::cout << "Disconnecting from camera at " << this->url << "\n"; }
void Impl::onImageReadyCallback( std::function< void( std::string const& img ) > f )
{
   this->func = f;
}

std::future< int > Impl::start()
{
   std::future< int > f = std::async( std::launch::async, std::bind( &Impl::waitForImage, this, std::chrono::seconds(10) ) );
   return std::move( f );
}

int Impl::waitForImage( std::chrono::seconds t )
{
   std::cout << "Waiting for image...\n";
   auto startTime = std::chrono::system_clock::now();
   auto stopTime = startTime + t;

   int count = 0;
   do
   {
      std::cout << "Sensor released image...Notifying user...\n";
      ++count;
      std::stringstream ss;
      ss << "Pretty Animal..." << count << "\n";
      this->func( ss.str() );
      //std::cout << ss.str() << "\n";
      std::this_thread::sleep_for( std::chrono::seconds(2) );

   }
   while( std::chrono::system_clock::now() < stopTime );
   return count;

}



//--------------------Publicly callable methods
CameraImageStreamer::CameraImageStreamer( std::string const& url ) { this->pimpl = new Impl( url ); }

CameraImageStreamer::~CameraImageStreamer() { delete pimpl; pimpl = nullptr; }

bool CameraImageStreamer::connect(){
   if( pimpl )
      return pimpl->connect();
   return false;
}

bool CameraImageStreamer::disconnect(){
   if( pimpl )
      return pimpl->disconnect();
   return false;
}

std::future< int > CameraImageStreamer::start(){
   if( pimpl ) return pimpl->start();
   return std::move( std::future<int>() );
}

void CameraImageStreamer::onImageReadyCallback( std::function< void( std::string const& img ) > callback ){
   if( pimpl )
      pimpl->onImageReadyCallback( callback );
}

} //namespace Bridge


int main()
{
   Bridge::CameraImageStreamer* s = new Bridge::CameraImageStreamer( "port 3" );
   s->connect();
   std::future<int> future = s->start();
   std::cout << "@@@Back in main thread...@@@\n";

   std::function< void( std::string const& ) > f = [] ( std::string const& s ) { std::cout << "My lambda is processing: " << s << "\n"; };
   s->onImageReadyCallback( f );

   
   for(int i = 0; i < 10; i++)
   {
      std::cout << "@@@In Main Doing Work@@@\n";
      std::this_thread::sleep_for( std::chrono::milliseconds(500) ); 
   }

   std::cout << "*****Camera Image acquired " << future.get() << " images...*****\n";
   std::cout << "@@@Main thread exit...@@@\n";
   return 0;
}