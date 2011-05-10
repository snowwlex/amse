-- Модуль, содержащий функцию для организации тестирования результатов

module UnitTests(unitTests) where

-- Функция, запускающая тесты и выдающая результат тестирования.
-- Аргумент: список пар (выражение, ожидаемый результат).
-- Результат: True, если значения всех выражений совпали с ожидаемыми результатами;
--            сообщение "Error in test # k, если обнаружено несовпадение значений в тесте № k.
unitTests :: Eq a => [(a,a)] -> Bool
unitTests = unitTests' 1

-- Вспомогательная функция с дополнительным аргументом - номером очередного
-- проверяемого теста.
-- Аргументы: (1) номер очередного теста;
--            (2) список пар (выражение, ожидаемый результат).
-- Результат: см. функцию unitTests
unitTests' :: Eq a => Int -> [(a,a)] -> Bool
unitTests' _ [] = True
unitTests' i ((x,y):s) | x /= y = error ("Error in test # " ++ show i)
                       | otherwise = unitTests' (i+1) s
