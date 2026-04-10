Assembly Image Processing Benchmark

ENGLISH

This project is a lab work focused on image processing and performance comparison between different implementations.

The task was to modify an image by recoloring half of it using three different approaches:
- C++
- Assembly
- Assembly with MMX instructions

The main goal was to compare performance and understand how low-level optimizations affect execution speed.

What was done:
- Load and process an image
- Recolor half of the image
- Implement the same logic in three different ways
- Measure and compare execution time

Results showed how SIMD instructions (MMX) can significantly improve performance compared to both plain assembly and high-level C++ code.

This project helped to better understand:
- how CPU processes data
- how memory access affects performance
- how SIMD optimizations work

---

РУССКИЙ

Лабораторная работа по обработке изображений и сравнению производительности.

Задача заключалась в том, чтобы изменить изображение (перекрасить половину) тремя способами:
- на C++
- на обычном ассемблере
- на ассемблере с использованием MMX

Основная цель — сравнить быстродействие и понять влияние низкоуровневых оптимизаций.

Что реализовано:
- загрузка и обработка изображения
- перекрашивание половины изображения
- три реализации одного и того же алгоритма
- измерение времени выполнения

Результаты показали, что использование SIMD (MMX) даёт прирост производительности, (но незначительный) по сравнению с обычным ассемблером и C++.

Проект помог лучше понять:
- как работает процессор
- влияние работы с памятью
- принципы SIMD-оптимизаций
