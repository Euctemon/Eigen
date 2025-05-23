#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <numbers>

extern "C" {
#include "..\\eigenvalues.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

static std::string enum_to_string(const Convergence& value) {
	switch (value) {
	case Convergence::unknown:   return "unknown";
	case Convergence::converged: return "converged";
	case Convergence::diverged:  return "diverged";
	default: return "oops";
	}
}

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		TEST_METHOD(Diagonal_matrix_vals) {
			const double check_tol = 1E-5;
			const size_t dim = 2;
			struct Matrix* mat_pt = mat_init(dim);
			struct EigenArray* arr = init_eigen_array(dim, dim);

			double mat_data[] = { 1.9,0,0,2.1 };
			memcpy(mat_pt->data, mat_data, 4 * sizeof(double));

			enum Convergence state = fill_eigen_array(arr, mat_pt, 1E-8);
			double first = arr->data[0].eigenval;
			double second = arr->data[1].eigenval;

			free(mat_pt);
			clear_eigen_array(arr);

			Assert::AreEqual(2.1, first, check_tol);
			Assert::AreEqual(1.9, second, check_tol);
			Assert::AreEqual(enum_to_string(converged), enum_to_string(state));
		}
		TEST_METHOD(Zero_matrix_vals) {
			const double check_tol = 1E-5;
			const size_t dim = 2;
			struct Matrix* mat_pt = mat_init(dim);
			struct EigenArray* arr = init_eigen_array(dim, dim);

			double mat_data[] = { 0.0,0.0,0.0,0.0 };
			memcpy(mat_pt->data, mat_data, 4 * sizeof(double));

			enum Convergence state = fill_eigen_array(arr, mat_pt, 1E-8);
			double first = arr->data[0].eigenval;
			
			free(mat_pt);
			clear_eigen_array(arr);

			Assert::IsTrue(isnan(first));
			Assert::AreEqual(enum_to_string(unknown), enum_to_string(state));
		}

		TEST_METHOD(Negative_vals) {
			const double check_tol = 1E-5;
			const size_t dim = 2;
			struct Matrix* mat_pt = mat_init(dim);
			struct EigenArray* arr = init_eigen_array(dim, dim);

			double mat_data[] = { 1.0,2.0,2.0,-2.0 };
			memcpy(mat_pt->data, mat_data, 4 * sizeof(double));

			enum Convergence state = fill_eigen_array(arr, mat_pt, 1E-8);
			double first = arr->data[0].eigenval;
			double second = arr->data[1].eigenval;

			free(mat_pt);
			clear_eigen_array(arr);

			Assert::AreEqual(-3.0, first, check_tol);
			Assert::AreEqual(2.0, second, check_tol);
			Assert::AreEqual(enum_to_string(converged), enum_to_string(state));
		}

		// eigenvalues are sqrt(17) and -sqrt(17)
		// the deflation technique used for obtaining the next eigenvalue is generally
		// not reliable when the matrix has multiple eigenvalues of the same magnitude
		TEST_METHOD(Same_magnitude) {
			const double check_tol = 1E-5;
			const size_t dim = 2;
			struct Matrix* mat_pt = mat_init(dim);
			struct EigenArray* arr = init_eigen_array(dim, dim);

			double mat_data[] = { 4.0,1.0,1.0,-4.0 };
			memcpy(mat_pt->data, mat_data, 4 * sizeof(double));

			enum Convergence state = fill_eigen_array(arr, mat_pt, 1E-8);
			double first = arr->data[0].eigenval;
			double second = arr->data[1].eigenval;

			free(mat_pt);
			clear_eigen_array(arr);

			Assert::AreEqual(sqrt(17.0), abs(first), check_tol);
			Assert::AreNotEqual(sqrt(17.0), abs(second), check_tol);
			Assert::AreEqual(enum_to_string(converged), enum_to_string(state));
		}

		// https://www.math.purdue.edu/~eremenko/dvi/beads2.pdf
		// for classical stiffness matrix of #dim beads with stiffness k = 1 funny things happen
		// for odd #dim the algorithm computes
		//		- greatest, third greatest, fifth greatest ...
		// for even #dim the algorithm computes
		//		- second greatest, fourth greatest ...
		TEST_METHOD(Stiffness_matrix) {
			const double check_tol = 1E-5;
			const size_t dim = 7;
			struct Matrix* mat_pt = mat_init(dim);
			struct EigenArray* arr = init_eigen_array(2, dim);

			for (size_t i = 0; i < dim * dim; i++) mat_pt->data[i] = 0.0;
			
			for (size_t i = 1; i < dim - 1; i++) {
				mat_pt->data[i + i * dim - 1] = -1.0;
				mat_pt->data[i + i * dim] = 2.0;
				mat_pt->data[i + i * dim + 1] = -1.0;
			}
			mat_pt->data[0] = 2.0;
			mat_pt->data[1] = -1.0;
			mat_pt->data[dim * dim - 2] = -1.0;
			mat_pt->data[dim * dim - 1] = 2.0;

			enum Convergence state = fill_eigen_array(arr, mat_pt, 1E-8);
			double first = arr->data[0].eigenval;
			double second = arr->data[1].eigenval;

			free(mat_pt);
			clear_eigen_array(arr);

			if (dim % 2 == 0) {
				// first computed eigenvalue is the second largest one
				// second computed eigenvalue is the fourth largest one
				Assert::AreEqual(4 * pow(sin(std::numbers::pi * (dim - 1) / (2 * (dim + 1))), 2), first, check_tol);
				Assert::AreEqual(4 * pow(sin(std::numbers::pi * (dim - 3) / (2 * (dim + 1))), 2), second, check_tol);
			}
			else {
				// first computed eigenvalue is truly the largest one
				// second computed eigenvalue is the third largest one
				Assert::AreEqual(4 * pow(sin(std::numbers::pi * dim / (2 * (dim + 1))), 2), first, check_tol);
				Assert::AreEqual(4 * pow(sin(std::numbers::pi * (dim - 2) / (2 * (dim + 1))), 2), second, check_tol);
			}
			
			Assert::AreEqual(enum_to_string(converged), enum_to_string(state));
		}
	};
}
