#include "pch.h"
#include "CppUnitTest.h"

#define restrict __restrict

extern "C" {
#include "../matrix.c"
#include "../vector.c"
#include "../eigenvalues.c"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests) {
	public:
		TEST_METHOD(List_delete_idempotent) {
			struct Vector* vec_pt = vec_init(2);
			struct Node* eigen_list = NULL;

			list_add(&eigen_list, { 1, vec_pt });
			list_delete(&eigen_list);
			list_delete(&eigen_list);

			Assert::IsNull(eigen_list);
		}
		TEST_METHOD(Diagonal_matrix_eigenvalues) {
			struct Matrix* mat_pt = mat_init(2);
			struct Node* eigen_list = NULL;
			struct EigenPair first = { 0 };
			struct EigenPair second = { 0 };

			const double tol_compute = 1E-8;
			const double tol_check = 1E-4;
			double first_val = 0;
			double second_val = 0;
			bool converged = false;
			bool empty_image = false;

			mat_pt->data[0] = 1.9;
			mat_pt->data[1] = 0;
			mat_pt->data[2] = 0;
			mat_pt->data[3] = 2.1;

			first = eigenpair_compute(&eigen_list, mat_pt, tol_compute, &converged, &empty_image);
			first_val = first.val;
			list_add(&eigen_list, first);

			second = eigenpair_compute(&eigen_list, mat_pt, tol_compute, &converged, &empty_image);
			second_val = second.val;
			list_add(&eigen_list, second);
			list_delete(&eigen_list);

			Assert::AreEqual(2.1, first_val, tol_check);
			Assert::AreEqual(1.9, second_val, tol_check);
		}
		TEST_METHOD(Zero_matrix_image) {
			struct Matrix* mat_pt = mat_init(2);
			struct Node* eigen_list = NULL;
			struct EigenPair first = { 0 };

			const double tol_compute = 1E-8;
			const double tol_check = 1E-4;
			double first_val = 0;
			bool converged = false;
			bool empty_image = false;

			mat_pt->data[0] = 0;
			mat_pt->data[1] = 0;
			mat_pt->data[2] = 0;
			mat_pt->data[3] = 0;

			first = eigenpair_compute(&eigen_list, mat_pt, tol_compute, &converged, &empty_image);
			free(first.vec);

			Assert::IsTrue(empty_image);
			Assert::IsFalse(converged);
		}
	};
}
