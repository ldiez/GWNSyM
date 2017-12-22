#include "PythonSolver.h"
#include <iostream>
#include <cmath>

#include "Log.h"

LOG_REGISTER_MODULE("PythonSolver");

#define IMPORTS "import numpy, cvxopt, scipy, gpkit\n"

PythonSolver::PythonSolver(std::string&& modPath, std::string&& scdcMod)
: m_modulesPath(std::forward<std::string>(modPath))
, m_scdcModule(std::forward<std::string>(scdcMod))
{
    BEG;
    Py_Initialize();
    END;
}

PythonSolver::~PythonSolver()
{
    BEG;
    Py_Finalize();
    END;
}

PythonSolver::Vec
PythonSolver::SolveScdc(Vec const& pow, Vec const& demand, Matrix const& inter, Vec noise)
{
    BEG;
    Vec ret_;
    PyRun_SimpleString(IMPORTS);
    PySys_SetPath(const_cast<char*> (m_modulesPath.c_str()));

    PyScope pModule_(PyImport_ImportModule(m_scdcModule.c_str())); // new ref
    MSG_ASSERT(pModule_ != nullptr, "Python module could not be loaded: " + m_scdcModule)

    if (pModule_ != nullptr)
    {
        INFO("Translating ... ");
        PyScope pFunc_(PyObject_GetAttrString(pModule_.get(), "Solve")); // new ref
        if (pFunc_ && PyCallable_Check(pFunc_.get()))
        {
            ret_ = DoSolveScdc(pFunc_, pow, demand, inter, noise);
        }
    }

    END;
    return ret_;
}

PythonSolver::Vec
PythonSolver::DoSolveScdc(PyScope& func, Vec const& pow, Vec const& demand, Matrix const& inter,
                          Vec noise)
{
    BEG;
    PyScope powList_(PyList_New(0));
    PyScope demList_(PyList_New(0));
    PyScope noiseList_(PyList_New(0));
    PyScope intList_(PyList_New(0));
    PyScope argList_(PyTuple_New(4));

    assert(powList_ != nullptr && "Failed in creating a list for python");
    assert(demList_ != nullptr && "Failed in creating a list for python");
    assert(intList_ != nullptr && "Failed in creating a list for python");
    assert(argList_ != nullptr && "Failed in creating argument list");

    VecToPyList(pow, powList_);
    VecToPyList(demand, demList_);
    VecToPyList(noise, noiseList_);
    MatrixToPyDoubleList(inter, intList_);

    PyTuple_SetItem(argList_.get(), 0, powList_.get());
    PyTuple_SetItem(argList_.get(), 1, demList_.get());
    PyTuple_SetItem(argList_.get(), 2, intList_.get());
    PyTuple_SetItem(argList_.get(), 3, noiseList_.get());

    PyScope sol_(PyObject_CallObject(func.get(), argList_.get()));

    Vec ret_;
    PyListToVec(sol_, ret_);
    END;
    return ret_;
}

void
PythonSolver::VecToPyList(PythonSolver::Vec const& vec, PyScope& pyList)
{
    BEG;
    assert(pyList != nullptr && PyList_Check(pyList.get())
           && "VecToPyList: Incorrect Python list object");

    for (auto& item : vec)
    {
        PyList_Append(pyList.get(), PyFloat_FromDouble(item));
    }
    END;

}

PyObject*
PythonSolver::VecToPyList(Vec const& vec)
{
    BEG;
    auto list_ = PyList_New(0);
    assert(list_ != nullptr && "Failed in creating a list for python");

    for (auto& item : vec)
    {
        PyList_Append(list_, PyFloat_FromDouble(item));
    }
    END;
    return list_;
}

void
PythonSolver::PyListToVec(PyScope const& pyList, PythonSolver::Vec& vec)
{
    BEG;
    //    assert(pyList != nullptr && "Results list is nullptr");
    //    assert(PyList_Check(pyList.get()) && "PyListToVec: Incorrect Python list object");

    if (pyList == nullptr)
    {
        return;
    }
    auto size_ = PyList_Size(pyList.get());
    for (decltype(size_) it_ = 0; it_ < size_; ++it_)
    {
        auto item_ = PyList_GetItem(pyList.get(), it_); // borrowed
        vec.push_back(PyFloat_AsDouble(item_));
    }
    END;
}

void
PythonSolver::MatrixToPyDoubleList(PythonSolver::Matrix const& mat, PythonSolver::PyScope& pyList)
{
    BEG;
    assert(pyList != nullptr && PyList_Check(pyList.get())
           && "MatrixToPyDoubleList: Incorrect Python list object");


    if (mat.empty())
    {
        return;
    }
    const auto size_ = mat.begin()->size();

    for (auto& item_ : mat)
    {
        assert(item_.size() == size_ && "Matrix is not uniform");
        PyScope aux_(VecToPyList(item_));
        PyList_Append(pyList.get(), aux_.get());
    }
    END;
}

void
PythonSolver::PyDoubleListToMatrix(PythonSolver::PyScope const& pyList, PythonSolver::Matrix& mat)
{
    BEG;
    assert(pyList != nullptr && PyList_Check(pyList.get())
           && "PyDoubleListToMatrix: Incorrect Python list object");
    END;
}