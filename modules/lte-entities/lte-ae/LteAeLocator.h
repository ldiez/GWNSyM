#ifndef LTEAELOCATOR_H
#define	LTEAELOCATOR_H

#include "LteEnb.h"
#include "Position.h"

class LteAeLocator
{
public:
    LteAeLocator ( std::string fileName );

    void operator() ( LteEnb& ae );
private:

    /**
     * \brief Load the access elements positions to be stored locally
     */
    void Load ( std::string fileName );

    // local store for the access elements positions
    std::vector<Position> m_positions;

};

#endif	/* LTEAELOCATOR_H */

