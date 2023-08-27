{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Use product" #-}

module A1 where

{--

Implement the following functions. 

For full credit, you must define each function using 
just one call to either map or foldr 
 
--}


-- squares [1, 2, 3] = [1, 4, 9]
-- squares [] = []
squares :: [Int] -> [Int]
squares = map (\x -> x * x)


-- 'lengths' should return sum of lenghts of all the strings in a list.
-- lengths ["car", "cdr"] = 6
-- lengths [] = 0
lengths :: [String] -> Int
lengths = foldr (\x y -> length x + y) 0


-- 'triplicate' makes three copies of all the elements in a list.
-- triplicate [1, 2, 3] = [1, 1, 1, 2, 2, 2, 3, 3, 3]
-- triplicate [] = []
triplicate :: [a] -> [a]
triplicate = foldr (\x y -> [x, x, x] ++ y) []


-- 'allEven' tests if all the elements in a list are even.
-- allEven [2, 4, 8] = True
-- allEven [1, 4, 8] = False
-- allEven [] = ?
allEven :: [Int] -> Bool
allEven = foldr (\x y -> even x && y) True


-- 'mul' multiplies all the elements in a list.
-- mul [1, 2, 3] = 6
-- mul [] = ?
mul :: [Int] -> Int
mul = foldr (\x y -> x * y) 1

-- 'checkSum' returns the sum of a list modulo 10.
-- checkSum [1, 2, 3] = 6
-- checkSum [4, 6, 2] = 2
checkSum :: [Int] -> Int
checkSum = foldr (\x y -> (x + y) `mod` 10) 0


-- 'largest' finds the largest element in a list.
-- largest [1, 2, 3] = 3
largest :: [Int] -> Int
largest xs = foldr (\x y -> if x > y then x else y) (head xs) xs


-- Logic: write the following functions


-- Every function type has its propositional logic equivalent formula on top of it.
-- Use both the type and your knowledge from propositonal logic to produce a term of that type.
-- Note that implication corresponds to the function type, conjunction corresponds to the
-- pair type, disjunction corresponds to the Either type, and negation corresponds to the Not type.

data Empty

type Not a = a -> Empty

-- ⊥ => A
absurd :: Empty -> a
absurd x = case x of {}


-- (¬A ∧ ¬B) => ¬(A ∨ B)
deMorgan1 :: (Not a, Not b) -> Not (Either a b)
deMorgan1 (notA, _) (Left a) = notA a
deMorgan1 (_, notB) (Right b) = notB b


-- ¬(A ∨ B) => ¬A ∧ ¬B
deMorgan2 :: Not (Either a b) -> (Not a, Not b)
deMorgan2 notAB = (notA, notB)
    where
        notA = notAB . Left
        notB = notAB . Right

-- ¬A => ¬B => ¬(A ∨ B)
deMorgan3 :: Not a -> Not b -> Not (Either a b)
deMorgan3 notA notB = \eitherAB -> case eitherAB of
    Left a -> notA a
    Right b -> notB b


-- (A => B) => ¬B => ¬A
contraPos :: (a -> b) -> Not b -> Not a
contraPos f notB = notB . f


-- A => B => A
imp1 :: a -> b -> a
imp1 x y = x


-- (A => B) => (B => C) => (A => C)
comp1 :: (a -> b) -> (b -> c) -> (a -> c)
comp1 f g x = g (f x)


-- (B => C) => (A => B) => (A => C)
comp2 :: (b -> c) -> (a -> b) -> (a -> c)
comp2 f g x = f (g x)


-- (A => B => C) => (A => B) => A => C
comp3 :: (a -> b -> c) -> (a -> b) -> a -> c
comp3 f g x = f x (g x)


-- ¬A => A => B
contra0 :: Not a -> a -> b
contra0 notA a = absurd (notA a)


-- A ∨ B => ¬A => B
contra1 :: Either a b -> Not a -> b
contra1 (Left a) notA = absurd (notA a)
contra1 (Right b) _ = b


-- ¬¬¬A => ¬A
contra2 :: Not (Not (Not a)) -> Not a
contra2 notNotNotA a = absurd (notNotNotA (contra3 a))


-- A => ¬¬A
contra3 :: a -> Not (Not a)
contra3 a = \notA -> notA a

distrAnd :: (a, Either b c) -> Either (a, b) (a, c)
distrAnd (a, eitherBC) = case eitherBC of
    (Left b) -> (a, b)
    (Right c) -> (a, c)