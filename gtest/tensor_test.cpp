#include "gtest/gtest.h"
#include "tensor.hpp"

TEST(tensorTest, CheckTensorInitialization)
{
    Tensor<float> float_inputs("float_inputs", {2, 2, 2, 1}, "ones");
    Tensor<int> int_inputs("int_inputs", {2, 2, 2, 1}, "ones");

    const std::vector<int> expected_int_inputs = {1, 1, 1, 1, 1, 1, 1, 1};

    const auto &serialized_int_inputs = int_inputs.get_serialized_tensor();
    const auto &serialized_float_inputs = float_inputs.get_serialized_tensor();

    EXPECT_EQ(serialized_float_inputs.size(), serialized_int_inputs.size());
    EXPECT_EQ(serialized_float_inputs.size(), expected_int_inputs.size());

    for (size_t i = 0; i < expected_int_inputs.size(); i++)
    {
        EXPECT_EQ(expected_int_inputs[i], serialized_int_inputs[i]);
        EXPECT_EQ((float)expected_int_inputs[i], serialized_float_inputs[i]);
    }
}

TEST(tensorTest, CheckIm2ColOutputDimensionIsFine)
{
    using TensorType = int;
    Tensor<TensorType> inputs("inputs", {1, 3, 3, 2}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -2, -3, -4, -5, -6, -7, -8});

    /*
    [inputs]
        0  1  2      9  -1  -2
        3  4  5     -3  -4  -5
        6  7  8     -6  -7  -8
     */

    std::cout << inputs;

    const auto &outputs = inputs.get_im2col(2, 2);
    std::vector<std::vector<TensorType>> expected_outputs = {
        {0, 1, 3, 4, 9, -1, -3, -4},
        {1, 2, 4, 5, -1, -2, -4, -5},
        {3, 4, 6, 7, -3, -4, -6, -7},
        {4, 5, 7, 8, -4, -5, -7, -8}};

    const int output_row = 4, output_col = 8;
    ASSERT_EQ(output_row, (int)outputs.size());
    ASSERT_EQ(output_col, (int)outputs[0].size());

    std::cout << "\n[`outputs`]\n";
    for (int row = 0; row < output_row; row++)
    {
        std::cout << " [ ";
        for (int col = 0; col < output_col; col++)
        {
            EXPECT_EQ(expected_outputs[row][col], outputs[row][col]);
            std::cout << outputs[row][col] << " ";
        }
        std::cout << "]\n";
    }
    std::cout << "\n";
}

TEST(tensorTest, CheckSerializedTensor)
{
    using TensorType = int;
    Tensor<TensorType> inputs("inputs", {1, 3, 3, 2}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -2, -3, -4, -5, -6, -7, -8});

    const auto &serialized_inputs = inputs.get_serialized_tensor();
    ASSERT_EQ(18, (int)serialized_inputs.size());

    Tensor<TensorType> inputs2("inputs2", {2, 3, 3, 1}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -2, -3, -4, -5, -6, -7, -8});

    const auto &serialized_inputs2 = inputs2.get_serialized_tensor();
    ASSERT_EQ(18, (int)serialized_inputs2.size());
}