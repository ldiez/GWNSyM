#ifndef UNITS_H
#define UNITS_H

#include <iostream>
#include <functional>
#include <cmath>
#include <string>

/**
 * \brief Minimal conversion from-to different units of a magnitude
 * -It implements Addition/subtraction for linear and logarithmic scale
 * -Multiplication and division is not provided as square magnitudes are not implemented
 * -Provides conversion from any linear/log to any log/linear
 */
namespace units {

    constexpr double Lin2log(double lin) {
        return 10.0 * std::log10(lin);
    }

    constexpr double Log2lin(double db) {
        return std::pow(10.0, db / 10.0);
    }


    // Linear prefixes

    enum class LinPref : std::uint8_t {
        Giga,
        Mega,
        Kilo,
        Ref, // it is unitary unit
        Milli,
        Micro,
        Nano,
        Pico,
    };

    // logarithmic prefixes

    enum class LogPref : std::uint8_t {
        dBM,
        dBk,
        dB,
        dBm,
        dBu
    };

    // linear multipliers
    template < LinPref >
    constexpr double LinMult;

    template < >
    constexpr double LinMult<LinPref::Giga> = 1.0e9;

    template < >
    constexpr double LinMult<LinPref::Mega> = 1.0e6;

    template < >
    constexpr double LinMult<LinPref::Kilo> = 1.0e3;

    template < >
    constexpr double LinMult<LinPref::Ref> = 1.0;

    template < >
    constexpr double LinMult<LinPref::Milli> = 1.0e-3;

    template < >
    constexpr double LinMult<LinPref::Micro> = 1.0e-6;

    template < >
    constexpr double LinMult<LinPref::Nano> = 1.0e-9;

    template < >
    constexpr double LinMult<LinPref::Pico> = 1.0e-12;


    // Logarithmic offsets
    template < LogPref >
    constexpr double LogOffset;

    template < >
    constexpr double LogOffset<LogPref::dBM> = 60.0;

    template < >
    constexpr double LogOffset<LogPref::dBk> = 30.0;

    template < >
    constexpr double LogOffset<LogPref::dB> = 0.0;

    template < >
    constexpr double LogOffset<LogPref::dBm> = -30.0;

    template < >
    constexpr double LogOffset<LogPref::dBu> = -60.0;

    /**
     * \ingroup utils
     * \brief Class to store linear magnitudes. 
     * It provides addition and subtraction
     */
    template < LinPref I >
    struct LinVal {
        constexpr static LinPref UNIT = I;

        explicit LinVal(double val) : m_val(val) {
        }

        LinVal& operator=(double v) {
            m_val = v;
            return *this;
        }

        double RefVal(void) const {
            return m_val*m_mult;
        }

        double RawVal(void) const {
            return m_val;
        }

        LinVal<I> operator+(LinVal<I> const& v) const {
            return LinVal<I>(m_val + v.RawVal());
        };

        template < LinPref P >
        LinVal<LinPref::Ref> operator+(LinVal<P> const& v) const {
            return LinVal<LinPref::Ref>(RefVal() + v.RefVal());
        };

        void operator+=(LinVal<I> const& v) {
            m_val += v.RawVal();
        };

        template < LinPref P >
        void operator+=(LinVal<P> const& v) {
            m_val += v.RefVal() / LinMult<P>;
        };

        LinVal<I> operator-(LinVal<I> const& v) const {
            return LinVal<I>(RawVal() - v.RawVal());
        };

        template < LinPref P >
        LinVal<LinPref::Ref> operator-(LinVal<P> const& v) const {
            return LinVal<LinPref::Ref>(RefVal() - v.RefVal());
        };

        void operator-=(LinVal<I> const& v) {
            m_val -= v.RawVal();
        };

        template < LinPref P >
        void operator-=(LinVal<P> const& v) {
            m_val -= v.RefVal() / LinMult<P>;
        };

        void operator*=(double amp) {
            m_val *= amp;
        }

        template< LinPref P >
        bool operator<(LinVal<P> const& v) const {
            return RawVal() < v.RawVal();
        }

        template< LinPref P >
        bool operator<=(LinVal<P> const& v) const {
            return RawVal() <= v.RawVal();
        }

        template< LinPref P >
        bool operator>(LinVal<P> const& v) const {
            return RawVal() > v.RawVal();
        }

        template< LinPref P >
        bool operator>=(LinVal<P> const& v) const {
            return RawVal() >= v.RawVal();
        }

        template< LinPref P >
        bool operator==(LinVal<P> const& v) const {
            return RawVal() == v.RawVal();
        }

        template< LinPref P >
        bool operator!=(LinVal<P> const& v) const {
            return RawVal() != v.RawVal();
        }

    private:
        LinVal() = delete;
        double m_val;
        constexpr static double m_mult = LinMult<I>;
    };

