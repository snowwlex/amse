-- Туркин Александр, задача № 2, вариант 3.
-- Написать функцию ch :: Double -> Integer -> Double
-- принимающую в качестве аргументов значение точки и количество суммируемых членов 
-- выдающую в качестве результата значение гиперболического косинуса в заданной точке, суммируя заданное число первых членов ряда в разложении этой функции в ряд Тейлора.


module Task2 where
         
-- Функция ch описана в условии задачи
-- Аргументы: значение точки и количество суммируемых членов 
-- Результат: значение гиперболического косинуса в заданной точке (сумма первых членов ряда Тейлора)
ch :: Double -> Integer -> Double
ch x n | n < 0 = error "number of member should be not-negative"
       | otherwise = sum ( map (\n -> nmember x n) [0..n-1] )       
               
-- Функция nmember вычисляет n-ый член ряда в разложении функции гиперболического косинуса в ряд Тейлора
-- Аргументы: значение точки и номер члена (начиная с 0)
-- Результат: значение n-го челна ряда в заданной точке
nmember :: Double -> Integer -> Double
nmember x n  | n < 0 = error "number of member should be not-negative"
          | otherwise = 1/(fromIntegral (factorial (2*n))) * 
            x ^ (fromIntegral(2*n))
  

-- Функция factorial вычисляет значение факториала,
-- используя концевую рекурсию
factorial :: Integer -> Integer
factorial' :: Integer -> Integer -> Integer
factorial 0 = 1
factorial n | n < 0 = error "non-positive arg"
            | otherwise = factorial' 1 n
factorial' a n | n == 1 = a
               | otherwise = factorial' (a*n) (n-1)
               
-- Тесты:
main = (
        (ch 5 10, cosh 5),
        (ch 10 50, cosh 10),
        (ch 20 20, cosh 20),
        (ch 30 30, cosh 30),
        (ch 40 40, cosh 40) 
       )


