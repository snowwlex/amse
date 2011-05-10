-- Туркин Александр, задача № 5, вариант 3.
-- Написать функцию optimalTree :: Ord a => [a] -> Tree
-- принимающую в качестве аргумента список элементов (условие: их можно сравнивать)
-- выдающую в качестве результата оптимальное двоичное дерево поиска, построенное из элементов заданного списка


module Task5 where

import UnitTests
import List

data Tree a = Empty | Node (Tree a) a (Tree a)         

-- Функция optimalTree описана в задании
-- Аргументы: список элементов, которые можно сравнивать
-- Результат: оптимальное двоичное дерево поиска
optimalTree   :: (Ord a) => [a] -> Tree a
optimalTree list = optimalTree' (sort list)

-- Функция optimalTree' - вспомогательная
-- Аргументы: отсортированный список элементов
-- Результат: оптимальное двоичное дерево поиска
-- Алгоритм работы: в качестве элемента узла берется элемент в середине отсортированного списка, а левый и правый сыновья рекурсивно строятся и оставшихся частей списка. Взятие элемента в середине отсортированного списка обеспечивает построение оптимального дерева.
optimalTree' :: (Ord a) => [a] -> Tree a
optimalTree' [] = Empty
optimalTree' list = Node (optimalTree' left)  middle  (optimalTree' right)
                    where (left,middle:right) = splitAt (div (length list) 2) list 


-- Функция height вычисляет высоту дерева
-- Аргументы: дерево
-- Результат: высота дерева
height :: Tree a -> Integer
height Empty = 0
height (Node t1 n t2) = 1 + max (height t1) (height t2)

-- Функция optHeight вычисляет высоту оптимального дерева для заданного количества элементов; используется для тестирования
-- Аргументы: количество элементов
-- Результат: высота оптимального дерева из заданного количества элементов
optHeight :: Integer -> Integer
optHeight number = floor (logBase 2 (fromIntegral number)) + 1

main = unitTests [
        ( height (optimalTree [3,5,12]) , optHeight 3 ),
        ( height (optimalTree [5,23,12,7,34,(-1),12]) , optHeight 7 ),
        ( height (optimalTree [6,3,23,6,43,234,45,87]) , optHeight 8 ),
        ( height (optimalTree "alexander") , optHeight 9 ),
        ( height (optimalTree [17,16,15,14,13,0,0,0,289]) , optHeight 9 ) ]


