#include "csc_matrix_conv.h"

namespace osqp
{

  CSC_Matrix calCSCMatrix(const Eigen::MatrixXd &mat)
{
  int elem = mat.nonZeros();
  int rows = mat.rows();
  int cols = mat.cols();

  std::vector<c_float> vals;
  vals.reserve(elem);
  std::vector<c_int> row_idxs;
  row_idxs.reserve(elem);
  std::vector<c_int> col_idxs;
  col_idxs.reserve(elem);

  // Construct CSC matrix arrays
  c_float val;
  int elem_count = 0;

  col_idxs.push_back(0);

  for (int j = 0; j < cols; j++) // col iteration
  {

    for (int i = 0; i <rows; i++) // row iteration
    {
      // Get values of nonzero elements
      val = mat(i, j);
      if (std::fabs(val) < 1e-9)
      {
        continue;
      }
      elem_count += 1;

      // Store values
      vals.push_back(val);
      row_idxs.push_back(i);
    }

    col_idxs.push_back(elem_count);
  }

  CSC_Matrix csc_matrix = {vals, row_idxs, col_idxs};

  return csc_matrix;

}

CSC_Matrix calCSCMatrixTrapesoidal(const Eigen::MatrixXd &mat)
{
  int elem = mat.nonZeros();
  int rows = mat.rows();
  int cols = mat.cols();

  if (rows != cols)
  {
    throw std::invalid_argument("Matrix must be square (n, n)");
  }

  std::vector<c_float> vals;
  vals.reserve(elem);
  std::vector<c_int> row_idxs;
  row_idxs.reserve(elem);
  std::vector<c_int> col_idxs;
  col_idxs.reserve(elem);

  // Construct CSC matrix arrays
  c_float val;
  int trap_last_idx = 0;
  int elem_count = 0;

  col_idxs.push_back(0);

  for (int j = 0; j < cols; j++) // col iteration
  {

    for (int i = 0; i <= trap_last_idx; i++) // row iteration
    {
      // Get values of nonzero elements
      val = mat(i, j);
      if (std::fabs(val) < 1e-9)
      {
        continue;
      }
      elem_count += 1;

      // Store values
      vals.push_back(val);
      row_idxs.push_back(i);
    }

    col_idxs.push_back(elem_count);
    trap_last_idx += 1;
  }

  CSC_Matrix csc_matrix = {vals, row_idxs, col_idxs};

  return csc_matrix;
}

void printCSCMatrix(CSC_Matrix &csc_mat)
{
  std::cout << "[";
  for (std::vector<c_float>::const_iterator it = csc_mat.vals.begin(); it != csc_mat.vals.end(); it++)
  {
    std::cout << *it << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "[";
  for (std::vector<c_int>::const_iterator it = csc_mat.row_idxs.begin(); it != csc_mat.row_idxs.end(); it++)
  {
    std::cout << *it << ", ";
  }
  std::cout << "]" << std::endl;

  std::cout << "[";
  for (std::vector<c_int>::const_iterator it = csc_mat.col_idxs.begin(); it != csc_mat.col_idxs.end(); it++)
  {
    std::cout << *it << ", ";
  }
  std::cout << "]" << std::endl;
}

} // namespace osqp