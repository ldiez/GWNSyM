#include <random>
#include <algorithm>
#include <cmath>

#include "PathLoss.h"
#include "Log.h"

LOG_REGISTER_MODULE("PathLoss");

static constexpr bool Shadow_s = true;

namespace LTE {

    using uDoubleDist_t = std::uniform_real_distribution<double>;

    static uDoubleDist_t m_dist(0.0, 1.0);
    static uDoubleDist_t m_hBuild(5.0, 50.0);
    static uDoubleDist_t m_wStreet(5.0, 50.0);

    constexpr double MaxLoss_s = std::numeric_limits<double>::max();
    constexpr double C_s = 3.0e8; // light speed (m/s))

    inline double
    LosProbabilityUma(double distance) {
        return std::min(18 / distance, 1.0) * (1.0 - std::exp(-distance / 63)) +
                std::exp(-distance / 63);
    };

    inline double
    LosProbabilityUmi(double distance) {
        return std::min(18.0 / distance, 1.0) * (1 - std::exp(-distance / 36)) +
                std::exp(-distance / 36);
    };

    inline double
    LosProbabilityUpi(double distance) {
        return (0.5) -std::min(0.5, 5 * std::exp(-156 / distance)) +
                std::min(0.5, std::exp(-distance / 30));
    };

    units::dB
    UmaPathloss_LOS(units::m distance, units::GHz frec, units::m hbs, units::m hut) {
        auto distance_ = distance.RawVal();
        if (distance_ < 10) {
            return units::dB(0.0);
        } else if (distance_ > 5000) {
            return units::dB(MaxLoss_s);
        }
        auto hbs_ = hbs.RawVal();
        auto hut_ = hut.RawVal();
        auto frec_ = frec.RawVal();
        auto pl_(0.0);
        auto dBp_ = 4.0 * (hbs_ - 1.0) * (hut_ - 1.0) * frec.RefVal() / C_s;
        if (distance_ >= 10.0 && distance_ <= dBp_) {
            pl_ = 22.0 * std::log10(distance_) + 28.0 + 20.0 * std::log10(frec_);
        } else {
            pl_ = 40.0 * log10(distance_) + 7.8 - 18.0 * log10(hbs_ - 1.0) -
                    18.0 * log10(hut_ - 1.0) + 2.0 * log10(frec_);
        }
        if (Shadow_s) {
            std::random_device rd_;
            std::default_random_engine rng_(rd_());
            auto sigma_ = 4.0;
            std::normal_distribution<> dst_(0.0, sigma_);
            pl_ += dst_(rng_);
        }
        return units::dB(pl_);
    }

    units::dB
    UmaPathloss_NLOS(units::m distance, units::GHz frec, units::m hbs, units::m hut) {
        auto distance_ = distance.RawVal();
        if (distance_ < 10) {
            return units::dB(0.0);
        } else if (distance_ > 5000) {
            return units::dB(MaxLoss_s);
        }
        auto hbs_ = hbs.RawVal();
        auto hut_ = hut.RawVal();
        auto frec_ = frec.RawVal();
        auto pl_(0.0);
        std::random_device rd_;
        std::default_random_engine rng_(rd_());
        auto wStreet_ = m_wStreet(rng_);
        auto hBuild_ = m_hBuild(rng_);
        pl_ = 161.04 - 7.1 * std::log10(wStreet_) + 7.5 * std::log10(hBuild_)
                - (24.37 - 3.7 * std::pow(hBuild_ / hbs_, 2.0)) * std::log10(hbs_) +
                (43.42 - 3.1 * std::log10(hbs_)) * (std::log10(distance_) - 3) +
                20.0 * std::log10(frec_) -
                (3.2 * std::pow(std::log10(11.75 * hut_), 2.0) - 4.97);
        if (Shadow_s) {
            auto sigma_ = 6.0;
            std::normal_distribution<> dst_(0.0, sigma_);
            pl_ += dst_(rng_);
        }
        return units::dB(pl_);
    }

    units::dB
    UmaPathloss(units::m distance, units::GHz frec, units::m hbs, units::m hut) {
        auto distance_ = distance.RawVal();
        if (distance_ < 10) {
            return units::dB(0.0);
        } else if (distance_ > 5000) {
            return units::dB(MaxLoss_s);
        }
        std::random_device rd_;
        std::default_random_engine rng_(rd_());
        auto pLos_ = LosProbabilityUma(distance_);
        auto r_ = m_dist(rng_);
        if (r_ <= pLos_) {
            return UmaPathloss_LOS(distance, frec, hbs, hut);
        }
        return UmaPathloss_NLOS(distance, frec, hbs, hut);
    }

