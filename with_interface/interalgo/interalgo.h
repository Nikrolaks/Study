// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// нечасто изменяются
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows



// установите здесь ссылки на дополнительные заголовки, требующиеся для программы
// Heap
#include "heap/heap.hpp"

// Sorts
#include "sorts/sorts_n2.hpp"
#include "sorts/sorts_nlog_n.hpp"

// Binary trees
#include "utils.hpp"

// Treap
#include "treap/treap.hpp"
// Red-black tree
//#include "rb_tree/rb_tree.hpp"