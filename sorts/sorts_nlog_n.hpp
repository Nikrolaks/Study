#pragma once


#include <iostream>
#include <vector>

#include "../utils.hpp"

template <typename T>
void heap_sort(std::vector<T> &mas, bool quiet) {
	if (!quiet) {
		std::cout << "Welcome to heap sort" << std::endl;
		std::cout << "Start information:" << std::endl;
		std::cout << "Size: " << mas.size() << std::endl;
		std::cout << mas << std::endl << std::endl;
	}
	
	utils::heap<T> h(mas);
	if (!quiet)
		std::cout << h << std::endl;
	for (std::size_t i = 0; i < mas.size(); ++i) {
		mas[i] = h.get_head();
		h.remove_head();
		if (!quiet)
			std::cout << h << std::endl;
	}
	if (!quiet)
		std::cout << "Sorted: " << mas << std::endl;
}
