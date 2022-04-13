#ifndef USEFULPYTHON_HH
#define USEFULPYTHON_HH

#include <Python.h>
#include <string>
#include <vector>

using namespace std;

double pyRun(std::string funcBody, double a, double b) {
    try {
        Py_Initialize();

        PyObject* moduleMain = PyImport_ImportModule("__main__");

        string command = "def fun(a, b):\n" + funcBody;
        PyRun_SimpleString(
            command.c_str()
        );

        PyObject* func = PyObject_GetAttrString(moduleMain, "fun");
        PyObject* args = PyTuple_Pack(2, PyFloat_FromDouble(a), PyFloat_FromDouble(b));

        PyObject* result = PyObject_CallObject(func, args);

        double dub = PyFloat_AsDouble(result);
        Py_Finalize();
        return dub;
    }
    catch (...) {
        return 0;// nullptr;
    }
}

// PyObject -> Vector
vector<float> listTupleToVector_Float(PyObject* incoming) {
    vector<float> data;
    if (PyTuple_Check(incoming)) {
        for (Py_ssize_t i = 0; i < PyTuple_Size(incoming); i++) {
            PyObject* value = PyTuple_GetItem(incoming, i);
            data.push_back(PyFloat_AsDouble(value));
        }
    }
    else {
        if (PyList_Check(incoming)) {
            for (Py_ssize_t i = 0; i < PyList_Size(incoming); i++) {
                PyObject* value = PyList_GetItem(incoming, i);
                data.push_back(PyFloat_AsDouble(value));
            }
        }
        else {
            // throw logic_error("Passed PyObject pointer was not a list or tuple!");
        }
    }
    return data;
}

std::vector<float> getVec(double begin, double end, double step) {
    string cmd = "np.arange(" + to_string(begin) + ", " + to_string(end) + ", " + to_string(step) + ")\n";
    return {}; // return listTupleToVector_Float(pyRun(""));
}

#endif  // USEFULPYTHON_HH
