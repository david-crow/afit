#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>


class GreatDane
{
   public:
   GreatDane( std::string const& dogName, int rank ) : name( dogName ), ranking( rank ) {}
   std::string toString() const;   
   int getRanking() const { return this->ranking; }
   std::string const& getName() const { return this->name; }
   private:
   std::string name = "";
   int ranking = 0;
};

std::function< int( GreatDane const&, GreatDane const& ) > makeFunctionCapturingNoState()
{
   std::function< int( GreatDane const&, GreatDane const& ) > rankAgainstEachOther = []( GreatDane const& a, GreatDane const& b )
   {
      std::cout << "Comparing " << a.getName() << " against " << b.getName() << "\n";
      return a.getRanking() - b.getRanking();
   };
   return std::move( rankAgainstEachOther );
}

std::function< int( GreatDane const& ) > makeFunctionCapturingScope()
{
   GreatDane winston{ "Winston", 5 };
   GreatDane kate{ "Kate", 7 };
   GreatDane scooby{ "Scooby", 10 };

   std::function< int( GreatDane const& ) > rankAgainstKate = [&kate]( GreatDane const& d )
   {
      std::cout << "Comparing " << d.getName() << " against " << kate.getName() << "\n";
      return d.getRanking() - kate.getRanking();
   };

   //Let's test our rank function:
   std::cout << rankAgainstKate( winston ) << "\n";
   std::cout << rankAgainstKate( kate ) << "\n";
   std::cout << rankAgainstKate( scooby ) << "\n";

   return std::move( rankAgainstKate );
} //what happens to ALL local variables when this functions returns???

int main()
{
   GreatDane henry{ "Henry", 4 };
   GreatDane indy{ "Indiana Jones", 8 };

   //What if we put "kate" back? This isn't really the captured Kate...
   //GreatDane kate{ "Kate", 99 };

   auto rankAgainstKate = makeFunctionCapturingScope();
   std::cout << "Now back in main()... Let's use our ranking function...\n";
   //std::cout << rankAgainstKate( henry );

   //auto rankAgainstEachOther = makeFunctionCapturingNoState();
   //std::cout << rankAgainstEachOther( henry, indy ) << "\n";
   
   return 0;
}