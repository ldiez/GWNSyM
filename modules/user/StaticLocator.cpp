#include "StaticLocator.h"
#include "Log.h"

#include "User.h"
#include "Distance.h"

LOG_REGISTER_MODULE("StaticLocator")
static const double UserHeight = 1.5;

StaticLocator::StaticLocator() {
    BEGEND;
}

void
StaticLocator::operator()(gnsm::Ptr_t<User> user) {
    BEG;
    auto x = 10*user->GetId();
    auto y = 10*user->GetId();
    
    user->SetPosition(Position(x,y,UserHeight));
    END;
}


