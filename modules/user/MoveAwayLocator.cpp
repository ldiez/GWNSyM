#include "MoveAwayLocator.h"
#include "User.h"
#include "Log.h"

LOG_REGISTER_MODULE("MoveAwayLocator");

MoveAwayLocator::MoveAwayLocator(Position init, units::m step)
: m_init(init)
, m_step(step)
, m_iter(0) {
    BEG;
    END;
}

void
MoveAwayLocator::SetIteration(std::uint32_t iter) {
    BEG;
    m_iter = iter;
    END;
}

void
MoveAwayLocator::operator()(gnsm::Vec_t<User> us) {
    BEG;
    for (auto& u : us) {
        auto aux = m_init;
        aux.SetX(m_step.RawVal() * (m_iter/1));
        u->SetPosition(aux);
    }
    END;
}



