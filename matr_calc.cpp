#include <vector>
#include <stack>

extern "C" {
	#include <Python.h>
}

namespace matr_calc
{
	typedef std::vector<long>  row_t;
	typedef std::vector<row_t>   matrix_t;

	static double calc_det(matrix_t d){
		long n = d.size();
		double det = 1;
		double EPS = 0.00009;
		for (int i = 0; i<n; ++i) {
			int k = i;
			for (int j = i + 1; j<n; ++j)
				if (abs(d[j][i]) > abs(d[k][i]))
					k = j;
			if (abs(d[k][i]) < EPS) {
				det = 0;
				break;
			}
			swap(d[i], d[k]);
			if (i != k)
				det = -det;
			det *= d[i][i];
			for (int j = i + 1; j<n; ++j)
				d[i][j] /= d[i][i];
			for (int j = 0; j<n; ++j)
				if (j != i && abs(d[j][i]) > EPS)
					for (int k = i + 1; k<n; ++k)
						d[j][k] -= d[i][k] * d[j][i];
		}
		return det;
	}
}


static PyObject * cxx_to_pyobject(const double &res)
{
	return PyFloat_FromDouble(res);
}

static matr_calc::matrix_t pyobject_to_cxx(PyObject * py_matrix)
{
	matr_calc::matrix_t result;
	result.resize(PyObject_Length(py_matrix));
	for (size_t i = 0; i < result.size(); ++i) {
		PyObject * py_row = PyList_GetItem(py_matrix, i);
		matr_calc::row_t & row = result[i];
		row.resize(PyObject_Length(py_row));
		for (size_t j = 0; j < row.size(); ++j) {
			PyObject * py_elem = PyList_GetItem(py_row, j);
			const long elem = PyLong_AsLong(py_elem);
			row[j] = elem;
		}
	}
	return result;
}

static PyObject* func(PyObject* mod, PyObject* args)
{
	PyObject* py_matrix = PyTuple_GetItem(args, 0);
	
	matr_calc::matrix_t matrix = pyobject_to_cxx(py_matrix);

	double result = matr_calc::calc_det(matrix);

	PyObject* py_result = cxx_to_pyobject(result);
	return py_result;
} 

PyMODINIT_FUNC PyInit_matr_calc()
{
	static PyMethodDef ModMethods[] = {
		{ "DET", func, METH_VARARGS, "Gauss Determinant Calculation" },
		{ NULL, NULL, 0, NULL }
	};

	static PyModuleDef ModDef = {
		PyModuleDef_HEAD_INIT,
		"matr_calc",
		"Matrix_calculations_Modula",
		-1, ModMethods,
		NULL, NULL, NULL, NULL
	};

	PyObject * module = PyModule_Create(&ModDef);
	return module;
}