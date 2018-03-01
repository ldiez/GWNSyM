#include "RandomAnulusLocator.h"
#include "Log.h"

LOG_REGISTER_MODULE("RandomAnulusLocator");

 RandomAnulusLocator::RandomAnulusLocator(Position center, units::m min, units::m max) :
    m_range(min.RawVal(), max.RawVal()),
    m_angle(0.0, 360.0),
    m_center(center) {
   
    }

void 

 RandomAnulusLocator::operator()(gnsm::Ptr_t<User> user) {
        BEG;
        auto range_ = m_range(m_re);
        auto angle_ = m_angle(m_re);

        auto x_ = range_ * std::cos(angle_);
        auto y_ = range_ * std::sin(angle_);
        double UserHeight = 1.5;
        user->SetPosition({x_ + m_center.GetX(), y_ + m_center.GetY(), UserHeight});
        INFO("User ", user->GetId(), " at ", user->GetPosition());
    END;
    }