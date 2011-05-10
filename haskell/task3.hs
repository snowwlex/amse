-- Туркин Александр, задача № 3, вариант 3.
-- Написать функцию uplength :: [Integer] -> Integer
-- принимающую в качестве аргумента список целых чисел
-- выдающую в качестве результата длину самой длинной неубывающей подпоследовательности идущих подряд членов исходного списка


module Task3 where

import UnitTests
         
-- Функция uplength описана в задании
-- Аргументы: список целых чисел
-- Результат: длина самой длинной неубывающей подпоследовательности идущих подряд членов исходного списка
uplength :: [Integer] -> Integer
uplength' :: [Integer] -> Integer -> Integer -> Integer -> Integer
uplength [] = 0
uplength (x:ls) = uplength' ls x 1 1
uplength' [] prev cur max = max
uplength' (x:ls) prev cur max | x >= prev && cur==max = uplength' ls x (cur+1) (cur+1)
                              | x >= prev             = uplength' ls x (cur+1) max
                              | x < prev && cur>max   = uplength' ls x 1 cur
                              | x < prev              = uplength' ls x 1 max


-- Тесты:
main = unitTests [
        ( uplength [] , 0 ),                              -- []
        ( uplength [1] , 1 ),                             -- [1]
        ( uplength [2, 1] , 1 ),                          -- [2]
        ( uplength [3,1,2,0,1,2,3,0,1,2,3,4] , 5 ),       -- [0,1,2,3,4]
        ( uplength [9,8,7,6,5,4,3,4,5,4,3,2,1] , 3 ),     -- [3,4,5]
        ( uplength [2, 5, 12, 9, 10, 4, 7, 8] , 3 ),      -- [4,7,8]
        ( uplength [1,2,3,4,5,0,1,2,3,4,5,6,(-1)] , 7 )   -- [0,1,2,3,4,5,6]
       ]


