#pragma once

#include <iostream>
#include <vector>

#include "../utils/utils.hpp"

template <typename T>
void selection_sort(std::vector<T> &mas, bool quiet) {
	if (!quiet) {
		std::cout << "Welcome to selection sort" << std::endl;
		std::cout << "Start information:" << std::endl;
		std::cout << "Size: " << mas.size() << std::endl;
		std::cout << mas << std::endl << std::endl;
	}

	for (std::size_t i = 0; i < mas.size() - 1; ++i) {
		std::size_t min_p = i;
		for (std::size_t pos = i; pos < mas.size(); ++pos)
			if (mas[pos] < mas[min_p])
				min_p = pos;
		std::swap(mas[i], mas[min_p]);
		if (!quiet)
			std::cout << "(" << i + 1 << ")\t" << mas << std::endl;
	}
}

template <typename T>
void insertion_sort(std::vector<T> &mas, bool quiet) {
	if (!quiet) {
		std::cout << "Welcome to insertion sort" << std::endl;
		std::cout << "Start information:" << std::endl;
		std::cout << "Size: " << mas.size() << std::endl;
		std::cout << mas << std::endl << std::endl;
	}

	for (std::size_t i = 1; i < mas.size(); ++i) {
		for (std::size_t pos = i; pos > 0 && mas[pos - 1] > mas[pos]; --pos)
			std::swap(mas[pos - 1], mas[pos]);
		if (!quiet)
			std::cout << "(" << i << ")\t" << mas << std::endl;
	}
}
