#ifndef PYTHONSOLVER_H
#define PYTHONSOLVER_H

#include <python2.7/Python.h>
#include <string>
#include <vector>
#include <memory>

/**
 * \brief This class implements the interface with GPKIT
 * Python solver.
 * From any module the function \Solve is called to perform the optimization.
 * A set of solvers are defined according to the problem they solve:
 * - Single Connection Demand Constraint (SCDC)
 */

struct PyObjectFakeDeleter
{

    void operator()(PyObject* b)
    {
        Py_DECREF(b);
    }
};

class PythonSolver
{
public:

    using Vec = std::vector<double>;
    using Matrix = std::vector<Vec>;
    using PyScope = std::unique_ptr<PyObject, PyObjectFakeDeleter>;

    /**
     * \brief Ctor
     * \param modPath --> Path of the python modules
     * \param scdcMod --> Name of the scdc module 
     */
    PythonSolver(std::string&& modPath, std::string&& scdcMod = "scdc");
    ~PythonSolver();

    /**
     * \brief Solve an SCDC problem instance.
     * The problem assigns the resources to N users once the selection is performed. 
     * \param pow --> Vector of powers received by each user, length N
     * \param demand --> Demand of users is Kbps.
     * \param inter --> Interference matrix with the interference coming. Size NxX
     * \param noise --> Noise floor to use
     * \param resBw --> Bandwith of the resources in KHz
     * from the connections of other users
     */
    Vec SolveScdc(Vec const& pow, Vec const& demand, Matrix const& inter, Vec noise);

private:
    Vec DoSolveScdc(PyScope& func, Vec const& pow, Vec const& demand, Matrix const& inter,
                    Vec noise);

    /**
     * \brief Insert a C++ vector into a Python list object. The elements 
     * are appended into the list
     * \param vec --> C++ vector
     * \param pyList <--> Python List
     * \return --
     */
    void VecToPyList(Vec const& vec, PyScope& pyList);

    /**
     * \brief Create a new Python list from a C++ vector
     * \param vec --> C++ vector
     * \return <-- Python list
     */
    PyObject* VecToPyList(Vec const& vec);

    /**
     * \brief Insert a Python list into a C++ vector. The elements are appended 
     * (pushed back) into the vector
     * \param pyList --> Python list
     * \param vec <--> C++ Vector
     * \return --
     */
    void PyListToVec(PyScope const& pyList, Vec& vec);

    /**
     * \brief Insert a C++ matrix within a Python list-of-lists. 
     * Each row of the matrix is a list. It is checked that all
     * the rows have the same size
     * \param mat --> C++ Matrix
     * \param pyList <---> List of List
     * \return --
     */
    void MatrixToPyDoubleList(Matrix const& mat, PyScope& pyList);

    /**
     * \brief Insert a Python list-of-lists within a C++ matrix. Each list 
     * is a row of the matrix. It is checked that all the lists have the 
     * same size
     * \param pyList --> Python list-of-lists
     * \param mat <--> C++ matrix 
     * \return --
     */
    void PyDoubleListToMatrix(PyScope const& pyList, Matrix& mat);

    std::string m_modulesPath; // path with the python solvers
    std::string m_scdcModule; // name of the SCDC module

};

#endif /* PYTHONSOLVER_H */

