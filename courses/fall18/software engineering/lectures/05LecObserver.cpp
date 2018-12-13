#include <iostream>
#include <string>
#include <future>
#include <random>
#include <functional>
#include <array>

//Compile this code with: g++ 05LecObserver.cpp -lpthread
// -l means link against a library
//  pthread is the library against which the linker links (probably called /lib/x86_64-linux-gnu/libpthread-2.27.so)

//This approach uses inheritance to implement the observer pattern
//Contrast this to 05LecObserverT.cpp
class Observer
{
   public:
   Observer() = default;
   virtual ~Observer() = default;
   
   virtual void onNotify( const std::string& msg ) = 0;
};

class CellPhone : public Observer
{
   public:
   CellPhone( const std::string number ) : number( number ) {}

   virtual void onNotify( const std::string& msg )
   {
      std::cout << "!!!Cell Phone " << this->number << " was notified that: " << msg << "!!!\n\n";
   }

   std::string number = "";
};

class GMail
{
   public:
   std::vector< std::shared_ptr<Observer> > clients;
   void subscribe( std::shared_ptr< Observer > newClient )
   {
      std::cout << "A Client has subscribed...\n";
      clients.push_back( std::move( newClient ) );
   }
   
   std::future<void> startSecurityService( int desiredParanoiaLevel, std::mt19937& gen )
   {
      std::cout << "Launching an async guard dog...\n";
      //This will launch a new thread (could be run on a separate processor)
      std::future<void> f = std::async( std::launch::async, std::bind( &GMail::asyncSecurityMonitor, this, desiredParanoiaLevel, gen ) );
      std::cout << "Done Launching an async guard dog... Now You're account will be safe, I promise, I'm equifax, trust me...\n";
      return std::move( f );
   }
   
   void asyncSecurityMonitor( int paranoiaLevel, std::mt19937& gen )
   {
      std::cout << "  **I am the guard dog thread... I'll alert all subscribers when their account is in danger of haxorz (at least until your trail expires)...\n";
      auto startTime = std::chrono::system_clock::now();
      auto duration = std::chrono::seconds(10);
      auto stopTime = startTime + duration;
      
      do
      {
         std::uniform_int_distribution<> distribution( 1, 100 );
         int worryLevel = distribution( gen );         
         std::cout << "  **Threat level is " << worryLevel << "\n";
         if( worryLevel > paranoiaLevel )
         {
            std::string msg = "  **Security Threat level " + std::to_string(worryLevel) + "... WORRY about your account...\n";
            for( auto& c : clients )
               c->onNotify( msg );
         }
         else
         {
            std::cout << "  **Equifax told me your account looks safe...Time to relax...\n\n\n";
         }
         
         auto doodleStopTime = std::chrono::system_clock::now() + std::chrono::seconds(2);
         const std::array< char, 4 > doodle = {'-','\\', '|', '/'};
         int i = 0;
         while( std::chrono::system_clock::now() < doodleStopTime )
         {  
            std::cout << ' ' << doodle[i++] << '\r' << std::flush;
            i = i % doodle.size();
         }
      }
      while( std::chrono::system_clock::now() < stopTime );
      
      std::cout << "  **Your free security trial has lapsed,\n"
                << "     **Would you like me sell your account\n"
                << "     **info to continue with your free trial?\n";
   }
};


int main( int argc, char* argv[] )
{
   std::cout << "Okay folks, let's get started...\n";
   
   //make a random number engine:
   std::random_device rd;    //C++ random device -- used to obtain a seed for the random number engine
   std::mt19937 gen( rd() ); //Standard mersenne_twister_engine seeded with random_device rd()
   std::vector< std::shared_ptr< CellPhone > > phones;

   GMail g;
   //Here we make a normal cell phone that is subscribable
   std::shared_ptr< CellPhone > myCell = std::make_shared<CellPhone>( "867 5309" );
   phones.push_back( std::move( myCell ) );
   std::shared_ptr< CellPhone > aCell = std::make_shared<CellPhone>( "444 4444" );
   phones.push_back( std::move( aCell ) );

   for( auto& p : phones )
   {
      g.subscribe( p ); //Gmail is now an Observer that watches for an event and will Notify the cell phone when that event occurs
   }

   std::future<void> future = g.startSecurityService( 42, gen ); //launch a watchdog thread to look for threats against your bank account
   for (int i = 0; i < 10 ; i++)
   {
      std::cout << "==================Main thread - Doing other things now... Let me know via my cell if I'm in trouble...\n";
      std::this_thread::sleep_for( std::chrono::seconds(2) );
   }

   std::cout << "Done... Press ENTER to exit...\n";
   return 0;
}