module Lib
    ( fact
    ) where

fact :: Int -> Int
fact 0 = 1
fact n = n * fact (n - 1)

fib :: Int -> Int
fib 0 = 0
fib 1 = 1
fib n = fib (n - 1) + fib (n - 2)

-- [] is a list, Eq a says that a is some type that is comparable
removeDups :: Eq a => [a] -> [a]
removeDups [] = []
removeDups (x : xs) = x : removeDups (filter (=/ x) xs)
