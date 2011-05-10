-- Туркин Александр, задача № 1, вариант 3.
-- Написать функцию sumdiv :: Integer -> Integer,
-- принимающую в качестве аргумента положительное число, 
-- выдающую в качестве результата сумму всех его делителей, включая единицу, но исключая само это число


module Task1 where
-- Функция sumdiv описана в условии задачи
-- Аргументы: положительное число
-- Результат: сумма всех делителей (включая единицу, исключая само число)
sumdiv :: Integer -> Integer
sumdiv' :: Integer -> Integer -> Integer -> Integer
sumdiv x | x <= 0    = error "argument should be positive"
         | otherwise = sumdiv' x 1 0
sumdiv' x y sum | y > x `div` 2   = sum
                | x `mod` y == 0  = sumdiv' x (y+1) sum+y
                | otherwise       = sumdiv' x (y+1) sum

-- Тесты:
-- sumdiv 36 -> 1+2+3+4+6+9+12+18 = 55
-- sumdiv 15 -> 1+3+5 = 9
-- sumdiv 70 -> 1+2+5+7+10+14+35 = 74
-- sumdiv 101 -> 1 = 1
main = (sumdiv 36,
        sumdiv 15,
        sumdiv 70,
        sumdiv 101)