    /*
     * \ingroup utils
     * \brief Class to store logarithmic magnitudes.
     * It provides addition and subtraction
     */

    template < LogPref I >
    struct LogVal {
        constexpr static LogPref UNIT = I;

        explicit LogVal(double val) : m_val(val) {
        }

        LogVal& operator=(double val) {
            m_val = val;
            return *this;
        }

        double RefVal(void) const {
            return m_val + m_offset;
        }

        double RawVal(void) const {
            return m_val;
        }

        LogVal<I> operator+(LogVal<I> const& v) const {
            return LogVal<I>(RawVal() + v.RawVal());
        };

        template < LogPref P >
        LogVal<LogPref::dB> operator+(LogVal<P> const& v) const {
            return LogVal<LogPref::dB>(RefVal() + v.RefVal());
        };

        void operator+=(LogVal<I> const& v) {
            m_val += v.RawVal();
        };

        template < LogPref P >
        void operator+=(LogVal<P> const& v) {
            m_val += v.RefVal() - LogOffset<P>;
        };

        LogVal<I> operator-(LogVal<I> const& v) const {
            return LogVal<I>(RawVal() - v.RawVal());
        };

        template < LogPref P >
        LogVal<LogPref::dB> operator-(LogVal<P> const& v) const {
            return LogVal<LogPref::dB>(RefVal() - v.RefVal());
        };

        void operator-=(LogVal<I> const& v) {
            m_val -= v.RawVal();
        };

        template < LogPref P >
        void operator-=(LogVal<P> const& v) {
            m_val -= v.RefVal() - LogOffset<P>;
        };

        void operator+=(double amp) {
            m_val += amp;
        }

        template< LogPref P >
        bool operator<(LogVal<P> const& v) const {
            return RawVal() < v.RawVal();
        }

        template< LogPref P >
        bool operator<=(LogVal<P> const& v) const {
            return RawVal() <= v.RawVal();
        }

        template< LogPref P >
        bool operator>(LogVal<P> const& v) const {
            return RawVal() > v.RawVal();
        }

        template< LogPref P >
        bool operator>=(LogVal<P> const& v) const {
            return RawVal() >= v.RawVal();
        }

        template< LogPref P >
        bool operator==(LogVal<P> const& v) const {
            return RawVal() == v.RawVal();
        }

        template< LogPref P >
        bool operator!=(LogVal<P> const& v) const {
            return RawVal() != v.RawVal();
        }

//    private:
        LogVal() = delete;
        double m_val;
        constexpr static double m_offset = LogOffset<I>;
    };

    template < typename I >
    struct To;

    template < LinPref I >
    struct To<LinVal<I>>
    {

        template <LinPref J >
                static LinVal<I> Do(LinVal<J> const& v) {
            return LinVal<I>(v.RefVal() / LinMult<I>);
        }

        template <LogPref J >
                static LinVal<I> Do(LogVal<J> const& v) {
            return LinVal<I>(Log2lin(v.RefVal()) / LinMult<I>);
        }
    };

    template < LogPref I >
    struct To<LogVal<I>>
    {

        template <LogPref J >
                static LogVal<I> Do(LogVal<J> const& v) {
            return LogVal<I>(v.RefVal() - LogOffset<I>);
        }

        template <LinPref J >
                static LogVal<I> Do(LinVal<J> const& v) {
            return LogVal<I>(Lin2log(v.RefVal()) - LogOffset<I>);
        }
    };

    // Conversion from linear to linear. It creates a new instance 

    template < LinPref I, LinPref J >
    LinVal<I> Convert(LinVal<J> const& v) {
        return LinVal<I>(v.RefVal() / LinMult<I>);
    }

    // Conversion from logarithmic to logarithmic. It creates a new instance 

    template <LogPref I, LogPref J >
    LogVal<I> Convert(LogVal<J> const& v) {
        return LogVal<I>(v.RefVal() - LogOffset<I>);
    }

    // conversion from linear/logarithmic from logarithmic/linear. It creates a new instance 

    template < LogPref I, LinPref J>
    LogVal<I> Convert(LinVal<J> const& v) {
        return LogVal<I>(Lin2log(v.RefVal()) - LogOffset<I>);
    }

    template < LinPref I, LogPref J>
    LinVal<I> Convert(LogVal<J> const& v) {
        return LinVal<I>(Log2lin(v.RefVal()) / LinMult<I>);
    }

    template < LinPref I >
    inline std::ostream& operator<<(std::ostream& os, LinVal<I> const& d) {
        os << d.RawVal();
        return os;
    }

    template < LogPref I >
    inline std::ostream& operator<<(std::ostream& os, LogVal<I> const& d) {
        os << d.RawVal();
        return os;
    }

} // namespace units
#endif /* UNITS_H */

