#ifndef PRINTCELLS_H
#define PRINTCELLS_H

#include "lte-ae/LteEnb.h"

class PrintCells
{
public:
    PrintCells()
    {
    }

    void operator()(gnsm::Vec_t<LteEnb> macro, gnsm::Vec_t<LteEnb> pico)
    {
        // open a file, print coordinates

        std::ofstream ofs_;
        ofs_.open("MACRO.dat", std::ios::out);

        if (ofs_.is_open())
        {
            for (auto& item_ : macro)
            {
                auto pos_ = item_->ReadPosition();
                ofs_ << pos_.GetX() << "\t" << pos_.GetY() << std::endl;
            }
            ofs_.close();
        }
        
        ofs_.open("PICO.dat", std::ios::out);
        if (ofs_.is_open())
        {
            for (auto& item_ : pico)
            {
                auto pos_ = item_->ReadPosition();
                ofs_ << pos_.GetX() << "\t" << pos_.GetY() << std::endl;
            }
            ofs_.close();
        }
    }
};


#endif /* PRINTCELLS_H */