    units::dB
    UmiPathloss_LOS(units::m distance, units::GHz frec, units::m hbs, units::m hut) {
        auto distance_ = distance.RawVal();
        if (distance_ < 10) {
            return units::dB(0.0);
        } else if (distance_ > 5000) {
            return units::dB(MaxLoss_s);
        }
        auto hbs_ = hbs.RawVal();
        auto hut_ = hut.RawVal();
        auto frec_ = frec.RawVal();
        auto pl_(0.0);
        auto dBp_ = 4.0 * (hbs_ - 1.0) * (hut_ - 1.0) * frec_ / C_s;
        if (distance_ >= 10.0 && distance_ <= dBp_) {
            pl_ = 22.0 * std::log10(distance_) + 28.0 + 20.0 * std::log10(frec_);
        } else {
            pl_ = 40.0 * std::log10(distance_) + 7.8 - 18.0 * log10(hbs_ - 1.0) -
                    18.0 * std::log10(hut_ - 1.0) + 2.0 * std::log10(frec_);
        }
        if (Shadow_s) {
            std::random_device rd_;
            std::default_random_engine rng_(rd_());
            auto sigma_ = 3.0;
            std::normal_distribution<> dst_(0.0, sigma_);
            pl_ += dst_(rng_);
        }
        return units::dB(pl_);
    }

    units::dB
    UmiPathloss_NLOS(units::m distance, units::GHz frec, units::m hbs, units::m hut) {
        auto distance_ = distance.RawVal();
        if (distance_ < 10) {
            return units::dB(0.0);
        } else if (distance_ > 5000) {
            return units::dB(MaxLoss_s);
        }
        auto frec_ = frec.RawVal();
        auto pl_(0.0);
        if (distance_ <= 2000) {
            pl_ = 36.7 * std::log10(distance_) + 22.7 + 26.0 * std::log10(frec_);
        } else {
            pl_ = MaxLoss_s;
        }
        if (Shadow_s) {
            std::random_device rd_;
            std::default_random_engine rng_(rd_());
            auto sigma_ = 4.0;
            std::normal_distribution<> dst_(0.0, sigma_);
            pl_ += dst_(rng_);
        }
        return units::dB(pl_);
    }

    units::dB
    UmiPathloss(units::m distance, units::GHz frec, units::m hbs, units::m hut) {
        auto distance_ = distance.RawVal();
        if (distance_ < 10) {
            return units::dB(0.0);
        } else if (distance_ > 5000) {
            return units::dB(MaxLoss_s);
        }
        auto pLos_ = LosProbabilityUmi(distance_);
        std::random_device rd_;
        std::default_random_engine rng_(rd_());
        auto r_ = m_dist(rng_);
        if (r_ <= pLos_) //LOS
        {
            return UmiPathloss_LOS(distance, frec, hbs, hut);
        }
        return UmiPathloss_NLOS(distance, frec, hbs, hut);
    }

    units::dB
    UpiPathloss_LOS(units::m distance, units::GHz frec, units::m hbs, units::m hut) {
        auto distance_ = distance.RawVal();
        if (distance_ > 500.0) {
            return units::dB(MaxLoss_s);
        }
        auto pl_(0.0);
        pl_ = 103.8 + (20.9) * (std::log10(distance_) - 3);
        if (Shadow_s) {
            std::random_device rd_;
            std::default_random_engine rng_(rd_());
            auto sigma_ = 6.0;
            std::normal_distribution<> dst_(0.0, sigma_);
            pl_ += dst_(rng_);
        }
        return units::dB(pl_);
    }

    units::dB
    UpiPathloss_NLOS(units::m distance, units::GHz frec, units::m hbs, units::m hut) {
        auto distance_ = distance.RawVal();
        if (distance_ > 500.0) {
            return units::dB(MaxLoss_s);
        }
        auto pl_(0.0);
        pl_ = 145.48 + (37.5) * (std::log10(distance_) - 3);
        if (Shadow_s) {
            std::random_device rd_;
            std::default_random_engine rng_(rd_());
            auto sigma_ = 6.0;
            std::normal_distribution<> dst_(0.0, sigma_);
            pl_ += dst_(rng_);
        }
        return units::dB(pl_);
    }

    units::dB
    UpiPathloss(units::m distance, units::GHz frec, units::m hbs, units::m hut) {
        auto distance_ = distance.RawVal();
        if (distance_ > 500.0) {
            return units::dB(MaxLoss_s);
        }
        std::random_device rd_;
        std::default_random_engine rng_(rd_());
        auto r_ = m_dist(rng_);
        auto losProb_ = LosProbabilityUpi(distance_);
        if (r_ <= losProb_) {
            return UpiPathloss_LOS(distance, frec, hbs, hut);
        }
        return UpiPathloss_NLOS(distance, frec, hbs, hut);
    }

} // namespace LTE