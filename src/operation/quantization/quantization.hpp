#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include "quantized_tensor.hpp"

namespace quantization
{
	template <typename QType>
	int get_bit()
	{
		int default_bit = 8;
		if (std::is_same<QType, uint8_t>::value)
		{
			default_bit = 8;
		}
		else if (std::is_same<QType, uint16_t>::value)
		{
			default_bit = 16;
		}
		else if (std::is_same<QType, float>::value)
		{
			default_bit = 32;
		}
		else if (std::is_same<QType, double>::value)
		{
			default_bit = 64;
		}
		else
		{
			std::string log_message = "[ERROR] This type is not supported yet...";
			throw std::runtime_error(log_message);
		}
		return default_bit;
	}

	template <typename QType, typename DQType>
	QuantizedTensor<QType, DQType> quantization(const Tensor<QType> &weights, const std::string &name, QType min_value, QType max_value)
	{
		try
		{
			int bit = get_bit<DQType>();

			QType scale = (QType)((max_value - min_value) / (QType)((1 << bit) - 1));
			DQType offset = (DQType)((-min_value) / scale + 0.5);

			const std::vector<QType> &weights_vector = weights.get_serialized_tensor();
			std::vector<DQType> output;
			for (const auto &weight : weights_vector)
			{
				DQType quantized_weight = (DQType)(weight / scale + (float)offset + 0.5);
				output.push_back(quantized_weight);
			}
			QuantizedTensor<QType, DQType> quantized_tensor(name, weights.get_dimension(), output);
			quantized_tensor.set_scale(scale);
			quantized_tensor.set_offset(offset);
			return std::move(quantized_tensor);
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
			exit(0);
		}
		return QuantizedTensor<QType, DQType>("");
	}

	template <typename QType, typename DQType>
	QuantizedTensor<QType, DQType> quantization(const Tensor<QType> &weights, const std::string &name)
	{

		try
		{
			const std::vector<QType> &weights_vector = weights.get_serialized_tensor();
			QType min_value = *std::min_element(weights_vector.begin(), weights_vector.end());
			QType max_value = *std::max_element(weights_vector.begin(), weights_vector.end());
			return std::move(quantization<QType, DQType>(weights, name, min_value, max_value));
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
		return QuantizedTensor<QType, DQType>("");
	}
};